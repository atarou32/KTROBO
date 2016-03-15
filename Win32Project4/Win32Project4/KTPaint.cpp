#include "KTPaint.h"
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "Resource.h"
#include "../../../../../ResEditProjects/ForKTPaint/resource.h"
#include "KTRoboGameError.h"
#include "KTRoboCS.h"
#include "KTRoboGraphics.h"
#include "CommCtrl.h"

LRESULT CALLBACK MessageWindowProc(HWND h, UINT i, WPARAM w, LPARAM l) {
	PAINTSTRUCT psPaint;
	HDC hdc;
	switch(i) {
	case WM_PAINT:
		if (hdc = BeginPaint(h, &psPaint)) 
		{
			TextOut(hdc , 0 , 0 , "計算中でございます・・" , lstrlen("計算中でございます・・"));
			//paint->paint();
		    //BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom, paint->getHdcMem(), 0, 0, SRCCOPY);
			
						EndPaint(h, &psPaint);
		}
	
		break;
	}
	return DefWindowProc(h,i,w,l);
}


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

	
	now_pen_index = 0;// = pens[0];
	is_activate = true;

	now_sheet = 0;
	root_sheet = 0;
	is_render_next_sheet=false;
	is_render_before_sheet=false;

	now_sheet = new KTPaintSheetList();
	now_sheet->now_sheet = new KTPaintSheet();
	root_sheet = now_sheet;
	sheets.push_back(now_sheet);
	now_count = 0;

	/*
	for (int i=0;i<3000;i++) {
	KTPaintSheetList* lis = new KTPaintSheetList();
	lis->now_sheet = new KTPaintSheet();
	sheets.push_back(lis);
	}
	*/
	now_penkyokuline_start = 0;
	temp_pressure = 0;
	is_render_pencil_line = true;
	is_mode_dougasaisei = false;
	is_mode_pausedougabyouga = true;
	message_window = 0;
}


KTPaint::~KTPaint(void)
{
	Release();
}

void KTPaint::setCursorNow() {
	SetCursor(temp_cursor);
}

void KTPaint::setCursorToHeipen() {
	static HCURSOR hCursor = NULL;
	if (!hCursor) {
		hCursor = LoadCursor(hInst, (LPCSTR)IDI_CURSOR1);
	}
	
	temp_cursor = hCursor;
	now_paint_id = KTPAINT_HEIPEN_ID;
	SetCursor(hCursor);
	this->now_penkyokuline_start = now_sheet->now_sheet->getHeiKyokuPLineMax();
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
	renderlineToTex();
	ShowWindow(message_window, SW_SHOW);
	UpdateWindow(message_window);

	nuridayo.koutenShori(now_sheet->now_sheet->getKyokuPLines(), 0, now_sheet->now_sheet->getKyokuPLineMax(), now_sheet->now_sheet->getPline());
	now_sheet->now_sheet->simulationStepStart(g->getScreenWidth(),g->getScreenHeight(),&nuridayo);
	now_sheet->now_sheet->copyto(&this->sheet_for_nuri);
	sheet_for_nuri.simulationBubbleStep(g->getScreenWidth(),g->getScreenHeight(), &nuridayo);
	//sheet_for_nuri.calcHeiryouiki(&nuridayo,0xFFFF0000);
	//Sleep(10000);
	ShowWindow(message_window, SW_HIDE);
	UpdateWindow(message_window);
	//now_sheet->now_sheet->calcHeiryouiki(&nuridayo,0xFFFFF000);
	
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

void KTPaint::Init(HWND hwnd,HINSTANCE hInst, int nCmdShow) {
	KTROBO::CS::instance()->Init();
	g = new KTROBO::Graphics();
	g->Init(hwnd);
	KTROBO::Graphics::InitMSS(g);

	loader.init(g);
	parent_window = hwnd;
	createKoWindow(hwnd);
	setCursorToPen();

	tex_class = loader.makeClass(g->getScreenWidth(),g->getScreenHeight());
	tex_class2 = loader.makeClass(g->getScreenWidth(),g->getScreenHeight());
	tex_class_back_buffer=  loader.makeClass(g->getScreenWidth(),g->getScreenHeight());
	render_tex_class = tex_class;
	back_tex_class = tex_class2;
	exam_class = loader.loadClass("resrc/gui.png");
	
	if (!douga.Init5(hwnd,loadsave_window, g->getScreenWidth(),g->getScreenHeight(),nCmdShow, g)) {
		douga.Del();
	}
	
	updateDispLineNum();
	//douga.Init3(hwnd,nCmdShow);
}

void KTPaint::clearSheetTransInfoNado() {
	now_sheet->transx = 0;
	now_sheet->transy = 0;
	now_sheet->zoom = 1;

	KTROBO::Graphics::setPenInfo(g,g->getScreenWidth(), g->getScreenHeight(), now_sheet->transx,now_sheet->transy,now_sheet->zoom,pens);
	
}

void KTPaint::renderlineToTex() {

	float clearColor[4] = {
		1.0f,0.6f,0.0f,1.0f};
	KTROBO::CS::instance()->enter(CS_DEVICECON_CS, "enter");
	g->getDeviceContext()->ClearRenderTargetView(render_tex_class->target_view,clearColor);
//	g->getDeviceContext()->ClearDepthStencilView(KTROBO::Graphics::pDepthStencilView,  D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f, 0 );
	ID3D11ShaderResourceView* null_view = NULL;
	g->getDeviceContext()->PSSetShaderResources(0,1,&null_view);
	g->getDeviceContext()->OMSetRenderTargets(1,&render_tex_class->target_view,NULL);//KTROBO::Graphics::pDepthStencilView);
	D3D11_VIEWPORT viewport;
	viewport.MinDepth=0.0f;
	viewport.MaxDepth=1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = g->getScreenWidth();
	viewport.Height = g->getScreenHeight();
	g->getDeviceContext()->RSSetViewports(1,&viewport);
	short ttransx = getTransX();
	short ttransy = getTransY();
	float zzoom = getZoom();

	this->clearSheetTransInfoNado();
	KTROBO::Graphics::drawTex(g,back_tex_class->width,back_tex_class->height,back_tex_class->view,now_sheet->transx,now_sheet->transy,now_sheet->zoom,pens);
//	KTROBO::Graphics::drawTex(g,exam_class->width, exam_class->height, exam_class->view,now_sheet->transx,now_sheet->transy,now_sheet->zoom,pens);
	
	int hei_max = now_sheet->now_sheet->getHeiMax();
	for (int i=0;i<hei_max;i++) {
		if (now_sheet->now_sheet->getHei()[i].is_use_pline) {
			KTROBO::Graphics::drawHeiryouiki(g,&now_sheet->now_sheet->getHei()[i], now_sheet->now_sheet->getHeiPart(), 
		now_sheet->now_sheet->getPline(),now_sheet->now_sheet->getHeiDaen());
		} else {
	KTROBO::Graphics::drawHeiryouiki(g,&now_sheet->now_sheet->getHei()[i], now_sheet->now_sheet->getHeiPart(), 
		now_sheet->now_sheet->getHeiPLine(),now_sheet->now_sheet->getHeiDaen());
	///KTROBO::Graphics::drawDaen(g,0xFFFFFFFF,center,200,250,12);
		}
	}

	
	
	KTROBO::Graphics::drawPenSpecial(g, this->now_sheet->now_sheet->getPline(),now_sheet->now_sheet->getPlineMax());

	g->getSwapChain()->Present(NULL,NULL);
	ID3D11RenderTargetView* gg = g->getRenderTargetView();
	g->getDeviceContext()->OMSetRenderTargets(1, &gg,NULL);
	g->getDeviceContext()->RSSetViewports(1,g->getViewPort());
	//now_sheet->now_sheet->clearP();
	MyTextureLoader::MY_TEXTURE_CLASS* temp;
	temp = render_tex_class;
	render_tex_class = back_tex_class;
	back_tex_class = temp;
	KTROBO::CS::instance()->leave(CS_DEVICECON_CS,"leave");
	setTransX(ttransx);
	setTransY(ttransy);
	setZoom(zzoom);

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
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject( NULL_BRUSH));//reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)); //グレーにする
    wc.lpszMenuName = NULL;  // 未サポート
    wc.lpszClassName=(LPCSTR)name;
	return RegisterClass(&wc);



}

