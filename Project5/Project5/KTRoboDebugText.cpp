#include "KTRoboDebugText.h"
#include "KTRoboGameError.h"
#include "KTRoboCS.h"


using namespace KTROBO;
void DebugText::setVertex(Font* f, int screen_width, int screen_height, int line_num, int height, int wchar_num, WCHAR* text) {

	WCHAR* new_text = new WCHAR[wchar_num+1];
	wcscpy_s(new_text, wchar_num+1, text);
	new_text[wchar_num] = '\0';
	if (raw_text) {
		delete[] raw_text;
		raw_text = 0;
	}
	if (texts) {
		delete[] texts;
		texts = 0;
	}

	raw_text = new_text;
	this->text_count = wchar_num;
	this->vertex_count = text_count * 6;

	this->texts = new KTROBO::SimpleDebugTextVertex[vertex_count];
//	memset(texts, 0, sizeof(SimpleDebugTextVertex) * vertex_count);

	// このなかにvertex定義を入れていく
	for (int i = 0; i < text_count; i++) {
		SimpleDebugTextVertex* v = &texts[i*6];
		WCHAR* w = &raw_text[i];
		int texture_index = (f->fonttexture_index)[(int)(*w)];
		int font_code = (f->fontindex)[(int)(*w)];

		float xcode_offset = MYFONT_FONTSIZE / (float)MYFONT_TEXTURE_WIDTH;
		float ycode_offset = MYFONT_FONTSIZE / (float)MYFONT_TEXTURE_HEIGHT;
		float xcode = (float)(font_code % MYFONT_TEXTURE_WIDTH) / (float)MYFONT_TEXTURE_WIDTH + xcode_offset;
		float ycode = (float)(font_code / MYFONT_TEXTURE_WIDTH) / (float)MYFONT_TEXTURE_HEIGHT;
	

		float xoffset = height/ (float)screen_width;
		float yoffset = height/ (float)screen_height;

		float x = -1 + xoffset * i;
		float y = 1 - line_num * yoffset - yoffset;



		v[0].pos.float3.x = x; v[0].pos.float3.y = y; v[0].pos.float3.z = 0.0f;
		v[0].tex_coord_plus_tex_index.float3.x = xcode; v[0].tex_coord_plus_tex_index.float3.y = ycode + ycode_offset; v[0].tex_coord_plus_tex_index.float3.z = (float)texture_index;

		v[1].pos.float3.x = x; v[1].pos.float3.y = y + yoffset; v[1].pos.float3.z = 0.0f;
		v[1].tex_coord_plus_tex_index.float3.x = xcode; v[1].tex_coord_plus_tex_index.float3.y = ycode; v[1].tex_coord_plus_tex_index.float3.z = (float)texture_index;

		v[2].pos.float3.x = x+xoffset; v[2].pos.float3.y = y; v[2].pos.float3.z = 0.0f;
		v[2].tex_coord_plus_tex_index.float3.x = xcode+xcode_offset; v[2].tex_coord_plus_tex_index.float3.y = ycode + ycode_offset; v[2].tex_coord_plus_tex_index.float3.z = (float)texture_index;

		v[3].pos.float3.x = x+xoffset; v[3].pos.float3.y = y; v[3].pos.float3.z = 0.0f;
		v[3].tex_coord_plus_tex_index.float3.x = xcode+xcode_offset; v[3].tex_coord_plus_tex_index.float3.y = ycode + ycode_offset; v[3].tex_coord_plus_tex_index.float3.z = (float)texture_index;

		v[4].pos.float3.x = x; v[4].pos.float3.y = y+yoffset; v[4].pos.float3.z = 0.0f;
		v[4].tex_coord_plus_tex_index.float3.x = xcode; v[4].tex_coord_plus_tex_index.float3.y = ycode; v[4].tex_coord_plus_tex_index.float3.z = (float)texture_index;

		v[5].pos.float3.x = x+xoffset; v[5].pos.float3.y = y+yoffset; v[5].pos.float3.z = 0.0f;
		v[5].tex_coord_plus_tex_index.float3.x = xcode+xcode_offset; v[5].tex_coord_plus_tex_index.float3.y = ycode; v[5].tex_coord_plus_tex_index.float3.z = (float)texture_index;
/*
		{XMFLOAT3(0.0f-te-xof,0.0f,0.5f),XMFLOAT2(0.0f,1.0f*de)},
		{XMFLOAT3(0.0f-te-xof,1.0f * te,0.5f),XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(1.0f* te-te-xof,0.0f,0.5f),XMFLOAT2(1.0f*de,1.0f*de)},
		{XMFLOAT3(1.0f* te-te-xof,0.0f,0.5f),XMFLOAT2(1.0f*de,1.0f*de)},
		{XMFLOAT3(0.0f-te-xof,1.0f* te,0.5f),XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(1.0f*te-te-xof,1.0f*te,0.5f),XMFLOAT2(1.0f*de,0.0f)},
*/


	}

}


