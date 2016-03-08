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

// �O���t�B�b�N�p�E�B���h�E�̃f�[�^���܂Ƃ߂��\����
typedef struct{				
	HINSTANCE			hi;
	int					x;			// �\���J�n�ʒu
	int					y;
	HWND				hwnd;		// �����̃E�B���h�E�n���h��
	BYTE				*lpBmpData; // BMP�̃f�[�^����
	BITMAPINFOHEADER	bih;
} IMG0;

int gr_reg( void );
void gr_init(IMG0 *);
DWORD th_Proc( void *);
LRESULT CALLBACK grProc( HWND, UINT, WPARAM, LPARAM );


// �O���t�B�b�N�p�̃E�B���h�E�N���X�̓o�^
int gr_reg( void )							
{	
	WNDCLASSEX	wc;							// �V��������E�C���h�N���X

	memset( &wc, 0, sizeof(WNDCLASSEX) );
	wc.cbSize        = sizeof(WNDCLASSEX);	
	wc.lpfnWndProc   = grProc;				// ���̃N���X�̎��E�C���h�v���V�[�W��
	wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
	wc.hInstance     = (HINSTANCE)GetWindowLong( HWND_DESKTOP, GWL_HINSTANCE ); 
	wc.lpszClassName = "GRC0";				// ���̃N���X�̖��O
	wc.cbWndExtra    = 10;					// �֘A����\���̂̃|�C���^�p�̃G���A���m��
	return RegisterClassEx( &wc );			// �E�B���h�N���X�̓o�^
}

//	�O���t�B�b�N�E�B���h�E�̐����C���b�Z�[�W���[�v�p�X���b�h�̋N��
void gr_init( IMG0 * pimg00)
{
	DWORD tid;

	pimg00 -> hwnd = NULL;
	CreateThread( NULL, 0,
				 (LPTHREAD_START_ROUTINE)th_Proc,
				 (void *)pimg00, 0, &tid );
												// ���b�Z�[�W���[�v�̃X���b�h���N��	
	while( !(pimg00 -> hwnd) );					// �E�B���h�E���\�������̂�҂�
}

//�@���b�Z�[�W���[�v�̂��߂̃X���b�h
DWORD th_Proc( void * pp)
{
	MSG msg;

	int sm0 = GetSystemMetrics( SM_CYCAPTION );
	int sm1 = GetSystemMetrics( SM_CXFIXEDFRAME );	// WS_OVRELAPP�̏ꍇ�A�g�̑�����
	int sm2 = GetSystemMetrics( SM_CYFIXEDFRAME );	// SM_C?FIXEDFRAME�ɂȂ�
	// �K���X���b�h�̒��ŃE�B���h�E�����
	((IMG0 *)pp) -> hwnd = CreateWindow( "GRC0",	// �N���X�̖��O
				"...",
				WS_OVERLAPPED | WS_VISIBLE,			// �E�B���h�E�̑���
				((IMG0 *)pp) -> x, ((IMG0 *)pp) -> y,		// �\���ʒu
				((IMG0 *)pp) -> bih.biWidth  + sm1 * 2, 	// �`��T�C�Y����傫�����v�Z
				((IMG0 *)pp) -> bih.biHeight + sm0 + sm2 * 2,
				HWND_DESKTOP,						// �e�̓f�B�X�N�g�b�v
				NULL, ((IMG0 *)pp)->hi, NULL 	     );					
	SetWindowLong(((IMG0 *)pp) -> hwnd, 0, (LONG)pp);
	while( GetMessage( &msg, NULL, 0, 0 ) ){
		DispatchMessage( &msg );
	}

	return 0;
}

