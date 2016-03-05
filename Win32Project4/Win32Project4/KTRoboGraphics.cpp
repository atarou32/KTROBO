#include "KTRoboGraphics.h"
#include "D3Dcompiler.h"
#include "KTRoboGameError.h"
#include <string>
#include "KTRoboCS.h"

using namespace KTROBO;
using namespace std;



MYSHADERSTRUCT Graphics::mss;
ID3D11Buffer* Graphics::render_buffer=0;
GRAPHICS_INFO_STRUCT Graphics::info;
ID3D11Buffer* Graphics::info_buffer=0;
ID3D11SamplerState* Graphics::p_sampler=0;
ID3D11Buffer* Graphics::render_buffer_pen = 0;
ID3D11Buffer* Graphics::render_buffer_pen_special = 0;
MYSHADERSTRUCT Graphics::mss_for_pen;
MYSHADERSTRUCT Graphics::mss_for_pen_special;
ID3D11Buffer* Graphics::index_buffer_pen=0;
ID3D11Buffer* Graphics::render_buffer_cbuf=0;
MYSHADERSTRUCT Graphics::mss_for_tex;
ID3D11Buffer* Graphics::render_buffer_tex=0;
ID3D11DepthStencilView* Graphics::pDepthStencilView = 0;
ID3D11Texture2D* Graphics::pDepthStencil=0;

void Graphics::InitMSS(Graphics* g) {
		// renderのためのvertexbufferを作る

	D3D11_BUFFER_DESC bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(GRAPHICS_RENDER_STRUCT)*KTROBO_GRAPHICS_RENDER_STRUCT_SIZE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA hSubResourceData;

	HRESULT hr = g->getDevice()->CreateBuffer(&bd, NULL ,&(render_buffer));
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}

	// 各種shaderstructのロード

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLORDAYO",0,DXGI_FORMAT_B8G8R8A8_UNORM, 0,12,D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	
	loadShader(g, &mss, KTROBO_GRAPHICS_SHADER_FILENAME, KTROBO_GRAPHICS_SHADER_VS, KTROBO_GRAPHICS_SHADER_GS,
		KTROBO_GRAPHICS_SHADER_PS, g->getScreenWidth(), g->getScreenHeight(),
								layout, 2, true);

	memset(&info, 0,  sizeof(GRAPHICS_INFO_STRUCT));
	D3D11_BUFFER_DESC des;
	des.ByteWidth = sizeof(GRAPHICS_INFO_STRUCT);
	des.Usage = D3D11_USAGE_DEFAULT;
	des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	des.CPUAccessFlags = 0;
	des.MiscFlags = 0;
	des.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA idat;
	idat.pSysMem = &info;
	idat.SysMemPitch = 0;
	idat.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer(&des, &idat, &info_buffer);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
	}

	D3D11_SAMPLER_DESC descS;
	memset(&descS, 0, sizeof(descS));
	descS.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
	
	descS.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	descS.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	descS.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	descS.BorderColor[0] = 0;
	descS.BorderColor[1] = 0;
	descS.BorderColor[2] = 0;
	descS.BorderColor[3] = 0;
	descS.ComparisonFunc = D3D11_COMPARISON_NEVER;
	descS.MaxAnisotropy = 1;
	descS.MinLOD = -D3D11_FLOAT32_MAX;
	descS.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g->getDevice()->CreateSamplerState(&descS, &p_sampler);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "sampler make error");;
		
	}





	D3D11_INPUT_ELEMENT_DESC layout2[] = {
		{"POSTEN", 0, DXGI_FORMAT_R16G16_UINT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"INFO1D",0,DXGI_FORMAT_R8G8_SINT, 0,4,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"INFO2D",0,DXGI_FORMAT_R8G8_UINT,0,6,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"OFFSET",0,DXGI_FORMAT_R32G32_UINT, 0,8,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	
	loadShader(g, &mss_for_pen, KTROBO_GRAPHICS_SHADER_FILENAME_PEN, KTROBO_GRAPHICS_SHADER_VS, KTROBO_GRAPHICS_SHADER_GS,
		KTROBO_GRAPHICS_SHADER_PS, g->getScreenWidth(), g->getScreenHeight(),
								layout2, 4, true);

	bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(GRAPHICS_RENDER_PEN_STRUCT)*KTROBO_GRAPHICS_RENDER_STRUCT_SIZE_PEN;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	hr = g->getDevice()->CreateBuffer(&bd, NULL ,&(render_buffer_pen));
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}


	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.ByteWidth = sizeof(unsigned short) * KTROBO_GRAPHICS_RENDER_STRUCT_SIZE_PEN*3;
	hBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = 0;
	hSubResourceData;

	unsigned short* indexs = new unsigned short[KTROBO_GRAPHICS_RENDER_STRUCT_SIZE_PEN*3];
	memset(indexs, 0, sizeof(unsigned short)*KTROBO_GRAPHICS_RENDER_STRUCT_SIZE_PEN*3);
	
	int temp_count = 0;
	for (int i=0;i<KTROBO_GRAPHICS_RENDER_STRUCT_SIZE_PEN;i++) {
		int now_id = i;
		indexs[3*temp_count] = (unsigned short)now_id;
		indexs[3*temp_count+1] = (unsigned short)now_id;
		indexs[3*temp_count+2] = (unsigned short)now_id;
		temp_count++;
		if (temp_count >= KTROBO_GRAPHICS_RENDER_STRUCT_SIZE_PEN) {
			// break;
			break;
		}
	}

    hSubResourceData.pSysMem = indexs;
	hSubResourceData.SysMemPitch = 0;
	hSubResourceData.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer( &hBufferDesc, &hSubResourceData, &index_buffer_pen );
	if( FAILED( hr ) ) {
	   delete[] indexs;
	   Del();
	   throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "index buffer make error");
    }

	delete[] indexs;



	des;
	des.ByteWidth = sizeof(GRAPHICS_RENDER_PEN_INFO_STRUCT);
	des.Usage = D3D11_USAGE_DEFAULT;
	des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	des.CPUAccessFlags = 0;
	des.MiscFlags = 0;
	des.StructureByteStride = 0;

	GRAPHICS_RENDER_PEN_INFO_STRUCT cbuf;
	cbuf.screen_x =(unsigned short) g->getScreenWidth();
	cbuf.screen_y = (unsigned short) g->getScreenHeight();
	cbuf.transx  = 0;
	cbuf.transy = 0;
	cbuf.zoom = 1;
	cbuf.offset = 0;
	cbuf.offset2 = 0;
	cbuf.offset3 = 0;

	for (int i=0;i<KTPAINT_PEN_NUM_MAX;i++) {
		cbuf.pen_width[i] = MYMATRIX(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
	}
	hSubResourceData.pSysMem = &cbuf;
	hSubResourceData.SysMemPitch = 0;
	hSubResourceData.SysMemSlicePitch = 0;

	hr = g->getDevice()->CreateBuffer(&des, &hSubResourceData, &Graphics::render_buffer_cbuf);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
	}

	D3D11_INPUT_ELEMENT_DESC layout3[] = {
		{"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT, 0,12,D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	
	loadShader(g, &mss_for_tex, KTROBO_GRAPHICS_SHADER_FILENAME_TEX, KTROBO_GRAPHICS_SHADER_VS, KTROBO_GRAPHICS_SHADER_GS,
		KTROBO_GRAPHICS_SHADER_PS, g->getScreenWidth(), g->getScreenHeight(),
								layout3, 2, true);

	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(GRAPHICS_RENDER_TEX_STRUCT)*KTROBO_GRAPHICS_RENDER_STRUCT_SIZE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	hr = g->getDevice()->CreateBuffer(&bd, NULL ,&(render_buffer_tex));
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}



	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof(descDepth) );
	descDepth.Width = g->getScreenWidth(); descDepth.Height = g->getScreenHeight();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1; descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
 
	//ID3D11Texture2D* pDepthStencil = 0;
	hr = g->getDevice()->CreateTexture2D( &descDepth, NULL, &pDepthStencil );
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "depth stencil make error");
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView;
	ZeroMemory( &descDepthStencilView, sizeof(descDepthStencilView) );
	descDepthStencilView.Format = descDepth.Format;
	descDepthStencilView.ViewDimension =  D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthStencilView.Texture2D.MipSlice = 0;
	//ID3D11DepthStencilView* pDepthStencilView = 0;
	hr = g->getDevice()->CreateDepthStencilView( pDepthStencil, &descDepthStencilView, &pDepthStencilView );
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "depth stencil make error");
	}



	D3D11_INPUT_ELEMENT_DESC layout4[] = {
		{"POSTEN", 0, DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"INFO1D",0,DXGI_FORMAT_R32G32_FLOAT, 0,8,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"INFO2D",0,DXGI_FORMAT_R8G8_UINT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"OFFSET",0,DXGI_FORMAT_R8G8_UINT, 0,18,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	
	loadShader(g, &mss_for_pen_special, KTROBO_GRAPHICS_SHADER_FILENAME_PEN_SPECIAL, KTROBO_GRAPHICS_SHADER_VS, KTROBO_GRAPHICS_SHADER_GS,
		KTROBO_GRAPHICS_SHADER_PS, g->getScreenWidth(), g->getScreenHeight(),
								layout4, 4, true);

	bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(GRAPHICS_RENDER_PEN_STRUCT_SPECIAL)*KTROBO_GRAPHICS_RENDER_STRUCT_SIZE_PEN_SPECIAL;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	hr = g->getDevice()->CreateBuffer(&bd, NULL ,&(render_buffer_pen_special));
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}



}


