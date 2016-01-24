#ifndef KTROBOGRAPHICS_H
#define KTROBOGRAPHICS_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

#include "xnamath.h"

#include "D3D11.h"
#include "D3DX11.h"
#include "D3DX11core.h"
#include "DxErr.h"

namespace KTROBO {


class MyBlob : public ID3DBlob {
public:
	size_t size;
	unsigned char* pointer;
	MyBlob(unsigned int s) {
		size = s;
		pointer = new unsigned char[s];
	}
	virtual ~MyBlob() {
		Release();
	}
	LPVOID _stdcall GetBufferPointer() { return (void*)pointer;}
	
	SIZE_T _stdcall GetBufferSize() {return size;}
	HRESULT _stdcall QueryInterface(const IID &riid, void** ppvObject) {return E_NOINTERFACE;}
	ULONG _stdcall AddRef() {return 0;}
	ULONG _stdcall Release() {if (pointer) {delete[] pointer;pointer =0;} return 0;}
};
HRESULT CompileShaderFromFile(char* filename, char* entrypoint, char* shadermodel, ID3DBlob** out, bool use_compiled=false);


class Graphics
{
private:
	unsigned int width;
	unsigned int height;
	bool is_copied;
	HWND h;
	D3D_DRIVER_TYPE driver_type;
	IDXGISwapChain* p_swapchain;
	ID3D11Device* p_device;
	D3D_FEATURE_LEVEL featurelevel;
	ID3D11DeviceContext* p_devicecontext;
	ID3D11Resource* p_backbuffer;
	ID3D11RenderTargetView* p_rendertargetview;
	D3D11_VIEWPORT vp;
	bool createDeviceAndSwapChain();
	bool createBackBuffer();
	bool createViewPort();
public:
	bool Init(HWND hwnd);
	void Release();
	HWND getHWND() {return h;}
	const D3D11_VIEWPORT* getViewPort() {return &vp;} 

	IDXGISwapChain* getSwapChain() {return p_swapchain;}
	ID3D11DeviceContext* getDeviceContext() {return p_devicecontext;}
	ID3D11RenderTargetView* getRenderTargetView() {return p_rendertargetview;}
	ID3D11Device* getDevice() {return p_device;}
	unsigned int getScreenHeight() {return height;}
	unsigned int getScreenWidth() {return width;}

	Graphics* makeGraphicsOfNewDeviceContext();

	Graphics(void);
	~Graphics(void);
};
}


#endif