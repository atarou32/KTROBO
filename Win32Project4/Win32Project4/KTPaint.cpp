#include "KTPaint.h"
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "Resource.h"
#include "../../../../../ResEditProjects/ForKTPaint/resource.h"
#include "KTRoboGameError.h"
#include "KTRoboCS.h"



#define KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT 95
KTPaint::KTPaint(HINSTANCE hins)
{
	hInst = hins;
	gradiation_circle_radius = KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT-1;
	gradiation_circle_theta = 0;
	gradiation_triangle_pos.x = 0;
	gradiation_triangle_pos.y = 0;
	now_gcolor_b = 0;
	now_gcolor_g = 0;
	now_gcolor_r = 0xFF;
}


KTPaint::~KTPaint(void)
{
	Release();
}

void KTPaint::setCursorNow() {
	SetCursor(temp_cursor);
}
void KTPaint::setCursorToPencil() {
	static HCURSOR hCursor=NULL;
	if (!hCursor) {
		hCursor = LoadCursor(hInst, (LPCSTR)IDI_CURSOR_PENCIL);
	}
	
	temp_cursor = hCursor;
	now_paint_id = KTPAINT_PENCIL_ID;
	SetCursor(hCursor);
}

void KTPaint::setCursorToPen() {

	static HCURSOR hCursor=NULL;
	if (!hCursor) {
		hCursor = LoadCursor(hInst, (LPCSTR)IDI_CURSOR_PEN);
	}
	
	temp_cursor = hCursor;
	now_paint_id = KTPAINT_PEN_ID;
	SetCursor(hCursor);

}

void KTPaint::setCursorToNuri() {
	static HCURSOR hCursor=NULL;
	if (!hCursor) {
		hCursor = LoadCursor(hInst, (LPCSTR)IDI_CURSOR_NURI);
	}
	
	temp_cursor = hCursor;
	now_paint_id = KTPAINT_NURI_ID;
	SetCursor(hCursor);

	
}


void KTPaint::setCursorToEraser() {

	static HCURSOR hCursor=NULL;
	if (!hCursor) {
		hCursor = LoadCursor(hInst, (LPCSTR)IDI_CURSOR_ERASER);
	}
	
	temp_cursor = hCursor;
	now_paint_id = KTPAINT_ERASER_ID;
	SetCursor(hCursor);

}

void KTPaint::Init(HWND hwnd) {
	KTROBO::CS::instance()->Init();
	g = new KTROBO::Graphics();
	g->Init(hwnd);
	KTROBO::Graphics::InitMSS(g);


	parent_window = hwnd;
	createKoWindow(hwnd);
	setCursorToPen();





}

bool MyRegisterClass(HINSTANCE hInst, LPCSTR name) {


	WNDCLASS wc;
	wc.style  = CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc = DefWindowProc;//LoadSaveWindowProc ; // �v���V�[�W����
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon  = NULL;
    wc.hCursor  = NULL;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject( NULL_BRUSH));//reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)); //�O���[�ɂ���
    wc.lpszMenuName = NULL;  // ���T�|�[�g
    wc.lpszClassName=(LPCSTR)name;
	return RegisterClass(&wc);



}

void KTPaint::createKoWindow(HWND p_window) {
	WNDCLASS wc;
	wc.style  = 0;// CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc = ColorPenWindowProc ; // �v���V�[�W����
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon  = NULL;
    wc.hCursor  = NULL;
    wc.hbrBackground= reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)); //�O���[�ɂ���
    wc.lpszMenuName = NULL;  // ���T�|�[�g
    wc.lpszClassName=(LPCSTR) "colorpen";
	bool t = RegisterClass(&wc);
	colorpen_window = CreateWindow("colorpen", "colorpen", WS_OVERLAPPED,
		1200, 0, 220,800,/*CW_USEDEFAULT, CW_USEDEFAULT,*/ NULL, NULL, hInst, NULL);
	DWORD tt = GetLastError();

	if (!t || !colorpen_window)
	{
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "error in create kowindowcolorpen");
	}

	wc.style  = CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc = LoadSaveWindowProc ; // �v���V�[�W����
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon  = NULL;
    wc.hCursor  = NULL;
    wc.hbrBackground= reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)); //�O���[�ɂ���
    wc.lpszMenuName = NULL;  // ���T�|�[�g
    wc.lpszClassName=(LPCSTR) "saveload";
	t = RegisterClass(&wc);

	loadsave_window = CreateWindow("saveload", "saveload",  WS_OVERLAPPED,
		0, 0, 200,800,/*CW_USEDEFAULT, CW_USEDEFAULT,*/ NULL, NULL, hInst, NULL);

	if (!t || !loadsave_window)
	{
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "error in create kowindow");
	}

	ShowWindow(loadsave_window, SW_SHOW);
	UpdateWindow(loadsave_window);

	ShowWindow(colorpen_window, SW_SHOW);
	UpdateWindow(colorpen_window);



	
	CreateWindow(TEXT("BUTTON"),TEXT("ERASERBUT"),
                 WS_CHILD|WS_VISIBLE| BS_BITMAP,
                 0,0,30,30,
                 colorpen_window,
				 (HMENU)1,
                 hInst,
                 NULL);
	
	HBITMAP hBitmap=LoadBitmap(hInst,LPCSTR(IDB_BITMAP1));
	SendDlgItemMessage(colorpen_window,1,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hBitmap);