void Graphics::loadShader(Graphics* g, MYSHADERSTRUCT* s, char* shader_filename, char* vs_func_name, char* gs_func_name,
								char* ps_func_name, unsigned int ds_width,unsigned int ds_height,
								D3D11_INPUT_ELEMENT_DESC* layout, int numoflayout, bool blend_enable) {
HRESULT hr = S_OK;
	ID3DBlob* pblob = 0;
	memset(s, 0, sizeof(MYSHADERSTRUCT));

	try {
		CompileShaderFromFile(shader_filename, vs_func_name, "vs_4_0", &pblob);
		hr = g->getDevice()->CreateVertexShader(pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			NULL,
			&s->vs);
		if (FAILED(hr)) {
			pblob->Release();
			pblob = 0;
			s->vs = 0;
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vs make error");;
		}

		// 入力レイアウトの定義
		
		
		unsigned int num = numoflayout;//sizeoflayout/ sizeof(layout[0]);
			
		hr = g->getDevice()->CreateInputLayout(
			layout,num,pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			&s->vertexlayout);
		if (FAILED(hr)) {
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "layout make error");;
		}
		if (pblob) {
			pblob->Release();
			pblob = 0;
		}
		 // Set the input layout
		
		CompileShaderFromFile(shader_filename, gs_func_name, "gs_4_0", &pblob);
		hr = g->getDevice()->CreateGeometryShader(pblob->GetBufferPointer(),
			pblob->GetBufferSize(), NULL, &s->gs);
		if (FAILED(hr)) {
			pblob->Release();
			pblob = 0;
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "gs make error");;
		}
		if (pblob) {
			pblob->Release();
			pblob = 0;
		}

		CompileShaderFromFile(shader_filename, ps_func_name, "ps_4_0", &pblob);
		hr = g->getDevice()->CreatePixelShader(pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			NULL,&s->ps);
		if (FAILED(hr)) {
			pblob->Release();
			pblob = 0;
			s->ps = 0;
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "ps make error");;
		}

		if (pblob) {
			pblob->Release();
			pblob = 0;
		}

		D3D11_BLEND_DESC BlendDesc;
		memset(&BlendDesc, 0, sizeof(BlendDesc));
		BlendDesc.AlphaToCoverageEnable =false;// blend_enable;
		BlendDesc.IndependentBlendEnable = blend_enable;
		BlendDesc.RenderTarget[0].BlendEnable = blend_enable;
		BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;//SRC_ALPHA;
		BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;//INV_SRC_ALPHA;
		if (blend_enable) {
			BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		}
		BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].SrcBlendAlpha =D3D11_BLEND_ONE;
		BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		HRESULT hr = g->getDevice()->CreateBlendState(&BlendDesc,&s->blendstate);
		if (FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in blend state create");
		}
		/*
		 BlendDesc.IndependentBlendEnable = FALSE;
   // アルファブレンドを無効
   BlendDesc.RenderTarget[0].BlendEnable = FALSE;
   BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
   BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
   BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
   BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
   BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
   BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
   BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
   hr = pD3DDevice->CreateBlendState( &BlendDesc, &m_Step01.pBlendState );
   if( FAILED( hr ) )
   */
	
	} catch (KTROBO::GameError* e) {
		if (pblob) {
			pblob->Release();
			pblob = 0;
		}

		Del();

		
	//	MessageBoxA(NULL,e->getMessage(), "shader compile error", MB_OK);
		throw e;
	}


	D3D11_RASTERIZER_DESC _rasterDesc;
	_rasterDesc.AntialiasedLineEnable = true; // changed to true
	_rasterDesc.CullMode = D3D11_CULL_NONE;
	_rasterDesc.DepthBias = 0;
	_rasterDesc.DepthBiasClamp = 0.0f;
	_rasterDesc.DepthClipEnable = false;
	_rasterDesc.FillMode = D3D11_FILL_SOLID;
	_rasterDesc.FrontCounterClockwise = false;
	_rasterDesc.MultisampleEnable = true; // changed to true
	_rasterDesc.ScissorEnable = false;
	_rasterDesc.SlopeScaledDepthBias = 0.0f;
	hr  = g->getDevice()->CreateRasterizerState( &_rasterDesc, &s->rasterstate );
	if( FAILED( hr ) ) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "raster make error");;
	}

