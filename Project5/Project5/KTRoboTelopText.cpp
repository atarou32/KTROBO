#include "KTRoboTelopText.h"
#include "KTRoboGameError.h"
#include "KTRoboDebugText.h"
#include "string.h"
#include <vector>
#include "stringconverter.h"


using namespace KTROBO;
using namespace std;

TelopTexts::TelopTexts(void)
{
	
	f = 0;
	sum_plustime = 0;
	line_num = 0;

	for (int i = 0 ; i < MYFONT_TEXTURE_COUNT; i++) {
		p_vertexbuffers[i] = 0;
		vertex_counts[i] = 0;
	}
	vs = 0;
	ps = 0;
	p_vertexlayout = 0;
	p_blendstate = 0;
	p_sampler = 0;
	p_rasterstate = 0;

	memset(&cbuf1, 0 , sizeof(TelopTextsCBuf1));
	memset(&cbuf2, 0, sizeof(TelopTextsCBuf2));
	cbuf1_buffer = 0;
	cbuf2_buffer = 0;
}


TelopTexts::~TelopTexts(void)
{
}


void TelopTexts::Init(Graphics* g, Font* f) {

	this->f = f;
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
		CompileShaderFromFile(KTROBO_TELOP_TEXT_SHADER_FILENAME, KTROBO_TELOP_TEXT_SHADER_VS, "vs_4_0", &pblob);
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
	

		CompileShaderFromFile(KTROBO_TELOP_TEXT_SHADER_FILENAME, KTROBO_TELOP_TEXT_SHADER_PS, "ps_4_0", &pblob);
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
		BlendDesc.RenderTarget[0].SrcBlendAlpha =D3D11_BLEND_ONE;
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


}


void TelopTexts::Del() {

	for (int i = 0 ; i < MYFONT_TEXTURE_COUNT; i++) {
		if (p_vertexbuffers[i]) {
			p_vertexbuffers[i]->Release();
			p_vertexbuffers[i] = 0;
		}
		vertex_counts[i] = 0;
	}

	if (vs) {
		vs->Release();
		vs = 0;
	}

	if (ps) {
		ps->Release();
		ps = 0;
	}

	if (p_vertexlayout) {
		p_vertexlayout->Release();
		p_vertexlayout = 0;
	}

	if (p_blendstate) {
		p_blendstate->Release();
		p_blendstate = 0;
	}

	if (p_sampler) {
		p_sampler->Release();
		p_sampler = 0;
	}

	if (p_rasterstate) {
		p_rasterstate->Release();
		p_rasterstate = 0;
	}

	if (cbuf1_buffer) {
		cbuf1_buffer->Release();
		cbuf1_buffer = 0;
	}

	if (cbuf2_buffer) {
		cbuf2_buffer->Release();
		cbuf2_buffer = 0;
	}
}

