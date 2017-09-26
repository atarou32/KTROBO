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

}


void Graphics::loadShader(Graphics* g, MYSHADERSTRUCT* s, char* shader_filename, char* vs_func_name, char* gs_func_name,
								char* ps_func_name, unsigned int ds_width,unsigned int ds_height,
								D3D11_INPUT_ELEMENT_DESC* layout, int numoflayout, bool blend_enable) {
HRESULT hr = S_OK;
	ID3DBlob* pblob = 0;
	//memset(s, 0, sizeof(MYSHADERSTRUCT));

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
	


	CS::instance()->enter(CS_DEVICECON_CS, "render");
	DWORD FontColor = color;
	
//	memset(sttr,0,sizeof(sttr));
	









GRAPHICS_RENDER_STRUCT sttr[] = {
 
      { point1.float3.x, point1.float3.y, point1.float3.z, /*1.0f,*/ FontColor},
      { point2.float3.x, point2.float3.y, point2.float3.z, /*1.0f,*/ FontColor}, 
      { point3.float3.x, point3.float3.y, point3.float3.z, /*1.0f,*/ FontColor}, 
      { point4.float3.x, point4.float3.y, point4.float3.z, /*1.0f,*/ FontColor},
	  { point3.float3.x, point3.float3.y, point3.float3.z, FontColor},
	  { point1.float3.x, point1.float3.y, point1.float3.z, FontColor},
	//  { point3.float3.x, point3.float3.y, point3.float3.z, FontColor},
	 // { point4.float3.x, point4.float3.y, point4.float3.z, FontColor},
	  
	  { point5.float3.x, point5.float3.y, point5.float3.z, /*1.0f,*/ FontColor},
      { point6.float3.x, point6.float3.y, point6.float3.z, /*1.0f,*/ FontColor}, 
      { point7.float3.x, point7.float3.y, point7.float3.z, /*1.0f,*/ FontColor}, 
      { point8.float3.x, point8.float3.y, point8.float3.z, /*1.0f,*/ FontColor},
	  { point7.float3.x, point7.float3.y, point7.float3.z, FontColor},
	  { point5.float3.x, point5.float3.y, point5.float3.z, FontColor},
	//  { point7.float3.x, point7.float3.y, point7.float3.z, FontColor},
	//  { point8.float3.x, point8.float3.y, point8.float3.z, FontColor},

	  { point1.float3.x, point1.float3.y, point1.float3.z, /*1.0f,*/ FontColor},
      { point5.float3.x, point5.float3.y, point5.float3.z, /*1.0f,*/ FontColor}, 
      { point2.float3.x, point2.float3.y, point2.float3.z, /*1.0f,*/ FontColor}, 
      { point6.float3.x, point6.float3.y, point6.float3.z, /*1.0f,*/ FontColor},
	  { point2.float3.x, point2.float3.y, point2.float3.z, FontColor},
	  { point5.float3.x, point5.float3.y, point5.float3.z, FontColor},
//	  { point8.float3.x, point8.float3.y, point8.float3.z, FontColor},
//	  { point4.float3.x, point4.float3.y, point4.float3.z, FontColor}

	  { point3.float3.x, point3.float3.y, point3.float3.z, /*1.0f,*/ FontColor},
      { point7.float3.x, point7.float3.y, point7.float3.z, /*1.0f,*/ FontColor}, 
      { point8.float3.x, point8.float3.y, point8.float3.z, /*1.0f,*/ FontColor}, 
      { point7.float3.x, point7.float3.y, point7.float3.z, /*1.0f,*/ FontColor},
	  { point8.float3.x, point8.float3.y, point8.float3.z, FontColor},
	  { point4.float3.x, point4.float3.y, point4.float3.z, FontColor},

      { point2.float3.x, point2.float3.y, point2.float3.z, /*1.0f,*/ FontColor},
      { point3.float3.x, point3.float3.y, point3.float3.z, /*1.0f,*/ FontColor}, 
      { point6.float3.x, point6.float3.y, point6.float3.z, /*1.0f,*/ FontColor}, 
      { point3.float3.x, point3.float3.y, point3.float3.z, /*1.0f,*/ FontColor},
	  { point6.float3.x, point6.float3.y, point6.float3.z, FontColor},
	  { point7.float3.x, point7.float3.y, point7.float3.z, FontColor},

      { point1.float3.x, point1.float3.y, point1.float3.z, /*1.0f,*/ FontColor},
      { point5.float3.x, point5.float3.y, point5.float3.z, /*1.0f,*/ FontColor}, 
      { point8.float3.x, point8.float3.y, point8.float3.z, /*1.0f,*/ FontColor}, 
      { point8.float3.x, point8.float3.y, point8.float3.z, /*1.0f,*/ FontColor},
	  { point4.float3.x, point4.float3.y, point4.float3.z, FontColor},
	  { point1.float3.x, point1.float3.y, point1.float3.z, FontColor},

   };

	info.proj = *proj;
	info.view = *view;
	info.world = *world;
	unsigned int stride = sizeof(GRAPHICS_RENDER_STRUCT);
	unsigned int offset = 0;
	
	g->getDeviceContext()->UpdateSubresource(info_buffer,0,NULL,&info,0,0);
	D3D11_MAPPED_SUBRESOURCE msr;
	
	g->getDeviceContext()->Map(render_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy( msr.pData, &sttr, sizeof(GRAPHICS_RENDER_STRUCT)*36 );
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
			
	g->getDeviceContext()->Draw(36,0);
	
	CS::instance()->leave(CS_DEVICECON_CS, "leave");
}

















void Graphics::drawRAY(KTROBO::Graphics* g, DWORD color, MYMATRIX* world, MYMATRIX* view, MYMATRIX* proj, float length, RAY* ray) {


		 
//	SIMPLEVERTEX v[]=
 //  {
//	   { ray->org.x, ray->org.y, ray->org.z, /*1.0f,*/ FontColor},
//	   { ray->org.x + length * ray->dir.x, ray->org.y + length * ray->dir.y, ray->org.z + length*ray->dir.z, /*1.0f,*/ FontColor}, 
 //    
  // };
	
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

	MYVECTOR3 point1;
	MYVECTOR3 point2;
	MYVECTOR3 point3;
	MYVECTOR3 point4;
	MYVECTOR3 point5;
	MYVECTOR3 point6;
	MYVECTOR3 point7;
	MYVECTOR3 point8;

	

	point1 = ray->org;//rec->c + rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point2 = ray->org + ray->dir * length;//rec->c + rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
//	point3 = *r;//rec->c - rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
//	point4 = rec->c - rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
//	point5 = rec->c + rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
//	point6 = rec->c + rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
//	point7 = rec->c - rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
//	point8 = rec->c - rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
	


	CS::instance()->enter(CS_DEVICECON_CS, "render");
//	DWORD FontColor = color;
	
//	memset(sttr,0,sizeof(sttr));
	









GRAPHICS_RENDER_STRUCT sttr[] = {
 
      { point1.float3.x, point1.float3.y, point1.float3.z, /*1.0f,*/ FontColor},
      { point2.float3.x, point2.float3.y, point2.float3.z, /*1.0f,*/ FontColor}, 
     // { point1.float3.x, point1.float3.y, point1.float3.z, /*1.0f,*/ FontColor}, 
     // { point3.float3.x, point3.float3.y, point3.float3.z, /*1.0f,*/ FontColor},
	 // { point3.float3.x, point3.float3.y, point3.float3.z, FontColor},
	 // { point2.float3.x, point2.float3.y, point2.float3.z, FontColor},
	  
	  //*{ point3.float3.x, point3.float3.y, point3.float3.z, FontColor},
	  //{ point4.float3.x, point4.float3.y, point4.float3.z, FontColor},
	  
	 // { point5.float3.x, point5.float3.y, point5.float3.z, /*1.0f,*/ FontColor},
      //{ point6.float3.x, point6.float3.y, point6.float3.z, /*1.0f,*/ FontColor}, 
      //{ point5.float3.x, point5.float3.y, point5.float3.z, /*1.0f,*/ FontColor}, 
      //{ point8.float3.x, point8.float3.y, point8.float3.z, /*1.0f,*/ FontColor},
	  //{ point7.float3.x, point7.float3.y, point7.float3.z, FontColor},
	  //{ point6.float3.x, point6.float3.y, point6.float3.z, FontColor},
	  //{ point7.float3.x, point7.float3.y, point7.float3.z, FontColor},
	  //{ point8.float3.x, point8.float3.y, point8.float3.z, FontColor},

	  //{ point1.float3.x, point1.float3.y, point1.float3.z, /*1.0f,*/ FontColor},
      //{ point5.float3.x, point5.float3.y, point5.float3.z, /*1.0f,*/ FontColor}, 
      //{ point2.float3.x, point2.float3.y, point2.float3.z, /*1.0f,*/ FontColor}, 
      //{ point6.float3.x, point6.float3.y, point6.float3.z, /*1.0f,*/ FontColor},
	  ///{ point3.float3.x, point3.float3.y, point3.float3.z, FontColor},
	  //{ point7.float3.x, point7.float3.y, point7.float3.z, FontColor},
	  //{ point8.float3.x, point8.float3.y, point8.float3.z, FontColor},
	  //{ point4.float3.x, point4.float3.y, point4.float3.z, FontColor}
	  //*/
   };

	info.proj = *proj;
	info.view = *view;
	info.world = *world;
	unsigned int stride = sizeof(GRAPHICS_RENDER_STRUCT);
	unsigned int offset = 0;
	
	g->getDeviceContext()->UpdateSubresource(info_buffer,0,NULL,&info,0,0);
	D3D11_MAPPED_SUBRESOURCE msr;
	
	g->getDeviceContext()->Map(render_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy( msr.pData, &sttr, sizeof(GRAPHICS_RENDER_STRUCT)*2 );
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
			
	g->getDeviceContext()->Draw(2,0);
	
	CS::instance()->leave(CS_DEVICECON_CS, "leave");


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

	MYVECTOR3 point1;
	MYVECTOR3 point2;
	MYVECTOR3 point3;
	MYVECTOR3 point4;
	MYVECTOR3 point5;
	MYVECTOR3 point6;
	MYVECTOR3 point7;
	MYVECTOR3 point8;

	

	point1 = *p;//rec->c + rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point2 = *q;//rec->c + rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point3 = *r;//rec->c - rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
//	point4 = rec->c - rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
//	point5 = rec->c + rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
//	point6 = rec->c + rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
//	point7 = rec->c - rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
//	point8 = rec->c - rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
	


	CS::instance()->enter(CS_DEVICECON_CS, "render");
//	DWORD FontColor = color;
	
//	memset(sttr,0,sizeof(sttr));
	









GRAPHICS_RENDER_STRUCT sttr[] = {
 
      { point1.float3.x, point1.float3.y, point1.float3.z, /*1.0f,*/ FontColor},
      { point2.float3.x, point2.float3.y, point2.float3.z, /*1.0f,*/ FontColor}, 
      { point1.float3.x, point1.float3.y, point1.float3.z, /*1.0f,*/ FontColor}, 
      { point3.float3.x, point3.float3.y, point3.float3.z, /*1.0f,*/ FontColor},
	  { point3.float3.x, point3.float3.y, point3.float3.z, FontColor},
	  { point2.float3.x, point2.float3.y, point2.float3.z, FontColor},
	  
	  //*{ point3.float3.x, point3.float3.y, point3.float3.z, FontColor},
	  //{ point4.float3.x, point4.float3.y, point4.float3.z, FontColor},
	  
	 // { point5.float3.x, point5.float3.y, point5.float3.z, /*1.0f,*/ FontColor},
      //{ point6.float3.x, point6.float3.y, point6.float3.z, /*1.0f,*/ FontColor}, 
      //{ point5.float3.x, point5.float3.y, point5.float3.z, /*1.0f,*/ FontColor}, 
      //{ point8.float3.x, point8.float3.y, point8.float3.z, /*1.0f,*/ FontColor},
	  //{ point7.float3.x, point7.float3.y, point7.float3.z, FontColor},
	  //{ point6.float3.x, point6.float3.y, point6.float3.z, FontColor},
	  //{ point7.float3.x, point7.float3.y, point7.float3.z, FontColor},
	  //{ point8.float3.x, point8.float3.y, point8.float3.z, FontColor},

	  //{ point1.float3.x, point1.float3.y, point1.float3.z, /*1.0f,*/ FontColor},
      //{ point5.float3.x, point5.float3.y, point5.float3.z, /*1.0f,*/ FontColor}, 
      //{ point2.float3.x, point2.float3.y, point2.float3.z, /*1.0f,*/ FontColor}, 
      //{ point6.float3.x, point6.float3.y, point6.float3.z, /*1.0f,*/ FontColor},
	  ///{ point3.float3.x, point3.float3.y, point3.float3.z, FontColor},
	  //{ point7.float3.x, point7.float3.y, point7.float3.z, FontColor},
	  //{ point8.float3.x, point8.float3.y, point8.float3.z, FontColor},
	  //{ point4.float3.x, point4.float3.y, point4.float3.z, FontColor}
	  //*/
   };

	info.proj = *proj;
	info.view = *view;
	info.world = *world;
	unsigned int stride = sizeof(GRAPHICS_RENDER_STRUCT);
	unsigned int offset = 0;
	
	g->getDeviceContext()->UpdateSubresource(info_buffer,0,NULL,&info,0,0);
	D3D11_MAPPED_SUBRESOURCE msr;
	
	g->getDeviceContext()->Map(render_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy( msr.pData, &sttr, sizeof(GRAPHICS_RENDER_STRUCT)*6 );
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
			
	g->getDeviceContext()->Draw(6,0);
	
	CS::instance()->leave(CS_DEVICECON_CS, "leave");

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
	p_immediatecontext = 0;
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

	MyMatrixPerspectiveFovRH(proj, 1.57/2*2*2/3, this->getScreenWidth() / (float)this->getScreenHeight(), 1, 10000);

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
		CS::instance()->enter(CS_DEVICECON_CS,"un");
		p_devicecontext->ClearState();
		p_devicecontext->Release();
		p_devicecontext = 0;
		CS::instance()->leave(CS_DEVICECON_CS, "un");
		
	}

	if (p_immediatecontext) {
		CS::instance()->enter(CS_DEVICECON_CS,"un");
		p_immediatecontext->ClearState();
		p_immediatecontext->Release();
		p_immediatecontext = 0;
		CS::instance()->leave(CS_DEVICECON_CS, "un");

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
		new_g->Release();
		delete new_g;
		new_g = 0;
		throw new GameError(KTROBO::FATAL_ERROR, "failed in copying graphics");
	}
	p_device->GetImmediateContext(&new_g->p_immediatecontext);
	if (FAILED(hr)) {
		new_g->Release();
		delete new_g;
		new_g = 0;
		throw new GameError(KTROBO::FATAL_ERROR, "failed in copying graphics");
	}


	return new_g;
}