//	bool as = MyRegisterClass(hInst, "PENBUT");
	
	DWORD last = GetLastError();
	
   
//	ShowWindow(hw,SW_SHOW);
//	UpdateWindow(hw);
	
	//MyRegisterClass(hInst, "PENCILBUT");
	CreateWindow(TEXT("BUTTON"),"NURIBUT",
                 WS_CHILD|WS_VISIBLE|BS_BITMAP,
                 30,0,30,30,
                 colorpen_window,
                 (HMENU)2,
                 hInst,
                 NULL);
    hBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP2));
	SendDlgItemMessage(colorpen_window,2,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hBitmap);


	//MyRegisterClass(hInst, "ERASERBUT");
	CreateWindow(TEXT("BUTTON"), "PENBUT",
                 WS_CHILD|WS_VISIBLE|BS_BITMAP,
                 60,0,30,30,
                 colorpen_window,
                 (HMENU)3,
                 hInst,
                 NULL);
	hBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP3));
	SendDlgItemMessage(colorpen_window,3,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hBitmap);


	//MyRegisterClass(hInst, "COLORPENBUT");
	CreateWindow(TEXT("BUTTON"), "PENCILBUT",
                 WS_CHILD|WS_VISIBLE|BS_BITMAP,
                 90,0,30,30,
                 colorpen_window,
                 (HMENU)4,
                 hInst,
                 NULL);
    hBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP4));
	SendDlgItemMessage(colorpen_window,4,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hBitmap);



	#ifndef HID_USAGE_PAGE_GENERIC
    #define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
    #endif
    #ifndef HID_USAGE_GENERIC_MOUSE
    #define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
    #endif
    RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC; 
    Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE; 
    Rid[0].dwFlags = RIDEV_INPUTSINK;   
    Rid[0].hwndTarget = colorpen_window;
    RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));


	HDC hdc;
	RECT rc;
    hdc = GetDC(colorpen_window);                      	// �E�C���h�E��DC���擾
    GetClientRect(colorpen_window, &rc);  	// �f�X�N�g�b�v�̃T�C�Y���擾
    hBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
    hdcMem = CreateCompatibleDC(NULL);		// �J�����g�X�N���[���݊�
    SelectObject(hdcMem, hBitmap);		// MDC�Ƀr�b�g�}�b�v������t��
	ReleaseDC(colorpen_window, hdc);
	


	InvalidateRect(colorpen_window, NULL,false);




}


void KTPaint::writeWithPen(POINT mpo, POINT po, UINT pressure_old, UINT pressure_new) {

	sheet.setPline(mpo,po,3,3);

}

void KTPaint::render() {
	float clearColor[4] = {
		0.2f,0.2f,0.2f,1.0f};
	KTROBO::CS::instance()->enter(CS_DEVICECON_CS, "enter");
	g->getDeviceContext()->ClearRenderTargetView(g->getRenderTargetView(),clearColor);
	//g->getDeviceContext()->ClearDepthStencilView(Mesh::pDepthStencilView,  D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f, 0 );
	KTROBO::Graphics::drawPen(g, this->sheet.getPline(),sheet.getPlineMax());

	g->getSwapChain()->Present(NULL,NULL);
	KTROBO::CS::instance()->leave(CS_DEVICECON_CS,"leave");
}
void KTPaint::Release()  {
	KTROBO::Graphics::Del();
	if (g) {
		g->Release();
		delete g;
		g = 0;
	}
	KTROBO::CS::instance()->Del();
}