void DebugTexts::makeVertexBuffer(Graphics* g) {
	// 各フォントテクスチャに対するvertexcountの総和を求める
	for (int i = 0; i < MYFONT_TEXTURE_COUNT;i++) {
		this->vertex_counts[i] = 0;
	}

	for (int k = 0; k < KTROBO_DEBUG_TEXT_NUM;k++) {
		int vertex_count = texts[k].getVertexCount();
		if (vertex_count) {
			for (int h = 0; h < vertex_count; h++) {
				SimpleDebugTextVertex* v = texts[k].getVertex();
				int texture_index = (int)floor((v[h].tex_coord_plus_tex_index.float3.z + 0.01f));
				if (texture_index >= MYFONT_TEXTURE_COUNT) {
					throw new GameError(KTROBO::FATAL_ERROR, "no texture count in debugtext");
				}
				vertex_counts[texture_index] += 1;
			}
		}
	}

	for (int i=0;i<MYFONT_TEXTURE_COUNT;i++) {
		if (this->p_vertexbuffers[i]) {
			p_vertexbuffers[i]->Release();
			p_vertexbuffers[i] = 0;
		}
	}

	for (int i = 0 ; i < MYFONT_TEXTURE_COUNT;i++) {
		if (!vertex_counts[i]) continue;

		SimpleDebugTextVertex* vs = new SimpleDebugTextVertex[vertex_counts[i]];
		int now_count = 0;
		for (int k = 0; k < KTROBO_DEBUG_TEXT_NUM;k++) {
			int vertex_count = texts[k].getVertexCount();
			if (vertex_count) {
				for (int h = 0; h < vertex_count; h++) {
					SimpleDebugTextVertex* v = texts[k].getVertex();
					int texture_index = (int)floor((v[h].tex_coord_plus_tex_index.float3.z + 0.01f));
					if (texture_index >= MYFONT_TEXTURE_COUNT) {
						delete[] vs;
						throw new GameError(KTROBO::FATAL_ERROR, "no texture count in debugtext");
					}

					if (texture_index == i) {
						// 加える
						vs[now_count] = v[h];
						now_count++;
					}
						
				}
			}
		}
		// vertexbuffer を作る
		
		D3D11_BUFFER_DESC bd;
		memset(&bd,0,sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleDebugTextVertex)*vertex_counts[i];
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		memset(&initData, 0, sizeof(initData));
		initData.pSysMem = vs;

		HRESULT hr = g->getDevice()->CreateBuffer(&bd, &initData,&p_vertexbuffers[i]);
		if (FAILED(hr)) {
			delete[] vs;
			Del();
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
		}

		delete[] vs;
	}




}


void DebugTexts::setText(Graphics* g, int wchar_num, WCHAR* text) {

	CS::instance()->enter(CS_RENDERDATA_CS, "render settext");
	int line_num = insert_text_point % KTROBO_DEBUG_TEXT_NUM;
	bool is_kakikae_line_num = false;
	if (insert_text_point / KTROBO_DEBUG_TEXT_NUM > 0 ) {
		//line_num = KTROBO_DEBUG_TEXT_NUM -1;
		is_kakikae_line_num = true;
	}

	
	if (is_kakikae_line_num) {
		// ひとつずつずらしていれていく
		for (int i = 1; i < KTROBO_DEBUG_TEXT_NUM; i++) {
			int now_line_num = (KTROBO_DEBUG_TEXT_NUM - i + line_num) % KTROBO_DEBUG_TEXT_NUM;
			if (texts[now_line_num].getTextCount()) {
				texts[now_line_num].setVertex(f, g->getScreenWidth(), g->getScreenHeight(),(KTROBO_DEBUG_TEXT_NUM-1-i) % KTROBO_DEBUG_TEXT_NUM, KTROBO_DEBUG_TEXT_HEIGHT, texts[now_line_num].getTextCount(), texts[now_line_num].getRawText());
			}


		}
	}

	if (is_kakikae_line_num) {
		this->texts[line_num].setVertex(this->f, g->getScreenWidth(), g->getScreenHeight(), KTROBO_DEBUG_TEXT_NUM-1, KTROBO_DEBUG_TEXT_HEIGHT, wchar_num, text);
	} else {
		this->texts[line_num].setVertex(this->f, g->getScreenWidth(), g->getScreenHeight(), line_num, KTROBO_DEBUG_TEXT_HEIGHT, wchar_num, text);
	
	}
	insert_text_point ++;
	try {
	makeVertexBuffer(g);
	}catch (GameError* err) {
		CS::instance()->leave(CS_RENDERDATA_CS, "render settext");
		throw err;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "render settext");

}

