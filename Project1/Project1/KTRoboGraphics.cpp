#include "KTRoboGraphics.h"
#include "D3Dcompiler.h"
#include "KTRoboGameError.h"
#include <string>

using namespace KTROBO;
using namespace std;




HRESULT KTROBO::CompileShaderFromFile(char* filename, char* entrypoint, char* shadermodel, ID3DBlob** out, bool use_compiled)
{
	HRESULT hr = S_OK;
	ID3DBlob* pErrorBlob;

	DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;// | D3DCOMPILE_SKIP_OPTIMIZATION;
#if defined(DEBUG) || defined(_DEBUG)
	shader_flags |= D3DCOMPILE_DEBUG;
#endif

	if (use_compiled) {
		
	    //バイナリファイルを読み込む//
		FILE* fp;
		fopen_s(&fp, filename, "rb");
		if( fp == NULL){
			return hr = INVALID_FILE_SIZE;
		}
		fseek(fp, 0, SEEK_END);
		long cso_sz = ftell(fp);
		fseek(fp, 0, SEEK_SET);
	
		if (cso_sz > 20000000) {
			fclose(fp);
			return hr = INVALID_FILE_SIZE;
		}
		//unsigned char* cso_data = new unsigned char[cso_sz];
		//fread(cso_data, cso_sz, 1, fp);
		//fclose(fp);
		*out = new KTROBO::MyBlob(cso_sz);
		//cso_sz, out;

	    if(*out != NULL){
		    fread((*out)->GetBufferPointer(), 1, cso_sz, fp);
		}

	    fclose(fp);
		return S_OK;

	}
	// fxc.exe simplemesh_i_compute_cm.fx /T cs_5_0 /E CalcCS /Ges /Fo simplemesh_i_compute_cm.ps
	hr = D3DX11CompileFromFileA(filename, NULL,NULL,
		entrypoint,shadermodel, shader_flags,0,NULL,
		out,&pErrorBlob,NULL);
	if (FAILED(hr)) {
	//	if (pErrorBlob != NULL) {

		if (pErrorBlob) {
			OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
			char buf[4096];
			memset(buf,0,4096);
			sprintf_s(buf,1024,"shader:%s",pErrorBlob->GetBufferPointer());
			buf[4095] = '\0';
			pErrorBlob->Release();
			pErrorBlob = 0;
		
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, string(buf));
		}
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "no shader");
//		}
	}

	if (pErrorBlob) {
		pErrorBlob->Release();
		pErrorBlob = 0;
	}
	return hr;
}

Graphics::Graphics(void)
{
	width = 0;
	height = 0;
	h = 0;
	p_swapchain = 0;
	p_device = 0;
	p_devicecontext = 0;
	p_backbuffer = 0;
	p_rendertargetview = 0;
	memset(&featurelevel,0,sizeof(D3D_FEATURE_LEVEL));
	memset(&driver_type,0,sizeof(D3D_DRIVER_TYPE));
	memset(&vp, 0 , sizeof(vp));
}


Graphics::~Graphics(void)
{
}


bool Graphics::Init(HWND hwnd) {

	h = hwnd;
	bool r = createDeviceAndSwapChain();

	if (!r) return false;

	r = createBackBuffer();

	if (!r) return false;

	r = createViewPort();

	if (!r) return false;

	return true;
}

bool Graphics::createDeviceAndSwapChain() {

	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(h, &rc);
	width  = rc.right - rc.left;
	height = rc.bottom - rc.top;
	unsigned int create_device_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) || defined(_DEBUG)
	create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driver_types[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	unsigned int num_driver_types = sizeof(driver_types) / sizeof(driver_types[0]);

	D3D_FEATURE_LEVEL feature_levels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		/*D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1*/
	};
	unsigned int num_feature_levels = sizeof(feature_levels) / sizeof(feature_levels[0]);

	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0, sizeof(sd));

	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = h;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	// デバイスとスワップチェインを作成する
	for (unsigned int idx = 0; idx < num_driver_types; idx++) {
		driver_type = driver_types[idx];
		hr = D3D11CreateDeviceAndSwapChain(NULL, driver_type, NULL, 
			create_device_flags, feature_levels, num_feature_levels, D3D11_SDK_VERSION,
			&sd, &p_swapchain, &p_device, &featurelevel, &p_devicecontext);
		if (SUCCEEDED(hr)) {
			break;
		}
	}
	if (FAILED(hr)) {
		return false;
	}


	return true;
}

bool Graphics::createBackBuffer() {

	HRESULT hr = S_OK;

	hr = p_swapchain->GetBuffer(0, __uuidof( ID3D11Texture2D), (LPVOID*) &p_backbuffer);
	if (FAILED(hr)) {
		return false;
	}

	hr = p_device->CreateRenderTargetView(p_backbuffer,NULL,&p_rendertargetview);

	if (FAILED(hr)) {
		return false;
	}

	p_devicecontext->OMSetRenderTargets(1, &p_rendertargetview,NULL);



	return true;
}

bool Graphics::createViewPort() {

	//D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	p_devicecontext->RSSetViewports(1, &vp);
	return true;
}
void Graphics::Release() {
	

	// コンテキストの開放
	if (p_devicecontext) {
		p_devicecontext->ClearState();
		p_devicecontext->Release();
		p_devicecontext = 0;
	}

	if (p_backbuffer) {
		p_backbuffer->Release();
		p_backbuffer = 0;
	}

	if (p_rendertargetview) {
		p_rendertargetview->Release();
		p_rendertargetview = 0;
	}

	if (p_swapchain) {
		p_swapchain->SetFullscreenState(FALSE,0);
		p_swapchain->Release();
		p_swapchain = 0;
	}
	if (p_device) {
		p_device->Release();
		p_device = 0;
	}


	
	return;
}



