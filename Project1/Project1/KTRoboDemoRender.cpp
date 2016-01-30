#include "KTRoboDemoRender.h"
#include "KTRoboGraphics.h"
#include "KTRoboGameError.h"


using namespace KTROBO;

struct SimpleVertex
{
    XMFLOAT3 Pos;
	XMFLOAT2 tex_coord;
};

void KTRoboDemoRender::Release() {
	if (vs) {
		vs->Release();
		vs = 0;
	}
	if (gs) {
		gs->Release();
		gs = 0;
	}
	if (ps) {
		ps->Release();
		ps = 0;
	}

	if (p_vertexlayout) {
		p_vertexlayout->Release();
		p_vertexlayout = 0;
	}

	if (p_vertexbuffer) {
		p_vertexbuffer->Release();
		p_vertexbuffer = 0;
	}

	if (font) {
		font->Release();
		delete font;
		font = 0;
	}

	if (p_blendstate) {
		p_blendstate->Release();
		p_blendstate = 0;
	}

	if (tex_loader) {
		tex_loader->del();
		delete tex_loader;
		tex_loader = 0;
	}

	if (p_sampler) {
		p_sampler->Release();
	//	delete p_sampler;
		p_sampler = 0;
	}

	if (p_rasterstate) {
		p_rasterstate->Release();
		p_rasterstate = 0;
	}
}