//�@�E�B���h�E�v���V�[�W���C�ĕ`��݂̂��s��
LRESULT CALLBACK grProc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp )
{
	IMG0 * pimg;				
	PAINTSTRUCT	ps;

	pimg = (IMG0 * )GetWindowLong( hwnd , 0 );	// �g���E�B���h�E���������
												// ���̃E�B���h�E�p�f�[�^�ւ�
												// �|�C���^�����o��
    switch (msg) {

	case WM_PAINT:								// �r�b�g�}�b�v�̕`��
		BeginPaint( hwnd, &ps );
		SetDIBitsToDevice( ps.hdc, 0, 0,			// �R�s�[��x,y���W
					pimg -> bih.biWidth,			// DIB�̕�
					pimg -> bih.biHeight,			// DIB�̍���
					0, 0,							// DIB�̍��W 
					0,								// ������
					pimg -> bih.biHeight,			// ��������
					pimg -> lpBmpData,
					(BITMAPINFO *)&( pimg -> bih),	// BITMAPINFO�ɃL���X�g
					DIB_RGB_COLORS );
        EndPaint( hwnd, &ps );
		return 0;

	case WM_DESTROY:							// �X���b�h��
		PostQuitMessage( 0 );					// ���b�Z�[�W���[�v���I��������
		break;			

	default:
		return DefWindowProc( hwnd, msg, wp, lp );
	}
	return 0;
	
}




HRESULT CheckMediaType(AM_MEDIA_TYPE *pmt)
{
    if (pmt == NULL) return E_POINTER;

    // ���W���[ �^�C�v���`�F�b�N����B �����ł́A�r�f�I���������Ă���B
    if (pmt->majortype != MEDIATYPE_Video)
    {
        return VFW_E_INVALIDMEDIATYPE;
    }

    // �T�u�^�C�v���`�F�b�N����B �����ł́A24 �r�b�g RGB ���������Ă���B
    if (pmt->subtype != MEDIASUBTYPE_RGB24)
    {
        return VFW_E_INVALIDMEDIATYPE;
    }

    // �t�H�[�}�b�g �^�C�v����уt�H�[�}�b�g �u���b�N�̃T�C�Y���`�F�b�N����B
    if ((pmt->formattype == FORMAT_VideoInfo) &&
         (pmt->cbFormat >= sizeof(VIDEOINFOHEADER)) &&
         (pmt->pbFormat != NULL))
    {
        // ����ŁA�t�H�[�}�b�g �u���b�N �|�C���^���A�t�H�[�}�b�g�^�C�v GUID �Œ�`���ꂽ
        // �������\���̂ɋ������Ă����S�ł���B
        VIDEOINFOHEADER *pVIH = (VIDEOINFOHEADER*)pmt->pbFormat;
    
        // pVIH (�ȗ�����Ă���) �𒲂ׂ�B ��肪�Ȃ��ꍇ�́AS_OK ��Ԃ��B
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
        // COM��������
        CoInitialize( NULL );
        // FilterGraph�𐶐�
        CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, ( LPVOID * )&pGraphBuilder );
        // SampleGrabber(Filter)�𐶐�
        CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC, IID_IBaseFilter, ( LPVOID * )&pSampleGrabberFilter );
        // Filter����ISampleGrabber�C���^�[�t�F�[�X���擾���܂�
        pSampleGrabberFilter->QueryInterface( IID_ISampleGrabber, ( LPVOID * )&pSampleGrabber );
        // SampleGrabber��ڑ�����t�H�[�}�b�g���w��B
        // �������|�C���g�ł��B
        // �����̎w��̎d���ɂ��SampleGrabber�̑}���ӏ���
        // ����ł��܂��B���̃T���v���̂悤�Ȏw��������
        // ��ʏo�͂̐��O�ŃT���v�����擾�ł��܂��B
        ZeroMemory(&am_media_type, sizeof(am_media_type));
        am_media_type.majortype = MEDIATYPE_Video;
        am_media_type.subtype = MEDIASUBTYPE_RGB24;
        am_media_type.formattype = FORMAT_VideoInfo;
        pSampleGrabber->SetMediaType(&am_media_type);
        // Graph��SampleGrabber Filter��ǉ�
        pGraphBuilder->AddFilter( pSampleGrabberFilter, L"Sample Grabber" );
        // MediaControl�C���^�[�t�F�[�X�擾
        pGraphBuilder->QueryInterface( IID_IMediaControl, (LPVOID *)&pMediaControl );
        // Graph�𐶐��B
        // ������SampleGrabber���܂�Graph�������I�ɍ쐬����܂��B
        pMediaControl->RenderFile( L"resrc/Wildlife.wmv" );
        // �ڑ����擾�B
        // ���̏�����RenderFile�ɂ��Graph���\�����ꂽ���
        // �s���K�v������܂��B
		ShowWindow(hWnd, nCmdShow);

		Sleep(1000);
        pSampleGrabber->GetConnectedMediaType(&am_media_type);
		if (FAILED(CheckMediaType(&am_media_type))) {
			  MessageBoxW( hWnd, L"Error.", L"Error", MB_OK | MB_ICONERROR );
                return;
        }
        VIDEOINFOHEADER *pVideoInfoHeader = (VIDEOINFOHEADER *)am_media_type.pbFormat;
  // �r�b�g�}�b�v����ۑ�
  memcpy( &bmi.bmiHeader, &pVideoInfoHeader->bmiHeader, sizeof( BITMAPINFOHEADER ) );
  // �r�b�g�}�b�v���쐬
  hBitmap = CreateDIBSection( 0, &bmi, DIB_RGB_COLORS, ( VOID ** )&pBuffer, NULL, 0 );
        if( !hBitmap ) {
                MessageBoxW( hWnd, L"Error.", L"Error", MB_OK | MB_ICONERROR );
                return;
        }
        HBITMAP hbOld = ( HBITMAP )SelectObject( hdcMem, hBitmap );
        if( hbOld ) {
                DeleteObject( hbOld );
        }
        // Grab���s������ݒ�
        // SetBufferSamples���s��Ȃ��ƃo�b�t�@����
        // �f�[�^���擾�ł��܂���B
        // �s�K�v�ɕ��ׂ����������Ȃ��ꍇ�ɂ�FALSE�ɂ��Ă����āA
        // �f�[�^���擾�������Ȃ�����ATRUE�ɕς���
        // �Ƃ������@���ł��܂��B
        pSampleGrabber->SetBufferSamples( TRUE );
        // �Đ��J�n
		Sleep(3000);
        pMediaControl->Run();
        // �Đ����Ƀv���O�������I����Ă��܂�Ȃ��悤��
        MessageBoxA( hWnd, "Block Execution", "Block", MB_OK );
        // OK����������BITMAP��ۑ�����
        // �o�b�t�@��p��
        // �K�v�ȃo�b�t�@�T�C�Y���擾
        long nBufferSize = 0;
        pSampleGrabber->GetCurrentBuffer( &nBufferSize, NULL );
        // ���ݕ\������Ă���f����Î~��Ƃ��Ď擾 ( CreateDIBSection() ���Ԃ��� pBuffer �ɏ������� )
        pSampleGrabber->GetCurrentBuffer( &nBufferSize, pBuffer );
        RECT rect;
        GetClientRect( hWnd, &rect );
        InvalidateRect( hWnd, &rect, FALSE );  // ��ʃ��t���b�V�� ( WM_PAINT �������Ă���悤�ɂ��� )
        // ���������
        pSampleGrabber->Release();
        pSampleGrabberFilter->Release();
        pMediaControl->Release();
        pGraphBuilder->Release();
        // COM�I��
        CoUninitialize();
}