void KTPaint::createKoWindow(HWND p_window) {
	WNDCLASS wc;
	wc.style  = 0;// CS_HREDRAW|CS_VREDRAW;
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
		1200, 0, 220,800,/*CW_USEDEFAULT, CW_USEDEFAULT,*/ NULL, NULL, hInst, NULL);
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





	wc.style  = CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc = MessageWindowProc ; // プロシージャ名
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon  = NULL;
    wc.hCursor  = NULL;
    wc.hbrBackground= reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)); //グレーにする
    wc.lpszMenuName = NULL;  // 未サポート
    wc.lpszClassName=(LPCSTR) "message";
	t = RegisterClass(&wc);

	message_window = CreateWindow("message", "message",  WS_OVERLAPPED,
		200+g->getScreenWidth()/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU/2-110, 
		g->getScreenHeight()/2/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU-25, 220,50,
		/*CW_USEDEFAULT, CW_USEDEFAULT,*/ NULL, NULL, hInst, NULL);

	if (!t || !message_window)
	{
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "error in create kowindow");
	}

	
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

	CreateWindow(TEXT("BUTTON"), "HEIPENBUT",
                 WS_CHILD|WS_VISIBLE|BS_BITMAP,
                 120,0,30,30,
                 colorpen_window,
                 (HMENU)5,
                 hInst,
                 NULL);
    hBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP5));
	SendDlgItemMessage(colorpen_window,5,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hBitmap);

	HWND enpi = CreateWindow(TEXT("BUTTON"), "鉛筆線描画",
                 WS_CHILD|WS_VISIBLE|BS_CHECKBOX,
                 0,500,30*4,30,
                 colorpen_window,
                 (HMENU)6,
                 hInst,
                 NULL);
    
	SendMessage(enpi, BM_SETCHECK , BST_CHECKED , 0);
	is_render_pencil_line = true;

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
    hdc = GetDC(colorpen_window);                      	// ウインドウのDCを取得
    GetClientRect(colorpen_window, &rc);  	// デスクトップのサイズを取得
    hBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
    hdcMem = CreateCompatibleDC(NULL);		// カレントスクリーン互換
    SelectObject(hdcMem, hBitmap);		// MDCにビットマップを割り付け
	ReleaseDC(colorpen_window, hdc);
	


	InvalidateRect(colorpen_window, NULL,false);

	g->setPenInfo(g,g->getScreenWidth(),g->getScreenHeight(),now_sheet->transx,now_sheet->transy,now_sheet->zoom,pens);
	nuridayo.koutenShori(now_sheet->now_sheet->getKyokuPLines(),now_sheet->now_sheet->getKyokuPLineMax(), now_sheet->now_sheet->getKyokuPLineMax(),now_sheet->now_sheet->getPline());

	now_sheet->now_sheet->simulationStepStart(g->getScreenWidth(), g->getScreenHeight(), &nuridayo);



}