/*
	D3D11_BUFFER_DESC des;
	des.ByteWidth = sizeof(TelopTextsCBuf1);
	des.Usage = D3D11_USAGE_DEFAULT;
	des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	des.CPUAccessFlags = 0;
	des.MiscFlags = 0;
	des.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA idat;
	idat.pSysMem = &this->cbuf1;
	idat.SysMemPitch = 0;
	idat.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer(&des, &idat, &cbuf1_buffer);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
	}

	des.ByteWidth = sizeof(TelopTextsCBuf2);
	des.Usage = D3D11_USAGE_DEFAULT;
	des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	des.CPUAccessFlags = 0;
	des.MiscFlags = 0;
	des.StructureByteStride = 0;

//	D3D11_SUBRESOURCE_DATA idat;
	idat.pSysMem = &this->cbuf2;
	idat.SysMemPitch = 0;
	idat.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer(&des, &idat, &cbuf2_buffer);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
	}
*/

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof(descDepth) );
	descDepth.Width = ds_width; descDepth.Height = ds_height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1; descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
 
	//ID3D11Texture2D* pDepthStencil = 0;
	hr = g->getDevice()->CreateTexture2D( &descDepth, NULL, &s->depthstencil );
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "depth stencil make error");
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView;
	ZeroMemory( &descDepthStencilView, sizeof(descDepthStencilView) );
	descDepthStencilView.Format = descDepth.Format;
	descDepthStencilView.ViewDimension =  D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthStencilView.Texture2D.MipSlice = 0;
	//ID3D11DepthStencilView* pDepthStencilView = 0;
	hr = g->getDevice()->CreateDepthStencilView( s->depthstencil, &descDepthStencilView, &s->depthstencilview );
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "depth stencil make error");
	}



}

void Graphics::setPenInfo(KTROBO::Graphics* g, unsigned short sc_x, unsigned short sc_y,short transx, short transy, float zoom, KTPAINT_pen* pens) {


	GRAPHICS_RENDER_PEN_INFO_STRUCT cbuf;
	cbuf.screen_x =sc_x;//(unsigned short) g->getScreenWidth();//*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	cbuf.screen_y = sc_y;//(unsigned short) g->getScreenHeight();//*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	cbuf.transx  = transx;//*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	cbuf.transy = transy;//*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	cbuf.zoom = zoom;//*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	cbuf.offset = 0;
	cbuf.offset2 = 0;
	cbuf.offset3 = 0;
	for (int i=0;i<KTPAINT_PEN_NUM_MAX;i++) {
		cbuf.pen_width[i] = pens[i].pen_width;
	}
	CS::instance()->enter(CS_DEVICECON_CS,"enter");
	g->getDeviceContext()->UpdateSubresource(render_buffer_cbuf,0,NULL,&cbuf,0,0);
	CS::instance()->leave(CS_DEVICECON_CS,"leave"); 

}



void Graphics::Del() {
	mss.Del();
	if (info_buffer) {
		info_buffer->Release();
		info_buffer =0;
	}
	if (render_buffer) {
		render_buffer->Release();
		render_buffer = 0;
	}
	if (p_sampler) {
		p_sampler->Release();
		p_sampler = 0;
	}

	mss_for_pen.Del();
	if (render_buffer_pen) {
		render_buffer_pen->Release();
		render_buffer_pen = 0;
	}

	mss_for_pen_special.Del();
	if (render_buffer_pen_special) {
		render_buffer_pen_special->Release();
		render_buffer_pen_special = 0;
	}

	if (index_buffer_pen) {
		index_buffer_pen->Release();
		index_buffer_pen = 0;
	}
	if (render_buffer_cbuf) {
		render_buffer_cbuf->Release();
		render_buffer_cbuf = 0;
	}

	mss_for_tex.Del();
	if (render_buffer_tex) {
		render_buffer_tex->Release();
		render_buffer_tex = 0;
	}

	if (pDepthStencil) {
		pDepthStencil->Release();
		pDepthStencil = 0;
	}
	if (pDepthStencilView) {
		pDepthStencilView->Release();
		pDepthStencilView = 0;
	}
}

