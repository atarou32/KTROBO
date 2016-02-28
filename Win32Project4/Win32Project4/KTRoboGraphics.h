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
#include "MyDefine.h"
//#include "MyTextureLoader.h"
#include "MyButukari3D.h"
#include "KTPaintSheet.h"

class MyTextureLoader;

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



#define KTROBO_GRAPHICS_SHADER_FILENAME "resrc/shader/renderlinesimple.fx"
#define KTROBO_GRAPHICS_SHADER_VS "VSFunc"
#define KTROBO_GRAPHICS_SHADER_GS "GSFunc"
#define KTROBO_GRAPHICS_SHADER_PS "PSFunc"
#define KTROBO_GRAPHICS_RENDER_STRUCT_SIZE 32

#define KTROBO_GRAPHICS_SHADER_FILENAME_PEN "resrc/shader/renderlinepen.fx"
#define KTROBO_GRAPHICS_RENDER_STRUCT_SIZE_PEN KTPAINT_SHEET_LINE_MAX


#define KTROBO_GRAPHICS_SHADER_FILENAME_TEX "resrc/shader/rendertex.fx"




class GRAPHICS_RENDER_STRUCT {
public:
	float x;
	float y;
	float z;
	DWORD color;
};

class GRAPHICS_RENDER_TEX_STRUCT {
public:
	float x;
	float y;
	float z;
	float tex_x;
	float tex_y;
};

class GRAPHICS_RENDER_PEN_INFO_STRUCT {
public:
	unsigned int screen_x;
	unsigned int screen_y;
	int transx;
	int transy;
	
	MYMATRIX pen_width[KTPAINT_PEN_NUM_MAX];
	float zoom;

	float offset;
	float offset2;
	float offset3;
};


class GRAPHICS_RENDER_PEN_STRUCT {
public:
	unsigned short x;
	unsigned short y;
	char dx;
	char dy;
	unsigned char width_and_nwidth;
	unsigned char pen_index;
	int offset;
	int offset2;
};

class GRAPHICS_INFO_STRUCT {
public:
	MYMATRIX world;
	MYMATRIX view;
	MYMATRIX proj;
};

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
	MYMATRIX proj;
public:
	static ID3D11SamplerState* p_sampler;
	static MYSHADERSTRUCT mss;
	static ID3D11Buffer* render_buffer;
	static GRAPHICS_INFO_STRUCT info;
	static ID3D11Buffer* info_buffer;


	static MYSHADERSTRUCT mss_for_pen;
	static ID3D11Buffer* render_buffer_pen;
	static ID3D11Buffer* index_buffer_pen;
	static ID3D11Buffer* render_buffer_cbuf;

	static MYSHADERSTRUCT mss_for_tex;
	static ID3D11Buffer* render_buffer_tex;
	static ID3D11DepthStencilView* pDepthStencilView;
	static ID3D11Texture2D* pDepthStencil;
	static void InitMSS(Graphics* g);
	static void loadShader(Graphics* g, MYSHADERSTRUCT* s, char* shader_filename, char* vs_func_name, char* gs_func_name,
								char* ps_func_name, unsigned int ds_width,unsigned int ds_height,
								D3D11_INPUT_ELEMENT_DESC* layout, int numoflayout, bool blend_enable);
	static void Del();

	static void drawOBB(KTROBO::Graphics* g, DWORD color, MYMATRIX* world, MYMATRIX* view, MYMATRIX* proj, OBB* rec);
	static void drawOBBFill(KTROBO::Graphics* g, DWORD color, MYMATRIX* world, MYMATRIX *view, MYMATRIX* proj, OBB* rec);
	static void drawRAY(KTROBO::Graphics* g, DWORD color, MYMATRIX* world, MYMATRIX* view, MYMATRIX* proj, RAY* ray, int length);
	static void drawTriangle(KTROBO::Graphics* g, DWORD color, MYMATRIX* world, MYMATRIX* view, MYMATRIX* proj, MYVECTOR3* p, MYVECTOR3* q, MYVECTOR3* r);
	static void drawPen(KTROBO::Graphics* g, KTPAINT_penline* penlines, int penline_max);
	static void setPenInfo(KTROBO::Graphics* g, short transx, short transy, float zoom, KTPAINT_pen* pens);
	static void drawTex(KTROBO::Graphics* g, MyShaderResourceView tex_class, short transx, short transy, float zoom,KTPAINT_pen* pens);

	MYMATRIX* getProj() {return &proj;}
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