void KTRoboDemoRender::Init(Graphics* g) {

	
	HRESULT hr = S_OK;
	ID3DBlob* pblob = 0;
	font = 0;
	p_blendstate = 0;
	p_sampler = 0;
	p_rasterstate = 0;
	try {
		CompileShaderFromFile("resrc/shader/sample.fx", "VSFunc", "vs_4_0", &pblob);
		hr = g->getDevice()->CreateVertexShader(pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			NULL,
			&vs);
		if (FAILED(hr)) {
			pblob->Release();
			pblob = 0;
			vs = 0;
		}

		// 入力レイアウトの定義
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
		};
		
		unsigned int num = sizeof(layout)/ sizeof(layout[0]);
			
		hr = g->getDevice()->CreateInputLayout(
			layout,num,pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			&p_vertexlayout);
		if (FAILED(hr)) {
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "layout make error");;
		}
		if (pblob) {
			pblob->Release();
			pblob = 0;
		}
		 // Set the input layout
		g->getDeviceContext()->IASetInputLayout( p_vertexlayout );

		/*
		
		CompileShaderFromFile("resrc/shader/sample.fx", "GSFunc", "gs_4_0", &pblob);
		hr = g->getDevice()->CreateGeometryShader(pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			NULL, &gs);
		if (FAILED(hr)) {
			pblob->Release();
			pblob = 0;
			gs = 0;
		}
			if (pblob) {
			pblob->Release();
			pblob = 0;
		}
		*/
		CompileShaderFromFile("resrc/shader/sample.fx", "PSFunc", "ps_4_0", &pblob);
		hr = g->getDevice()->CreatePixelShader(pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			NULL,&ps);
		if (FAILED(hr)) {
			pblob->Release();
			pblob = 0;
			ps = 0;
		}

		if (pblob) {
			pblob->Release();
			pblob = 0;
		}

		D3D11_BLEND_DESC BlendDesc;
		memset(&BlendDesc, 0, sizeof(BlendDesc));
		BlendDesc.AlphaToCoverageEnable = FALSE;
		BlendDesc.IndependentBlendEnable = FALSE;
		BlendDesc.RenderTarget[0].BlendEnable = FALSE;
		BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_ALPHA;
		BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = g->getDevice()->CreateBlendState(&BlendDesc,&p_blendstate);
		if (FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in blend state create");
		}

	} catch (KTROBO::GameError* e) {
		if (pblob) {
			pblob->Release();
			pblob = 0;
		}

		Release();

		
	//	MessageBoxA(NULL,e->getMessage(), "shader compile error", MB_OK);
		throw e;
	}
	float de = 1/(float)4;//512.0/2049;//*512/4096.0f;
	float te = 1.0f/2 * 1  * 2;
	float of = 1.0f/4 * 1  * 2 ;
	float xof = 1.0f/4 * 1  * 2 ;
	// 頂点バッファの作成
	SimpleVertex vertices[] = {

		{XMFLOAT3(0.0f-te-xof,0.0f,0.0f),XMFLOAT2(0.0f,1.0f*de)},
		{XMFLOAT3(0.0f-te-xof,1.0f * te,0.0f),XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(1.0f* te-te-xof,0.0f,0.0f),XMFLOAT2(1.0f*de,1.0f*de)},
		{XMFLOAT3(1.0f* te-te-xof,0.0f,0.0f),XMFLOAT2(1.0f*de,1.0f*de)},
		{XMFLOAT3(0.0f-te-xof,1.0f* te,0.0f),XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(1.0f*te-te-xof,1.0f*te,0.0f),XMFLOAT2(1.0f*de,0.0f)},


		{XMFLOAT3(0.0f-xof,0.0f,0.0f),XMFLOAT2(0.0f,1.0f*de/2+de/2)},
		{XMFLOAT3(0.0f-xof,1.0f * te/2,0.0f),XMFLOAT2(0.0f,0.0f+de/2)},
		{XMFLOAT3(1.0f* te/2-xof,0.0f,0.0f),XMFLOAT2(1.0f*de/2,1.0f*de/2+de/2)},
		{XMFLOAT3(1.0f* te/2-xof,0.0f,0.0f),XMFLOAT2(1.0f*de/2,1.0f*de/2+de/2)},
		{XMFLOAT3(0.0f-xof,1.0f* te/2,0.0f),XMFLOAT2(0.0f,0.0f+de/2)},
		{XMFLOAT3(1.0f*te/2-xof,1.0f*te/2,0.0f),XMFLOAT2(1.0f*de/2,0.0f+de/2)},

		{XMFLOAT3(0.0f + of-xof ,0.0f,0.0f),XMFLOAT2(0.0f+de/2,1.0f*de/2+de/2)},
		{XMFLOAT3(0.0f+of-xof,1.0f * te/2,0.0f),XMFLOAT2(0.0f+de/2,0.0f+de/2)},
		{XMFLOAT3(1.0f* te/2+of-xof,0.0f,0.0f),XMFLOAT2(1.0f*de/2+de/2,1.0f*de/2+de/2)},
		{XMFLOAT3(1.0f* te/2+of-xof,0.0f,0.0f),XMFLOAT2(1.0f*de/2+de/2,1.0f*de/2+de/2)},
		{XMFLOAT3(0.0f+of-xof,1.0f* te/2,0.0f),XMFLOAT2(0.0f+de/2,0.0f+de/2)},
		{XMFLOAT3(1.0f*te/2+of-xof,1.0f*te/2,0.0f),XMFLOAT2(1.0f*de/2+de/2,0.0f+de/2)},
	
	
		{XMFLOAT3(0.0f-xof,0.0f+of,0.0f),XMFLOAT2(0.0f,1.0f*de/2)},
		{XMFLOAT3(0.0f-xof,1.0f * te/2+of,0.0f),XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(1.0f* te/2-xof,0.0f+of,0.0f),XMFLOAT2(1.0f*de/2,1.0f*de/2)},
		{XMFLOAT3(1.0f* te/2-xof,0.0f+of,0.0f),XMFLOAT2(1.0f*de/2,1.0f*de/2)},
		{XMFLOAT3(0.0f-xof,1.0f* te/2+of,0.0f),XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(1.0f*te/2-xof,1.0f*te/2+of,0.0f),XMFLOAT2(1.0f*de/2,0.0f)},
	
		{XMFLOAT3(0.0f+of-xof,0.0f+of,0.0f),XMFLOAT2(0.0f+de/2,1.0f*de/2)},
		{XMFLOAT3(0.0f+of-xof,1.0f * te/2+of,0.0f),XMFLOAT2(0.0f+de/2,0.0f)},
		{XMFLOAT3(1.0f* te/2+of-xof,0.0f+of,0.0f),XMFLOAT2(1.0f*de/2+de/2,1.0f*de/2)},
		{XMFLOAT3(1.0f* te/2+of-xof,0.0f+of,0.0f),XMFLOAT2(1.0f*de/2+de/2,1.0f*de/2)},
		{XMFLOAT3(0.0f+of-xof,1.0f* te/2+of,0.0f),XMFLOAT2(0.0f+de/2,0.0f)},
		{XMFLOAT3(1.0f*te/2+of-xof,1.0f*te/2+of,0.0f),XMFLOAT2(1.0f*de/2+de/2,0.0f)},
	};

	D3D11_BUFFER_DESC bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex)*30;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	memset(&initData, 0, sizeof(initData));
	initData.pSysMem = vertices;

	hr = g->getDevice()->CreateBuffer(&bd, &initData,&p_vertexbuffer);
	if (FAILED(hr)) {
		Release();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
		
	}

	unsigned int stride = sizeof(SimpleVertex);
	unsigned int offset = 0;
	
	g->getDeviceContext()->IASetInputLayout(p_vertexlayout);
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, &p_vertexbuffer, &stride, &offset );
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	
	font = new Font();
	font->Init("resrc/azukiL.ttf","あずきフォントL",g);


	tex_loader = new MyTextureLoader();
	tex_loader->init(g);
	render_target_tex = tex_loader->makeClass(1024,1024);
	msaa_tex = tex_loader->makeClass(1024,1024);


	D3D11_SAMPLER_DESC descS;
	memset(&descS, 0, sizeof(descS));
	descS.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	descS.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	descS.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	descS.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	descS.BorderColor[0] = 0;
	descS.BorderColor[1] = 0;
	descS.BorderColor[2] = 0;
	descS.BorderColor[3] = 0;
	descS.ComparisonFunc = D3D11_COMPARISON_NEVER;
	descS.MaxAnisotropy = 1;
	descS.MinLOD = 1;
	descS.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g->getDevice()->CreateSamplerState(&descS, &p_sampler);
	if (FAILED(hr)) {
		Release();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "sampler make error");;
		
	}





	D3D11_RASTERIZER_DESC _rasterDesc;
	_rasterDesc.AntialiasedLineEnable = false;//true; // changed to true
	_rasterDesc.CullMode = D3D11_CULL_NONE;
	_rasterDesc.DepthBias = 0;
	_rasterDesc.DepthBiasClamp = 0.0f;
	_rasterDesc.DepthClipEnable = false;
	_rasterDesc.FillMode = D3D11_FILL_SOLID;
	_rasterDesc.FrontCounterClockwise = false;
	_rasterDesc.MultisampleEnable = false;//true; // changed to true
	_rasterDesc.ScissorEnable = false;
	_rasterDesc.SlopeScaledDepthBias = 0.0f;
	hr  = g->getDevice()->CreateRasterizerState( &_rasterDesc, &p_rasterstate );
	if( FAILED( hr ) ) {
		Release();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "raster make error");;
	}
}