void Graphics::drawPen(KTROBO::Graphics* g, KTPAINT_penline* penlines, int penline_max) {

	CS::instance()->enter(CS_DEVICECON_CS, "render");

	if (penline_max > KTROBO_GRAPHICS_RENDER_STRUCT_SIZE_PEN) {
		CS::instance()->leave(CS_DEVICECON_CS, "render");
		return;
	}


	unsigned int stride = sizeof(GRAPHICS_RENDER_PEN_STRUCT);
	unsigned int offset = 0;
	GRAPHICS_RENDER_PEN_STRUCT xdayo[KTROBO_GRAPHICS_RENDER_STRUCT_SIZE_PEN];
	for (int i=0;i<penline_max;i++) {
		xdayo[i].x = penlines[i].x;
		xdayo[i].y = penlines[i].y;
		xdayo[i].dx = penlines[i].dx;
		xdayo[i].dy = penlines[i].dy;
		xdayo[i].width_and_nwidth = penlines[i].width_and_nwidth;
		xdayo[i].offset = 0;
		xdayo[i].offset2 = 0;
		xdayo[i].pen_index = penlines[i].pen_index;
	}
	//g->getDeviceContext()->UpdateSubresource(info_buffer,0,NULL,&info,0,0);
	D3D11_MAPPED_SUBRESOURCE msr;
	
	g->getDeviceContext()->Map(render_buffer_pen, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy( msr.pData, &xdayo, sizeof(GRAPHICS_RENDER_PEN_STRUCT)*penline_max );
	g->getDeviceContext()->Unmap(render_buffer_pen, 0);

	g->getDeviceContext()->IASetInputLayout( mss_for_pen.vertexlayout );
	g->getDeviceContext()->GSSetConstantBuffers(0,1,&render_buffer_cbuf);
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, &render_buffer_pen, &stride, &offset );
	g->getDeviceContext()->IASetIndexBuffer(index_buffer_pen,DXGI_FORMAT_R16_UINT,0);
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	g->getDeviceContext()->RSSetState(mss_for_pen.rasterstate);

	float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

	g->getDeviceContext()->OMSetBlendState(mss.blendstate, blendFactor,0xFFFFFFFF/*0xFFFFFFFF*/);
	g->getDeviceContext()->VSSetShader(mss_for_pen.vs, NULL, 0);
	g->getDeviceContext()->GSSetShader(mss_for_pen.gs,NULL,0);
//	g->getDeviceContext()->PSSetShaderResources(0,1,&f->fonttextureviews[i]);//render_target_tex->view);
	g->getDeviceContext()->PSSetSamplers(0,1,&p_sampler);
		
	g->getDeviceContext()->PSSetShader(mss_for_pen.ps, NULL, 0);
			
	g->getDeviceContext()->DrawIndexed(penline_max*3,0,0);//penline_max,0);
	
	CS::instance()->leave(CS_DEVICECON_CS, "leave");








}

MYVECTOR3 getPosOfDaen(MYVECTOR3 center, float yoko, float tate, float theta_from_x, float theta) {
	MYVECTOR3 ans = center;

	ans.float3.x += (yoko * cos(theta)) * cos(theta_from_x) - tate * sin(theta) * sin(theta_from_x);
	ans.float3.y += (yoko * cos(theta)) * sin(theta_from_x) + tate * sin(theta) * cos(theta_from_x);
	ans.float3.z = 0;
	return MYVECTOR3(ans.float3.x, ans.float3.y, ans.float3.z);
}

void Graphics::drawDaen(KTROBO::Graphics* g, DWORD color,  MYVECTOR3 center,float yoko, float tate, float theta_from_x) {


	

	CS::instance()->enter(CS_DEVICECON_CS, "render");
	DWORD FontColor = color;//0xFF000000;
	MYVECTOR3 points[32];

	for (int i=0;i<32;i++) {
		points[i] = getPosOfDaen(center, yoko, tate, theta_from_x, 3.14/2/2/4*i);
	}






GRAPHICS_RENDER_STRUCT sttr[64];
int temp=0;
for (int i=0;i<64;) {
	sttr[i].color = FontColor;
	sttr[i].x = -1 + 2*points[temp].float3.x / (float)g->getScreenWidth() * KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	sttr[i].y = 1 - 2*points[temp].float3.y / (float)g->getScreenHeight() * KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	sttr[i].z = points[temp].float3.z;
	sttr[i+1].color = FontColor;
	sttr[i+1].x = -1 + 2*points[(temp+1) % 32].float3.x/ (float)g->getScreenWidth() * KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	sttr[i+1].y = 1 - 2*points[(temp+1) % 32].float3.y/ (float)g->getScreenHeight() * KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	sttr[i+1].z = points[(temp+1) % 32].float3.z;
	i += 2;
	temp += 1;

}
	MYMATRIX proj;
	MYMATRIX view;
	MYMATRIX world;
	MyMatrixIdentity(proj);
	MyMatrixIdentity(view);
	MyMatrixIdentity(world);
	info.proj = proj;
	info.view = view;
	info.world = world;
	unsigned int stride = sizeof(GRAPHICS_RENDER_STRUCT);
	unsigned int offset = 0;
	
	g->getDeviceContext()->UpdateSubresource(info_buffer,0,NULL,&info,0,0);
	D3D11_MAPPED_SUBRESOURCE msr;
	
	g->getDeviceContext()->Map(render_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy( msr.pData, &sttr, sizeof(GRAPHICS_RENDER_STRUCT)*64 );
	g->getDeviceContext()->Unmap(render_buffer, 0);

	g->getDeviceContext()->IASetInputLayout( mss.vertexlayout );
	g->getDeviceContext()->VSSetConstantBuffers(0,1,&info_buffer);
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, &render_buffer, &stride, &offset );
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
	g->getDeviceContext()->RSSetState(mss.rasterstate);

	float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

	g->getDeviceContext()->OMSetBlendState(mss.blendstate, blendFactor,0xFFFFFFFF/*0xFFFFFFFF*/);
	g->getDeviceContext()->VSSetShader(mss.vs, NULL, 0);
	g->getDeviceContext()->GSSetShader(NULL,NULL,0);
//	g->getDeviceContext()->PSSetShaderResources(0,1,&f->fonttextureviews[i]);//render_target_tex->view);
	g->getDeviceContext()->PSSetSamplers(0,1,&p_sampler);
		
	g->getDeviceContext()->PSSetShader(mss.ps, NULL, 0);
			
	g->getDeviceContext()->Draw(64,0);
	
	CS::instance()->leave(CS_DEVICECON_CS, "leave");







}


