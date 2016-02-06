#include "KTRoboText.h"
#include "KTRoboCS.h"
#include "KTRoboDebugText.h"

using namespace KTROBO;
Text::Text(WCHAR* m_str, int length)
{
	memset(str,0,sizeof(WCHAR)*MYTEXT_LENGTH);
	memcpy(str,m_str,length*sizeof(WCHAR));
	memset(return_str,0,sizeof(char)*MYTEXT_LENGTH*2);
}


Text::~Text(void)
{
}

void Text::changeText(WCHAR* new_str, int length) {
	CS::instance()->enter(CS_RENDERDATA_CS, "text settext");
	memset(str,0,sizeof(WCHAR)*MYTEXT_LENGTH);
	memcpy(str,new_str,length*sizeof(WCHAR));
	CS::instance()->leave(CS_RENDERDATA_CS, "text settext");

}
MYSHADERSTRUCT Text::mss;

void Text::loadShader(Graphics* g, MYSHADERSTRUCT* s, char* shader_filename, char* vs_func_name, char* gs_func_name,
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

Font* Text::fo = 0;

void Text::Init(Graphics* g, Font* f) {
 fo = f;
 D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POS", 0, DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0,16,D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

 loadShader(g, &mss, MYTEXT_SHADER_FILENAME, MYTEXT_SHADER_VS, MYTEXT_SHADER_GS, MYTEXT_SHADER_PS,
	 g->getScreenWidth(),g->getScreenHeight(), layout, 3,true);


 // vertex buffer 作る
 	D3D11_BUFFER_DESC bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(MYTEXT_RENDER_STRUCT)*MYTEXT_RENDER_STRUCT_SIZE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	HRESULT hr = g->getDevice()->CreateBuffer(&bd, NULL ,&(render_vertexbuffer));
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}

}
void Text::Del() {

	mss.Del();
	if (render_vertexbuffer) {
		render_vertexbuffer->Release();
		render_vertexbuffer = 0;
	}

}

ID3D11Buffer* Text::render_vertexbuffer=0;

