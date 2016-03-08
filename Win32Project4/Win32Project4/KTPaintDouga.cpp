#include "KTPaintDouga.h"
#include "wxdebug.h"
#include "KTPaint.h"
#define	WM_GRAPH_NOTIFY		(WM_APP + 1)

KTPaintDouga::KTPaintDouga(void)
{
	pGB =0;
	pBF=0;
	pConf =0;
	pCGB2=0;
	pMediaCont=0;
	inited = false;
	g_pD3D = 0;
	g_pD3DDev = 0;
	pBuffer = 0;
	pMediaEventEx = 0;
	pSampleGrabberFilter=0;
    pSampleGrabber=0;
	pVideoWindow = NULL;
	Cont = 0;
}


KTPaintDouga::~KTPaintDouga(void)
{
}

// グラフィック用ウィンドウのデータをまとめた構造体
typedef struct{				
	HINSTANCE			hi;
	int					x;			// 表示開始位置
	int					y;
	HWND				hwnd;		// 自分のウィンドウハンドル
	BYTE				*lpBmpData; // BMPのデータ部分
	BITMAPINFOHEADER	bih;
} IMG0;

int gr_reg( void );
void gr_init(IMG0 *);
DWORD th_Proc( void *);
LRESULT CALLBACK grProc( HWND, UINT, WPARAM, LPARAM );


// グラフィック用のウィンドウクラスの登録
int gr_reg( void )							
{	
	WNDCLASSEX	wc;							// 新しくつくるウインドクラス

	memset( &wc, 0, sizeof(WNDCLASSEX) );
	wc.cbSize        = sizeof(WNDCLASSEX);	
	wc.lpfnWndProc   = grProc;				// このクラスの持つウインドプロシージャ
	wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
	wc.hInstance     = (HINSTANCE)GetWindowLong( HWND_DESKTOP, GWL_HINSTANCE ); 
	wc.lpszClassName = "GRC0";				// このクラスの名前
	wc.cbWndExtra    = 10;					// 関連する構造体のポインタ用のエリアを確保
	return RegisterClassEx( &wc );			// ウィンドクラスの登録
}

//	グラフィックウィンドウの生成，メッセージループ用スレッドの起動
void gr_init( IMG0 * pimg00)
{
	DWORD tid;

	pimg00 -> hwnd = NULL;
	CreateThread( NULL, 0,
				 (LPTHREAD_START_ROUTINE)th_Proc,
				 (void *)pimg00, 0, &tid );
												// メッセージループのスレッドを起動	
	while( !(pimg00 -> hwnd) );					// ウィンドウが表示されるのを待つ
}

//　メッセージループのためのスレッド
DWORD th_Proc( void * pp)
{
	MSG msg;

	int sm0 = GetSystemMetrics( SM_CYCAPTION );
	int sm1 = GetSystemMetrics( SM_CXFIXEDFRAME );	// WS_OVRELAPPの場合、枠の太さは
	int sm2 = GetSystemMetrics( SM_CYFIXEDFRAME );	// SM_C?FIXEDFRAMEになる
	// 必ずスレッドの中でウィンドウを作る
	((IMG0 *)pp) -> hwnd = CreateWindow( "GRC0",	// クラスの名前
				"...",
				WS_OVERLAPPED | WS_VISIBLE,			// ウィンドウの属性
				((IMG0 *)pp) -> x, ((IMG0 *)pp) -> y,		// 表示位置
				((IMG0 *)pp) -> bih.biWidth  + sm1 * 2, 	// 描画サイズから大きさを計算
				((IMG0 *)pp) -> bih.biHeight + sm0 + sm2 * 2,
				HWND_DESKTOP,						// 親はディスクトップ
				NULL, ((IMG0 *)pp)->hi, NULL 	     );					
	SetWindowLong(((IMG0 *)pp) -> hwnd, 0, (LONG)pp);
	while( GetMessage( &msg, NULL, 0, 0 ) ){
		DispatchMessage( &msg );
	}

	return 0;
}