void Graphics::drawHeiryouiki(KTROBO::Graphics* g, DWORD color, KTPAINT_penheiryouiki* hei, 
							  KTPAINT_penheiryouikipart* heipart, KTPAINT_penline* lines,
							  KTPAINT_penheiryouikidaen* daens) {


	

GRAPHICS_RENDER_STRUCT sttr[1024];
memset(sttr,0,sizeof(GRAPHICS_RENDER_STRUCT)*1024);
int temp=0;
int i=0;
//drawDaen(g, 0xFF0000FF,MYVECTOR3(daens[hei->daen_index].x, daens[hei->daen_index].y,0),100,100,0);
for (int k=hei->startheiryouiki;k<=hei->endheiryouiki;k++) {
for (int tt=0;tt<=heipart[k].keiro_last_index-heipart[k].keiro_first_index;) {
	
	
	float x1 = lines[heipart[k].keiro_first_index+tt].x;
	float x2 = lines[heipart[k].keiro_first_index+1+tt].x;
	float x3 = daens[hei->daen_index].x;
	float y1 = lines[heipart[k].keiro_first_index+tt].y;
	if ((tt ==0)) {
		x1 = heipart[k].kouten_x;
		y1 = heipart[k].kouten_y;
	}

	float y2 = lines[heipart[k].keiro_first_index+1+tt].y;

	if (tt == heipart[k].keiro_last_index-heipart[k].keiro_first_index) {
		if (k == hei->endheiryouiki) {
			x2 = heipart[hei->startheiryouiki].kouten_x;
			y2 = heipart[hei->startheiryouiki].kouten_y;
		} else {
			x2 = heipart[k+1].kouten_x;
			y2 = heipart[k+1].kouten_y;
		}
	}


	float y3 = daens[hei->daen_index].y;
/*	MYVECTOR3 center;
	center.float3.x = x1/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	center.float3.y = y1/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	center.float3.z = 0;
	drawDaen(g,0xFFFF00FF,center, 10,10,0);
	center.float3.x = 0.3+x2/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	center.float3.y = 0.3+y2/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	center.float3.z = 0;
	drawDaen(g,0xFFFFFF00,center, 10,10,0);
	*/
	MYVECTOR3 center;
	center.float3.x = x3/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	center.float3.y = y3/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	center.float3.z = 0;
	drawDaen(g,0xFFFF0000,center, 10,10,0);
	tt++;
	continue;
	sttr[i].color = 0xFFFFFFFF;//color;
	sttr[i].x = -1 + 2*x1 / (float)g->getScreenWidth();//* KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	sttr[i].y = 1 - 2*y1 / (float)g->getScreenHeight();//* KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	sttr[i].z = 0;
	sttr[i+1].color = 0xFFFFFFFF;//color;
	sttr[i+1].x = -1 + 2*x2/ (float)g->getScreenWidth();//* KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	sttr[i+1].y = 1 - 2*y2/ (float)g->getScreenHeight();//* KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	sttr[i+1].z = 0;
	sttr[i+2].color = 0xFFFFFFFF;//color;
	sttr[i+2].x =  -1 + 2*x3/ (float)g->getScreenWidth();//* KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	sttr[i+2].y = 1 - 2*y3/ (float)g->getScreenHeight();//* KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	sttr[i+2].z = 0;
	i += 3;
	tt += 1;
	temp+=1;
}
}
	MYMATRIX proj;
	MYMATRIX view;
	MYMATRIX world;
	MyMatrixIdentity(proj);
	MyMatrixIdentity(view);
	MyMatrixIdentity(world);
	info.proj = proj;
	info.view = view;
	info.world = world;
	unsigned int stride = sizeof(GRAPHICS_RENDER_STRUCT);
	unsigned int offset = 0;
	
	g->getDeviceContext()->UpdateSubresource(info_buffer,0,NULL,&info,0,0);
	D3D11_MAPPED_SUBRESOURCE msr;
	
	g->getDeviceContext()->Map(render_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy( msr.pData, &sttr, sizeof(GRAPHICS_RENDER_STRUCT)*temp*3 );
	g->getDeviceContext()->Unmap(render_buffer, 0);

	g->getDeviceContext()->IASetInputLayout( mss.vertexlayout );
	g->getDeviceContext()->VSSetConstantBuffers(0,1,&info_buffer);
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, &render_buffer, &stride, &offset );
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	g->getDeviceContext()->RSSetState(mss.rasterstate);

	float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

	g->getDeviceContext()->OMSetBlendState(mss.blendstate, blendFactor,0xFFFFFFFF/*0xFFFFFFFF*/);
	g->getDeviceContext()->VSSetShader(mss.vs, NULL, 0);
	g->getDeviceContext()->GSSetShader(NULL,NULL,0);
//	g->getDeviceContext()->PSSetShaderResources(0,1,&f->fonttextureviews[i]);//render_target_tex->view);
	g->getDeviceContext()->PSSetSamplers(0,1,&p_sampler);
		
	g->getDeviceContext()->PSSetShader(mss.ps, NULL, 0);
			
	g->getDeviceContext()->Draw(temp*3,0);
	
	CS::instance()->leave(CS_DEVICECON_CS, "leave");














}