KTRoboDemoRender::KTRoboDemoRender() {
	vs = 0;
	gs = 0;
	ps = 0;
	p_vertexbuffer = 0;
	p_vertexlayout = 0;

	tex_loader = 0;
	render_target_tex = 0;
	
}
KTRoboDemoRender::~KTRoboDemoRender(void)
{
	
}



void KTRoboDemoRender::Render(Graphics* g, MyTextureLoader::MY_TEXTURE_CLASS* cc) {

	
	float clearColor2[4] = {
		1.0f,1.0f,1.0f,0.9f};
	unsigned int stride = sizeof(SimpleVertex);
	unsigned int offset = 0;
	//g->getDeviceContext()->ClearRenderTargetView(g->getRenderTargetView(),clearColor);
	g->getDeviceContext()->IASetInputLayout(p_vertexlayout);
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, &p_vertexbuffer, &stride, &offset );
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	

	//g->getDeviceContext()->IASetInputLayout(p_vertexlayout);

	//g->getDeviceContext()->OMSetRenderTargets(1, &this->msaa_tex->target_view, NULL);
	/*D3D11_VIEWPORT pvp;
	pvp.MaxDepth = 1.0f;
	pvp.MinDepth = 0.0f;
	pvp.Height = (int)render_target_tex->height;
	pvp.Width = (int)render_target_tex->width;
	pvp.TopLeftX = 0;
	pvp.TopLeftY = 0;

	
	g->getDeviceContext()->RSSetViewports(1,&pvp);
	g->getDeviceContext()->ClearRenderTargetView(this->render_target_tex->target_view, clearColor2);
	g->getDeviceContext()->PSSetShaderResources(0,1,&font->fonttextureviews[2]);
	g->getDeviceContext()->VSSetShader(vs,NULL,0);
	g->getDeviceContext()->GSSetShader(gs,NULL,0);
	g->getDeviceContext()->PSSetShader(ps,NULL,0);
	
	g->getDeviceContext()->Draw(6,0);
	*/
	//g->getDeviceContext()->ResolveSubresource(msaa_tex->tex, 0, font->fonttextures[0],0, DXGI_FORMAT_B8G8R8A8_UNORM);

	//ID3D11RenderTargetView* r = g->getRenderTargetView();
	
//	g->getDeviceContext()->OMSetRenderTargets(1, &r, NULL);
//	g->getDeviceContext()->RSSetViewports(1,g->getViewPort());

	
/*	g->getDeviceContext()->PSSetShaderResources(0,1,&font->fonttextureviews[0]);
	g->getDeviceContext()->VSSetShader(vs,NULL,0);
	//g->getDeviceContext()->GSSetShader(gs,NULL,0);
	g->getDeviceContext()->PSSetShader(ps,NULL,0);*/


	//for (int i=0;i<4;i++) { 
	g->getDeviceContext()->RSSetState(p_rasterstate);
	g->getDeviceContext()->VSSetShader(vs,NULL,0);
	//g->getDeviceContext()->GSSetShader(gs,NULL,0);
	g->getDeviceContext()->PSSetShader(ps,NULL,0);
	g->getDeviceContext()->PSSetShaderResources(0,1,&cc->view);
	g->getDeviceContext()->PSSetSamplers(0,1, &p_sampler);
	
	g->getDeviceContext()->Draw(30,0);
	
/*
	g->getDeviceContext()->PSSetShaderResources(0,1,&font->fonttextureviews[2]);
	g->getDeviceContext()->Draw(6,0);
	*/
	//}
	//g->getSwapChain()->Present(0,0);

}