bool KTPaintDouga::Init2(HWND hwnd,HINSTANCE hinst) {

	
	// �C���^�[�t�F�[�X�p�̃|�C���^
	// �t�B���^�O���t�p
	IGraphBuilder * pigb = NULL;
	IMediaControl * pimc = NULL;
	IMediaEvent   *pimex = NULL;
	IMediaSeeking *pims  = NULL;	 
	// DV�f�R�[�_�p
	IIPDVDec      *pdv   = NULL;
	// �T���v���O���o�p
	IBaseFilter     *pF  = NULL;
	ISampleGrabber  *pGrab = NULL;  // �����͌�ŉ�����邱�ƁB

	IMG0	img00;
	BYTE  *buffer;
	AM_MEDIA_TYPE   amt;
	WCHAR filename[ MAX_PATH ];
	HRESULT hr;

	// �t�@�C�����̕ϊ�
	MultiByteToWideChar( CP_ACP, 0, "resrc/Wildlife.wmv", -1, filename, MAX_PATH );

	img00.hi = (HINSTANCE)hinst;
	img00.x = 100; img00.y = 100;
	gr_reg();					// �\���p�E�B���h�E�̓o�^

	CoInitialize(NULL);				// COM�̏���

	// FilterGraph�̏�����
	CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
						IID_IGraphBuilder, (void**)&pigb);
	// �O���o�t�B���^�����t�B���^�O���t�ɒǉ�
	CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, 
                    IID_IBaseFilter, (LPVOID *)&pF);
	pF -> QueryInterface( IID_ISampleGrabber, (void **)&pGrab);
	pigb -> AddFilter(pF, L"SamGra");

	// �O���o�t�B���^�̑}���ꏊ�̓���̂��߂̐ݒ�
	ZeroMemory(&amt, sizeof(AM_MEDIA_TYPE));
	amt.majortype  = MEDIATYPE_Video;
	amt.subtype    = MEDIASUBTYPE_RGB24;
	amt.formattype = FORMAT_VideoInfo; 
	pGrab -> SetMediaType(&amt);

	// �t�B���^�O���t�̃C���^�[�t�F�[�X�𓾂�
	pigb -> QueryInterface( IID_IMediaControl, (void**)&pimc );
	pigb -> QueryInterface( IID_IMediaEvent, (void **)&pimex);
	pigb -> QueryInterface( IID_IMediaSeeking, (void **)&pims);

	// �Đ�����t�@�C�����w��
	hr = pigb -> RenderFile( filename, NULL );
	printf("RenderFile hr %x\n",hr);
	if(hr != 0)		return false;

	// DV�f�R�[�h�̉𑜓x��FULL�ɐݒ肷��
	IEnumFilters *pEnum = NULL;
	IBaseFilter *pFilter;
	ULONG cFetched;
	int nnn;
	pigb ->EnumFilters(&pEnum);
	// �g�p�����t�B���^�𒲂ׂ�
	while(pEnum->Next(1, &pFilter, &cFetched) == S_OK){
        	FILTER_INFO FilterInfo;
        	char szName[256];
        
        	pFilter->QueryFilterInfo(&FilterInfo);
        	WideCharToMultiByte(CP_ACP, 0, FilterInfo.achName, 
							-1, szName, 256, 0, 0);
		printf("�g�p�t�B���^ = %s \n",szName); 
        	if( ! strcmp( szName, "DV Video Decoder") ){
			// DV�f�R�[�_�Ȃ�𑜓x��FULL�ɐݒ�
			pFilter ->QueryInterface(IID_IIPDVDec, (void **)& pdv);
			pdv -> get_IPDisplay( &nnn);
			printf(" ���݃f�R�[�h�𑜓x IP = %d \n",nnn);
			pdv -> put_IPDisplay( 1000);
			pdv -> get_IPDisplay( &nnn);
			printf(" ���݃f�R�[�h�𑜓x IP = %d \n",nnn);
		}
		FilterInfo.pGraph->Release();
        pFilter->Release();
	}
	pEnum->Release();

	// �r�b�g�}�b�v���̎擾
	pGrab->GetConnectedMediaType(&amt); 
	// �r�f�I �w�b�_�[�ւ̃|�C���^���l������B
	printf("amt.lSampleSize = %d \n", amt.lSampleSize );
	VIDEOINFOHEADER *pVideoHeader = 
			(VIDEOINFOHEADER*)amt.pbFormat;
	// �r�f�I �w�b�_�[�ɂ́A�r�b�g�}�b�v��񂪊܂܂��B
	// �r�b�g�}�b�v���� BITMAPINFO �\���̂ɃR�s�[����B
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
	printf("�J�n�t���[��?");
	//scanf("%d",&n0);
	n0=0;
	printf("�I���t���[��?");
	//scanf("%d",&n1);
	n1 = 1000;
	LONGLONG nn = n0;
	gr_init(&img00);				// �O���t�B�b�N�E�B���h�E�𐶐�
	pims -> SetTimeFormat(&(TIME_FORMAT_FRAME)) ;	
						// �V�[�N���t���[���P�ʂōs���悤�ݒ�
	pGrab->SetBufferSamples(TRUE);	// �O���u�J�n
	for(int i = n0; i <= n1;i++){
		// �V�[�N
		pims ->SetPositions(&nn,AM_SEEKING_AbsolutePositioning,
					&nn,AM_SEEKING_AbsolutePositioning);
		pimc -> Run();				// �����_�����O	
		printf("run at %d\n", nn);
		pimex -> WaitForCompletion(100, &evCode);
						// �����_�����O������҂�
		pGrab -> GetCurrentBuffer(&n, (long *)buffer);
									// �O���u
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

    // �t���X�N���[���p�ɏ������p�����[�^��ݒ�
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
        // Filter����ISampleGrabber�C���^�[�t�F�[�X���擾���܂�
        pSampleGrabberFilter->QueryInterface( IID_ISampleGrabber, ( LPVOID * )&pSampleGrabber );
        // SampleGrabber��ڑ�����t�H�[�}�b�g���w��B
        // �������|�C���g�ł��B
        // �����̎w��̎d���ɂ��SampleGrabber�̑}���ӏ���
        // ����ł��܂��B���̃T���v���̂悤�Ȏw��������
        // ��ʏo�͂̐��O�ŃT���v�����擾�ł��܂��B
        ZeroMemory(&am_media_type, sizeof(am_media_type));
        am_media_type.majortype = MEDIATYPE_Video;
        am_media_type.subtype = MEDIASUBTYPE_RGB24;
        am_media_type.formattype = FORMAT_VideoInfo;
        pSampleGrabber->SetMediaType(&am_media_type);
		 HANDLE hFile = CreateFile("GraphLog.txt", GENERIC_WRITE, 0, NULL, 
OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
		pGB->SetLogFile((DWORD_PTR)hFile);
        // Graph��SampleGrabber Filter��ǉ�
        pGB->AddFilter( pSampleGrabberFilter, L"Sample Grabber" );
        // MediaControl�C���^�[�t�F�[�X�擾
        pGB->QueryInterface( IID_IMediaControl, (LPVOID *)&pMediaControl );
        // Graph�𐶐��B
        // ������SampleGrabber���܂�Graph�������I�ɍ쐬����܂��B
     
		
		
			IVMRFilterConfig* pConfig = 0;
	  // �����_�����O ���[�h�ƃX�g���[���̐���ݒ肷��B
  
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

        // �����̃r�f�I �X�g���[�����K�v�ȏꍇ�A�܂��͐ÓI�r�b�g�}�b�v���r�f�I���
        // �~�L�V���O����ꍇ�́AVMR-7 ���~�L�V���O ���[�h�ɐݒ肷��B
        // (VMR-9 �̓f�t�H���g�� 4 �̓��͂����~�L�V���O ���[�h�ɂȂ�B)
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
        // �ڑ����擾�B
        // ���̏�����RenderFile�ɂ��Graph���\�����ꂽ���
        // �s���K�v������܂��B
		//5ShowWindow(hwnd, nCmdShow);

		Sleep(1000);
        pSampleGrabber->GetConnectedMediaType(&am_media_type);
		if (FAILED(CheckMediaType(&am_media_type))) {
			  MessageBoxW( hwnd, L"Error.", L"Error", MB_OK | MB_ICONERROR );
                return false;
        }
        VIDEOINFOHEADER *pVideoInfoHeader = (VIDEOINFOHEADER *)am_media_type.pbFormat;
  // �r�b�g�}�b�v����ۑ�
  memcpy( &bmi.bmiHeader, &pVideoInfoHeader->bmiHeader, sizeof( BITMAPINFOHEADER ) );
  // �r�b�g�}�b�v���쐬
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
        // Grab���s������ݒ�
        // SetBufferSamples���s��Ȃ��ƃo�b�t�@����
        // �f�[�^���擾�ł��܂���B
        // �s�K�v�ɕ��ׂ����������Ȃ��ꍇ�ɂ�FALSE�ɂ��Ă����āA
        // �f�[�^���擾�������Ȃ�����ATRUE�ɕς���
        // �Ƃ������@���ł��܂��B
        pSampleGrabber->SetBufferSamples( TRUE );
      
	


	

		Sleep(1000);

		SetForegroundWindow(hwnd);

		pMediaControl->Run();
        // �Đ����Ƀv���O�������I����Ă��܂�Ȃ��悤��
        MessageBox( hwnd, "Block Execution", "Block", MB_OK );

		#ifdef _DEBUG
		DumpGraph(pGB);
		#endif
        // OK����������BITMAP��ۑ�����
        // �o�b�t�@��p��
        // �K�v�ȃo�b�t�@�T�C�Y���擾
        long nBufferSize = 0;
        pSampleGrabber->GetCurrentBuffer( &nBufferSize, NULL );
        // ���ݕ\������Ă���f����Î~��Ƃ��Ď擾 ( CreateDIBSection() ���Ԃ��� pBuffer �ɏ������� )
        pSampleGrabber->GetCurrentBuffer( &nBufferSize, pBuffer );
        RECT rect;
        GetClientRect( hwnd, &rect );
        InvalidateRect( hwnd, &rect, FALSE );  // ��ʃ��t���b�V�� ( WM_PAINT �������Ă���悤�ɂ��� )
        // ���������
        pSampleGrabber->Release();
        pSampleGrabberFilter->Release();
        pMediaControl->Release();
        pGB->Release();
		pGB = 0;
        // COM�I��
        CoUninitialize();

		 PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
  // CreateDIBSection() �ɂ���č쐬���ꂽ�r�b�g�}�b�v��`��
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
	  // �����_�����O ���[�h�ƃX�g���[���̐���ݒ肷��B
  
	// *Cont = NULL;
	int dwNumStreams = 1;
	bool fBlendAppImage = false;
	
    hr = pBF->QueryInterface(IID_IVMRFilterConfig, (void**)&pConfig);
    if (SUCCEEDED(hr)) 
    {
        pConfig->SetRenderingMode(VMR9Mode_Windowless);

        // �����̃r�f�I �X�g���[�����K�v�ȏꍇ�A�܂��͐ÓI�r�b�g�}�b�v���r�f�I���
        // �~�L�V���O����ꍇ�́AVMR-7 ���~�L�V���O ���[�h�ɐݒ肷��B
        // (VMR-9 �̓f�t�H���g�� 4 �̓��͂����~�L�V���O ���[�h�ɂȂ�B)
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
        // Filter����ISampleGrabber�C���^�[�t�F�[�X���擾���܂�
        pSampleGrabberFilter->QueryInterface( IID_ISampleGrabber, ( LPVOID * )&pSampleGrabber );




  hr = pGB->QueryInterface(IID_IVideoWindow, (LPVOID*)&pVideoWindow);
 
    // �O���t���쐬����B �d�v : ���̕�������V�X�e����̃t�@�C���ɒu��������B  
 //   hr = pGraph->RenderFile(L"G:\\sampleMovie\\avi_divx5_mp3.avi", NULL);  
 
    // �`���E�B���h�E�̐ݒ�
   // HWND hWnd = reinterpret_cast< HWND & >(this->Handle);
    hr = pVideoWindow->put_Owner((OAHWND)hwnd);
    hr = pVideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPSIBLINGS);
    //hr = pVideoWindow->SetWindowPosition(0,0,320,240);
    hr = pVideoWindow->put_Visible(OATRUE);
 












        // SampleGrabber��ڑ�����t�H�[�}�b�g���w��B
        // �������|�C���g�ł��B
        // �����̎w��̎d���ɂ��SampleGrabber�̑}���ӏ���
        // ����ł��܂��B���̃T���v���̂悤�Ȏw��������
        // ��ʏo�͂̐��O�ŃT���v�����擾�ł��܂��B
        ZeroMemory(&am_media_type, sizeof(am_media_type));
        am_media_type.majortype = MEDIATYPE_Video;
        am_media_type.subtype = MEDIASUBTYPE_RGB24;
        am_media_type.formattype = FORMAT_VideoInfo;
        pSampleGrabber->SetMediaType(&am_media_type);
		 HANDLE hFile = CreateFile("GraphLog.txt", GENERIC_WRITE, 0, NULL, 
OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
		pGB->SetLogFile((DWORD_PTR)hFile);
        // Graph��SampleGrabber Filter��ǉ�
        pGB->AddFilter( pSampleGrabberFilter, L"Sample Grabber" );
        // MediaControl�C���^�[�t�F�[�X�擾
        pGB->QueryInterface( IID_IMediaControl, (LPVOID *)&pMediaControl );
        // Graph�𐶐��B
        // ������SampleGrabber���܂�Graph�������I�ɍ쐬����܂��B
     
	
		
		
			pGB->QueryInterface(IID_IMediaEventEx,
		(LPVOID *)&pMediaEventEx);
	pMediaEventEx->SetNotifyWindow((OAHWND)hwnd,
					WM_GRAPH_NOTIFY, NULL);

		
		
		hr = pGB->QueryInterface(IID_IVideoWindow, (LPVOID*)&pVideoWindow);
 
    // �O���t���쐬����B �d�v : ���̕�������V�X�e����̃t�@�C���ɒu��������B  
   // hr = pGraph->RenderFile(L"G:\\sampleMovie\\avi_divx5_mp3.avi", NULL);  
 
  
 
		
		
		
		pMediaControl->RenderFile( L"resrc/hundred.wmv" );
        // �ڑ����擾�B
        // ���̏�����RenderFile�ɂ��Graph���\�����ꂽ���
        // �s���K�v������܂��B
	//	ShowWindow(hwnd, nCmdShow);


		  // �`���E�B���h�E�̐ݒ�
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
  // �r�b�g�}�b�v����ۑ�
  memcpy( &bmi.bmiHeader, &pVideoInfoHeader->bmiHeader, sizeof( BITMAPINFOHEADER ) );
  // �r�b�g�}�b�v���쐬
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
        // Grab���s������ݒ�
        // SetBufferSamples���s��Ȃ��ƃo�b�t�@����
        // �f�[�^���擾�ł��܂���B
        // �s�K�v�ɕ��ׂ����������Ȃ��ꍇ�ɂ�FALSE�ɂ��Ă����āA
        // �f�[�^���擾�������Ȃ�����ATRUE�ɕς���
        // �Ƃ������@���ł��܂��B
        pSampleGrabber->SetBufferSamples( TRUE );
      
	


	

	//	Sleep(1000);

		SetForegroundWindow(hwnd);

	//	pMediaControl->Run();
        // �Đ����Ƀv���O�������I����Ă��܂�Ȃ��悤��
       // MessageBox( hwnd, "Block Execution", "Block", MB_OK );

		#ifdef _DEBUG
		DumpGraph(pGB);
		#endif
        // OK����������BITMAP��ۑ�����
        // �o�b�t�@��p��
        // �K�v�ȃo�b�t�@�T�C�Y���擾

		/*
        long nBufferSize = 0;
        pSampleGrabber->GetCurrentBuffer( &nBufferSize, NULL );
        // ���ݕ\������Ă���f����Î~��Ƃ��Ď擾 ( CreateDIBSection() ���Ԃ��� pBuffer �ɏ������� )
        pSampleGrabber->GetCurrentBuffer( &nBufferSize, pBuffer );
        RECT rect;
        GetClientRect( hwnd, &rect );
        InvalidateRect( hwnd, &rect, FALSE );  // ��ʃ��t���b�V�� ( WM_PAINT �������Ă���悤�ɂ��� )
        // ���������
        pSampleGrabber->Release();
        pSampleGrabberFilter->Release();
		*/
        pMediaControl->Release();
		/*
        pGB->Release();
		pGB = 0;
        // COM�I��
        CoUninitialize();
		*/

		/*
		 PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
  // CreateDIBSection() �ɂ���č쐬���ꂽ�r�b�g�}�b�v��`��
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
// �C�x���g����������ƌĂяo�����
LRESULT
KTPaintDouga::OnGraphNotify(WPARAM wParam, LPARAM lParam)
{
 long evCode;
 LONG param1, param2;

 // �C�x���g��S�Ď擾
 while (SUCCEEDED(pMediaEventEx->GetEvent(&evCode,
					&param1, &param2, 0))) {
	pMediaEventEx->FreeEventParams(evCode, param1, param2);

	switch (evCode) {
	case EC_ACTIVATE:
		InvalidateRect(paint->parent_window, NULL, TRUE);
		MessageBox( paint->parent_window, "Block Execution", "Block", MB_OK);
		break;
		case EC_COMPLETE:
			// �Đ��I��
			MessageBox( paint->parent_window, "Block Execution", "Block", MB_OK);
			break;
	}
 }

 return NOERROR;
}

void KTPaintDouga::Run(HWND hwnd,HDC hdc){
		if (pMediaCont) {
		HRESULT hr = pMediaCont->Run();
	//	HWND child_window=GetWindow(hwnd, GW_CHILD);//hwnd�͐e�E�B���h�E
	//	pVideoWindow->rePaint(child_window);
	//	InvalidateRect(child_window, 0, TRUE);
		Cont->RepaintVideo(hwnd, hdc);
		if (FAILED(hr)) {
			Beep(3,1);
		}
		}
	}