void strReplace (std::string& str, const std::string& from, const std::string& to) {
    std::string::size_type pos = 0;
    while(pos = str.find(from, pos), pos != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
}


void TelopTexts::readFile(Graphics* g , char* filename, int height, int transparent_text_line_num, MYVECTOR4* text_color, float text_speed) {

	for (int i = 0 ; i < MYFONT_TEXTURE_COUNT;i++) {
		if (p_vertexbuffers[i]) {
			p_vertexbuffers[i]->Release();
			p_vertexbuffers[i] = 0;
		}
	}

	sum_plustime = 0;
	cbuf1.offset = 0;
	cbuf1.offset2 = 0;
	cbuf1.plustime = sum_plustime;
	cbuf1.text_speed = text_speed;
	g->getDeviceContext()->UpdateSubresource(cbuf1_buffer,0,NULL,&cbuf1,0,0);
	cbuf2.screen_height = g->getScreenHeight();
	cbuf2.screen_width = g->getScreenWidth();
	cbuf2.text_color = *text_color;
	cbuf2.text_height = height;
	cbuf2.transparent_text_line_num = transparent_text_line_num;
	g->getDeviceContext()->UpdateSubresource(cbuf2_buffer,0,NULL,&cbuf2,0,0);

	SimpleDebugTextVertex* temp_vertexs[MYFONT_TEXTURE_COUNT];
	memset(temp_vertexs, 0, sizeof(temp_vertexs));
	line_num = 0;
	FILE* fp;
	errno_t t = fopen_s(&fp, filename, "rb");
		
	if (t != 0) {
		char buf[1024];
		memset(buf,0,1024);
		sprintf_s(buf,512, "teloptext error errcode=%d",t);
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, buf);
	}
		/* ファイルサイズを調査 */
	fpos_t fsize;
	fpos_t fsize_all=0;
	fseek(fp,0,SEEK_END); 
	fgetpos(fp,&fsize);
	//mylog::writelog("fsizedayo:%d fsize_all=%d\n",(int)fsize,(int)fsize_all);
	if (fsize > 10000000 || fsize == 0) {
		fclose(fp);
		char buf[1024];
		memset(buf,0,1024);
		sprintf_s(buf,512, "teloptextは大きすぎる またはちいさすぎる　%d",fsize);
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, buf);
		
	}
	char* buffer=new char[(unsigned int)fsize+1];
	fseek(fp,0,SEEK_SET);
	size_t readsize=0;
	while (fsize_all < fsize) {
		if (readsize > fsize - fsize_all) {

			fclose(fp);
			char buf[1024];
			memset(buf,0,1024);
			sprintf_s(buf,512, "%s の読み込みで不具合 %d %d",filename,readsize,fsize);
			
			delete[] buffer;
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, buf);
		}

		readsize = fread_s(&buffer[fsize_all],(size_t)(fsize-fsize_all),1,(size_t)(fsize-fsize_all),fp);
		fsize_all = readsize + fsize_all;
		fseek(fp,(long)fsize_all,SEEK_SET);
		//mylog::writelog("%d:size %d:readsize %d:fsize\n",(int)fsize_all,(int)readsize,(int)fsize);
	}
	buffer[fsize]='\0';
	fclose(fp);

	int temp_now = 0;
	int temp_next =0;
	int temp_line = 0;
	vector<string> strings;

	while (temp_next < fsize && buffer[temp_next] != '\0') {

		if (buffer[temp_next] == '\n') {

			char buf[512];
			memset(buf,0,512);
			int now_index = 0;
			for (int i = temp_now; i < temp_next;i++) {
				buf[now_index] = buffer[i];
				now_index++;
				if (now_index > 511) {
					break;
				}
			}
			buf[511] = '\0';
			string s = string(buf);
			strReplace(s, "\r", "");
			strReplace(s, "\t", "");
			strReplace(s, "\f", "");
			strReplace(s, "\v", "");
			strReplace(s, "\b", "");
			strings.push_back(s);

			temp_line++;
			temp_next++;
			temp_now = temp_next;
			continue;
		}

		temp_next++;
	}

	if (temp_now != temp_next) {
		char buf[512];
		memset(buf,0,512);
		int now_index = 0;
		for (int i = temp_now; i < temp_next;i++) {
			buf[now_index] = buffer[i];
			now_index++;
			if (now_index > 511) {
				break;
			}
		}
		buf[511] = '\0';
		string s = string(buf);
		strReplace(s, "\r", "");
		strReplace(s, "\t", "");
		strReplace(s, "\f", "");
		strReplace(s, "\v", "");
		strReplace(s, "\b", "");
		strings.push_back(s);

		temp_line++;
	}
	delete[] buffer;

	// sに分割できたので次はvertexを作る
	// text_count を計測する
	int text_counts[MYFONT_TEXTURE_COUNT];
	memset(text_counts,0, sizeof(text_counts));
	int ssize = strings.size();
	line_num = ssize;
	for (int i=0;i<ssize;i++) {
		string* s = &strings[i];
		WCHAR buf[512];
		memset(buf, 0, sizeof(buf));
		stringconverter sc;
		sc.charToWCHAR(s->c_str(),buf);
		int wlen = wcslen(buf);
		for (int k = 0 ; k < wlen;k++) {
			WCHAR w = buf[k];
			if (f->fonttexture_index.find((int)w) != f->fonttexture_index.end()) {
				int texture_index = f->fonttexture_index[(int)w];
				if (texture_index < MYFONT_TEXTURE_COUNT) {
					text_counts[texture_index]++;
				}
			}
		}
	}


	// text_counts * 6 分のvertexの領域を確保する

	for (int i = 0 ; i <MYFONT_TEXTURE_COUNT; i++) {
		if (text_counts[i]) {
			temp_vertexs[i] = new SimpleDebugTextVertex[text_counts[i]*6];
			memset(temp_vertexs[i], 0, sizeof(SimpleDebugTextVertex) * text_counts[i]*6);
		}
	}


	// forループからvertexの情報を入れ込む
	int temp_text_counts[MYFONT_TEXTURE_COUNT];
	int screen_width = g->getScreenWidth();
	int screen_height = g->getScreenHeight();
	memset(temp_text_counts, 0, sizeof(temp_text_counts));

	for (int i=0;i<ssize;i++) {
		string* s = &strings[i];
		WCHAR buf[512];
		memset(buf, 0, sizeof(buf));
		stringconverter sc;
		sc.charToWCHAR(s->c_str(),buf);
		int wlen = wcslen(buf);
		for (int k = 0 ; k < wlen;k++) {
			WCHAR w = buf[k];
			if (f->fonttexture_index.find((int)w) != f->fonttexture_index.end()) {
				int texture_index = f->fonttexture_index[(int)w];
				int font_code = f->fontindex[(int)w];
				if (texture_index < MYFONT_TEXTURE_COUNT && texture_index >= 0) {
					SimpleDebugTextVertex* v = &temp_vertexs[texture_index][temp_text_counts[texture_index]];

					float xcode_offset = MYFONT_FONTSIZE / (float)MYFONT_TEXTURE_WIDTH;
					float ycode_offset = MYFONT_FONTSIZE / (float)MYFONT_TEXTURE_HEIGHT;
					float xcode = (float)(font_code % MYFONT_TEXTURE_WIDTH) / (float)MYFONT_TEXTURE_WIDTH + xcode_offset;
					float ycode = (float)(font_code / MYFONT_TEXTURE_WIDTH) / (float)MYFONT_TEXTURE_HEIGHT;
	

					float xoffset = height/ (float)screen_width;
					float yoffset = height/ (float)screen_height;

					float x = 0 - wlen * xoffset/2 + xoffset * k;
					float y = 0 - i * yoffset - yoffset;



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



					temp_text_counts[texture_index] += 6;
				}

			}
		}
	}

	// vertex情報ができたので後はvertexbuffer を作る
	for (int i = 0 ; i < MYFONT_TEXTURE_COUNT; i++) {
		vertex_counts[i] = temp_text_counts[i];
		if (vertex_counts[i]) {
		D3D11_BUFFER_DESC bd;
		memset(&bd,0,sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleDebugTextVertex)*vertex_counts[i];
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		memset(&initData, 0, sizeof(initData));
		initData.pSysMem = temp_vertexs[i];

		HRESULT hr = g->getDevice()->CreateBuffer(&bd, &initData,&p_vertexbuffers[i]);
		if (FAILED(hr)) {
			for (int k = 0 ; k < MYFONT_TEXTURE_COUNT;k++) {
				if (temp_vertexs[k]) {
					delete[] temp_vertexs[k];
				}
			}
			Del();
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
		}
		}
	}
	
	for (int k = 0 ; k < MYFONT_TEXTURE_COUNT;k++) {
		if (temp_vertexs[k]) {
		delete[] temp_vertexs[k];
		}
	}


}
bool TelopTexts::isRenderFinished() {

	int num = (sum_plustime * cbuf1.text_speed - cbuf2.screen_height/2)/cbuf2.text_height;

	if (num > line_num) {
		return true;
	}
	return false;
}

void TelopTexts::plusTime(Graphics* g, float dt) {
	sum_plustime += dt;
	cbuf1.plustime = sum_plustime;
	
	g->getDeviceContext()->UpdateSubresource(cbuf1_buffer,0,NULL,&cbuf1,0,0);
}

void TelopTexts::render(Graphics* g) {

	unsigned int stride = sizeof(SimpleDebugTextVertex);
	unsigned int offset = 0;
	
	//g->getDeviceContext()->IASetInputLayout(p_vertexlayout);

	for (int i =0;i<MYFONT_TEXTURE_COUNT;i++) {
		if (this->vertex_counts[i]) {
			g->getDeviceContext()->VSSetConstantBuffers(0,1, &cbuf1_buffer);
			g->getDeviceContext()->VSSetConstantBuffers(1,1, &cbuf2_buffer);
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

	//g->getSwapChain()->Present(0,0);
}