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
	wc.lpfnWndProc = DefWindowProc;//LoadSaveWindowProc ; // プロシージャ名
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon  = NULL;
    wc.hCursor  = NULL;
    wc.hbrBackground= reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)); //グレーにする
    wc.lpszMenuName = NULL;  // 未サポート
    wc.lpszClassName=(LPCSTR)name;
	return RegisterClass(&wc);



}

void KTPaint::createKoWindow(HWND p_window) {
	WNDCLASS wc;
	wc.style  = CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc = ColorPenWindowProc ; // プロシージャ名
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon  = NULL;
    wc.hCursor  = NULL;
    wc.hbrBackground= reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)); //グレーにする
    wc.lpszMenuName = NULL;  // 未サポート
    wc.lpszClassName=(LPCSTR) "colorpen";
	bool t = RegisterClass(&wc);
	colorpen_window = CreateWindow("colorpen", "colorpen", WS_OVERLAPPED,
		1200, 0, 200,800,/*CW_USEDEFAULT, CW_USEDEFAULT,*/ NULL, NULL, hInst, NULL);
	DWORD tt = GetLastError();

	if (!t || !colorpen_window)
	{
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "error in create kowindowcolorpen");
	}

	wc.style  = CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc = LoadSaveWindowProc ; // プロシージャ名
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon  = NULL;
    wc.hCursor  = NULL;
    wc.hbrBackground= reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)); //グレーにする
    wc.lpszMenuName = NULL;  // 未サポート
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


#define KTROBO_MOUSESTATE_L_DOWN 0x01
#define KTROBO_MOUSESTATE_L_UP 0x02
#define KTROBO_MOUSESTATE_R_DOWN 0x04
#define KTROBO_MOUSESTATE_R_UP 0x08

extern KTPaint* paint;
LRESULT CALLBACK ColorPenWindowProc(HWND h, UINT i, WPARAM w, LPARAM l) {
	HDC hdc;
	PAINTSTRUCT psPaint;
	POINT center;
	POINT mouse_xy;
	ULONG mouse_button;
	MYTRIANGLEPOINT points[3];

	switch(i) {
	case WM_INPUT:
		{
		 UINT dwSize;
      
		GetRawInputData((HRAWINPUT)l, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
			 unsigned char* lpb = new unsigned char[dwSize];
			 if (lpb == NULL)
			{
				// エラー
				break;
			}

		if (GetRawInputData((HRAWINPUT)l, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
		{
				// エラー
				delete[] lpb;
				break;
		}

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEMOUSE) 
        {
				mouse_xy.x = raw->data.mouse.lLastX;
				mouse_xy.y = raw->data.mouse.lLastY;
				mouse_button = raw->data.mouse.ulButtons;
			
			}

			delete[] lpb; 
			}

			if (mouse_button &KTROBO_MOUSESTATE_L_DOWN) {
			}
			break;

	case WM_PAINT:
		if (hdc = BeginPaint(h, &psPaint)) 
		{
			center.x = 100;
			center.y = 200;
			paint->getGUI()->drawGradiationCircleToWindow(hdc,center,KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT);
			points[0].b = paint->now_gcolor_b;
			points[0].g = paint->now_gcolor_g;
			points[0].r = paint->now_gcolor_r;
			points[1].b = 0;
			points[1].g = 0;
			points[1].r = 0;
			points[2].b = 0xFF;
			points[2].g = 0xFF;
			points[2].r = 0xFF;
			points[0].x = center.x + KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT * 0.7 * cos(paint->gradiation_circle_theta);
			points[0].y = center.y + KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT * 0.7 * sin(paint->gradiation_circle_theta);
			points[1].x = center.x + KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT * 0.7 * cos(paint->gradiation_circle_theta+2*3.14/3);
			points[1].y = center.y + KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT * 0.7 * sin(paint->gradiation_circle_theta+2*3.14/3);
			points[2].x = center.x + KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT * 0.7 * cos(paint->gradiation_circle_theta+2*3.14*2/3);
			points[2].y = center.y + KTPAINT_GRADIATION_CIRCLE_RADIUS_DEFAULT * 0.7 * sin(paint->gradiation_circle_theta+2*3.14*2/3);
			paint->getGUI()->drawGradiationTriangleToWindow(hdc,points);
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