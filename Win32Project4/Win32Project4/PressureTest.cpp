
#include "stdafx.h"

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

#include "msgpack.h"
#include "wintab.h"
#define PACKETDATA	(PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE)
#define PACKETMODE	PK_BUTTONS
#include "pktdef.h"
#include "PressureTest.h"
#include "KTPaint.h"
#pragma comment(lib,"Comctl32.lib")
#include "CommCtrl.h"
#include "KTRoboLog.h"
#include "KTRoboCS.h"
#include "KTRoboGameError.h"
#include "Utils.h"
#include "KTPaintGUI.h"

#define MAX_LOADSTRING 100

//////////////////////////////////////////////////////////////////////////////
// Global Variables:
HINSTANCE hInst;								// current instance
char szTitle[MAX_LOADSTRING];					// The title bar text
char szWindowClass[MAX_LOADSTRING];			// the main window class name

char* gpszProgramName = "KTPAINT";
static LOGCONTEXT	glogContext = {0};
KTPaint* paint=0;
//////////////////////////////////////////////////////////////////////////////
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HCTX static NEAR TabletInit(HWND hWnd);
void Cleanup( void );

//////////////////////////////////////////////////////////////////////////////
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg = {0};
	
	HACCEL hAccelTable;
	InitCommonControls();
	// Initialize global strings
//	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//	LoadString(hInstance, IDC_PRESSURETEST, szWindowClass, MAX_LOADSTRING);
	memcpy(szTitle,gpszProgramName,strlen(gpszProgramName));
	memcpy(szWindowClass,gpszProgramName,strlen(gpszProgramName));
	
	MyRegisterClass(hInstance);
	KTPaintGUI::startUp();
	KTROBO::CS::instance()->Init();
	// Perform application initialization:

	try {
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	} catch (KTROBO::GameError* err) {
		KTROBO::mylog::writelog(err->getErrorCode(), err->getMessage());
		delete err;
		return FALSE;
	}
	//hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PRESSURETEST));
	try {
	while( WM_QUIT != msg.message ){
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}else{
			if (!paint->is_mode_dougasaisei) {
			paint->render();
			}
			Sleep(1);
			
		}
	}
	} catch (KTROBO::GameError* err) {
		KTROBO::mylog::writelog(err->getErrorCode(), err->getMessage());
		delete err;
	//	return FALSE;
	}

	if (paint) {
		delete paint;
		paint = 0;
	}

	KTPaintGUI::shutDown();
	KTROBO::CS::instance()->Del();
	// Return Wintab resources.
	Cleanup();

	return (int) msg.wParam;
}


//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;// LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PRESSURETEST));
	wcex.hCursor		= NULL;//LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE + 1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_PRESSURETEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//////////////////////////////////////////////////////////////////////////////
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	if ( !LoadWintab( ) )
	{
		ShowError( "Wintab not available" );
		return FALSE;
	}

	/* check if WinTab available. */
	if (!gpWTInfoA(0, 0, NULL)) 
	{
		ShowError( "WinTab Services Not Available." );
		return FALSE;
	}

	hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE | WS_SYSMENU,
		200, 0, 1000, 800, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	paint = new KTPaint(hInstance);
	paint->Init(hWnd, hInstance, nCmdShow);