//　ウィンドウプロシージャ，再描画のみを行う
LRESULT CALLBACK grProc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp )
{
	IMG0 * pimg;				
	PAINTSTRUCT	ps;

	pimg = (IMG0 * )GetWindowLong( hwnd , 0 );	// 拡張ウィンドウメモリより
												// このウィンドウ用データへの
												// ポインタを取り出す
    switch (msg) {

	case WM_PAINT:								// ビットマップの描画
		BeginPaint( hwnd, &ps );
		SetDIBitsToDevice( ps.hdc, 0, 0,			// コピー先x,y座標
					pimg -> bih.biWidth,			// DIBの幅
					pimg -> bih.biHeight,			// DIBの高さ
					0, 0,							// DIBの座標 
					0,								// 走査線
					pimg -> bih.biHeight,			// 走査線数
					pimg -> lpBmpData,
					(BITMAPINFO *)&( pimg -> bih),	// BITMAPINFOにキャスト
					DIB_RGB_COLORS );
        EndPaint( hwnd, &ps );
		return 0;

	case WM_DESTROY:							// スレッドの
		PostQuitMessage( 0 );					// メッセージループを終了させる
		break;			

	default:
		return DefWindowProc( hwnd, msg, wp, lp );
	}
	return 0;
	
}




HRESULT CheckMediaType(AM_MEDIA_TYPE *pmt)
{
    if (pmt == NULL) return E_POINTER;

    // メジャー タイプをチェックする。 ここでは、ビデオを検索している。
    if (pmt->majortype != MEDIATYPE_Video)
    {
        return VFW_E_INVALIDMEDIATYPE;
    }

    // サブタイプをチェックする。 ここでは、24 ビット RGB を検索している。
    if (pmt->subtype != MEDIASUBTYPE_RGB24)
    {
        return VFW_E_INVALIDMEDIATYPE;
    }

    // フォーマット タイプおよびフォーマット ブロックのサイズをチェックする。
    if ((pmt->formattype == FORMAT_VideoInfo) &&
         (pmt->cbFormat >= sizeof(VIDEOINFOHEADER)) &&
         (pmt->pbFormat != NULL))
    {
        // これで、フォーマット ブロック ポインタを、フォーマットタイプ GUID で定義された
        // 正しい構造体に強制しても安全である。
        VIDEOINFOHEADER *pVIH = (VIDEOINFOHEADER*)pmt->pbFormat;
    
        // pVIH (省略されている) を調べる。 問題がない場合は、S_OK を返す。
        return S_OK;
    }

    return VFW_E_INVALIDMEDIATYPE;
}


void KTPaintDouga::Init3(HWND hWnd,int nCmdShow) {
      IGraphBuilder *pGraphBuilder;
        IMediaControl *pMediaControl;
        IBaseFilter *pSampleGrabberFilter;
        ISampleGrabber *pSampleGrabber;
        AM_MEDIA_TYPE am_media_type;
        // COMを初期化
        CoInitialize( NULL );
        // FilterGraphを生成
        CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, ( LPVOID * )&pGraphBuilder );
        // SampleGrabber(Filter)を生成
        CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC, IID_IBaseFilter, ( LPVOID * )&pSampleGrabberFilter );
        // FilterからISampleGrabberインターフェースを取得します
        pSampleGrabberFilter->QueryInterface( IID_ISampleGrabber, ( LPVOID * )&pSampleGrabber );
        // SampleGrabberを接続するフォーマットを指定。
        // ここがポイントです。
        // ここの指定の仕方によりSampleGrabberの挿入箇所を
        // 決定できます。このサンプルのような指定をすると
        // 画面出力の寸前でサンプルを取得できます。
        ZeroMemory(&am_media_type, sizeof(am_media_type));
        am_media_type.majortype = MEDIATYPE_Video;
        am_media_type.subtype = MEDIASUBTYPE_RGB24;
        am_media_type.formattype = FORMAT_VideoInfo;
        pSampleGrabber->SetMediaType(&am_media_type);
        // GraphにSampleGrabber Filterを追加
        pGraphBuilder->AddFilter( pSampleGrabberFilter, L"Sample Grabber" );
        // MediaControlインターフェース取得
        pGraphBuilder->QueryInterface( IID_IMediaControl, (LPVOID *)&pMediaControl );
        // Graphを生成。
        // ここでSampleGrabberを含んだGraphが自動的に作成されます。
        pMediaControl->RenderFile( L"resrc/Wildlife.wmv" );
        // 接続情報取得。
        // この処理はRenderFileによりGraphが構成された後に
        // 行う必要があります。
		ShowWindow(hWnd, nCmdShow);

		Sleep(1000);
        pSampleGrabber->GetConnectedMediaType(&am_media_type);
		if (FAILED(CheckMediaType(&am_media_type))) {
			  MessageBoxW( hWnd, L"Error.", L"Error", MB_OK | MB_ICONERROR );
                return;
        }
        VIDEOINFOHEADER *pVideoInfoHeader = (VIDEOINFOHEADER *)am_media_type.pbFormat;
  // ビットマップ情報を保存
  memcpy( &bmi.bmiHeader, &pVideoInfoHeader->bmiHeader, sizeof( BITMAPINFOHEADER ) );
  // ビットマップを作成
  hBitmap = CreateDIBSection( 0, &bmi, DIB_RGB_COLORS, ( VOID ** )&pBuffer, NULL, 0 );
        if( !hBitmap ) {
                MessageBoxW( hWnd, L"Error.", L"Error", MB_OK | MB_ICONERROR );
                return;
        }
        HBITMAP hbOld = ( HBITMAP )SelectObject( hdcMem, hBitmap );
        if( hbOld ) {
                DeleteObject( hbOld );
        }
        // Grabを行う事を設定
        // SetBufferSamplesを行わないとバッファから
        // データを取得できません。
        // 不必要に負荷をかけたくない場合にはFALSEにしておいて、
        // データを取得したくなったら、TRUEに変える
        // という方法もできます。
        pSampleGrabber->SetBufferSamples( TRUE );
        // 再生開始
		Sleep(3000);
        pMediaControl->Run();
        // 再生中にプログラムが終わってしまわないように
        MessageBoxA( hWnd, "Block Execution", "Block", MB_OK );
        // OKが押されるとBITMAPを保存する
        // バッファを用意
        // 必要なバッファサイズを取得
        long nBufferSize = 0;
        pSampleGrabber->GetCurrentBuffer( &nBufferSize, NULL );
        // 現在表示されている映像を静止画として取得 ( CreateDIBSection() が返した pBuffer に書き込む )
        pSampleGrabber->GetCurrentBuffer( &nBufferSize, pBuffer );
        RECT rect;
        GetClientRect( hWnd, &rect );
        InvalidateRect( hWnd, &rect, FALSE );  // 画面リフレッシュ ( WM_PAINT が送れれてくるようにする )
        // 資源を解放
        pSampleGrabber->Release();
        pSampleGrabberFilter->Release();
        pMediaControl->Release();
        pGraphBuilder->Release();
        // COM終了
        CoUninitialize();
}