void Graphics::drawPenSpecial(KTROBO::Graphics* g, KTPAINT_penline* penlines, int penline_max) {

	CS::instance()->enter(CS_DEVICECON_CS, "render");

	if (penline_max > KTROBO_GRAPHICS_RENDER_STRUCT_SIZE_PEN_SPECIAL) {
		CS::instance()->leave(CS_DEVICECON_CS, "render");
		return;
	}


	unsigned int stride = sizeof(GRAPHICS_RENDER_PEN_STRUCT_SPECIAL);
	unsigned int offset = 0;
	GRAPHICS_RENDER_PEN_STRUCT_SPECIAL xdayo[KTROBO_GRAPHICS_RENDER_STRUCT_SIZE_PEN_SPECIAL];
	for (int i=0;i<penline_max;i++) {
	//	if ( (i<penline_max-1)) {
			xdayo[i].x = (penlines[i].x);//*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
			xdayo[i].y = (penlines[i].y);//*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
			xdayo[i].dx = (penlines[i].dx);//*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
			xdayo[i].dy = (penlines[i].dy);//*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	//	} else {
		//	xdayo[i].x = penlines[i].x;
		//	xdayo[i].y = penlines[i].y;
		//	xdayo[i].dx = penlines[i].dx;
		//	xdayo[i].dy = penlines[i].dy;
	//	}
		xdayo[i].width_and_nwidth = penlines[i].width_and_nwidth;
		xdayo[i].offset = 0;
		xdayo[i].offset2 = 0;
		xdayo[i].pen_index = penlines[i].pen_index;
	}
	
	//g->getDeviceContext()->UpdateSubresource(info_buffer,0,NULL,&info,0,0);
	D3D11_MAPPED_SUBRESOURCE msr;
	
	g->getDeviceContext()->Map(render_buffer_pen_special, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy( msr.pData, &xdayo, sizeof(GRAPHICS_RENDER_PEN_STRUCT_SPECIAL)*penline_max );
	g->getDeviceContext()->Unmap(render_buffer_pen_special, 0);

	g->getDeviceContext()->IASetInputLayout( mss_for_pen_special.vertexlayout );
	g->getDeviceContext()->GSSetConstantBuffers(0,1,&render_buffer_cbuf);
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, &render_buffer_pen_special, &stride, &offset );
	g->getDeviceContext()->IASetIndexBuffer(index_buffer_pen,DXGI_FORMAT_R16_UINT,0);
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	g->getDeviceContext()->RSSetState(mss_for_pen_special.rasterstate);

	float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

	g->getDeviceContext()->OMSetBlendState(mss.blendstate, blendFactor,0xFFFFFFFF/*0xFFFFFFFF*/);
	g->getDeviceContext()->VSSetShader(mss_for_pen_special.vs, NULL, 0);
	g->getDeviceContext()->GSSetShader(mss_for_pen_special.gs,NULL,0);
//	g->getDeviceContext()->PSSetShaderResources(0,1,&f->fonttextureviews[i]);//render_target_tex->view);
	g->getDeviceContext()->PSSetSamplers(0,1,&p_sampler);
		
	g->getDeviceContext()->PSSetShader(mss_for_pen_special.ps, NULL, 0);
			
	g->getDeviceContext()->DrawIndexed(penline_max*3,0,0);//penline_max,0);
	
	CS::instance()->leave(CS_DEVICECON_CS, "leave");








}





void Graphics::drawTex(KTROBO::Graphics* g, unsigned short tex_width, unsigned short tex_height, MyShaderResourceView tex_class, short transx, short transy, float zoom, KTPAINT_pen* pens) {

	GRAPHICS_RENDER_TEX_STRUCT stt[] = {
		{0,0,0,0,0},
		{tex_width,0,0,1,0},
		{0,tex_height,0,0,1},

		{tex_width,0,0,1,0},
		{0,tex_height,0,0,1},
		{tex_width,tex_height,0,1,1}
	};
	Graphics::setPenInfo(g,tex_width,tex_height,transx,transy,zoom,pens);
	CS::instance()->enter(CS_DEVICECON_CS, "render");

	unsigned int stride = sizeof(GRAPHICS_RENDER_TEX_STRUCT);
	unsigned int offset = 0;
	
	//g->getDeviceContext()->UpdateSubresource(info_buffer,0,NULL,&info,0,0);
	D3D11_MAPPED_SUBRESOURCE msr;
	
	g->getDeviceContext()->Map(render_buffer_tex, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy( msr.pData, &stt, sizeof(GRAPHICS_RENDER_TEX_STRUCT)*6 );
	g->getDeviceContext()->Unmap(render_buffer_tex, 0);

	g->getDeviceContext()->IASetInputLayout( mss_for_tex.vertexlayout );
	g->getDeviceContext()->VSSetConstantBuffers(0,1,&render_buffer_cbuf);
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, &render_buffer_tex, &stride, &offset );
	//g->getDeviceContext()->IASetIndexBuffer(index_buffer_tex,DXGI_FORMAT_R16_UINT,0);
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	g->getDeviceContext()->RSSetState(mss_for_tex.rasterstate);

	float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

	g->getDeviceContext()->OMSetBlendState(mss.blendstate, blendFactor,0xFFFFFFFF/*0xFFFFFFFF*/);
	g->getDeviceContext()->VSSetShader(mss_for_tex.vs, NULL, 0);
	g->getDeviceContext()->GSSetShader(NULL,NULL,0);
	g->getDeviceContext()->PSSetShaderResources(0,1,&tex_class);//render_target_tex->view);
	g->getDeviceContext()->PSSetSamplers(0,1,&p_sampler);
		
	g->getDeviceContext()->PSSetShader(mss_for_tex.ps, NULL, 0);
			
	g->getDeviceContext()->Draw(6,0);//penline_max,0);
	
	CS::instance()->leave(CS_DEVICECON_CS, "leave");

}