//		delete paint;
//		return FALSE;
//	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	HDC hdc;

	static HCTX hCtx = NULL;
	static POINT ptOld, ptNew;
	static UINT prsOld, prsNew;
	static RECT rcClient;
	PAINTSTRUCT psPaint;
	PACKET pkt;
	BOOL fHandled = TRUE;
	LRESULT lResult = 0L;
	static int xMousePos = 0;
	static int yMousePos = 0;
	unsigned char keys[256];
	int delta;
	float zo;
	static bool is_rdragged=false;
	static POINT dragpoint;
	POINT po;
	POINT mpo;
	switch (message)
	{
	case WM_CREATE:
		hCtx = TabletInit(hWnd);
		if (!hCtx) 
		{
			KTROBO::mylog::writelog(KTROBO::FATAL_ERROR,"Could Not Open Tablet Context.");
			SendMessage(hWnd, WM_DESTROY, 0, 0L);
		}
		break;

	case WM_RBUTTONDOWN:
		is_rdragged = true;
		dragpoint.x = xMousePos;
		dragpoint.y = yMousePos;
		break;
	case WM_RBUTTONUP:
		is_rdragged = false;
		break;
	case WM_LBUTTONUP:
		paint->endDrawLine();
		paint->deactivate();
		break;
	case WM_LBUTTONDOWN:
		paint->activate();
		paint->startDrawLine();
		break;

	case WM_MOUSEWHEEL:
		delta = GET_WHEEL_DELTA_WPARAM(wParam); // ‚±‚Ì’l‚Í•‰‚É‚È‚é‚±‚Æ‚à‚ ‚é
		zo = paint->getZoom();
		if (delta <0) {
			zo = zo / 1.10f;
		} else {
			zo = zo * 1.10f;
		}
		paint->setZoom(zo);
		break;
	case WM_KEYDOWN:
			if (GetKeyboardState(keys)) {
				for (int i=0;i<256;i++) {
					if (keys[VK_LEFT] & 0x80) {
						paint->setSheetBefore();

					} else if(keys[VK_RIGHT] & 0x80) {

						if(!paint->setSheetNext()) {
							paint->makeNewSheet();
						}

					} else if(keys[VK_UP] & 0x80) {
						if (paint->getIsRenderBeforeSheet()) {
							paint->setIsRenderBeforeSheet(false);
							paint->setSheetBefore();
						} else {
							paint->setIsRenderBeforeSheet(true);
						}
					} else if(keys[VK_DOWN] & 0x80) {
						if (paint->getIsRenderNextSheet()) {
							paint->setIsRenderNextSheet(false);
							paint->setSheetNext();
						} else {
							paint->setIsRenderNextSheet(true);
						}
					}
				}
			}
			
			break;






	case WM_MOVE:
	case WM_SIZE:
		GetClientRect(hWnd, &rcClient);
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		/*case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
			*/
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

		case WM_PAINT:
		if (hdc = BeginPaint(hWnd, &psPaint)) 
		{
			RECT clientRect;
			POINT scrPoint = {ptNew.x, ptNew.y};
			ScreenToClient(hWnd, &scrPoint);

			GetClientRect(hWnd, &clientRect);
			/*
			// Draw horizontal line of cross.
			PatBlt(hdc, rcClient.left, scrPoint.y,
				rcClient.right, 1, DSTINVERT);

			// Draw vertical line of cross.
			PatBlt(hdc, scrPoint.x, rcClient.top,
				1, rcClient.bottom, DSTINVERT);

			// Draw ellipse at cross intersection.
			Ellipse(hdc, scrPoint.x - prsNew, scrPoint.y - prsNew,
				scrPoint.x + prsNew, scrPoint.y + prsNew);*/
		/*	if (!paint->is_mode_dougasaisei && paint->is_mode_pausedougabyouga) {
			SetStretchBltMode(hdc , COLORONCOLOR);
			StretchBlt(
				hdc , 0 , 0 , clientRect.right , clientRect.bottom ,
				paint->douga.hdcMem , 0,0,
				paint->douga.bmi.bmiHeader.biWidth,paint->douga.bmi.bmiHeader.biHeight , SRCCOPY
			);
			}
			*/
			/*
			BitBlt(hdc,0,0,,paint->douga.hdcMem,0,0,SRCCOPY);*/
			EndPaint(hWnd, &psPaint);
		}
		break;

	case WM_DESTROY:
      if (hCtx)
      {
		   gpWTClose(hCtx);
      }
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
			xMousePos = GET_X_LPARAM(lParam);
			yMousePos = GET_Y_LPARAM(lParam);
			if (is_rdragged){
				int dx = xMousePos - dragpoint.x;
				int dy = yMousePos - dragpoint.y;
				paint->setTransX(paint->getTransX() + dx);
				paint->setTransY(paint->getTransY() + dy);
				dragpoint.x = xMousePos;
				dragpoint.y = yMousePos;
			}
		break;

	case WT_PACKET:
		if (gpWTPacket((HCTX)lParam, wParam, &pkt)) 
		{
			if (HIWORD(pkt.pkButtons)==TBN_DOWN) 
			{
				MessageBeep(0);
			}
			ptOld = ptNew;
			prsOld = prsNew;

			ptNew.x = pkt.pkX;
			ptNew.y = pkt.pkY;

			prsNew = pkt.pkNormalPressure;

			if (ptNew.x != ptOld.x ||
				ptNew.y != ptOld.y //||
				/*prsNew != prsOld*/) 
			{
				po = ptNew;
				ScreenToClient(hWnd, &po);
				po.x;// *= KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
				po.y;// *= KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
				mpo = ptOld;
				ScreenToClient(hWnd, &mpo);
				mpo.x;// *= KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
				mpo.y;// *= KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
				if (prsNew > 300) {
					paint->writeWithPen(mpo,po,prsOld,prsNew,false);
				} else {
					paint->writeWithPen(mpo,po,prsOld,prsNew,true);
				}
				//InvalidateRect(hWnd, NULL, TRUE);
			}
		}
		break;

	case WM_ACTIVATE:
		if (GET_WM_ACTIVATE_STATE(wParam, lParam))
      {
			InvalidateRect(hWnd, NULL, TRUE);
      } else {
		  is_rdragged = false;
		  //paint->endDrawLine();
		  paint->deactivate();
		}
		/* if switching in the middle, disable the region */
		if (hCtx) 
      {
			gpWTEnable(hCtx, GET_WM_ACTIVATE_STATE(wParam, lParam));
			if (hCtx && GET_WM_ACTIVATE_STATE(wParam, lParam))
         {
				gpWTOverlap(hCtx, TRUE);
         }
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

///////////////////////////////////////////////////////////////////////////////
HCTX static NEAR TabletInit(HWND hWnd)
{
	HCTX hctx = NULL;
	UINT wDevice = 0;
	UINT wExtX = 0;
	UINT wExtY = 0;
	UINT wWTInfoRetVal = 0;
	AXIS TabletX = {0};
	AXIS TabletY = {0};

	// Set option to move system cursor before getting default system context.
	glogContext.lcOptions |= CXO_SYSTEM;

	// Open default system context so that we can get tablet data
	// in screen coordinates (not tablet coordinates).
	wWTInfoRetVal = gpWTInfoA(WTI_DEFSYSCTX, 0, &glogContext);
	WACOM_ASSERT( wWTInfoRetVal == sizeof( LOGCONTEXT ) );

	WACOM_ASSERT( glogContext.lcOptions & CXO_SYSTEM );

	// modify the digitizing region
	wsprintf(glogContext.lcName, "PrsTest Digitizing %x", hInst);

	// We process WT_PACKET (CXO_MESSAGES) messages.
	glogContext.lcOptions |= CXO_MESSAGES;

	// What data items we want to be included in the tablet packets
	glogContext.lcPktData = PACKETDATA;

	// Which packet items should show change in value since the last
	// packet (referred to as 'relative' data) and which items
	// should be 'absolute'.
	glogContext.lcPktMode = PACKETMODE;

	// This bitfield determines whether or not this context will receive
	// a packet when a value for each packet field changes.  This is not
	// supported by the Intuos Wintab.  Your context will always receive
	// packets, even if there has been no change in the data.
	glogContext.lcMoveMask = PACKETDATA;

	// Which buttons events will be handled by this context.  lcBtnMask
	// is a bitfield with one bit per button.
	glogContext.lcBtnUpMask = glogContext.lcBtnDnMask;

	// Set the entire tablet as active
	wWTInfoRetVal = gpWTInfoA( WTI_DEVICES + 0, DVC_X, &TabletX );
	WACOM_ASSERT( wWTInfoRetVal == sizeof( AXIS ) );

	wWTInfoRetVal = gpWTInfoA( WTI_DEVICES, DVC_Y, &TabletY );
	WACOM_ASSERT( wWTInfoRetVal == sizeof( AXIS ) );

	glogContext.lcInOrgX = 0;
	glogContext.lcInOrgY = 0;
	glogContext.lcInExtX = TabletX.axMax;
	glogContext.lcInExtY = TabletY.axMax;

	// Guarantee the output coordinate space to be in screen coordinates.  
	glogContext.lcOutOrgX = GetSystemMetrics( SM_XVIRTUALSCREEN );
	glogContext.lcOutOrgY = GetSystemMetrics( SM_YVIRTUALSCREEN );
	glogContext.lcOutExtX = GetSystemMetrics( SM_CXVIRTUALSCREEN ); //SM_CXSCREEN );

	// In Wintab, the tablet origin is lower left.  Move origin to upper left
	// so that it coincides with screen origin.
	glogContext.lcOutExtY = -GetSystemMetrics( SM_CYVIRTUALSCREEN );	//SM_CYSCREEN );

	// Leave the system origin and extents as received:
	// lcSysOrgX, lcSysOrgY, lcSysExtX, lcSysExtY

	// open the region
	// The Wintab spec says we must open the context disabled if we are 
	// using cursor masks.  
	hctx = gpWTOpenA( hWnd, &glogContext, FALSE );

	WacomTrace("HCTX: %i\n", hctx);

	return hctx;
}

///////////////////////////////////////////////////////////////////////////////
void Cleanup( void )
{
	UnloadWintab( );
}