bool KTPaintDouga::Init2(HWND hwnd,HINSTANCE hinst) {

	
	// インターフェース用のポインタ
	// フィルタグラフ用
	IGraphBuilder * pigb = NULL;
	IMediaControl * pimc = NULL;
	IMediaEvent   *pimex = NULL;
	IMediaSeeking *pims  = NULL;	 
	// DVデコーダ用
	IIPDVDec      *pdv   = NULL;
	// サンプルグラバ用
	IBaseFilter     *pF  = NULL;
	ISampleGrabber  *pGrab = NULL;  // これらは後で解放すること。

	IMG0	img00;
	BYTE  *buffer;
	AM_MEDIA_TYPE   amt;
	WCHAR filename[ MAX_PATH ];
	HRESULT hr;

	// ファイル名の変換
	MultiByteToWideChar( CP_ACP, 0, "resrc/Wildlife.wmv", -1, filename, MAX_PATH );

	img00.hi = (HINSTANCE)hinst;
	img00.x = 100; img00.y = 100;
	gr_reg();					// 表示用ウィンドウの登録

	CoInitialize(NULL);				// COMの準備

	// FilterGraphの初期化
	CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
						IID_IGraphBuilder, (void**)&pigb);
	// グラバフィルタを作りフィルタグラフに追加
	CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, 
                    IID_IBaseFilter, (LPVOID *)&pF);
	pF -> QueryInterface( IID_ISampleGrabber, (void **)&pGrab);
	pigb -> AddFilter(pF, L"SamGra");

	// グラバフィルタの挿入場所の特定のための設定
	ZeroMemory(&amt, sizeof(AM_MEDIA_TYPE));
	amt.majortype  = MEDIATYPE_Video;
	amt.subtype    = MEDIASUBTYPE_RGB24;
	amt.formattype = FORMAT_VideoInfo; 
	pGrab -> SetMediaType(&amt);

	// フィルタグラフのインターフェースを得る
	pigb -> QueryInterface( IID_IMediaControl, (void**)&pimc );
	pigb -> QueryInterface( IID_IMediaEvent, (void **)&pimex);
	pigb -> QueryInterface( IID_IMediaSeeking, (void **)&pims);

	// 再生するファイルを指定
	hr = pigb -> RenderFile( filename, NULL );
	printf("RenderFile hr %x\n",hr);
	if(hr != 0)		return false;

	// DVデコードの解像度をFULLに設定する
	IEnumFilters *pEnum = NULL;
	IBaseFilter *pFilter;
	ULONG cFetched;
	int nnn;
	pigb ->EnumFilters(&pEnum);
	// 使用されるフィルタを調べる
	while(pEnum->Next(1, &pFilter, &cFetched) == S_OK){
        	FILTER_INFO FilterInfo;
        	char szName[256];
        
        	pFilter->QueryFilterInfo(&FilterInfo);
        	WideCharToMultiByte(CP_ACP, 0, FilterInfo.achName, 
							-1, szName, 256, 0, 0);
		printf("使用フィルタ = %s \n",szName); 
        	if( ! strcmp( szName, "DV Video Decoder") ){
			// DVデコーダなら解像度をFULLに設定
			pFilter ->QueryInterface(IID_IIPDVDec, (void **)& pdv);
			pdv -> get_IPDisplay( &nnn);
			printf(" 既設デコード解像度 IP = %d \n",nnn);
			pdv -> put_IPDisplay( 1000);
			pdv -> get_IPDisplay( &nnn);
			printf(" 仮設デコード解像度 IP = %d \n",nnn);
		}
		FilterInfo.pGraph->Release();
        pFilter->Release();
	}
	pEnum->Release();

	// ビットマップ情報の取得
	pGrab->GetConnectedMediaType(&amt); 
	// ビデオ ヘッダーへのポインタを獲得する。
	printf("amt.lSampleSize = %d \n", amt.lSampleSize );
	VIDEOINFOHEADER *pVideoHeader = 
			(VIDEOINFOHEADER*)amt.pbFormat;
	// ビデオ ヘッダーには、ビットマップ情報が含まれる。
	// ビットマップ情報を BITMAPINFO 構造体にコピーする。
	BITMAPINFO BitmapInfo;
	ZeroMemory(&BitmapInfo, sizeof(BitmapInfo));
	CopyMemory(&BitmapInfo.bmiHeader, 
			&(pVideoHeader->bmiHeader), sizeof(BITMAPINFOHEADER));
	printf("BitmapInfo.bmiHeader.biBitCount = %d\n", 
			BitmapInfo.bmiHeader.biBitCount);
	img00.bih = BitmapInfo.bmiHeader;
	buffer = (BYTE *)malloc(amt.lSampleSize);
	img00.lpBmpData=buffer;
	long n = amt.lSampleSize;
	long evCode;
	int n0, n1;
	printf("開始フレーム?");
	//scanf("%d",&n0);
	n0=0;
	printf("終了フレーム?");
	//scanf("%d",&n1);
	n1 = 1000;
	LONGLONG nn = n0;
	gr_init(&img00);				// グラフィックウィンドウを生成
	pims -> SetTimeFormat(&(TIME_FORMAT_FRAME)) ;	
						// シークをフレーム単位で行うよう設定
	pGrab->SetBufferSamples(TRUE);	// グラブ開始
	for(int i = n0; i <= n1;i++){
		// シーク
		pims ->SetPositions(&nn,AM_SEEKING_AbsolutePositioning,
					&nn,AM_SEEKING_AbsolutePositioning);
		pimc -> Run();				// レンダリング	
		printf("run at %d\n", nn);
		pimex -> WaitForCompletion(100, &evCode);
						// レンダリング完了を待つ
		pGrab -> GetCurrentBuffer(&n, (long *)buffer);
									// グラブ
		InvalidateRect( img00.hwnd, NULL, FALSE);
		nn++;
	}
	Sleep(100);
	return true;
}
bool KTPaintDouga::Init(HWND hwnd, HWND loadsave, int width, int height,int nCmdShow) {
	UpdateWindow(hwnd);
	
	 IMediaControl *pMediaControl;
	  AM_MEDIA_TYPE am_media_type;

	/*
    if( !(g_pD3D = Direct3DCreate9( D3D_SDK_VERSION )) ) return 0;

    // フルスクリーン用に初期化パラメータを設定
    D3DPRESENT_PARAMETERS d3dpp = {width,height,D3DFMT_A8R8G8B8,0,D3DMULTISAMPLE_NONE,0,
                                                      D3DSWAPEFFECT_DISCARD,hwnd,TRUE,0,D3DFMT_UNKNOWN,D3DPRESENT_RATE_DEFAULT,0}; 

   if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDev ) ) )
   if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDev ) ) )
   {
      g_pD3D->Release();
	  g_pD3D = 0;
      return 0;
   }




	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&pGB);
	if (FAILED(hr)) return false;





    CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC, IID_IBaseFilter, ( LPVOID * )&pSampleGrabberFilter );
        // FilterからISampleGrabberインターフェースを取得します
        pSampleGrabberFilter->QueryInterface( IID_ISampleGrabber, ( LPVOID * )&pSampleGrabber );
        // SampleGrabberを接続するフォーマットを指定。
        // ここがポイントです。
        // ここの指定の仕方によりSampleGrabberの挿入箇所を
        // 決定できます。このサンプルのような指定をすると
        // 画面出力の寸前でサンプルを取得できます。
        ZeroMemory(&am_media_type, sizeof(am_media_type));
        am_media_type.majortype = MEDIATYPE_Video;
        am_media_type.subtype = MEDIASUBTYPE_RGB24;
        am_media_type.formattype = FORMAT_VideoInfo;
        pSampleGrabber->SetMediaType(&am_media_type);
		 HANDLE hFile = CreateFile("GraphLog.txt", GENERIC_WRITE, 0, NULL, 
OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
		pGB->SetLogFile((DWORD_PTR)hFile);
        // GraphにSampleGrabber Filterを追加
        pGB->AddFilter( pSampleGrabberFilter, L"Sample Grabber" );
        // MediaControlインターフェース取得
        pGB->QueryInterface( IID_IMediaControl, (LPVOID *)&pMediaControl );
        // Graphを生成。
        // ここでSampleGrabberを含んだGraphが自動的に作成されます。
     
		
		
			IVMRFilterConfig* pConfig = 0;
	  // レンダリング モードとストリームの数を設定する。
  
	IVMRWindowlessControl *Cont = NULL;
	int dwNumStreams = 1;
	bool fBlendAppImage = false;
	hr = CoCreateInstance(CLSID_VideoMixingRenderer9, NULL,CLSCTX_INPROC,IID_IBaseFilter, (void**)&pBF);
	
	if (FAILED(hr)) return false;

	hr = pGB->AddFilter(pBF,L"VMR9");
    hr = pBF->QueryInterface(IID_IVMRFilterConfig9, (void**)&pConfig);
    if (SUCCEEDED(hr)) 
    {
        pConfig->SetRenderingMode(VMRMode_Windowless);

        // 複数のビデオ ストリームが必要な場合、または静的ビットマップをビデオ上に
        // ミキシングする場合は、VMR-7 をミキシング モードに設定する。
        // (VMR-9 はデフォルトで 4 つの入力を持つミキシング モードになる。)
        if (dwNumStreams > 1 || fBlendAppImage) 
        {
            pConfig->SetNumberOfStreams(dwNumStreams);
        }
        pConfig->Release();

        hr = pBF->QueryInterface(IID_IVMRWindowlessControl, (void**)&Cont);
        if (SUCCEEDED(hr)) 
        {
            Cont->SetVideoClippingWindow(hwnd);
            
        }
    }
 //
		
		
		
			pGB->QueryInterface(IID_IMediaEventEx,
		(LPVOID *)&pMediaEventEx);
	pMediaEventEx->SetNotifyWindow((OAHWND)hwnd,
					WM_GRAPH_NOTIFY, NULL);

		
		
		
		
		
		
		pMediaControl->RenderFile( L"resrc/Wildlife.wmv" );
        // 接続情報取得。
        // この処理はRenderFileによりGraphが構成された後に
        // 行う必要があります。
		//5ShowWindow(hwnd, nCmdShow);

		Sleep(1000);
        pSampleGrabber->GetConnectedMediaType(&am_media_type);
		if (FAILED(CheckMediaType(&am_media_type))) {
			  MessageBoxW( hwnd, L"Error.", L"Error", MB_OK | MB_ICONERROR );
                return false;
        }
        VIDEOINFOHEADER *pVideoInfoHeader = (VIDEOINFOHEADER *)am_media_type.pbFormat;
  // ビットマップ情報を保存
  memcpy( &bmi.bmiHeader, &pVideoInfoHeader->bmiHeader, sizeof( BITMAPINFOHEADER ) );
  // ビットマップを作成
  hBitmap = CreateDIBSection( 0, &bmi, DIB_RGB_COLORS, ( VOID ** )&pBuffer, NULL, 0 );
        if( !hBitmap ) {
                MessageBoxW( hwnd, L"Error.", L"Error", MB_OK | MB_ICONERROR );
                return false;
        }

		Sleep(1000);
        HBITMAP hbOld = ( HBITMAP )SelectObject( hdcMem, hBitmap );
        if( hbOld ) {
                DeleteObject( hbOld );
        }
        // Grabを行う事を設定
        // SetBufferSamplesを行わないとバッファから
        // データを取得できません。
        // 不必要に負荷をかけたくない場合にはFALSEにしておいて、
        // データを取得したくなったら、TRUEに変える
        // という方法もできます。
        pSampleGrabber->SetBufferSamples( TRUE );
      
	


	

		Sleep(1000);

		SetForegroundWindow(hwnd);

		pMediaControl->Run();
        // 再生中にプログラムが終わってしまわないように
        MessageBox( hwnd, "Block Execution", "Block", MB_OK );

		#ifdef _DEBUG
		DumpGraph(pGB);
		#endif
        // OKが押されるとBITMAPを保存する
        // バッファを用意
        // 必要なバッファサイズを取得
        long nBufferSize = 0;
        pSampleGrabber->GetCurrentBuffer( &nBufferSize, NULL );
        // 現在表示されている映像を静止画として取得 ( CreateDIBSection() が返した pBuffer に書き込む )
        pSampleGrabber->GetCurrentBuffer( &nBufferSize, pBuffer );
        RECT rect;
        GetClientRect( hwnd, &rect );
        InvalidateRect( hwnd, &rect, FALSE );  // 画面リフレッシュ ( WM_PAINT が送れれてくるようにする )
        // 資源を解放
        pSampleGrabber->Release();
        pSampleGrabberFilter->Release();
        pMediaControl->Release();
        pGB->Release();
		pGB = 0;
        // COM終了
        CoUninitialize();

		 PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
  // CreateDIBSection() によって作成されたビットマップを描画
  BitBlt( hdc, 0, 0, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight, hdcMem, 0, 0, SRCCOPY );
  EndPaint(hwnd, &ps);

		return false;





		*/


	





	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&pGB);
	if (FAILED(hr)) return false;


	hr = CoCreateInstance(CLSID_VideoMixingRenderer, NULL,CLSCTX_INPROC,IID_IBaseFilter, (void**)&pBF);
	
	if (FAILED(hr)) return false;

	hr = pGB->AddFilter(pBF,L"VMR9");



	IVMRFilterConfig* pConfig = 0;
	  // レンダリング モードとストリームの数を設定する。
  
	// *Cont = NULL;
	int dwNumStreams = 1;
	bool fBlendAppImage = false;
	
    hr = pBF->QueryInterface(IID_IVMRFilterConfig, (void**)&pConfig);
    if (SUCCEEDED(hr)) 
    {
        pConfig->SetRenderingMode(VMR9Mode_Windowless);

        // 複数のビデオ ストリームが必要な場合、または静的ビットマップをビデオ上に
        // ミキシングする場合は、VMR-7 をミキシング モードに設定する。
        // (VMR-9 はデフォルトで 4 つの入力を持つミキシング モードになる。)
        if (dwNumStreams > 1 || fBlendAppImage) 
        {
            pConfig->SetNumberOfStreams(dwNumStreams);
        }
        pConfig->Release();

        hr = pBF->QueryInterface(IID_IVMRWindowlessControl, (void**)&Cont);
        if (SUCCEEDED(hr)) 
        {
            Cont->SetVideoClippingWindow(hwnd);
            
        }
    }
 //   pConfig->Release();
	if (!Cont) return false;
	
	

	//Cont->SetVideoClippingWindow( hwnd );

	WCHAR filename[] = L"resrc/hundred.wmv";
	IBaseFilter* psource = 0;