void Text::render(Graphics* g, DWORD color, float sx, float sy, float height,float wd, float h) {
	MYTEXT_RENDER_STRUCT structdayo[MYTEXT_RENDER_STRUCT_SIZE];
	unsigned int screen_width = g->getScreenWidth();
	unsigned int screen_height = g->getScreenHeight();

	if (fo) {
		// MYFONT_TEXTURE_COUNT の数だけループする
		for (int fon=0; fon < MYFONT_TEXTURE_COUNT; fon++) {
			int temp_count = 0;
			CS::instance()->enter(CS_RENDERDATA_CS, "render text");
			int leng = wcslen(str);
			for (int i=0;i<leng;i++) {
				MYTEXT_RENDER_STRUCT* v = &structdayo[temp_count*6];
				WCHAR* w = &str[i];
				int texture_index = (fo->fonttexture_index)[(int)(*w)];
				if (texture_index != fon) continue;
				int font_code = (fo->fontindex)[(int)(*w)];

				float xcode_offset = MYFONT_FONTSIZE / (float)MYFONT_TEXTURE_WIDTH;
				float ycode_offset = MYFONT_FONTSIZE / (float)MYFONT_TEXTURE_HEIGHT;
				float xcode = (float)(font_code % MYFONT_TEXTURE_WIDTH) / (float)MYFONT_TEXTURE_WIDTH + xcode_offset;
				float ycode = (float)(font_code / MYFONT_TEXTURE_WIDTH) / (float)MYFONT_TEXTURE_HEIGHT;
	

				float xoffset =  2 * height/ (float)wd;//screen_width;
				float yoffset =  2 * height/ (float)h;//screen_height;

				float x = -1 + xoffset * i;// + sx / (float)screen_width;
				float y = 1;// -  sy / (float)screen_height;



				v[0].pos.x = x; v[0].pos.y = y; /*v[0].pos.float3.z = 0.0f;*/
				v[0].tex.x = xcode; v[0].tex.y = ycode; /*v[0].tex_coord_plus_tex_index.float3.z = (float)texture_index;*/

				v[1].pos.x = x; v[1].pos.y = y - yoffset; /*v[1].pos.float3.z = 0.0f;*/
				v[1].tex.x = xcode; v[1].tex.y = ycode + ycode_offset; /*v[1].tex_coord_plus_tex_index.float3.z = (float)texture_index;*/

				v[2].pos.x = x+xoffset; v[2].pos.y = y;/* v[2].pos.float3.z = 0.0f;*/
				v[2].tex.x = xcode+xcode_offset; v[2].tex.y = ycode;/* v[2].tex_coord_plus_tex_index.float3.z = (float)texture_index;*/

				v[3].pos.x = x+xoffset; v[3].pos.y = y; /*v[3].pos.z = 0.0f;*/
				v[3].tex.x = xcode+xcode_offset; v[3].tex.y = ycode; /*v[3].tex_coord_plus_tex_index.float3.z = (float)texture_index;*/

				v[4].pos.x = x; v[4].pos.y = y-yoffset; //v[4].pos.z = 0.0f;
				v[4].tex.x = xcode; v[4].tex.y = ycode+ycode_offset;/* v[4].tex_coord_plus_tex_index.float3.z = (float)texture_index;*/

				v[5].pos.x = x+xoffset; v[5].pos.y = y-yoffset; //v[5].pos.z = 0.0f;
				v[5].tex.x = xcode+xcode_offset; v[5].tex.y = ycode+ycode_offset;/* v[5].tex_coord_plus_tex_index.float3.z = (float)texture_index;*/
				
				for (int k=0;k<6;k++) {
					v[k].color = color;
				}
				
				
				temp_count++;
			}
			CS::instance()->leave(CS_RENDERDATA_CS, "render text");
			if (temp_count > 0 ) {
				// 描画するものがあるので
				unsigned int stride = sizeof(MYTEXT_RENDER_STRUCT);
				unsigned int offset = 0;
				CS::instance()->enter(CS_DEVICECON_CS, "render exec");
				D3D11_MAPPED_SUBRESOURCE sub;
				sub.pData = 0;
				sub.DepthPitch = 0;
				sub.RowPitch = 0;
				D3D11_VIEWPORT vp;
				vp.Height = h;
				vp.MaxDepth = 1.0f;
				vp.MinDepth = 0.0f;
				vp.TopLeftX = sx;
				vp.TopLeftY = sy;
				vp.Width = wd;
				g->getDeviceContext()->RSSetViewports(1,&vp);
				g->getDeviceContext()->Map(render_vertexbuffer,0,D3D11_MAP_WRITE_DISCARD,0,&sub);
				memcpy(sub.pData,structdayo, sizeof(MYTEXT_RENDER_STRUCT)*temp_count*6);
				g->getDeviceContext()->Unmap(render_vertexbuffer,0);

				g->getDeviceContext()->IASetInputLayout(mss.vertexlayout );
				g->getDeviceContext()->IASetVertexBuffers( 0, 1, &render_vertexbuffer, &stride, &offset );
				g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
				g->getDeviceContext()->RSSetState(mss.rasterstate);

				float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

				g->getDeviceContext()->OMSetBlendState(mss.blendstate, blendFactor,0xFFFFFFFF/*0xFFFFFFFF*/);
				g->getDeviceContext()->VSSetShader(mss.vs, NULL, 0);
				g->getDeviceContext()->GSSetShader(NULL,NULL,0);
				g->getDeviceContext()->PSSetShaderResources(0,1,&fo->fonttextureviews[fon]);//render_target_tex->view);
				g->getDeviceContext()->PSSetSamplers(0,1, &(DebugTexts::instance()->p_sampler));
		
				g->getDeviceContext()->PSSetShader(mss.ps, NULL, 0);
			
				g->getDeviceContext()->Draw(temp_count*6,0);

				CS::instance()->leave(CS_DEVICECON_CS, "render exec");

			}



		}
	}
}