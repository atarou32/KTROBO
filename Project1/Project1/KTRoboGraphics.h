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

class MYSHADERSTRUCT {
public:
	ID3D11ComputeShader* cs;
	ID3D11VertexShader* vs;
	ID3D11GeometryShader* gs;
	ID3D11PixelShader* ps;
	ID3D11InputLayout* vertexlayout;
	ID3D11BlendState* blendstate;
	ID3D11RasterizerState* rasterstate;
	ID3D11Texture2D* depthstencil;
	ID3D11DepthStencilView* depthstencilview;

	MYSHADERSTRUCT::MYSHADERSTRUCT() {
		vs = 0;
		cs = 0;
		gs = 0;
		ps = 0;
		vertexlayout = 0;
		blendstate = 0;
		rasterstate = 0;
		depthstencil = 0;
		depthstencilview = 0;
	}

	MYSHADERSTRUCT::~MYSHADERSTRUCT() {
		Del();
	}

	void Del() {
		if (cs) {
			cs->Release();
			cs = 0;
		}

		if (vs) {
			vs->Release();
			vs = 0;
		}
		if (gs ) {
			gs->Release();
			gs = 0;
		}
		if (ps ) {
			ps->Release();
			ps = 0;
		}
		if (vertexlayout) {
			vertexlayout->Release();
			vertexlayout = 0;
		}
		if (blendstate) {
			blendstate->Release();
			blendstate = 0;
		}
		if (rasterstate) {
			rasterstate->Release();
			rasterstate = 0;
		}
		if (depthstencilview) {
			depthstencilview->Release();
			depthstencilview = 0;
		}
		if (depthstencil) {
			depthstencil->Release();
			depthstencil = 0;
		}
	}

};

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
	MyTextureLoader* tex_loader;
	bool createDeviceAndSwapChain();
	bool createBackBuffer();
	bool createViewPort();
public:
	bool Init(HWND hwnd);
	void Release();

	void setTexLoader(MyTextureLoader* tex) {
		tex_loader = tex;
	}

	MyTextureLoader* getTexLoader() {
		return tex_loader;
	}

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