void KTPaint::writeWithPen(POINT mpo, POINT po, UINT pressure_old, UINT pressure_new,bool reset) {

	static POINT mpo_c;
	static POINT po_c;
	static int count=0;
	if (reset || !is_activate) {
		if (count >0) {
		count=0;
		}
		return;
	}


	float w = g->getScreenWidth()/2;///KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	float h = g->getScreenHeight()/2;///KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	float zoom = now_sheet->zoom;
	short transx  =now_sheet->transx;
	short transy = now_sheet->transy;




	if (count > 0) {
//	mpo_c.x = (mpo.x - w)/zoom-transx+w;
//	mpo_c.y = (mpo.y - h)/zoom-transy+h;
		po_c.x = (po.x*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU - w)/zoom-transx+w;
		po_c.y = (po.y*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU - h)/zoom-transy+h;
		count++;
	} else {
		mpo_c.x = (mpo.x*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU - w)/zoom-transx+w;
		mpo_c.y = (mpo.y*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU - h)/zoom-transy+h;
		po_c.x = (po.x*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU - w)/zoom-transx+w;
		po_c.y = (po.y*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU - h)/zoom-transy+h;
		
	}



	unsigned char width = pens[now_pen_index].getWidthIndexFromPressure(pressure_old);
	unsigned char nwidth = pens[now_pen_index].getWidthIndexFromPressure(pressure_new);

	if (mpo_c.x >=0 && mpo_c.x <= g->getScreenWidth()) {
		if (po_c.x >= 0 && po_c.x <= g->getScreenWidth()) {
			if (mpo_c.y >= 0 && mpo_c.y <= g->getScreenHeight()) {
				if (po_c.y >= 0 && po_c.y <= g->getScreenHeight()) {
					

						if (this->now_paint_id == KTPAINT_PEN_ID) {
							if (count > 1) {
							now_sheet->now_sheet->setPline(mpo_c,po_c,width,nwidth,now_pen_index);
								mpo_c = po_c;
								count = 1;
							} else {
								count++;
								return;
							}
						} else if(this->now_paint_id == KTPAINT_HEIPEN_ID) {
							if (count > 5) {
								now_sheet->now_sheet->setHeiPline(mpo_c,po_c,width,nwidth,now_pen_index);
								mpo_c = po_c;
								count = 1;
							} else {
								count++;
								return;
							}
						} else if(this->now_paint_id == KTPAINT_PENCIL_ID) {
							if (count > 2) {
								now_sheet->now_sheet->setEline(mpo_c,po_c,0xFF, 0);
								mpo_c = po_c;
								count=1;
							} else {
								count++;
								return;
							}
						}
				}
			}
		}
	}

	if (count >0) {
		count =0;
	}
	temp_pressure = pressure_new;
	updateDispLineNum();
}