void Graphics::drawOBB(KTROBO::Graphics* g, DWORD color, MYMATRIX* world, MYMATRIX* view, MYMATRIX* proj, OBB* rec) {
	MYVECTOR3 point1;
	MYVECTOR3 point2;
	MYVECTOR3 point3;
	MYVECTOR3 point4;
	MYVECTOR3 point5;
	MYVECTOR3 point6;
	MYVECTOR3 point7;
	MYVECTOR3 point8;

	

	point1 = rec->c + rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point2 = rec->c + rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point3 = rec->c - rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point4 = rec->c - rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point5 = rec->c + rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
	point6 = rec->c + rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
	point7 = rec->c - rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
	point8 = rec->c - rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
	


	CS::instance()->enter(CS_DEVICECON_CS, "render");
	DWORD FontColor = color;
	
//	memset(sttr,0,sizeof(sttr));
	









GRAPHICS_RENDER_STRUCT sttr[] = {
 
      { point1.float3.x, point1.float3.y, point1.float3.z, /*1.0f,*/ FontColor},
      { point2.float3.x, point2.float3.y, point2.float3.z, /*1.0f,*/ FontColor}, 
      { point1.float3.x, point1.float3.y, point1.float3.z, /*1.0f,*/ FontColor}, 
      { point4.float3.x, point4.float3.y, point4.float3.z, /*1.0f,*/ FontColor},
	  { point3.float3.x, point3.float3.y, point3.float3.z, FontColor},
	  { point2.float3.x, point2.float3.y, point2.float3.z, FontColor},
	  { point3.float3.x, point3.float3.y, point3.float3.z, FontColor},
	  { point4.float3.x, point4.float3.y, point4.float3.z, FontColor},
	  
	  { point5.float3.x, point5.float3.y, point5.float3.z, /*1.0f,*/ FontColor},
      { point6.float3.x, point6.float3.y, point6.float3.z, /*1.0f,*/ FontColor}, 
      { point5.float3.x, point5.float3.y, point5.float3.z, /*1.0f,*/ FontColor}, 
      { point8.float3.x, point8.float3.y, point8.float3.z, /*1.0f,*/ FontColor},
	  { point7.float3.x, point7.float3.y, point7.float3.z, FontColor},
	  { point6.float3.x, point6.float3.y, point6.float3.z, FontColor},
	  { point7.float3.x, point7.float3.y, point7.float3.z, FontColor},
	  { point8.float3.x, point8.float3.y, point8.float3.z, FontColor},

	  { point1.float3.x, point1.float3.y, point1.float3.z, /*1.0f,*/ FontColor},
      { point5.float3.x, point5.float3.y, point5.float3.z, /*1.0f,*/ FontColor}, 
      { point2.float3.x, point2.float3.y, point2.float3.z, /*1.0f,*/ FontColor}, 
      { point6.float3.x, point6.float3.y, point6.float3.z, /*1.0f,*/ FontColor},
	  { point3.float3.x, point3.float3.y, point3.float3.z, FontColor},
	  { point7.float3.x, point7.float3.y, point7.float3.z, FontColor},
	  { point8.float3.x, point8.float3.y, point8.float3.z, FontColor},
	  { point4.float3.x, point4.float3.y, point4.float3.z, FontColor}

   };

	info.proj = *proj;
	info.view = *view;
	info.world = *world;
	unsigned int stride = sizeof(GRAPHICS_RENDER_STRUCT);
	unsigned int offset = 0;
	
	g->getDeviceContext()->UpdateSubresource(info_buffer,0,NULL,&info,0,0);
	D3D11_MAPPED_SUBRESOURCE msr;
	
	g->getDeviceContext()->Map(render_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy( msr.pData, &sttr, sizeof(GRAPHICS_RENDER_STRUCT)*24 );
	g->getDeviceContext()->Unmap(render_buffer, 0);

	g->getDeviceContext()->IASetInputLayout( mss.vertexlayout );
	g->getDeviceContext()->VSSetConstantBuffers(0,1,&info_buffer);
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, &render_buffer, &stride, &offset );
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
	g->getDeviceContext()->RSSetState(mss.rasterstate);

	float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

	g->getDeviceContext()->OMSetBlendState(mss.blendstate, blendFactor,0xFFFFFFFF/*0xFFFFFFFF*/);
	g->getDeviceContext()->VSSetShader(mss.vs, NULL, 0);
	g->getDeviceContext()->GSSetShader(NULL,NULL,0);
//	g->getDeviceContext()->PSSetShaderResources(0,1,&f->fonttextureviews[i]);//render_target_tex->view);
	g->getDeviceContext()->PSSetSamplers(0,1,&p_sampler);
		
	g->getDeviceContext()->PSSetShader(mss.ps, NULL, 0);
			
	g->getDeviceContext()->Draw(24,0);
	
	CS::instance()->leave(CS_DEVICECON_CS, "leave");
}


void Graphics::drawOBBFill(KTROBO::Graphics* g, DWORD color, MYMATRIX* world, MYMATRIX *view, MYMATRIX* proj, OBB* rec) {
	
	MYVECTOR3 point1;
	MYVECTOR3 point2;
	MYVECTOR3 point3;
	MYVECTOR3 point4;
	MYVECTOR3 point5;
	MYVECTOR3 point6;
	MYVECTOR3 point7;
	MYVECTOR3 point8;



	point1 = rec->c + rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point2 = rec->c + rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point3 = rec->c - rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point4 = rec->c - rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point5 = rec->c + rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
	point6 = rec->c + rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
	point7 = rec->c - rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
	point8 = rec->c - rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];




	DWORD FontColor = color;