//	pGB->AddSourceFilter(filename, filename, &psource);
/*	
	*pCGB2;
	hr = CoCreateInstance( CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void**)&pCGB2);
	if (FAILED(hr)) {
		Cont->Release();
		return false;
	}
	hr = pCGB2->SetFiltergraph( pGB );
	if (FAILED(hr)) {
		Cont->Release();
		return false;
	}
	
	hr = pCGB2->RenderStream(0,0,psource,0,pBF);
	if (FAILED(hr)) {
		Cont->Release();
		return false;
	}
	*/

	  CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC, IID_IBaseFilter, ( LPVOID * )&pSampleGrabberFilter );
        // FilterからISampleGrabberインターフェースを取得します
        pSampleGrabberFilter->QueryInterface( IID_ISampleGrabber, ( LPVOID * )&pSampleGrabber );




  hr = pGB->QueryInterface(IID_IVideoWindow, (LPVOID*)&pVideoWindow);
 
    // グラフを作成する。 重要 : この文字列をシステム上のファイルに置き換える。  
 //   hr = pGraph->RenderFile(L"G:\\sampleMovie\\avi_divx5_mp3.avi", NULL);  
 
    // 描画先ウィンドウの設定
   // HWND hWnd = reinterpret_cast< HWND & >(this->Handle);
    hr = pVideoWindow->put_Owner((OAHWND)hwnd);
    hr = pVideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPSIBLINGS);
    //hr = pVideoWindow->SetWindowPosition(0,0,320,240);
    hr = pVideoWindow->put_Visible(OATRUE);
 












        // SampleGrabberを接続するフォーマットを指定。
        // ここがポイントです。
        // ここの指定の仕方によりSampleGrabberの挿入箇所を
        // 決定できます。このサンプルのような指定をすると
        // 画面出力の寸前でサンプルを取得できます。
        ZeroMemory(&am_media_type, sizeof(am_media_type));
        am_media_type.majortype = MEDIATYPE_Video;
        am_media_type.subtype = MEDIASUBTYPE_RGB24;
        am_media_type.formattype = FORMAT_VideoInfo;
        pSampleGrabber->SetMediaType(&am_media_type);
		 HANDLE hFile = CreateFile("GraphLog.txt", GENERIC_WRITE, 0, NULL, 
OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
		pGB->SetLogFile((DWORD_PTR)hFile);
        // GraphにSampleGrabber Filterを追加
        pGB->AddFilter( pSampleGrabberFilter, L"Sample Grabber" );
        // MediaControlインターフェース取得
        pGB->QueryInterface( IID_IMediaControl, (LPVOID *)&pMediaControl );
        // Graphを生成。
        // ここでSampleGrabberを含んだGraphが自動的に作成されます。
     
	
		
		
			pGB->QueryInterface(IID_IMediaEventEx,
		(LPVOID *)&pMediaEventEx);
	pMediaEventEx->SetNotifyWindow((OAHWND)hwnd,
					WM_GRAPH_NOTIFY, NULL);

		
		
		hr = pGB->QueryInterface(IID_IVideoWindow, (LPVOID*)&pVideoWindow);
 
    // グラフを作成する。 重要 : この文字列をシステム上のファイルに置き換える。  
   // hr = pGraph->RenderFile(L"G:\\sampleMovie\\avi_divx5_mp3.avi", NULL);  
 
  
 
		
		
		
		pMediaControl->RenderFile( L"resrc/hundred.wmv" );
        // 接続情報取得。
        // この処理はRenderFileによりGraphが構成された後に
        // 行う必要があります。
	//	ShowWindow(hwnd, nCmdShow);


		  // 描画先ウィンドウの設定
 //   HWND hWnd = reinterpret_cast< HWND & >(this->Handle);
    hr = pVideoWindow->put_Owner((OAHWND)hwnd);
//    hr = pVideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPSIBLINGS);
 //   hr = pVideoWindow->SetWindowPosition(0,0,320,240);
    hr = pVideoWindow->put_Visible(OATRUE);






	//	Sleep(1000);
        pSampleGrabber->GetConnectedMediaType(&am_media_type);
		if (FAILED(CheckMediaType(&am_media_type))) {
			  MessageBoxW( hwnd, L"Error.", L"Error", MB_OK | MB_ICONERROR );
                return false;
        }
        VIDEOINFOHEADER *pVideoInfoHeader = (VIDEOINFOHEADER *)am_media_type.pbFormat;
  // ビットマップ情報を保存
  memcpy( &bmi.bmiHeader, &pVideoInfoHeader->bmiHeader, sizeof( BITMAPINFOHEADER ) );
  // ビットマップを作成
  hBitmap = CreateDIBSection( 0, &bmi, DIB_RGB_COLORS, ( VOID ** )&pBuffer, NULL, 0 );
        if( !hBitmap ) {
                MessageBoxW( hwnd, L"Error.", L"Error", MB_OK | MB_ICONERROR );
                return false;
        }

		Sleep(1000);
        HBITMAP hbOld = ( HBITMAP )SelectObject( hdcMem, hBitmap );
        if( hbOld ) {
                DeleteObject( hbOld );
        }
        // Grabを行う事を設定
        // SetBufferSamplesを行わないとバッファから
        // データを取得できません。
        // 不必要に負荷をかけたくない場合にはFALSEにしておいて、
        // データを取得したくなったら、TRUEに変える
        // という方法もできます。
        pSampleGrabber->SetBufferSamples( TRUE );
      
	


	

	//	Sleep(1000);

		SetForegroundWindow(hwnd);

	//	pMediaControl->Run();
        // 再生中にプログラムが終わってしまわないように
       // MessageBox( hwnd, "Block Execution", "Block", MB_OK );

		#ifdef _DEBUG
		DumpGraph(pGB);
		#endif
        // OKが押されるとBITMAPを保存する
        // バッファを用意
        // 必要なバッファサイズを取得

		/*
        long nBufferSize = 0;
        pSampleGrabber->GetCurrentBuffer( &nBufferSize, NULL );
        // 現在表示されている映像を静止画として取得 ( CreateDIBSection() が返した pBuffer に書き込む )
        pSampleGrabber->GetCurrentBuffer( &nBufferSize, pBuffer );
        RECT rect;
        GetClientRect( hwnd, &rect );
        InvalidateRect( hwnd, &rect, FALSE );  // 画面リフレッシュ ( WM_PAINT が送れれてくるようにする )
        // 資源を解放
        pSampleGrabber->Release();
        pSampleGrabberFilter->Release();
		*/
        pMediaControl->Release();
		/*
        pGB->Release();
		pGB = 0;
        // COM終了
        CoUninitialize();
		*/

		/*
		 PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
  // CreateDIBSection() によって作成されたビットマップを描画
  BitBlt( hdc, 0, 0, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight, hdcMem, 0, 0, SRCCOPY );
  EndPaint(hwnd, &ps);
  */
	





		


	


















































/*
    hr = pCGB2->RenderStream(0, &MEDIATYPE_Audio, psource, 0, 0);

//	hr = pCGB2->RenderStream(0,&MEDIATYPE_Audio, psource,0,pBF);
	if (FAILED(hr)) {
		Cont->Release();
		return false;
	}
	
*/
	LONG W=994;
	LONG H=775;
	RECT SrcR, DestR;
	hr = Cont->GetNativeVideoSize(&W, &H, NULL, NULL);
	if (FAILED(hr)) {
		Cont->Release();
		return false;
	}

	SetRect(&SrcR, 0, 0, W, H);
	GetClientRect(hwnd, &DestR);
	hr = Cont->SetVideoPosition(&SrcR, &DestR);
	if (FAILED(hr)) {
		Cont->Release();
		return false;
	}

	//Cont->Release();
	pGB->QueryInterface( IID_IMediaControl, (void**)&pMediaCont );
	/*
	while(!pMediaCont->Run()) {
	Sleep(100);
		//pMediaCont->Run();
	}
	//Sleep(10000);*/
	inited = true;
	return true;
}

