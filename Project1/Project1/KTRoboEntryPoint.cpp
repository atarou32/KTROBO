#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <SDKDDKVer.h>

#include <locale.h>
#include <crtdbg.h>
#include "KTRoboGame.h"
#include "KTRoboGameError.h"
#include "KTRoboLog.h"
#include "KTRoboInput.h"
#include "MyGyouretuKeisan.h"
#include "KTRoboCS.h"


HWND g_hWnd = NULL;

HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow, Input* input );
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM );
KTROBO::Game* game;
const WCHAR CLASS_NAME[] = L"KTROBO ver 0.01";
const WCHAR APP_TITLE[] = L"KTROBO ver 0.01";



void ClearMainWindow(HINSTANCE hInstance) {
	UnregisterClass(CLASS_NAME,hInstance);
}



bool AnotherInstance() {
	HANDLE mutex;
	mutex = CreateMutex(NULL, true, L"heiwadearimasuyouni");
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return  true;
	}
	return false;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{

	MyGyouretuKeisan k;

	MYMATRIX mat;
	float matf[16];
	for (int i=0;i<16;i++) {
		mat.m[i/4][i%4] = rand() %30 + 1;
		matf[i] = mat.m[i/4][i%4];
	}
	float mat_out[16];
	float edayo[16];

	k.getMatrixInverse(matf,4,mat_out);
	k.getMatrixMultiply(4,4,matf,4,4,mat_out, edayo);

	MYMATRIX inv_mat;
	for (int i=0;i<16;i++) {
		inv_mat.m[i/4][i%4] = mat_out[i];
	}
//	MyMatrixMultiply(mat,inv_mat,mat);
	MyMatrixInverse(mat,NULL,mat);






	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
#ifdef _DEBUG	
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#define D3D_DEBUG_INFO
#endif
	setlocale( LC_ALL, "Japanese" );

	if (AnotherInstance()) {
		return 0;
	}

	Input* input = new Input();
	MSG msg = {0};
	try {
	if(FAILED( InitWindow(hInstance, nCmdShow, input))){
		return 0;
	}
	} catch (KTROBO::GameError* err) {
		KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "init device failure");
		KTROBO::mylog::writelog(err->getErrorCode(),err->getMessage());
	//	MessageBoxA(g_hWnd,err->getMessage(),KTROBO::GameError::getErrorCodeString(err->getErrorCode()),MB_OK);
		delete err;
	
		goto ktrobo_error;
	} catch (...) {
		goto ktrobo_error;
	}
	
	

	game = new KTROBO::Game();
	
	try {
	
		game->Init(g_hWnd);
	} catch (KTROBO::GameError* err) {

		KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "init device failure");
		KTROBO::mylog::writelog(err->getErrorCode(),err->getMessage());
	//	MessageBoxA(g_hWnd,err->getMessage(),KTROBO::GameError::getErrorCodeString(err->getErrorCode()),MB_OK);
		delete err;
		

		goto ktrobo_error;
	} catch (...) {
		goto ktrobo_error;
	}

	// Main message loop
	//MSG msg = {0};
	try {
		
	
	while( WM_QUIT != msg.message ){
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}else{
			Sleep(1);
			InputMessageDispatcher::messageMake();
		}
	}
	}catch (KTROBO::GameError& err) {
		KTROBO::mylog::writelog(err.getErrorCode(),err.getMessage());
//		MessageBoxA(g_hWnd,err.getMessage(),KTROBO::GameError::getErrorCodeString(err.getErrorCode()),MB_OK);
	} catch (...) {
//		MessageBoxW(g_hWnd,L"unknown error",L"ERROR",MB_OK);
	}

ktrobo_error:

	ClearMainWindow(hInstance);
	if (input) {
		delete input;
		input = 0;
	}
	if (game) {
		game->Del();
		delete game;
		game = 0;
	}
	KTROBO::CS::instance()->Del();

	return 0;//(int)msg.wParam;
}

HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow, Input* input )
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= input->myWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName = CLASS_NAME;
	wcex.hIconSm		=  NULL;

	if(!RegisterClassEx(&wcex)){
		return E_FAIL;
	}
	//ウインドウのクライアント領域（=DirectXの描画領域）を指定
	RECT rc = { 0, 0, 1000, 800 };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	g_hWnd = CreateWindow( CLASS_NAME, CLASS_NAME, WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
						NULL, NULL, hInstance, NULL);
	

	if (!g_hWnd){
		return E_FAIL;
	}
	KTROBO::CS::instance()->Init();
	Input::Init(g_hWnd);
	InputMessageDispatcher::Init();
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

    return S_OK;
}


LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}