void KTPaint::render() {
	
	float clearColor[4] = {
		1.0f,1.0f,1.0f,1.0f};
	float clearColor2[4] = {
		0.9f,0.9f,0.9f,1.0f};

	KTROBO::CS::instance()->enter(CS_DEVICECON_CS, "enter");
	


	KTROBO::Graphics::setPenInfo(g,g->getScreenWidth(),g->getScreenHeight(),now_sheet->transx*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU, now_sheet->transy*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU,now_sheet->zoom*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU,pens);
	D3D11_VIEWPORT viewport;
	viewport.MinDepth=0.0f;
	viewport.MaxDepth=1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = g->getScreenWidth();//*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	viewport.Height = g->getScreenHeight();//*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	//Sleep(5000);
//	g->getDeviceContext()->RSSetViewports(1,&viewport);
	ID3D11RenderTargetView* vv = g->getRenderTargetView();
	//	g->getDeviceContext()->OMSetRenderTargets(1,&vv/*&tex_class_back_buffer->target_view*/, KTROBO::Graphics::pDepthStencilView);

	if (!is_mode_pausedougabyouga) {
	g->getDeviceContext()->ClearRenderTargetView(g->getRenderTargetView(),clearColor2);
//	g->getDeviceContext()->ClearDepthStencilView(KTROBO::Graphics::pDepthStencilView,
//		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
 //       1.0f,
  //      0 );

		KTROBO::Graphics::drawTex(g,back_tex_class->width, back_tex_class->height,back_tex_class->view,now_sheet->transx,now_sheet->transy,now_sheet->zoom,pens);
	} else {
			g->getDeviceContext()->ClearRenderTargetView(g->getRenderTargetView(),clearColor2);
	//	g->getDeviceContext()->ClearDepthStencilView(KTROBO::Graphics::pDepthStencilView,
	//		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
	 //       1.0f,
	  //      0 );

			/*
			PAINTSTRUCT psPaint;
			HDC hdc = BeginPaint(parent_window, &psPaint);
			BitBlt(hdc,0,0,douga.getBitmapClass()->width,douga.getBitmapClass()->height,douga.hdcMem,0,0,SRCCOPY);
			EndPaint(parent_window, &psPaint);
			*/


			KTROBO::Graphics::drawTex(g,back_tex_class->width, back_tex_class->height,back_tex_class->view,now_sheet->transx,now_sheet->transy,now_sheet->zoom,pens);

			if (douga.getInited()) {
				KTROBO::Graphics::drawTex(g,douga.getBitmapClass()->width, douga.getBitmapClass()->height,douga.getBitmapClass()->view,now_sheet->transx,now_sheet->transy,now_sheet->zoom,pens);
			}
	
	}

	if (is_render_pencil_line || (now_paint_id == KTPAINT_PENCIL_ID)) {
		KTROBO::Graphics::setPenInfo(g,g->getScreenWidth(), g->getScreenHeight(), 
				now_sheet->transx,now_sheet->transy,now_sheet->zoom,pens);
	KTROBO::Graphics::drawPencil(g,now_sheet->now_sheet->getEline(),now_sheet->now_sheet->getElineMax(),
		now_color_r,now_color_g,now_color_b,0xFF);
	}

	




	if (this->is_render_next_sheet) {
		if (now_sheet->next_sheet) {
			KTROBO::Graphics::setPenInfo(g,g->getScreenWidth(), g->getScreenHeight(), 
				now_sheet->next_sheet->transx,now_sheet->next_sheet->transy,now_sheet->next_sheet->zoom,pens);
			KTROBO::Graphics::drawPen(g, now_sheet->next_sheet->now_sheet->getPline(),now_sheet->next_sheet->now_sheet->getPlineMax());
		}
	}

	if (this->is_render_before_sheet) {
		if (now_sheet->mae_sheet) {
			KTROBO::Graphics::setPenInfo(g,g->getScreenWidth(), g->getScreenHeight(), 
				now_sheet->mae_sheet->transx,now_sheet->mae_sheet->transy,now_sheet->mae_sheet->zoom,pens);
		
			KTROBO::Graphics::drawPen(g, now_sheet->mae_sheet->now_sheet->getPline(),now_sheet->mae_sheet->now_sheet->getPlineMax());
		}
	}
	KTROBO::Graphics::setPenInfo(g,g->getScreenWidth(), g->getScreenHeight(), 
				now_sheet->transx,now_sheet->transy,now_sheet->zoom,pens);
	

//	g->getDeviceContext()->ClearRenderTargetView(tex_class_back_buffer->target_view, clearColor2);
	//g->getDeviceContext()->ClearDepthStencilView(Mesh::pDepthStencilView,  D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f, 0 );

	
	if (now_paint_id == KTPAINT_HEIPEN_ID) {
		if (now_penkyokuline_start) {
			int temp_now = now_penkyokuline_start;
			if (now_sheet->now_sheet->getHeiKyokuPLine()[now_penkyokuline_start].start_index ==0) {
				temp_now = now_penkyokuline_start-1;

				KTROBO::Graphics::drawPenSpecial(g, 
				&now_sheet->now_sheet->getHeiPLine()[now_sheet->now_sheet->getHeiKyokuPLine()[temp_now].end_index+1],
				now_sheet->now_sheet->getHeiPLineMax()-now_sheet->now_sheet->getHeiKyokuPLine()[temp_now].end_index-1);
		

			} else {
			KTROBO::Graphics::drawPenSpecial(g, 
			&now_sheet->now_sheet->getHeiPLine()[now_sheet->now_sheet->getHeiKyokuPLine()[temp_now].start_index],
			now_sheet->now_sheet->getHeiPLineMax()-now_sheet->now_sheet->getHeiKyokuPLine()[temp_now].start_index);
			}
		} else {

			KTROBO::Graphics::drawPenSpecial(g, 
			&now_sheet->now_sheet->getHeiPLine()[now_sheet->now_sheet->getHeiKyokuPLine()[now_penkyokuline_start].start_index],
			now_sheet->now_sheet->getHeiPLineMax()-now_sheet->now_sheet->getHeiKyokuPLine()[now_penkyokuline_start].start_index);
	
			/*
				KTROBO::Graphics::drawPenSpecialSitei(g, now_sheet->now_sheet->getHeiKyokuPLine(), this->now_penkyokuline_start,
			now_sheet->now_sheet->getHeiKyokuPLineMax(),now_sheet->now_sheet->getHeiPLine(),now_sheet->now_sheet->getHeiPLineMax());
			*/
		}
	}
	

	KTROBO::Graphics::drawPenSpecial(g, now_sheet->now_sheet->getPline(),now_sheet->now_sheet->getPlineMax());
	ID3D11RenderTargetView* tt = g->getRenderTargetView();
	MYVECTOR3 center;
	center.float3.x = 400;
	center.float3.y = 500;
	center.float3.z = 0;
	nuridayo.printKouten(g, now_sheet->now_sheet->getHeiPLine());
	



	KTPAINT_bubble* bubbles = nuridayo.getBubble();
	int bubble_max = nuridayo.getBubbleMax();

	if (now_paint_id == KTPAINT_NURI_ID) {
		
		for (int i=0;i<bubble_max;i++) {
			MYVECTOR3 center(bubbles[i].x,bubbles[i].y,0);
			float radius = bubbles[i].radius;
			LONGLONG colors[] = {
				0xFFFFFF00,
				0xFFFF00FF,
				0xFFFF0000,
				0xFF00FF00,
				0xFF0000FF,
				0xFF00FFFF,
				0xFF000000,
				0xFFFFFFFF,
				0xFF33FF33,
				0xFFFF33FF,
				0xFFFFFF33,
				0xFF33FFFF,
				0xFF3333FF,
				0xFF333333,
				0xFF221100,
				0xFFAAAAAA
			};
			if (bubbles[i].status == KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_ONLINE) {
				KTROBO::Graphics::drawDaen(g,colors[( 15)% 16],center,radius,radius,0);
			} else {
			KTROBO::Graphics::drawDaen(g,colors[(/*bubbles[i].label_index_tate*/ bubbles[i].hei_index )% 15],center,radius,radius,0);
			}		
		}
	}




	g->getSwapChain()->Present(0,NULL);
	KTROBO::CS::instance()->leave(CS_DEVICECON_CS, "leave");
	return;



	

	

//	g->getDeviceContext()->OMSetRenderTargets(1,&tt,NULL);
//	viewport.MinDepth=0.0f;
//	viewport.MaxDepth=1.0f;
//	viewport.TopLeftX = 0;
//	viewport.TopLeftY = 0;
//	viewport.Width = g->getScreenWidth();//*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
//	viewport.Height = g->getScreenHeight();//*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
//	g->getDeviceContext()->RSSetViewports(1,&viewport);
//	KTROBO::Graphics::drawTex(g,g->getScreenWidth(),g->getScreenHeight(),/*tex_class_back_buffer->width,tex_class_back_buffer->height,*/tex_class_back_buffer->view,transx,transy,zoom,pens);
//	g->getSwapChain()->Present(1,NULL);
//	KTROBO::CS::instance()->leave(CS_DEVICECON_CS,"leave");
}
void KTPaint::Release()  {
	KTROBO::Graphics::Del();
	if (g) {
		g->Release();
		delete g;
		g = 0;
	}
	KTROBO::CS::instance()->Del();
	loader.del();

	list<KTPaintSheetList*>::iterator it = sheets.begin();
	while( it != sheets.end()) {
		KTPaintSheetList* pp = *it;
		if (pp) {
		if (pp->now_sheet) {
			delete pp->now_sheet;
			pp->now_sheet = 0;
		}
		
		delete pp;
		pp = 0;
		}

		it++;
	}
	sheets.clear();
	douga.Del();
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
  HDC hdc,          // デバイスコンテキストハンドル
  int nXStart,      // 開始点の x 座標
  int nYStart,      // 開始点の y 座標
  COLORREF crColor, // 色
  UINT fuFillType   // 種類
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
	// 現在の色を塗る
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
	int checkbox =0;
	switch(i) {
	
	case WM_ERASEBKGND:
		return 0;
	case WM_PAINT:
		if (hdc = BeginPaint(h, &psPaint)) 
		{
			paint->paint();
		    BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom, paint->getHdcMem(), 0, 0, SRCCOPY);
			
						EndPaint(h, &psPaint);
		}
	break;

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

				// 円に入っているときは色を決定
				// 三角形に入っているときも色を決定
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

			case 5:
				paint->setCursorToHeipen();
				break;
			case 6:
				 checkbox = SendMessage( GetDlgItem(h, 6), BM_GETCHECK, 0, 0 );
				 if (checkbox) {
					 	 paint->setIsRenderPencilLine(false);
					 SendMessage(GetDlgItem(h,6) , BM_SETCHECK , BST_UNCHECKED , 0);
				 } else {
				
					 paint->setIsRenderPencilLine(true);
		 		 	SendMessage(GetDlgItem(h,6) , BM_SETCHECK , BST_CHECKED , 0);
				 }
				 break;
			}
		}
		break;
	}
	return DefWindowProc(h,i,w,l);
}