void KTPaintDouga::Del() {
	if (pMediaEventEx) {
		pMediaEventEx->Release();
		pMediaEventEx = 0;
	}
	
	if (pMediaCont) {
		pMediaCont->Stop();
		pMediaCont->Release();
		pMediaCont = 0;
	}
	if (pVideoWindow) {
		pVideoWindow->Release();
		pVideoWindow = 0;
	}

	if (pSampleGrabberFilter) {
		pSampleGrabberFilter->Release();
		pSampleGrabberFilter =0;
	}
    if (pSampleGrabber) {
		pSampleGrabber->Release();
		pSampleGrabber = 0;
	}
	if (Cont) {
		Cont->Release();
		Cont =0;
	}
	if (pBF) {
		pBF->Release();
		pBF = 0;
	}
	if (pCGB2) {
		pCGB2->Release();
		pCGB2 = 0;
	}
	if (pGB) {
		pGB->Release();
		pGB = 0;
	}

	
	if (g_pD3DDev) {
		g_pD3DDev->Release();
		g_pD3DDev = 0;
	}


	if (g_pD3D) {
		g_pD3D->Release();
		g_pD3D = 0;
	}

	if (inited) {
	CoUninitialize();
	inited = false;
	}

}

extern KTPaint* paint;
// イベントが発生すると呼び出される
LRESULT
KTPaintDouga::OnGraphNotify(WPARAM wParam, LPARAM lParam)
{
 long evCode;
 LONG param1, param2;

 // イベントを全て取得
 while (SUCCEEDED(pMediaEventEx->GetEvent(&evCode,
					&param1, &param2, 0))) {
	pMediaEventEx->FreeEventParams(evCode, param1, param2);

	switch (evCode) {
	case EC_ACTIVATE:
		InvalidateRect(paint->parent_window, NULL, TRUE);
		MessageBox( paint->parent_window, "Block Execution", "Block", MB_OK);
		break;
		case EC_COMPLETE:
			// 再生終了
			MessageBox( paint->parent_window, "Block Execution", "Block", MB_OK);
			break;
	}
 }

 return NOERROR;
}

void KTPaintDouga::Run(HWND hwnd,HDC hdc){
		if (pMediaCont) {
		HRESULT hr = pMediaCont->Run();
	//	HWND child_window=GetWindow(hwnd, GW_CHILD);//hwndは親ウィンドウ
	//	pVideoWindow->rePaint(child_window);
	//	InvalidateRect(child_window, 0, TRUE);
		Cont->RepaintVideo(hwnd, hdc);
		if (FAILED(hr)) {
			Beep(3,1);
		}
		}
	}