void DebugTexts::render(Graphics* g) {
	
	unsigned int stride = sizeof(SimpleDebugTextVertex);
	unsigned int offset = 0;
	
	//g->getDeviceContext()->IASetInputLayout(p_vertexlayout);
	CS::instance()->enter(CS_RENDERDATA_CS, "render text");
	for (int i =0;i<MYFONT_TEXTURE_COUNT;i++) {
		if (this->vertex_counts[i]) {
			g->getDeviceContext()->IASetInputLayout( p_vertexlayout );
			g->getDeviceContext()->IASetVertexBuffers( 0, 1, &p_vertexbuffers[i], &stride, &offset );
			g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			g->getDeviceContext()->RSSetState(p_rasterstate);

			float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

			g->getDeviceContext()->OMSetBlendState(p_blendstate, blendFactor,0xFFFFFFFF/*0xFFFFFFFF*/);
			g->getDeviceContext()->VSSetShader(this->vs, NULL, 0);
			g->getDeviceContext()->GSSetShader(NULL,NULL,0);
			g->getDeviceContext()->PSSetShaderResources(0,1,&f->fonttextureviews[i]);//render_target_tex->view);
			g->getDeviceContext()->PSSetSamplers(0,1, &p_sampler);
		
			g->getDeviceContext()->PSSetShader(this->ps, NULL, 0);
			
			g->getDeviceContext()->Draw(this->vertex_counts[i],0);
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "render settext");
	//g->getSwapChain()->Present(0,0);
}
void DebugTexts::Init(Graphics* g, Font* f) {
	this->f = f;
	insert_text_point = 0;
	memset(vertex_counts,0,sizeof(vertex_counts));
	
	for (int i = 0; i<MYFONT_TEXTURE_COUNT;i++) {
		this->p_vertexbuffers[i] = 0;
	}


	HRESULT hr = S_OK;
	ID3DBlob* pblob = 0;
	p_blendstate = 0;
	p_sampler = 0;
	p_vertexlayout = 0;
	vs = 0;
	ps = 0;
	p_rasterstate = 0;
	try {
		CompileShaderFromFile(KTROBO_DEBUG_TEXT_SHADER_FILENAME, KTROBO_DEBUG_TEXT_SHADER_VS, "vs_4_0", &pblob);
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
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
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
	

		CompileShaderFromFile(KTROBO_DEBUG_TEXT_SHADER_FILENAME, KTROBO_DEBUG_TEXT_SHADER_PS, "ps_4_0", &pblob);
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
		BlendDesc.RenderTarget[0].BlendEnable = TRUE;
		BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].SrcBlendAlpha =D3D11_BLEND_ZERO;
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

		Del();

		
	//	MessageBoxA(NULL,e->getMessage(), "shader compile error", MB_OK);
		throw e;
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
	hr  = g->getDevice()->CreateRasterizerState( &_rasterDesc, &p_rasterstate );
	if( FAILED( hr ) ) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "raster make error");;
	}

}
void DebugTexts::Del() {
	
	for (int i = 0 ; i < MYFONT_TEXTURE_COUNT;i++) {
		if (this->p_vertexbuffers[i]) {
			this->p_vertexbuffers[i]->Release();
			p_vertexbuffers[i] = 0;
		}
	}




	if (p_blendstate) {
		p_blendstate->Release();
		p_blendstate = 0;
	}
	if (p_sampler) {
		p_sampler->Release();
		p_sampler = 0;
	}
	if (p_vertexlayout) {
		p_vertexlayout->Release();
		p_vertexlayout = 0;
	}
	if (vs) {
		vs->Release();
		vs = 0;
	}
	if (ps) {
		ps->Release();
		ps = 0;
	}
	if (p_rasterstate) {
		p_rasterstate->Release();
		p_rasterstate = 0;
	}


}