LPCSTR strItem[] = {
	"text10000.000000"
};

WNDPROC OrgTrackProc; //元々のトラックバーのプロシージャ


void KTPaint::updateNowSheetPos() {
	KTPaintSheetList* temp = root_sheet;
	int index = SendMessage(combo,CB_GETCURSEL,0,0);
	for (int i=0;i<index;i++) {
		if (temp->next_sheet) {
		temp = temp->next_sheet;
		} else {
			break;
		}
	}
	now_sheet = temp;
}


LRESULT CALLBACK LoadSaveWindowProc(HWND h, UINT i, WPARAM w, LPARAM l) {
static HWND combo;
static HWND makesheet_button;
static HWND sheet_index_label;
static HWND sheet_linenum_label;
static HWND sheet_frame;
static HWND hTrack;
static HWND hTrack2;
static HWND dougad;
static char labeltext[1024];
static PAINTSTRUCT ps;
static HDC hdc;
static HBITMAP hBitmap;
char strtext[1024];
	switch (i) {
	case WM_PAINT:
		hdc = BeginPaint(h, &ps);
		// CreateDIBSection() によって作成されたビットマップを描画
		  BitBlt(hdc, 0, 0, paint->douga.bmi.bmiHeader.biWidth, paint->douga.bmi.bmiHeader.biHeight, paint->douga.hdcMem, 0, 0, SRCCOPY );
		  EndPaint(h, &ps);
		  break;
	case WM_CREATE:
		combo = CreateWindow(
			TEXT("COMBOBOX") , NULL , 
			WS_CHILD | WS_VISIBLE | CBS_SORT | CBS_DROPDOWNLIST | WS_VSCROLL, 
			0 , 0 , 300 , 100 , h , (HMENU)1 ,
			paint->getHInst(), NULL
		);
		paint->setCombo(combo);

		makesheet_button = CreateWindow(
			TEXT("BUTTON"), TEXT("MKSHEET"), WS_CHILD | WS_VISIBLE|BS_DEFPUSHBUTTON,100,300,80,50,h,(HMENU)2,paint->getHInst(),NULL);
		sheet_index_label = CreateWindow(
			TEXT("STATIC") , NULL , 
			WS_CHILD | WS_VISIBLE , 
			0 , 50 , 200 , 250 , h , (HMENU)3 ,
			paint->getHInst() , NULL
		);
		paint->setSheetIndexLabel(sheet_index_label);
		/*
		sheet_linenum_label = 
			CreateWindow(
			TEXT("STATIC") , NULL , 
			WS_CHILD | WS_VISIBLE , 
			0 , 350 , 200 , 250 , h , (HMENU)4 ,
			paint->getHInst() , NULL
		);*/
		SendMessage(combo , CB_SETCURSEL , 0 , 0);
		

		   
		sheet_frame =CreateWindow(
                TEXT("EDIT"),             //ウィンドウクラス名
                NULL,                   //キャプション
                WS_CHILD | WS_VISIBLE | WS_BORDER //|
                    /*    WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL |*/
                     //   ES_LEFT | ES_MULTILINE,         //スタイル指定
                ,0,500,                  //位置 ｘ、ｙ
                100,20,                //幅、高さ
                h,                   //親ウィンドウ
                (HMENU)10,               // メニューハンドルまたは子ウィンドウID
                paint->getHInst(),                  //インスタンスハンドル
                NULL);                  //その他の作成データ


		CreateWindow(
			TEXT("BUTTON"), TEXT("SETFRAME"), WS_CHILD | WS_VISIBLE|BS_DEFPUSHBUTTON,100,500,80,50,h,(HMENU)11,paint->getHInst(),NULL);
	
		dougad = CreateWindow(TEXT("BUTTON"), "動画静止時描画",
                 WS_CHILD|WS_VISIBLE|BS_CHECKBOX,
                 0,700,30*4,30,
                 h,
                 (HMENU)14,
                 paint->getHInst(),
                 NULL);
    
		SendMessage(dougad, BM_SETCHECK , BST_CHECKED , 0);
	




		paint->updateDispLineNum();
		

		for (i = 0 ; i < 1 ; i++)
			SendMessage(combo , CB_INSERTSTRING , 0 , (LPARAM)strItem[i]);
		
		SendMessage(combo, CB_SETCURSEL,0,0);




		hTrack2 = CreateWindowEx(0,TRACKBAR_CLASS,//拡張ウィンドウスタイル
                //ウィンドウクラス
                "",WS_CHILD | WS_VISIBLE //|
               // TBS_AUTOTICKS | TBS_ENABLESELRANGE,//ウィンドウスタイル
                ,0, 650, 150, 20,//位置、大きさ
                h,//親ウィンドウ
                (HMENU)15,//コントロールID
                paint->getHInst(),//インスタンスハンドル
                NULL);
            //元々のプロシージャを保存してトラックバーをサブクラス化
           // OrgTrackProc = (WNDPROC)GetWindowLong(hTrack, GWL_WNDPROC);
           // SetWindowLong(hTrack, GWL_WNDPROC, (LONG)MyTrackProc);
            //トラックバーの範囲設定
            SendMessage(hTrack2, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELPARAM(0, 255));
			SendMessage(hTrack2, TBM_SETTICFREQ, 1, 0);   // 目盛りの増分
			   SendMessage(hTrack2, TBM_SETPAGESIZE, 0, 1); // クリック時の移動量
			//OrgTrackProc = (WNDPROC)GetWindowLong(hTrack, GWL_WNDPROC);
            //SetWindowLong(hTrack, GWL_WNDPROC, (LONG)MyTrackProc);
         


			   hTrack = CreateWindowEx(0,TRACKBAR_CLASS,//拡張ウィンドウスタイル
                //ウィンドウクラス
                "",WS_CHILD | WS_VISIBLE //|
               // TBS_AUTOTICKS | TBS_ENABLESELRANGE,//ウィンドウスタイル
                ,0, 550, 150, 20,//位置、大きさ
                h,//親ウィンドウ
                (HMENU)12,//コントロールID
                paint->getHInst(),//インスタンスハンドル
                NULL);
            //元々のプロシージャを保存してトラックバーをサブクラス化
           // OrgTrackProc = (WNDPROC)GetWindowLong(hTrack, GWL_WNDPROC);
           // SetWindowLong(hTrack, GWL_WNDPROC, (LONG)MyTrackProc);
            //トラックバーの範囲設定
            SendMessage(hTrack, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELPARAM(0, 200));
			SendMessage(hTrack, TBM_SETTICFREQ, 1, 0);   // 目盛りの増分
			   SendMessage(hTrack, TBM_SETPAGESIZE, 0, 1); // クリック時の移動量
			//OrgTrackProc = (WNDPROC)GetWindowLong(hTrack, GWL_WNDPROC);
            //SetWindowLong(hTrack, GWL_WNDPROC, (LONG)MyTrackProc);
        



		CreateWindow(
			TEXT("BUTTON"), TEXT("Play"), WS_CHILD | WS_VISIBLE|BS_BITMAP,0,450,40,40,h,(HMENU)20,paint->getHInst(),NULL);
		CreateWindow(
			TEXT("BUTTON"), TEXT("Pause"), WS_CHILD | WS_VISIBLE|BS_BITMAP,50,450,40,40,h,(HMENU)21,paint->getHInst(),NULL);
		CreateWindow(
			TEXT("BUTTON"), TEXT("Stop"), WS_CHILD | WS_VISIBLE|BS_BITMAP,100,450,40,40,h,(HMENU)22,paint->getHInst(),NULL);
	
		hBitmap=LoadBitmap(paint->getHInst(),LPCSTR(IDB_BITMAP6));
			SendDlgItemMessage(h,20,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hBitmap);
		hBitmap=LoadBitmap(paint->getHInst(),LPCSTR(IDB_BITMAP7));
			SendDlgItemMessage(h,21,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hBitmap);
		hBitmap=LoadBitmap(paint->getHInst(),LPCSTR(IDB_BITMAP8));
			SendDlgItemMessage(h,22,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hBitmap);

		return 0;		
		case WM_HSCROLL:
            if(hTrack == (HWND)l)
            {
                int pos = SendMessage(hTrack, TBM_GETPOS, NULL, NULL); // 現在の値の取得
				hdc = GetDC(paint->parent_window);
				memset(strtext,0,1024);
				GetWindowText(sheet_frame , strtext , 1024);
				paint->douga.setFrame(paint->parent_window,hdc,strtext,pos);
				paint->douga.transportBitmapToTextureClass(paint->getGraphics());
				paint->is_mode_dougasaisei = false;
				//paint->douga.copyBufferOfVideoFrame(paint->parent_window);
				//free(strtext);
				ReleaseDC(paint->parent_window,hdc);
            }

			 if(hTrack2 == (HWND)l)
            {
                int pos = SendMessage(hTrack2, TBM_GETPOS, NULL, NULL); // 現在の値の取得
				//hdc = GetDC(paint->parent_window);
				//memset(strtext,0,1024);
				//GetWindowText(sheet_frame , strtext , 1024);
				//paint->douga.setFrame(paint->parent_window,hdc,strtext,pos);
				paint->douga.setAlpha((unsigned char)pos);
				paint->douga.transportBitmapToTextureClass(paint->getGraphics());
				//paint->is_mode_dougasaisei = false;
				//paint->douga.copyBufferOfVideoFrame(paint->parent_window);
				//free(strtext);
				ReleaseDC(paint->parent_window,hdc);
            }
            //_stprintf_s(str,10,TEXT("%d ％"), Pos);
            //SetWindowText(GetDlgItem(hWnd, IDC_STATIC1), (LPCTSTR)str); 
            break;
		case WM_COMMAND:
		if (HIWORD(w) == CBN_SELCHANGE) {
			paint->updateNowSheetPos();
			paint->updateDispLineNum();
		}
		/*
		if (LOWORD(w) == 12) {
			int pos =  SendDlgItemMessage(hTrack, 13, TBM_GETPOS, 0, 0 );
			hdc = GetDC(paint->parent_window);
			memset(strtext,0,1024);
			GetWindowText(sheet_frame , strtext , 1024);
			paint->douga.setFrame(paint->parent_window,hdc,strtext,pos);
			paint->is_mode_dougasaisei = false;
			//paint->douga.copyBufferOfVideoFrame(paint->parent_window);
			//free(strtext);
			ReleaseDC(paint->parent_window,hdc);
		}
		*/

		if (LOWORD(w) == 14) {
			
			int checkbox = SendMessage( GetDlgItem(h, 14), BM_GETCHECK, 0, 0 );
			if (checkbox) {
				paint->setModeByougaSeisiDouga(false);
				SendMessage(GetDlgItem(h,14) , BM_SETCHECK , BST_UNCHECKED , 0);
			} else {
				
				paint->setModeByougaSeisiDouga(true);
		 		SendMessage(GetDlgItem(h,14) , BM_SETCHECK , BST_CHECKED , 0);
			}
		
		}
		if (LOWORD(w) == 11) {
			// テキスト
			hdc = GetDC(paint->parent_window);
			memset(strtext,0,1024);
			GetWindowText(sheet_frame , strtext , 1024);
			paint->douga.setFrame(paint->parent_window,hdc,strtext,0);
			paint->douga.transportBitmapToTextureClass(paint->getGraphics());
			paint->is_mode_dougasaisei = false;
			//paint->douga.copyBufferOfVideoFrame(paint->parent_window);
			//free(strtext);
			ReleaseDC(paint->parent_window,hdc);
		}
		if (HIWORD(w) == BN_CLICKED) {
			switch(LOWORD(w)) {
			
			case 2:
				paint->makeNewSheet();
				break;
			case 20:
				paint->playdouga();
				break;
			case 21:
				paint->pausedouga();
				break;
			case 22:
				paint->stopdouga();
				break;
			}
		}
		break;
	}



	return DefWindowProc(h,i,w,l);
}
void KTPaint::makeNewSheet() {


	KTPaintSheet* sheet = new KTPaintSheet();
	KTPaintSheetList* sheetlist = new KTPaintSheetList();
	sheetlist->now_sheet = sheet;
	sheetlist->mae_sheet = now_sheet;

	if (now_sheet->next_sheet) {
		sheetlist->next_sheet = now_sheet->next_sheet;
	
		sheetlist->count = now_sheet->count + 1000;
		if (sheetlist->count >= now_sheet->next_sheet->count) {
			sheetlist->count = now_sheet->count + 100;
			if (sheetlist->count >= now_sheet->next_sheet->count) {
				sheetlist->count = now_sheet->count + 10;
				if (sheetlist->count >= now_sheet->next_sheet->count) {
					sheetlist->count = now_sheet->count+1;
					if (sheetlist->count >= now_sheet->next_sheet->count) {
						sheetlist->count = now_sheet->count+1/10.0f;
						if (sheetlist->count >= now_sheet->next_sheet->count) {
						sheetlist->count = now_sheet->count+1/100.0f;
						if (sheetlist->count >= now_sheet->next_sheet->count) {
						sheetlist->count = now_sheet->count+1/1000.0f;
						if (sheetlist->count >= now_sheet->next_sheet->count) {
						sheetlist->count = now_sheet->count+1/10000.0f;
						if (sheetlist->count >= now_sheet->next_sheet->count) {
						sheetlist->count = now_sheet->count+1/100000.0f;
						if (sheetlist->count >= now_sheet->next_sheet->count) {
						sheetlist->count = now_sheet->count+1/1000000.0f;
						if (sheetlist->count >= now_sheet->next_sheet->count) {
						sheetlist->count = now_sheet->count+1/10000000.0f;
						}
						}
						}
						}
						}
						}
					}
				}
			}
		}

		now_sheet->next_sheet->mae_sheet = sheetlist;
		now_sheet->next_sheet = sheetlist;


	} else {
		now_sheet->next_sheet = sheetlist;
		sheetlist->count = now_sheet->count+10000;
	}

	
	static char str[1024];
	memset(str,0,1024);
	sprintf_s(str,1024,"text%f",sheetlist->count);
	int index =0;
	int ssize = sheets.size();
	list<KTPaintSheetList*>::iterator it = sheets.begin();
	while(it != sheets.end()) {
		if (*it == now_sheet) {
			break;
		}
		index++;
		it++;
	}

	SendMessage(combo,CB_INSERTSTRING,index+1,(LPARAM)str);
	SendMessage(combo,CB_SETCURSEL,index+1,0);
	//now_sheet->next_sheet = sheetlist;

	now_sheet = sheetlist;
	if (it != sheets.end()) {
	sheets.insert((++it),sheetlist);
	} else {
	sheets.push_back(sheetlist);
	}
	clearSheetTransInfoNado();
	float clearColor[4] = {
		1.0f,1.0f,1.0f,1.0f};

	KTROBO::CS::instance()->enter(CS_DEVICECON_CS,"enter");
	g->getDeviceContext()->ClearRenderTargetView(this->tex_class->target_view, clearColor);
	g->getDeviceContext()->ClearRenderTargetView(this->tex_class2->target_view, clearColor);
	KTROBO::CS::instance()->leave(CS_DEVICECON_CS, "leave");

	updateDispLineNum();
}