bool KTPaint::isInCircleColorPen(ULONG mouse_x, ULONG mouse_y) {

	float dd = (mouse_x - 100)*(mouse_x-100) + (mouse_y - 200) * (mouse_y-200);
	if (dd < KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT*KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT) {
		if (dd > KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT*KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT*0.7*0.7) {
			return true;
		}
	}


	return false;
}
bool KTPaint::isInTriangleColorPen(ULONG mouse_x, ULONG mouse_y) {

	float dd = (mouse_x - 100)*(mouse_x-100) + (mouse_y - 200) * (mouse_y-200);
	if (dd < KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT*KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT) {
		if (dd < KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT*KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT*0.7*0.7) {
			
			return true;
		}
	}




	return false;
}


#define KTROBO_MOUSESTATE_L_DOWN 0x01
#define KTROBO_MOUSESTATE_L_UP 0x02
#define KTROBO_MOUSESTATE_R_DOWN 0x04
#define KTROBO_MOUSESTATE_R_UP 0x08
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))


void KTPaint::setNowColor(COLORREF c) {
	now_color_b = GetBValue(c);
	now_color_g = GetGValue(c);
	now_color_r = GetRValue(c);
}

void KTPaint::setNowGColor(COLORREF c) {
	now_gcolor_b = GetBValue(c);
	now_gcolor_g = GetGValue(c);
	now_gcolor_r = GetRValue(c);
}


void KTPaint::setGColorTheta(ULONG mouse_x, ULONG mouse_y) {
	POINT center;
	center.x = 100;
	center.y = 200;

	int dy = mouse_y - center.y;
	int dx = mouse_x - center.x;
	int dx2dy2 = dy*dy+dx*dx;

	if (dx2dy2) {
		if (dy >0) {
			float dq = sqrt(dx2dy2);
			float theta = acos(dx/dq);
			this->gradiation_circle_theta = theta;
			this->gradiation_circle_radius = sqrt(dx2dy2);
		} else {
			float dq = sqrt(dx2dy2);
			float theta = acos(dx/dq);
			this->gradiation_circle_theta = 2*3.14-theta;
			this->gradiation_circle_radius = sqrt(dx2dy2);
		}
	}
}
void KTPaint::paint() {


	POINT center;
	MYTRIANGLEPOINT points[3];

/*	BOOL ExtFloodFill(
  HDC hdc,          // �f�o�C�X�R���e�L�X�g�n���h��
  int nXStart,      // �J�n�_�� x ���W
  int nYStart,      // �J�n�_�� y ���W
  COLORREF crColor, // �F
  UINT fuFillType   // ���
);
*/
	getGUI()->clear(hdcMem);
	center.x = 100;
	center.y = 200;
	getGUI()->drawGradiationCircleToWindow(hdcMem,center,KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT);
	points[0].b = now_gcolor_b;
	points[0].g = now_gcolor_g;
	points[0].r = now_gcolor_r;
	points[1].b = 0;
	points[1].g = 0;
	points[1].r = 0;
	points[2].b = 0xFF;
	points[2].g = 0xFF;
	points[2].r = 0xFF;
	points[0].x = center.x + KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT * 0.7 * cos(gradiation_circle_theta);
	points[0].y = center.y + KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT * 0.7 * sin(gradiation_circle_theta);
	points[1].x = center.x + KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT * 0.7 * cos(gradiation_circle_theta+2*3.14/3);
	points[1].y = center.y + KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT * 0.7 * sin(gradiation_circle_theta+2*3.14/3);
	points[2].x = center.x + KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT * 0.7 * cos(gradiation_circle_theta+2*3.14*2/3);
	points[2].y = center.y + KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT * 0.7 * sin(gradiation_circle_theta+2*3.14*2/3);
	getGUI()->drawGradiationTriangleToWindow(hdcMem,points);
	// ���݂̐F��h��
	getGUI()->drawRectangleToWindow(hdcMem, now_color_r,now_color_g,now_color_b,0,50,200,100);
}


