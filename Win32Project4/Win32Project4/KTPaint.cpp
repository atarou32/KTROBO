#include "KTPaint.h"
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "Resource.h"
#include "../../../../../ResEditProjects/ForKTPaint/resource.h"
#include "KTRoboGameError.h"
#include "KTRoboCS.h"




KTPaint::KTPaint(HINSTANCE hins)
{
	hInst = hins;
}


KTPaint::~KTPaint(void)
{
	Release();
}

void KTPaint::setCursorToPencil() {
	static HCURSOR hCursor=NULL;
	if (!hCursor) {
		hCursor = LoadCursor(hInst, (LPCSTR)IDI_CURSOR_PENCIL);
	}
	
	SetCursor(hCursor);
}

void KTPaint::setCursorToPen() {

	static HCURSOR hCursor=NULL;
	if (!hCursor) {
		hCursor = LoadCursor(hInst, (LPCSTR)IDI_CURSOR_PEN);
	}
	
	SetCursor(hCursor);

}

void KTPaint::setCursorToNuri() {
	static HCURSOR hCursor=NULL;
	if (!hCursor) {
		hCursor = LoadCursor(hInst, (LPCSTR)IDI_CURSOR_NURI);
	}
	
	SetCursor(hCursor);

	
}


void KTPaint::setCursorToEraser() {

	static HCURSOR hCursor=NULL;
	if (!hCursor) {
		hCursor = LoadCursor(hInst, (LPCSTR)IDI_CURSOR_ERASER);
	}
	
	SetCursor(hCursor);

}

void KTPaint::Init(HWND hwnd) {
	KTROBO::CS::instance()->Init();
	g = new KTROBO::Graphics();
	g->Init(hwnd);
	KTROBO::Graphics::InitMSS(g);





	parent_window = hwnd;
	createKoWindow(hwnd);
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



LRESULT CALLBACK ColorPenWindowProc(HWND h, UINT i, WPARAM w, LPARAM l) {
	return DefWindowProc(h,i,w,l);
}
LRESULT CALLBACK LoadSaveWindowProc(HWND h, UINT i, WPARAM w, LPARAM l) {
	return DefWindowProc(h,i,w,l);
}