void KTPaint::updateDispLineNum() {
	char labeltext[1024];
	memset(labeltext,0,1024);
	sprintf_s(labeltext ,1024, "line数 = %d/%d\n選択項目 = %d\n筆圧=%d\nhline数=%d/%d\nぬり領域数=%d/%d\n曲line数=%d/%d\n鉛筆line数%d/%d\n\n動画時間数=%d" ,
		getNowSheetLineNum() , KTPAINT_SHEET_LINE_MAX,
				SendMessage(combo , CB_GETCURSEL , 0 , 0),
				temp_pressure,
				now_sheet->now_sheet->getHeiPLineMax(), KTPAINT_SHEET_HEILINE_MAX,
				now_sheet->now_sheet->getHeiMax(), KTPAINT_SHEET_KYOKULINE_MAX,
				now_sheet->now_sheet->getKyokuPLineMax(), KTPAINT_SHEET_KYOKULINE_MAX,
				now_sheet->now_sheet->getElineMax(), KTPAINT_SHEET_DUMMY_MAX,
				douga.getAllFrame()
			);
	SetWindowText(sheet_index_label , labeltext);

}


void KTPaint::fill(POINT po) {




	float w = g->getScreenWidth()/2;///KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	float h = g->getScreenHeight()/2;///KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	float zoom = now_sheet->zoom;
	short transx  =now_sheet->transx;
	short transy = now_sheet->transy;
	POINT po_c;



	
	po_c.x = (po.x*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU - w)/zoom-transx+w;
	po_c.y = (po.y*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU - h)/zoom-transy+h;
	
	KTPAINT_bubble* bubbles = nuridayo.getBubble();
	KTPAINT_bubblehei* bubble_heis = nuridayo.getBubbleHeis();
	int bhei_max = nuridayo.getBubbleHeiMax();
	
	// 指定されたポイントに一番近い円をまず求める
	int bwidth = g->getScreenWidth()/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU/(int)(bubbles[0].radius*2) + 1;
	int width = po_c.x / KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU / (int)(bubbles[0].radius*2);
	int height = po_c.y / KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU / (int)(bubbles[0].radius*2);

	int inde = width + height * bwidth;

	if (bubbles[inde].status != KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_ONLINE) {
		KTPAINT_bubble* bs = &bubbles[inde];
		DWORD color = (0xFF << 24) + (now_color_r << 16) + (now_color_g << 8) + now_color_b;
		POINT ppo;
		ppo = po_c;
		ShowWindow(message_window, SW_SHOW);
		UpdateWindow(message_window);
		sheet_for_nuri.calcHeiryouiki(&nuridayo,&bubble_heis[bs->hei_index],color, ppo);
		sheet_for_nuri.copyHeiTo(now_sheet->now_sheet);
		renderlineToTex();
		ShowWindow(message_window, SW_HIDE);
		UpdateWindow(message_window);
	}
	//sheet_for_nuri.calcurateHeiAndFill(

}