extern KTPaint* paint;
LRESULT CALLBACK ColorPenWindowProc(HWND h, UINT i, WPARAM w, LPARAM l) {
	HDC hdc;
	PAINTSTRUCT psPaint;
	POINT center;
	static ULONG mouse_button=0;
	static bool mouse_l=false;
	static RECT rcClient;
	MYTRIANGLEPOINT points[3];
	POINT mouse_pos;
	static ULONG xMousePos=0;
	static ULONG yMousePos=0;
	bool redraw = false;
	switch(i) {
	case WM_ERASEBKGND:
		return 0;
	case WM_MOUSEMOVE:
			xMousePos = GET_X_LPARAM(l);
			yMousePos = GET_Y_LPARAM(l);
			if (mouse_l) {
				if (paint->isInCircleColorPen(xMousePos, yMousePos)) {
					hdc = GetDC(h);
					COLORREF c = GetPixel(hdc,xMousePos,yMousePos);
					paint->setNowGColor(c);
					paint->setNowColor(c);
					paint->setGColorTheta(xMousePos,yMousePos);
					redraw = true;
					ReleaseDC(h,hdc);
				}

				if (paint->isInTriangleColorPen(xMousePos, yMousePos)) {
					hdc = GetDC(h);
					COLORREF c = GetPixel(hdc,xMousePos,yMousePos);
					paint->setNowColor(c);
					redraw = true;	
					ReleaseDC(h,hdc);
					
				}

				if (redraw) {
					RECT r;
					r.left = 0;
					r.right = 220;
					r.top = 50;
					r.bottom = 300;
					InvalidateRect(h,&r,false);
				}
			}
		break;
	case WM_MOVE:
	case WM_SIZE:
		GetClientRect(h, &rcClient);
		InvalidateRect(h, NULL, false);
		break;

	case WM_INPUT:
		{
		 UINT dwSize;
      
		GetRawInputData((HRAWINPUT)l, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
			 unsigned char* lpb = new unsigned char[dwSize];
			 if (lpb == NULL)
			{
				// �G���[
				break;
			}

		if (GetRawInputData((HRAWINPUT)l, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
		{
				// �G���[
				delete[] lpb;
				break;
		}

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEMOUSE) 
        {
				//mouse_xy.x = raw->data.mouse.lLastX;
				//mouse_xy.y = raw->data.mouse.lLastY;
				mouse_button = raw->data.mouse.ulButtons;
			
			}

			delete[] lpb; 
			}
		if (mouse_button & KTROBO_MOUSESTATE_L_UP) {
			mouse_l = false;
		}
		if (mouse_button & KTROBO_MOUSESTATE_L_DOWN) {
			mouse_l = true;
		}
			if (mouse_button &KTROBO_MOUSESTATE_L_DOWN) {
				GetCursorPos(&mouse_pos);
				ScreenToClient(h,&mouse_pos);
				xMousePos = mouse_pos.x;
				yMousePos = mouse_pos.y;

				// �~�ɓ����Ă���Ƃ��͐F������
				// �O�p�`�ɓ����Ă���Ƃ����F������
				//xMousePos = mouse_xy.x;
				//yMousePos = mouse_xy.y;
			if (mouse_button & KTROBO_MOUSESTATE_L_DOWN) {
				if (paint->isInCircleColorPen(xMousePos, yMousePos)) {
					hdc = GetDC(h);
					COLORREF c = GetPixel(hdc,xMousePos,yMousePos);
					paint->setNowGColor(c);
					paint->setNowColor(c);
					paint->setGColorTheta(xMousePos,yMousePos);
					redraw = true;
					ReleaseDC(h,hdc);
				}

				if (paint->isInTriangleColorPen(xMousePos, yMousePos)) {
					hdc = GetDC(h);
					COLORREF c = GetPixel(hdc,xMousePos,yMousePos);
					paint->setNowColor(c);
					redraw = true;
				ReleaseDC(h,hdc);

				}
				if (redraw) {
					RECT r;
					r.left = 0;
					r.right = 220;
					r.top = 50;
					r.bottom = 300;
					InvalidateRect(h,&r,false);
				}
			}
			}
			break;
	case WM_PAINT:
		if (hdc = BeginPaint(h, &psPaint)) 
		{
			paint->paint();
		    BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom, paint->getHdcMem(), 0, 0, SRCCOPY);
			
			
			EndPaint(h, &psPaint);
		}
	break;

	case WM_COMMAND:

		if (HIWORD(w) == BN_CLICKED) {
			switch(LOWORD(w)) {
			case 1:
				paint->setCursorToEraser();
				break;
			case 2:
				paint->setCursorToNuri();
				break;
			case 3:
				paint->setCursorToPen();
				break;
			case 4:
				paint->setCursorToPencil();
				break;



			}
		}
		break;
	}
	return DefWindowProc(h,i,w,l);
}
LRESULT CALLBACK LoadSaveWindowProc(HWND h, UINT i, WPARAM w, LPARAM l) {
	return DefWindowProc(h,i,w,l);
}