//	  SIMPLEVERTEX v[]=
 //  {
  //    { point1.x, point1.y, point1.z, /*1.0f,*/ FontColor},
   //   { point2.x, point2.y, point2.z, /*1.0f,*/ FontColor}, 
	//  { point3.x, point3.y, point3.z, FontColor},

 //     { point1.x, point1.y, point1.z, /*1.0f,*/ FontColor}, 
//	  { point3.x, point3.y, point3.z, FontColor},
 //     { point4.x, point4.y, point4.z, /*1.0f,*/ FontColor},
	  
//	  { point1.x, point1.y, point1.z, FontColor},
//	  { point4.x, point4.y, point4.z, FontColor},
//	  { point5.x, point5.y, point5.z, FontColor},

//	  { point4.x, point4.y, point4.z, FontColor},
//	  { point8.x, point8.y, point8.z, /*1.0f,*/ FontColor}, 
 //     { point5.x, point5.y, point5.z, /*1.0f,*/ FontColor}, 

//	  { point2.x, point2.y, point2.z, /*1.0f,*/ FontColor},
 //     { point6.x, point6.y, point6.z, FontColor},
//	  { point3.x, point3.y, point3.z, FontColor},
      
//	  { point3.x, point3.y, point3.z, FontColor},
//	  { point6.x, point6.y, point6.z, FontColor},
//	  { point7.x, point7.y, point7.z, FontColor},
//	  
//	  { point4.x, point4.y, point4.z, FontColor},
//	  { point3.x, point3.y, point3.z, FontColor},
//	  { point7.x, point7.y, point7.z, FontColor},

//	  { point4.x, point4.y, point4.z, FontColor},
//	  { point7.x, point7.y, point7.z, FontColor},
//	  { point8.x, point8.y, point8.z, FontColor},

//	  { point5.x, point5.y, point5.z, FontColor},
//	  { point6.x, point6.y, point6.z, FontColor},
//	  { point1.x, point1.y, point1.z, /*1.0f,*/ FontColor},

//	  { point1.x, point1.y, point1.z, /*1.0f,*/ FontColor},
 //     { point6.x, point6.y, point6.z, FontColor},
//	  { point2.x, point2.y, point2.z, /*1.0f,*/ FontColor}, 
	   
//	  { point6.x, point6.y, point6.z, FontColor},
///	  { point5.x, point5.y, point5.z, /*1.0f,*/ FontColor}, 
//	  { point8.x, point8.y, point8.z, /*1.0f,*/ FontColor},
	  
	  
	  
	  
//	  { point7.x, point7.y, point7.z, FontColor},
//	  { point6.x, point6.y, point6.z, FontColor},
//	  { point8.x, point8.y, point8.z, FontColor},

	
//   };



}


void Graphics::drawRAY(KTROBO::Graphics* g, DWORD color, MYMATRIX* world, MYMATRIX* view, MYMATRIX* proj, RAY* ray, int length) {


		 
//	SIMPLEVERTEX v[]=
 //  {
//	   { ray->org.x, ray->org.y, ray->org.z, /*1.0f,*/ FontColor},
//	   { ray->org.x + length * ray->dir.x, ray->org.y + length * ray->dir.y, ray->org.z + length*ray->dir.z, /*1.0f,*/ FontColor}, 
 //    
  // };


}


void Graphics::drawTriangle(KTROBO::Graphics* g, DWORD color, MYMATRIX* world, MYMATRIX* view, MYMATRIX* proj, MYVECTOR3* p, MYVECTOR3* q, MYVECTOR3* r) {

	DWORD FontColor = color;
//	  SIMPLEVERTEX v[]=
 //  {
  //    { p->x, p->y, p->z, /*1.0f,*/ FontColor},
   //   { q->x, q->y, q->z, /*1.0f,*/ FontColor}, 
    //  { r->x, r->y, r->z, /*1.0f,*/ FontColor}, 
	//  { p->x, p->y, p->z, /**/ FontColor},
	//  { q->x, q->y, q->z, FontColor},
	//  { r->x, r->y, r->z, FontColor},
	 // };



}


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
	is_copied = false;
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

	MyMatrixPerspectiveFovRH(proj, 1, this->getScreenWidth() / (float)this->getScreenHeight(), 1, 1000);

	return true;
}

bool Graphics::createDeviceAndSwapChain() {

	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(h, &rc);
	width  = KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU* ( rc.right - rc.left);
	height = KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU* (rc.bottom - rc.top);
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
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	sd.BufferDesc.Width = width;//*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	sd.BufferDesc.Height = height;//*KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
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
	vp.Width = (float)width;///KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	vp.Height = (float)height;///KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
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

	if (is_copied) return; // コピーされたものの場合はコピー元でリリースさせる

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
//		p_device->Release();
		p_device = 0;
	}


	
	return;
}


Graphics* Graphics::makeGraphicsOfNewDeviceContext() {
	
	Graphics* new_g  = new Graphics();
	new_g->driver_type = this->driver_type;
	new_g->featurelevel = this->featurelevel;
	new_g->h = this->h;
	new_g->height = this->height;
	new_g->is_copied = true;
	new_g->p_backbuffer = this->p_backbuffer;
	new_g->p_device = this->p_device;
	new_g->width = this->width;
	new_g->p_rendertargetview = this->p_rendertargetview;
	new_g->p_backbuffer = this->p_backbuffer;
	new_g->p_swapchain = this->p_swapchain;
	new_g->vp = this->vp;
	new_g->tex_loader = this->tex_loader;
	new_g->proj = this->proj;
	// デバイスコンテキストを作成する
	HRESULT hr =  p_device->CreateDeferredContext(0, &new_g->p_devicecontext);
	if (FAILED(hr)) {
		throw new GameError(KTROBO::FATAL_ERROR, "failed in copying graphics");
	}

	return new_g;
}

