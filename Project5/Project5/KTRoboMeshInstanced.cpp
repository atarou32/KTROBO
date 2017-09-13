#include "KTRoboMeshInstanced.h"

#include "KTRoboCS.h"
#include "KTRoboDebugText.h"
#include "memory.h"

using namespace KTROBO;
MeshInstanced::MeshInstanced(void)
{
	MyMatrixIdentity(world);
	MyMatrixIdentity(rootbone_matrix_local_kakeru);
	is_render= true;
	for (int i = 0 ; i < KTROBO_MESH_INSTANCED_COLOR_MAX; i++) {
		colors[i] = MYVECTOR4(0,0,0,0);
	}
	for (int i=0; i<KTROBO_MESH_INSTANCED_BONE_MAX; i++) {
		bones_anime_first_index[i]=0;
		bones_anime_last_index[i]=0;
		bones_anime_first_weight[i]=0;
	}
	mesh_index =0; // もとになるメッシュのインデックス
	skeleton_index = 0;// もとになるスケルトンのインデックス
	instance_index = 0; // このインスタンスのインデックス
//	instance_combined_matrix_place_index = 0; //このインスタンス の計算されたコンバインド行列のインデックス
	is_need_combined_matrix_load = true;
	is_need_color_texture_load = true;
	parent_instance = 0;
	parent_bone_index=0;
	connect_without_matrix_local = false;
}


MeshInstanced::~MeshInstanced(void)
{
}


MYSHADERSTRUCT MeshInstanceds::mss_for_matrix_local;
MYSHADERSTRUCT MeshInstanceds::mss_for_matrix_basis;
MYSHADERSTRUCT MeshInstanceds::mss_for_combined_matrix;
MYSHADERSTRUCT MeshInstanceds::mss_for_render;
ID3D11Buffer* MeshInstanceds::anime_matrix_load_vertexbuffer= NULL;
ID3D11Buffer* MeshInstanceds::matrix_local_load_vertexbuffer = NULL;
ID3D11Buffer* MeshInstanceds::combined_matrix_calc_vertexbuffer = NULL;
ID3D11Buffer* MeshInstanceds::combined_matrix_watasu_vertexbuffer = 0;
ID3D11UnorderedAccessView* MeshInstanceds::combined_matrix_calc_vertexbuffer_view = 0;
ID3D11UnorderedAccessView* MeshInstanceds::combined_matrix_watasu_vertexbuffer_view = 0;
ID3D11Buffer* MeshInstanceds::combined_matrix_watasu_indexbuffer=0;
COMBINEDMATRIXCALC_CBUF MeshInstanceds::cbuf1;
ID3D11Buffer* MeshInstanceds::cbuf1_buffer = 0;
ID3D11Buffer* MeshInstanceds::cbuf2_buffer = 0;
ID3D11Buffer* MeshInstanceds::cbuf3_buffer = 0;
ID3D11Buffer* MeshInstanceds::combined_matrix_watasu_offsetbuffer=0;
ID3D11Buffer* MeshInstanceds::color_texture_vertexbuffer=0;
MYSHADERSTRUCT MeshInstanceds::mss_for_color;
ID3D11SamplerState* MeshInstanceds::combined_matrix_sampler_state=0;
ID3D11Buffer* MeshInstanceds::render_instance_vertexbuffer=0;
MESHINSTANCED_CBUF2 MeshInstanceds::cbuf2;
MESHINSTANCED_CBUF3 MeshInstanceds::cbuf3;

void MeshInstanceds::loadShader(Graphics* g, MYSHADERSTRUCT* s, char* shader_filename, char* vs_func_name, char* gs_func_name,
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
		BlendDesc.AlphaToCoverageEnable = blend_enable;
		BlendDesc.IndependentBlendEnable = blend_enable;
		BlendDesc.RenderTarget[0].BlendEnable = blend_enable;
		BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;//SRC_ALPHA;
		BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;//INV_SRC_ALPHA;
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


void MeshInstanceds::_loadColorToTexture(Graphics* g, SHADERCOLORTEXTUREINPUTSTRUCT* stt, int temp) {



	D3D11_MAPPED_SUBRESOURCE subresource;
	unsigned int stride = sizeof(SHADERCOLORTEXTUREINPUTSTRUCT);
	unsigned int offset = 0;

	CS::instance()->enter(CS_DEVICECON_CS, "load color texture");
	g->getDeviceContext()->Map(MeshInstanceds::color_texture_vertexbuffer,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	memcpy(subresource.pData, stt, sizeof(SHADERCOLORTEXTUREINPUTSTRUCT)*temp);//KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE);
	g->getDeviceContext()->Unmap(MeshInstanceds::color_texture_vertexbuffer,0);
	D3D11_VIEWPORT vp;

	vp.Height = color_texture->height;
	vp.Width = color_texture->width;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	g->getDeviceContext()->RSSetViewports(1, &vp);
	float ccc[] = {
		0.2f,0.2f,0.8f,0.7f};


	g->getDeviceContext()->OMSetRenderTargets(1, &color_texture->target_view, this->mss_for_color.depthstencilview);
	g->getDeviceContext()->ClearDepthStencilView(mss_for_color.depthstencilview,  D3D11_CLEAR_DEPTH/* | D3D11_CLEAR_STENCIL*/,1.0f, 0 );
//	g->getDeviceContext()->ClearRenderTargetView(anime_matrix_basis_texture->target_view, ccc);

	g->getDeviceContext()->IASetInputLayout(MeshInstanceds::mss_for_color.vertexlayout );
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, &color_texture_vertexbuffer, &stride, &offset );
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	
	g->getDeviceContext()->RSSetState(mss_for_color.rasterstate);

	float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

	g->getDeviceContext()->OMSetBlendState(mss_for_color.blendstate, blendFactor,0xFFFFFFFF);
	g->getDeviceContext()->VSSetShader(mss_for_color.vs, NULL, 0);
	g->getDeviceContext()->GSSetShader(NULL,NULL,0);
		
	g->getDeviceContext()->PSSetShader(mss_for_color.ps, NULL, 0);
//	g->getDeviceContext()->PSSetShaderResources(0,1,&matrix_local_texture->view);
	g->getDeviceContext()->Draw(temp,0);
	g->getDeviceContext()->Flush();
	
	g->getDeviceContext()->RSSetViewports(1, g->getViewPort());
	ID3D11RenderTargetView* t = g->getRenderTargetView();
	g->getDeviceContext()->OMSetRenderTargets(1, &t, NULL);	
//	ID3D11ShaderResourceView* testtt = NULL;
//	g->getDeviceContext()->PSSetShaderResources(0,1,&testtt);

	
	CS::instance()->leave(CS_DEVICECON_CS, "load color texture leave");
	





}


void MeshInstanceds::loadColorToTexture(Graphics* g) {
	// まずはじめにインスタンス毎に回す
	SHADERCOLORTEXTUREINPUTSTRUCT stt[KTROBO_MESH_INSTANCED_COLOR_TEXTURE_STRUCT_TEMPSIZE];
	memset(stt, 0, sizeof(SHADERCOLORTEXTUREINPUTSTRUCT) * KTROBO_MESH_INSTANCED_COLOR_TEXTURE_STRUCT_TEMPSIZE);
	int temp = 0;
	int temp_max = KTROBO_MESH_INSTANCED_COLOR_TEXTURE_STRUCT_TEMPSIZE;

	int isize = mesh_instanceds.size();
	for (int i=0;i<isize;i++) {
		MeshInstanced* mm = mesh_instanceds[i];
		if (mm->getIsNeedColorTextureLoad()) {
			for (int c=0; c< KTROBO_MESH_INSTANCED_COLOR_MAX;c++) {
				for (int k=0;k<6;k++) {
				stt[temp].color_index = c;
//				stt[temp].color = *mm->getColor(c);
				mm->getColor(c, &stt[temp].color);
				stt[temp].instance_id = i;
				stt[temp].offset = 0;
				stt[temp].vertex_index = k;
				temp++;
				if (temp >= temp_max) {
					_loadColorToTexture(g, stt, temp);
					temp = 0;
					memset(stt, 0, sizeof(SHADERCOLORTEXTUREINPUTSTRUCT) * KTROBO_MESH_INSTANCED_COLOR_TEXTURE_STRUCT_TEMPSIZE);
				}
				}
			}
		}
		mm->setIsNeedColorTextureLoad(false);
	}

	if (temp >0) {
		_loadColorToTexture(g, stt, temp);
		temp = 0;
		memset(stt, 0, sizeof(SHADERCOLORTEXTUREINPUTSTRUCT) * KTROBO_MESH_INSTANCED_COLOR_TEXTURE_STRUCT_TEMPSIZE);
	}
}


void MeshInstanceds::changeInstanceMeshSkeleton(int instance_id, Mesh* mesh, Mesh* new_skeleton) {

	if (instance_id >=0 && instance_id < mesh_instanceds.size()) {

		MeshInstanced *mm = mesh_instanceds[instance_id];
		mm->setMeshIndex(this->getMeshIndexOrSet(mesh));
		mm->setSkeletonIndex(this->getSkeletonIndexOrSet(new_skeleton));
		MYVECTOR4 colors[KTROBO_MESH_INSTANCED_COLOR_MAX];
		memset(colors, 0, sizeof(colors));
		
		int msize = mesh->Materials.size();
		for (int h = 0; h < msize && h < KTROBO_MESH_INSTANCED_COLOR_MAX; h++) {
			colors[h] = mesh->Materials[h]->color;
		}
		mm->setColor(colors);

		mm->setIsNeedColorTextureLoad(true);
		mm->setIsNeedCombinedMatrixLoad(true);
		is_need_calc_max_depth = true;
	} else {
		throw new GameError(KTROBO::WARNING, "out side vector of change skeleton");
	}
}


void MeshInstanceds::loadShaderForColor(Graphics* g) {
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"INDEXSDAYO", 0, DXGI_FORMAT_R32G32B32A32_UINT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"MCOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	
	loadShader(g, &mss_for_color, KTROBO_MESH_INSTANCED_SHADER_FILENAME_COLOR, 
		KTROBO_MESH_INSTANCED_SHADER_VS, KTROBO_MESH_INSTANCED_SHADER_GS, KTROBO_MESH_INSTANCED_SHADER_PS,
		KTROBO_MESH_INSTANCED_MATRIX_COMBINED_TEXTURE_WIDTH_HEIGHT, 
		KTROBO_MESH_INSTANCED_MATRIX_COMBINED_TEXTURE_WIDTH_HEIGHT, layout,2, false);
	

	D3D11_BUFFER_DESC bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(SHADERCOLORTEXTUREINPUTSTRUCT)*KTROBO_MESH_INSTANCED_COLOR_TEXTURE_STRUCT_TEMPSIZE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	
	HRESULT hr = g->getDevice()->CreateBuffer(&bd, NULL ,&(MeshInstanceds::color_texture_vertexbuffer));
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}
}


void MeshInstanceds::Init(Graphics* g) {
	loadShaderForCombinedMatrix(g);
	loadShaderForMatrixBasis(g);
	loadShaderForMatrixLocal(g);
	loadShaderForRender(g);
	loadShaderForColor(g);
	D3D11_BUFFER_DESC bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(ANIMETEXTURELOADSTRUCT) * KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//ANIMETEXTURELOADSTRUCT* ss = new ANIMETEXTURELOADSTRUCT[KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE];
	//D3D11_SUBRESOURCE_DATA initData;
	//memset(&initData, 0, sizeof(initData));
	//initData.pSysMem = ss;
	
	HRESULT hr = g->getDevice()->CreateBuffer(&bd, NULL ,&(MeshInstanceds::anime_matrix_load_vertexbuffer));
	if (FAILED(hr)) {
	//	delete[] ss;
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}
	//delete[] ss;

	memset(&cbuf2,0,sizeof(MESHINSTANCED_CBUF2));
	memset(&cbuf3,0, sizeof(MESHINSTANCED_CBUF3));





}
void MeshInstanceds::loadShaderForMatrixLocal(Graphics* g) {
	D3D11_INPUT_ELEMENT_DESC layout[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"MVALUE", 0, DXGI_FORMAT_R32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
		};
	loadShader(g, &mss_for_matrix_local, KTROBO_MESH_INSTANCED_SHADER_FILENAME_MATRIX_LOCAL,
		KTROBO_MESH_INSTANCED_SHADER_VS, KTROBO_MESH_INSTANCED_SHADER_GS, KTROBO_MESH_INSTANCED_SHADER_PS,
		KTROBO_MESH_INSTANCED_MATRIX_LOCAL_TEXTURE_WIDTH_HEIGHT, 
		KTROBO_MESH_INSTANCED_MATRIX_LOCAL_TEXTURE_WIDTH_HEIGHT, layout,2,false);

}





void MeshInstanceds::loadShaderForMatrixBasis(Graphics* g) {
	D3D11_INPUT_ELEMENT_DESC layout[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		//	{"MATRIXOFFSET", 0, DXGI_FORMAT_R8G8_UNORM,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
		//	{"INDEXS",0, DXGI_FORMAT_R16G16_UNORM, 0, 14, D3D11_INPUT_PER_VERTEX_DATA,0},
		//	{"INDEXST", 0, DXGI_FORMAT_R16_UNORM, 0, 18, D3D11_INPUT_PER_VERTEX_DATA,0 },
			{"MVALUE", 0, DXGI_FORMAT_R32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA,0}
		};
	loadShader(g, &mss_for_matrix_basis, KTROBO_MESH_INSTANCED_SHADER_FILENAME_MATRIX_BASIS, 
		KTROBO_MESH_INSTANCED_SHADER_VS, KTROBO_MESH_INSTANCED_SHADER_GS, KTROBO_MESH_INSTANCED_SHADER_PS,
		KTROBO_MESH_INSTANCED_MATRIX_BASIS_TEXTURE_WIDTH_HEIGHT, 
		KTROBO_MESH_INSTANCED_MATRIX_BASIS_TEXTURE_WIDTH_HEIGHT, layout,2, false);

	

}

void MeshInstanceds::loadShaderForCombinedMatrix(Graphics* g) {
	/*unsigned short instance_index;
	unsigned short skeleton_index;
	
	unsigned short parent_skeleton_index;
	unsigned short parent_instance_index;

	unsigned char bone_index;
	unsigned char parent_bone_index;
	unsigned short control_value;
	unsigned short anime_first_index;
	unsigned short anime_last_index;
	float anime_first_weight;
	MYMATRIX matrix_kakeru;
	*/
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"INDEXSDAYO", 0, DXGI_FORMAT_R32G32B32A32_UINT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0} ,
	//	{"BONE_INDEX", 0, DXGI_FORMAT_R32_UINT,0,4,D3D11_INPUT_PER_VERTEX_DATA,0} ,
		{"KAKERUM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"KAKERUM", 1, DXGI_FORMAT_R32G32B32A32_FLOAT,0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"KAKERUM", 2, DXGI_FORMAT_R32G32B32A32_FLOAT,0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"KAKERUM", 3, DXGI_FORMAT_R32G32B32A32_FLOAT,0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"OFFSETDAYO",0, DXGI_FORMAT_R32G32B32A32_UINT, 1,  0 , D3D11_INPUT_PER_INSTANCE_DATA, 1},
		};
	
	loadShader(g, &mss_for_combined_matrix, KTROBO_MESH_INSTANCED_SHADER_FILENAME_COMBINED_MATRIX, 
		KTROBO_MESH_INSTANCED_SHADER_VS, KTROBO_MESH_INSTANCED_SHADER_GS, KTROBO_MESH_INSTANCED_SHADER_PS,
		KTROBO_MESH_INSTANCED_MATRIX_COMBINED_TEXTURE_WIDTH_HEIGHT, 
		KTROBO_MESH_INSTANCED_MATRIX_COMBINED_TEXTURE_WIDTH_HEIGHT, layout,6, false);
	

	D3D11_BUFFER_DESC bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(COMBINEDMATRIXCALCSTRUCT) *  KTROBO_MESH_INSTANCED_COMBINED_MATRIX_CALC_STRUCT_TEMPSIZE;
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	bd.CPUAccessFlags = 0;//D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	
	//ANIMETEXTURELOADSTRUCT* ss = new ANIMETEXTURELOADSTRUCT[KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE];
	//D3D11_SUBRESOURCE_DATA initData;
	//memset(&initData, 0, sizeof(initData));
	//initData.pSysMem = ss;
	
	HRESULT hr = g->getDevice()->CreateBuffer(&bd, NULL ,&(MeshInstanceds::combined_matrix_calc_vertexbuffer));
	if (FAILED(hr)) {
	//	delete[] ss;
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory( &uavDesc, sizeof(uavDesc) );
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = sizeof(COMBINEDMATRIXCALCSTRUCT) *KTROBO_MESH_INSTANCED_COMBINED_MATRIX_CALC_STRUCT_TEMPSIZE/4;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;

	hr = g->getDevice()->CreateUnorderedAccessView(combined_matrix_calc_vertexbuffer, &uavDesc, &combined_matrix_calc_vertexbuffer_view);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "uav make error");;
	}


	bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(COMBINEDMATRIXWATASUSTRUCT) * KTROBO_MESH_INSTANCED_COMBINED_MATRIX_WATASU_STRUCT_TEMPSIZE;//KTROBO_MESH_INSTANCED_COMBINED_MATRIX_CALC_STRUCT_TEMPSIZE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	//ANIMETEXTURELOADSTRUCT* ss = new ANIMETEXTURELOADSTRUCT[KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE];
	//D3D11_SUBRESOURCE_DATA initData;
	//memset(&initData, 0, sizeof(initData));
	//initData.pSysMem = ss;
	
	hr = g->getDevice()->CreateBuffer(&bd, NULL ,&(MeshInstanceds::combined_matrix_watasu_vertexbuffer));
	if (FAILED(hr)) {
	//	delete[] ss;
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}
	uavDesc.Buffer.NumElements = sizeof(COMBINEDMATRIXWATASUSTRUCT)* KTROBO_MESH_INSTANCED_COMBINED_MATRIX_WATASU_STRUCT_TEMPSIZE/4;
	hr = g->getDevice()->CreateUnorderedAccessView(combined_matrix_watasu_vertexbuffer, &uavDesc, &combined_matrix_watasu_vertexbuffer_view);
//	pDev->CreateUnorderedAccessView( m_pVertexBuffer, &uavDesc, &m_pVertexView );
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "uav make error");
	}

	
	ID3DBlob* pblob = 0;
	try {
		CompileShaderFromFile(KTROBO_MESH_INSTANCED_SHADER_FILENAME_COMBINED_MATRIX_COMPUTE, "CalcCS", "cs_5_0",&pblob,true);
		hr = g->getDevice()->CreateComputeShader(pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			NULL,
			&mss_for_combined_matrix.cs);
		if (FAILED(hr)) {
			pblob->Release();
			delete pblob;
			pblob = 0;
			mss_for_combined_matrix.cs = 0;
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cs make error");;
		}
		if (pblob) {
			pblob->Release();
			delete pblob;
			pblob = 0;
		}

	//delete[] ss;
	}catch (GameError* err) {
		if (pblob) {
			pblob->Release();
			pblob = 0;
			
		}
		Del();
		throw err;
	}



	D3D11_BUFFER_DESC des;
	des.ByteWidth = sizeof(COMBINEDMATRIXCALC_CBUF);
	des.Usage = D3D11_USAGE_DYNAMIC;
	des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	des.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	des.MiscFlags = 0;
	des.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA idat;
	memset(&cbuf1, 0, sizeof(COMBINEDMATRIXCALC_CBUF));

	idat.pSysMem = &cbuf1;
	idat.SysMemPitch = 0;
	idat.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer(&des, &idat, &cbuf1_buffer);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
	}





	D3D11_BUFFER_DESC hBufferDesc;
    hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.ByteWidth = sizeof( UINT) * KTROBO_MESH_INSTANCED_COMBINED_MATRIX_WATASU_STRUCT_TEMPSIZE* 3;
    hBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    hBufferDesc.CPUAccessFlags = 0;
    hBufferDesc.MiscFlags = 0;
    hBufferDesc.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA hSubResourceData;

	UINT indexs[KTROBO_MESH_INSTANCED_COMBINED_MATRIX_WATASU_STRUCT_TEMPSIZE*3];
	for (int i=0;i<KTROBO_MESH_INSTANCED_COMBINED_MATRIX_WATASU_STRUCT_TEMPSIZE;i++) {
		indexs[3*i] = i;
		indexs[3*i+1] = i;
		indexs[3*i+2] = i;
	}
    hSubResourceData.pSysMem = indexs;
    hSubResourceData.SysMemPitch = 0;
    hSubResourceData.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer( &hBufferDesc, &hSubResourceData, &combined_matrix_watasu_indexbuffer );
    if( FAILED( hr ) ) {
       Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "index buffer make error");
	}




	bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(COMBINEDMATRIXCALC_CBUF)*16*2;//KTROBO_MESH_INSTANCED_COMBINED_MATRIX_CALC_STRUCT_TEMPSIZE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;//D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	COMBINEDMATRIXCALC_CBUF* ss = new COMBINEDMATRIXCALC_CBUF[16*2];

	for (int i=0;i<2;i++) {
		for (int k=0;k<16;k++) {
			
			
			ss[i*16+k].depth = i*16+k;
			ss[i*16+k].vertex_num = i*16+k;//1;
			ss[i*16+k].omoteura =(unsigned int)i;
			ss[i*16+k].matrix_offset = k;

		}
	}

	D3D11_SUBRESOURCE_DATA initData;
	memset(&initData, 0, sizeof(initData));
	initData.pSysMem = ss;
	
	hr = g->getDevice()->CreateBuffer(&bd, &initData ,&(MeshInstanceds::combined_matrix_watasu_offsetbuffer));
	if (FAILED(hr)) {
		delete[] ss;
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}
	delete[] ss;


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
	hr = g->getDevice()->CreateSamplerState(&descS, &combined_matrix_sampler_state);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "sampler make error");;
	}

}

void MeshInstanceds::loadShaderForRender(Graphics* g) {
	D3D11_INPUT_ELEMENT_DESC layout[] = {

			{ "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "INDone",  0, DXGI_FORMAT_R8_UINT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "INDtwo",  0, DXGI_FORMAT_R8_UINT,  0, 13, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "INDthree",  0, DXGI_FORMAT_R8_UINT,  0, 14, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "INDfour",  0, DXGI_FORMAT_R8_UINT,  0, 15, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "INDfive", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTfive", 0, DXGI_FORMAT_R32_FLOAT, 0 , 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, 52, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			{"INSTANCEIDDAYO", 0, DXGI_FORMAT_R32_UINT,1,0,D3D11_INPUT_PER_INSTANCE_DATA,1},
			{"WORLDMAT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,1,4,D3D11_INPUT_PER_INSTANCE_DATA,1},
			{"WORLDMAT", 1, DXGI_FORMAT_R32G32B32A32_FLOAT,1,20,D3D11_INPUT_PER_INSTANCE_DATA,1},
			{"WORLDMAT", 2, DXGI_FORMAT_R32G32B32A32_FLOAT,1,36,D3D11_INPUT_PER_INSTANCE_DATA,1},
			{"WORLDMAT", 3, DXGI_FORMAT_R32G32B32A32_FLOAT,1,52,D3D11_INPUT_PER_INSTANCE_DATA,1},
		};
	loadShader(g, &mss_for_render, KTROBO_MESH_INSTANCED_SHADER_FILENAME_RENDER, 
		KTROBO_MESH_INSTANCED_SHADER_VS, KTROBO_MESH_INSTANCED_SHADER_GS, KTROBO_MESH_INSTANCED_SHADER_PS,
		g->getScreenWidth(), g->getScreenHeight(), layout,15, true);
	
	D3D11_BUFFER_DESC bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(RENDERINSTANCEINFOSTRUCT)*KTROBO_MESH_INSTANCED_RENDER_INSTANCE_STRUCT_TEMPSIZE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	
	HRESULT hr = g->getDevice()->CreateBuffer(&bd, NULL ,&(MeshInstanceds::render_instance_vertexbuffer));
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}

	D3D11_BUFFER_DESC des;
	des.ByteWidth = sizeof(MESHINSTANCED_CBUF2);
	des.Usage = D3D11_USAGE_DYNAMIC;
	des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	des.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	des.MiscFlags = 0;
	des.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA idat;
	memset(&cbuf2, 0, sizeof(MESHINSTANCED_CBUF2));

	idat.pSysMem = &cbuf2;
	idat.SysMemPitch = 0;
	idat.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer(&des, &idat, &cbuf2_buffer);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
	}

	des;
	des.ByteWidth = sizeof(MESHINSTANCED_CBUF3);
	des.Usage = D3D11_USAGE_DYNAMIC;
	des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	des.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	des.MiscFlags = 0;
	des.StructureByteStride = 0;

	idat;
	memset(&cbuf3, 0, sizeof(MESHINSTANCED_CBUF3));

	idat.pSysMem = &cbuf3;
	idat.SysMemPitch = 0;
	idat.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer(&des, &idat, &cbuf3_buffer);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
	}
}

void MeshInstanceds::_loadAnimeMatrixBasisToTexture(Graphics* g, ANIMETEXTURELOADSTRUCT* st, int temp) {
	D3D11_MAPPED_SUBRESOURCE subresource;
	unsigned int stride = sizeof(ANIMETEXTURELOADSTRUCT);
	unsigned int offset = 0;

	CS::instance()->enter(CS_DEVICECON_CS, "load anime matrix");
	//ANIMETEXTURELOADSTRUCT* stt = new ANIMETEXTURELOADSTRUCT[6];
	/*st[0].pos.float3.x = 0;
	st[0].pos.float3.y = 0;
	st[0].pos.float3.z = 0;
	st[0].matrix_value = 1.0f;

	st[1].pos.float3.x = 0;
	st[1].pos.float3.y = 0.3f;
	st[1].pos.float3.z = 0;
	st[1].matrix_value = 1.0f;

	st[2].pos.float3.x = 0.3f;
	st[2].pos.float3.y = 0;
	st[2].pos.float3.z = 0;
	st[2].matrix_value = 1.0f;

	st[3].pos.float3.x = 0.3;
	st[3].pos.float3.y = 0;
	st[3].pos.float3.z = 0;
	st[3].matrix_value = 1.0f;

	st[4].pos.float3.x = 0;
	st[4].pos.float3.y = 0.3;
	st[4].pos.float3.z = 0;
	st[4].matrix_value = 1.0f;
	
	st[5].pos.float3.x = 0.3f;
	st[5].pos.float3.y = 0.3f;
	st[5].pos.float3.z = 0;
	st[5].matrix_value = 1.0f;*/

	g->getDeviceContext()->Map(MeshInstanceds::anime_matrix_load_vertexbuffer,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	memcpy(subresource.pData, st, sizeof(ANIMETEXTURELOADSTRUCT) * temp);//KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE);
	g->getDeviceContext()->Unmap(MeshInstanceds::anime_matrix_load_vertexbuffer,0);
	D3D11_VIEWPORT vp;

	vp.Height = anime_matrix_basis_texture->height;
	vp.Width = anime_matrix_basis_texture->width;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	g->getDeviceContext()->RSSetViewports(1, &vp);
	float ccc[] = {
		0.2f,0.2f,0.8f,0.7f};

	ID3D11ShaderResourceView* te = NULL;
	g->getDeviceContext()->VSSetShaderResources(0,1, &te);
	g->getDeviceContext()->VSSetShaderResources(1,1, &te);
	g->getDeviceContext()->OMSetRenderTargets(1, &anime_matrix_basis_texture->target_view, this->mss_for_matrix_basis.depthstencilview);
	g->getDeviceContext()->ClearDepthStencilView(mss_for_matrix_basis.depthstencilview,  D3D11_CLEAR_DEPTH/* | D3D11_CLEAR_STENCIL*/,1.0f, 0 );
//	g->getDeviceContext()->ClearRenderTargetView(anime_matrix_basis_texture->target_view, ccc);

	g->getDeviceContext()->IASetInputLayout(MeshInstanceds::mss_for_matrix_basis.vertexlayout );
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, &anime_matrix_load_vertexbuffer, &stride, &offset );
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	
	g->getDeviceContext()->RSSetState(mss_for_matrix_basis.rasterstate);

	float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

	g->getDeviceContext()->OMSetBlendState(mss_for_matrix_basis.blendstate, blendFactor,0xFFFFFFFF);
	g->getDeviceContext()->VSSetShader(mss_for_matrix_basis.vs, NULL, 0);
//	g->getDeviceContext()->GSSetShader(mss_for_matrix_basis.gs,NULL,0);
		
	g->getDeviceContext()->PSSetShader(mss_for_matrix_basis.ps, NULL, 0);
			
	g->getDeviceContext()->Draw(temp,0);
	g->getDeviceContext()->Flush();
	

	g->getDeviceContext()->RSSetViewports(1, g->getViewPort());
	ID3D11RenderTargetView* t = g->getRenderTargetView();
	g->getDeviceContext()->OMSetRenderTargets(1, &t, NULL);	
	
	
	CS::instance()->leave(CS_DEVICECON_CS, "load anime matrix leave");
				
}

void MeshInstanceds::_loadMatrixLocalToTexture(Graphics* g, ANIMETEXTURELOADSTRUCT* st, int temp) {
	
	D3D11_MAPPED_SUBRESOURCE subresource;
	unsigned int stride = sizeof(ANIMETEXTURELOADSTRUCT);
	unsigned int offset = 0;

	CS::instance()->enter(CS_DEVICECON_CS, "load local matrix");
	g->getDeviceContext()->Map(MeshInstanceds::anime_matrix_load_vertexbuffer,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	memcpy(subresource.pData, st, sizeof(ANIMETEXTURELOADSTRUCT)*temp);//KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE);
	g->getDeviceContext()->Unmap(MeshInstanceds::anime_matrix_load_vertexbuffer,0);
	D3D11_VIEWPORT vp;

	vp.Height = matrix_local_texture->height;
	vp.Width = matrix_local_texture->width;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	g->getDeviceContext()->RSSetViewports(1, &vp);
	float ccc[] = {
		0.2f,0.2f,0.8f,0.7f};

	ID3D11ShaderResourceView* te = NULL;
	g->getDeviceContext()->VSSetShaderResources(0,1, &te);
	g->getDeviceContext()->VSSetShaderResources(1,1, &te);
	g->getDeviceContext()->OMSetRenderTargets(1, &matrix_local_texture->target_view, this->mss_for_matrix_local.depthstencilview);
	g->getDeviceContext()->ClearDepthStencilView(mss_for_matrix_local.depthstencilview,  D3D11_CLEAR_DEPTH/* | D3D11_CLEAR_STENCIL*/,1.0f, 0 );
//	g->getDeviceContext()->ClearRenderTargetView(anime_matrix_basis_texture->target_view, ccc);

	g->getDeviceContext()->IASetInputLayout(MeshInstanceds::mss_for_matrix_local.vertexlayout );
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, &anime_matrix_load_vertexbuffer, &stride, &offset );
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	
	g->getDeviceContext()->RSSetState(mss_for_matrix_local.rasterstate);

	float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

	g->getDeviceContext()->OMSetBlendState(mss_for_matrix_local.blendstate, blendFactor,0xFFFFFFFF);
	g->getDeviceContext()->VSSetShader(mss_for_matrix_local.vs, NULL, 0);
	g->getDeviceContext()->GSSetShader(NULL,NULL,0);
		
	g->getDeviceContext()->PSSetShader(mss_for_matrix_local.ps, NULL, 0);
//	g->getDeviceContext()->PSSetShaderResources(0,1,&matrix_local_texture->view);
	g->getDeviceContext()->Draw(temp,0);
	g->getDeviceContext()->Flush();
	
	g->getDeviceContext()->RSSetViewports(1, g->getViewPort());
	ID3D11RenderTargetView* t = g->getRenderTargetView();
	g->getDeviceContext()->OMSetRenderTargets(1, &t, NULL);	
//	ID3D11ShaderResourceView* testtt = NULL;
//	g->getDeviceContext()->PSSetShaderResources(0,1,&testtt);

	
	CS::instance()->leave(CS_DEVICECON_CS, "load local matrix leave");
	





}


void MeshInstanceds::loadMatrixLocalToTexture(Graphics* g) {

	// matrixlocaltexture に matrix_local の情報を書き込む
		
	int size = skeletons.size();
	int te=0;
	for (te = 0; te < size; te++) {
		if(!this->skeleton_matrix_local_is_loaded[te]) {
			break;
		}
	}
	if (te == size) return;
	
	
	ANIMETEXTURELOADSTRUCT st[KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE];
	memset(st, 0, sizeof(ANIMETEXTURELOADSTRUCT) * KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE);

	
	for (int i = 0 ;i < size ; i++) {
		bool is_loaded = this->skeleton_matrix_local_is_loaded[i];
		Mesh* m = skeletons[i];
		if (is_loaded) continue;
		int temp = 0;
		int temp_max = KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE;

		// 何回かに分ける
		int bsize = m->Bones.size();
		for (int b = 0;b < bsize; b++) {
			for (int offset = 0; offset < 16 ; offset++) {
				for (int p = 0; p < 6; p++) {
					//	st[temp].bone_index = b;
					//	st[temp].skeleton_index = i;
					//	st[temp].matrix_offset = offset;
					//	if (offset < 16) {
					st[temp].matrix_value = m->Bones[b]->matrix_local.m[offset%4][offset/4];
					//	}
					int texel_per_bone = 16;
					int texel_per_skeleton = texel_per_bone * KTROBO_MESH_INSTANCED_BONE_MAX;
					int place = texel_per_skeleton * i + texel_per_bone * b + offset;
					int x = place % KTROBO_MESH_INSTANCED_MATRIX_LOCAL_TEXTURE_WIDTH_HEIGHT;
					int y = place / KTROBO_MESH_INSTANCED_MATRIX_LOCAL_TEXTURE_WIDTH_HEIGHT;
					float xoffset = 2 / (float)KTROBO_MESH_INSTANCED_MATRIX_LOCAL_TEXTURE_WIDTH_HEIGHT;
					float yoffset = 2 / (float)KTROBO_MESH_INSTANCED_MATRIX_LOCAL_TEXTURE_WIDTH_HEIGHT;
					int sample[] = {
						x,y,0,
						x,y+1,0,
						x+1, y, 0,
						x+1, y, 0,
						x, y+1, 0,
						x+1, y+1, 0
					};
					float xc =  -1+ sample[3*p] * xoffset;
					float yc = 1-  sample[3*p+1] * yoffset;
					st[temp].pos.float3.x = xc;
					st[temp].pos.float3.y = yc;
					st[temp].pos.float3.z = 0.0f;
					temp++;
					if (temp >= temp_max) {

						_loadMatrixLocalToTexture(g, st, temp);
						temp = 0;
						memset(st, 0, sizeof(ANIMETEXTURELOADSTRUCT) * KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE);
						
					}
				}
			
			}
		}

		if (temp >0) {
				
			_loadMatrixLocalToTexture(g, st, temp);
			temp = 0;
			memset(st, 0, sizeof(ANIMETEXTURELOADSTRUCT) * KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE);
					
		}

		temp = 0;
		memset(st, 0, sizeof(ANIMETEXTURELOADSTRUCT) * KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE);
		skeleton_matrix_local_is_loaded[i] = true;

	}
	//delete[] st;
}


void MeshInstanceds::loadAnimeMatrixBasisToTexture(Graphics* g) {
	
	// animetexture にmatrix_basisの情報を書き込む


	int size = skeletons.size();
	int te=0;
	for (int te=0; te < size; te++) {
		if (!skeleton_anime_is_loaded[te]) break;
	}
	if (te == size) {
		return;
	}

	ANIMETEXTURELOADSTRUCT st[KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE];
	memset(st, 0, sizeof(ANIMETEXTURELOADSTRUCT) * KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE);

	for (int i = 0 ; i < size; i++) {

		bool t = this->skeleton_anime_is_loaded[i];
		Mesh* m = skeletons[i];
		if (t) continue;
		int temp = 0;
		int temp_max = KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE;
		
		// まだアニメがロードされていないのでアニメを計算する 何回かに分ける
		for (int frame=0; frame < KTROBO_MESH_INSTANCED_ANIME_MATRIX_BASIS_NUM_MAX; frame++) {
			m->calculateOffsetMatrix(m->RootBone, (float)frame);
			int bsize = m->Bones.size();
			for (int b = 0;b < bsize; b++) {
				for (int offset = 0; offset < 16 ; offset++) {
					for (int p = 0; p < 6; p++) {
					//	st[temp].bone_index = b;
					//	st[temp].skeleton_index = i;
					//	st[temp].matrix_offset = offset;
					
						st[temp].matrix_value = m->Bones[b]->offset_matrix.m[offset%4][offset/4];
					//	}
						int texel_per_bone = KTROBO_MESH_INSTANCED_ANIME_MATRIX_BASIS_NUM_MAX * 16;
						int texel_per_skeleton = texel_per_bone * KTROBO_MESH_INSTANCED_BONE_MAX;
						int place = texel_per_skeleton * i + texel_per_bone * b + frame * 16 + offset;
						int x = place % KTROBO_MESH_INSTANCED_MATRIX_BASIS_TEXTURE_WIDTH_HEIGHT;
						int y = place / KTROBO_MESH_INSTANCED_MATRIX_BASIS_TEXTURE_WIDTH_HEIGHT;
						float xoffset = 2 / (float)KTROBO_MESH_INSTANCED_MATRIX_BASIS_TEXTURE_WIDTH_HEIGHT;
						float yoffset = 2 / (float)KTROBO_MESH_INSTANCED_MATRIX_BASIS_TEXTURE_WIDTH_HEIGHT;
						int sample[] = {
							x,y,0,
							x,y+1,0,
							x+1, y, 0,
							x+1, y, 0,
							x, y+1, 0,
							x+1, y+1, 0
						};
						float xc =  -1+ sample[3*p] * xoffset;
						float yc = 1-  sample[3*p+1] * yoffset;
						st[temp].pos.float3.x = xc;
						st[temp].pos.float3.y = yc;
						st[temp].pos.float3.z = 0.0f;

						temp++;
						if (temp >= temp_max) {

							_loadAnimeMatrixBasisToTexture(g, st, temp);
							temp = 0;
							memset(st, 0, sizeof(ANIMETEXTURELOADSTRUCT) * KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE);
						}
					}
				}
			
			}
		}

		if (temp >0) {
				
			_loadAnimeMatrixBasisToTexture(g, st, temp);
			temp = 0;
			memset(st, 0, sizeof(ANIMETEXTURELOADSTRUCT) * KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE);
					
		}

		temp = 0;
		memset(st, 0, sizeof(ANIMETEXTURELOADSTRUCT) * KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE);
		skeleton_anime_is_loaded[i] = true;
		
		
	}

	//delete[] st;
}


void MeshInstanceds::setSkeleton(Mesh* m) {

	// まずあるかどうか探す
	vector<Mesh*>::iterator it = skeletons.begin();
	while (it != skeletons.end()) {
		Mesh* ms = *it;
		if (ms ==m) return;

		it = it + 1;
	}

	// ないので登録する
	skeletons.push_back(m);
	skeleton_anime_is_loaded.push_back(false);
	skeleton_matrix_local_is_loaded.push_back(false);

}

void MeshInstanceds::calcCombinedMatrixToTexture(Graphics* g) {


	

	// まず
	COMBINEDMATRIXCALCSTRUCT stt[ KTROBO_MESH_INSTANCED_COMBINED_MATRIX_CALC_STRUCT_TEMPSIZE];
	memset(stt, 0 , sizeof(COMBINEDMATRIXCALCSTRUCT)*KTROBO_MESH_INSTANCED_COMBINED_MATRIX_CALC_STRUCT_TEMPSIZE);

	// max_depth を計算する

	int isize = mesh_instanced_size;
	
	if (is_need_calc_max_depth) {
		
		for (int i=0;i<KTROBO_MESH_INSTANCED_COMBINED_MATRIX_INSTANCE_SIZE;i++) {
			for (int j=0;j<KTROBO_MAP_OF_DEPTH_DEPTH_MAX;j++) {
				for (int k=0;k<KTROBO_MESH_BONE_MAX;k++) {
				map_of_depth[j][i][k] = KTROBO_MAP_OF_DEPTH_NULL;
				}
			}
		}
		for (int i = 0 ; i < isize; i++) {
			MeshInstanced* instance = mesh_instanceds[i];
			for (int bone_i=0; bone_i < KTROBO_MESH_INSTANCED_BONE_MAX; bone_i ++) {
				int temp_depth = this->getDepth(instance, bone_i);
				if (temp_depth == KTROBO_MESH_BONE_NULL) continue;
				//map_of_depth.insert(pair<pair<int,int>,int>(pair<int,int>(i,bone_i),temp_depth));
				map_of_depth[temp_depth][i][bone_i] = 1;
				if (temp_depth != KTROBO_MESH_INSTANCED_BONE_DEPTH_NULL) {
					if (temp_depth > max_depth) {
						max_depth = temp_depth;
						if (max_depth >= KTROBO_MAP_OF_DEPTH_DEPTH_MAX) {
							throw new GameError(KTROBO::WARNING, "errorof mesh depth");
						}
					}
				}
			}
		}
		is_need_calc_max_depth = false;
	}
	int temp = 0;
	int temp_max =  KTROBO_MESH_INSTANCED_COMBINED_MATRIX_CALC_STRUCT_TEMPSIZE;

	for (int i=0; i <= max_depth; i++) {

		for (int k=0;k<isize;k++) {
			for (int j=0;j<KTROBO_MESH_BONE_MAX;j++) {
			if (map_of_depth[i][k][j] == KTROBO_MAP_OF_DEPTH_NULL){ continue;}

			//if (map_of_depth[k][p] == i) {

				
				// st に情報を入れ込む
			int instance_index = k;
			int bone_index = j;
			int skeleton_index=0;
			int parent_bone_index=0;
			int parent_skeleton_index=0;
			int parent_instance_index=0;
				
			MYMATRIX matrix_kakeru;
			MyMatrixIdentity(matrix_kakeru);
			int is_root_bone = 0;
			int connect_without_matrix_local = 0;

			MeshInstanced* instance = mesh_instanceds[instance_index];
			if (!instance->getIsNeedCombinedMatrixLoad()) {continue;}

			unsigned short anime_first[KTROBO_MESH_INSTANCED_BONE_MAX];
			unsigned short anime_last[KTROBO_MESH_INSTANCED_BONE_MAX];
			float anime_weight[KTROBO_MESH_INSTANCED_BONE_MAX];
			memset(anime_first, 0, sizeof(anime_first));
			memset(anime_last, 0, sizeof(anime_last));
			memset(anime_weight, 0, sizeof(anime_weight));

			skeleton_index = instance->getSkeletonIndex();
			Mesh* skeleton = skeletons[skeleton_index];
			MeshBone* bone = skeleton->Bones[bone_index];
			if (bone == skeleton->RootBone) {
				is_root_bone = 1;

				if (instance->getConnectWithoutMatrixLocal()) {
					connect_without_matrix_local = 1;
				}
				matrix_kakeru = *instance->getRootBoneMatrixLocalKakeru();

				MeshInstanced* parent_instance = instance->getParentInstance();
				if (parent_instance) {
					parent_bone_index = instance->getParentBoneIndex();
					parent_skeleton_index = parent_instance->getSkeletonIndex();
					parent_instance_index = parent_instance->getInstanceIndex();
				} else {
					parent_bone_index = KTROBO_MESH_BONE_NULL;
					parent_skeleton_index = skeleton_index;
					parent_instance_index = instance_index;
				}
			} else {
				is_root_bone = 0; connect_without_matrix_local = 0;
				if (bone->parent_bone) {
					parent_bone_index = bone->parent_bone->bone_index;
				} else {
					parent_bone_index = KTROBO_MESH_BONE_NULL;
				}
				parent_skeleton_index = skeleton_index;
				parent_instance_index = instance_index;
			}
			instance->getBoneIndexInfo(anime_first, anime_last, anime_weight);
				

			// 構造体に入れ込む
			COMBINEDMATRIXCALCSTRUCT* v = &stt[temp];
			v->bone_index = (unsigned int)bone_index;
			v->anime_first_index = anime_first[bone_index];
			v->anime_last_index = anime_last[bone_index];
			v->anime_first_weight = anime_weight[bone_index];
			v->control_value = is_root_bone * KTROBO_COMBINED_MATRIX_CONTROL_VALUE_ROOT_BONE 
			+ connect_without_matrix_local * KTROBO_COMBINED_MATRIX_CONTROL_VALUE_CONNECT_WITHOUT_MATRIX_LOCAL
			;
			v->instance_index = (unsigned int)instance_index;
			v->matrix_kakeru = matrix_kakeru;
			v->parent_bone_index = (unsigned int)parent_bone_index;
			v->parent_instance_index = (unsigned int)parent_instance_index;
			v->parent_skeleton_index = (unsigned int)parent_skeleton_index;
			v->skeleton_index = (unsigned int )skeleton_index;
			v->depth = i;
			temp++;
			if (temp >= temp_max) {
				_calcCombinedMatrixToTexture(g, stt, temp,i);
				temp = 0;
				memset(stt, 0, sizeof(COMBINEDMATRIXCALCSTRUCT)* temp_max);

			}
			}
		}

		if (temp >0) {
			_calcCombinedMatrixToTexture(g, stt, temp,i );
			temp = 0;
			memset(stt, 0, sizeof(COMBINEDMATRIXCALCSTRUCT)* temp_max);
		}
		temp = 0;
		memset(stt, 0, sizeof(COMBINEDMATRIXCALCSTRUCT)* temp_max);
	}
	//delete[] stt;
	
	for (int i = 0 ; i < isize; i++) {
		MeshInstanced* instance = mesh_instanceds[i];
		instance->setIsNeedCombinedMatrixLoad(false);
	}
	// 頂点がもつ情報 
	// 必要なのはmatrix_local matrix_basis を取得するのに必要な情報と
	// ルートボーンかどうかとconnectwithout_matrixの情報 matrix_kakeru の情報

	// depth: 深度
	// instance_index:
	// anime_first_index: 
	// anime_last_index:
	// anime_first_weight:
	// skeleton_index:
	// parent_skeleton_index:
	// bone_index: 
	// parent_instance_index:
	// parent_bone_index:
	// matrix_kakeru
	// is_connect_without_matrix_local
	// is_root_bone
	// 
	// 深度情報を計算して０からmax_depth までループして描画する
}


void MeshInstanceds::_calcCombinedMatrixToTexture(Graphics* g, COMBINEDMATRIXCALCSTRUCT* stt, int temp, unsigned int depth) {

	D3D11_MAPPED_SUBRESOURCE subresource;
	unsigned int stride = sizeof(COMBINEDMATRIXWATASUSTRUCT);
	unsigned int offset = 0;


	CS::instance()->enter(CS_DEVICECON_CS, "calc combined matrix");
	
	/*
	g->getDeviceContext()->Map(MeshInstanceds::combined_matrix_calc_vertexbuffer,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	memcpy(subresource.pData, stt, sizeof(COMBINEDMATRIXCALCSTRUCT)*temp);//KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE);
	g->getDeviceContext()->Unmap(MeshInstanceds::combined_matrix_calc_vertexbuffer,0);
	*/

	g->getDeviceContext()->UpdateSubresource(combined_matrix_calc_vertexbuffer, 0, 0, (void*)stt, 0, 0);
	cbuf1.depth = depth;
	cbuf1.vertex_num = (unsigned int)temp;
	g->getDeviceContext()->Map(cbuf1_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	memcpy( subresource.pData, &cbuf1, sizeof(COMBINEDMATRIXCALC_CBUF) );
	g->getDeviceContext()->Unmap(cbuf1_buffer, 0);
	ID3D11ShaderResourceView* testtt = NULL;
	g->getDeviceContext()->PSSetShaderResources(0,1,&testtt);
	g->getDeviceContext()->CSSetShader(mss_for_combined_matrix.cs, NULL,0);
	g->getDeviceContext()->CSSetConstantBuffers(0,1, &cbuf1_buffer);
	ID3D11UnorderedAccessView* test2[]  = {combined_matrix_calc_vertexbuffer_view, combined_matrix_watasu_vertexbuffer_view};
	g->getDeviceContext()->CSSetUnorderedAccessViews(0,2, test2, 0);
//	g->getDeviceContext()->CSSetUnorderedAccessViews(0,1, &combined_matrix_calc_vertexbuffer_view,0);
//	g->getDeviceContext()->CSSetUnorderedAccessViews(1,1, &combined_matrix_watasu_vertexbuffer_view,0);
	g->getDeviceContext()->CSSetShaderResources(0,1,&anime_matrix_basis_texture->view);
	g->getDeviceContext()->CSSetShaderResources(1,1,&matrix_local_texture->view);
	g->getDeviceContext()->CSSetShaderResources(2,1,&combined_matrix_texture->view);
	for (int i = 0 ; i < 16;i++) {
	g->getDeviceContext()->CSSetSamplers(i,1,&combined_matrix_sampler_state);
	}
	g->getDeviceContext()->Dispatch(KTROBO_MESH_INSTANCED_COMBINED_MATRIX_INSTANCE_SIZE,1,1);
	g->getDeviceContext()->Flush();
	
	ID3D11UnorderedAccessView* test[] = {
		{NULL},
		{NULL}
	};
	g->getDeviceContext()->CSSetUnorderedAccessViews(0,2, test, 0);
	//g->getDeviceContext()->CSSetUnorderedAccessViews(1,1, &test, 0);
	ID3D11ShaderResourceView* test3 = NULL;
	g->getDeviceContext()->CSSetShaderResources(0,1,&test3);
	g->getDeviceContext()->CSSetShaderResources(1,1,&test3);
	g->getDeviceContext()->CSSetShaderResources(2,1,&test3);
	g->getDeviceContext()->CSSetShader(NULL,0,0);
	D3D11_VIEWPORT vp;
	
	vp.Height = combined_matrix_texture->height;
	vp.Width = combined_matrix_texture->width;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;


	float ccc[] = {
		0.2f,0.2f,0.8f,0.7f};


	g->getDeviceContext()->OMSetRenderTargets(1, &combined_matrix_texture->target_view, this->mss_for_combined_matrix.depthstencilview);
	g->getDeviceContext()->RSSetViewports(1, &vp);
	g->getDeviceContext()->ClearDepthStencilView(mss_for_combined_matrix.depthstencilview,  D3D11_CLEAR_DEPTH/* | D3D11_CLEAR_STENCIL*/,1.0f, 0 );
	//g->getDeviceContext()->ClearRenderTargetView(combined_matrix_texture->target_view, ccc);

	g->getDeviceContext()->IASetInputLayout(MeshInstanceds::mss_for_combined_matrix.vertexlayout );
	ID3D11Buffer* tt[] = {combined_matrix_watasu_vertexbuffer, combined_matrix_watasu_offsetbuffer};
	unsigned int ttt[] = { sizeof(COMBINEDMATRIXWATASUSTRUCT), sizeof(COMBINEDMATRIXCALC_CBUF)};
	unsigned int tttt[] = {0,0};
	g->getDeviceContext()->IASetVertexBuffers( 0,2, tt, ttt,tttt );

	stride = sizeof(COMBINEDMATRIXCALC_CBUF);

	//g->getDeviceContext()->IASetVertexBuffers( 1, 1, &combined_matrix_watasu_offsetbuffer, &stride, &offset );
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	g->getDeviceContext()->IASetIndexBuffer(combined_matrix_watasu_indexbuffer, DXGI_FORMAT_R32_UINT,0);
	g->getDeviceContext()->RSSetState(mss_for_combined_matrix.rasterstate);

	float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

	g->getDeviceContext()->OMSetBlendState(mss_for_combined_matrix.blendstate, blendFactor,0xFFFFFFFF);
	g->getDeviceContext()->VSSetShader(mss_for_combined_matrix.vs, NULL, 0);
	g->getDeviceContext()->GSSetShader(mss_for_combined_matrix.gs, NULL,0);
	
	g->getDeviceContext()->PSSetShader(mss_for_combined_matrix.ps, NULL, 0);
	g->getDeviceContext()->VSSetShaderResources(0,1,&anime_matrix_basis_texture->view);
	g->getDeviceContext()->VSSetShaderResources(1,1,&matrix_local_texture->view);

	g->getDeviceContext()->DrawIndexedInstanced(3*temp,32,0,0,0);
	//g->getDeviceContext()->DrawIndexedInstanced(3*temp,1,0,0,1);
	//g->getDeviceContext()->DrawIndexedInstanced(3*temp,1,0,0,2);
	//(3*temp,16*2, 0, 0, 0);
	g->getDeviceContext()->Flush();
	g->getDeviceContext()->PSSetShaderResources(0,1,&testtt);
	g->getDeviceContext()->GSSetShader(NULL, NULL,0);
	
	ID3D11RenderTargetView* t = g->getRenderTargetView();
	g->getDeviceContext()->OMSetRenderTargets(1, &t, NULL);
	g->getDeviceContext()->RSSetViewports(1, g->getViewPort());
	g->getDeviceContext()->PSSetShaderResources(0,1,&testtt);
	tt[0] = NULL;
	tt[1] = NULL;
	ttt[0] =0;
	ttt[1] = 0;
	g->getDeviceContext()->IASetVertexBuffers( 0,2, tt, ttt,tttt );
	CS::instance()->leave(CS_DEVICECON_CS, "calc combined matrix leave");	
}

void MeshInstanceds::setViewProj(Graphics* g, MYMATRIX* view, MYMATRIX* proj, MYVECTOR4* lightdir) {

	cbuf2.proj = *proj;
	cbuf2.view = *view;
	cbuf2.lightdir = *lightdir;
	D3D11_MAPPED_SUBRESOURCE subresource;
	CS::instance()->enter(CS_DEVICECON_CS, "setviewproj");
	g->getDeviceContext()->Map(cbuf2_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	memcpy( subresource.pData, &cbuf2, sizeof(MESHINSTANCED_CBUF2) );
	g->getDeviceContext()->Unmap(cbuf2_buffer, 0);
	//g->getDeviceContext()->UpdateSubresource(cbuf2_buffer,0,NULL,&cbuf2,0,0);
	CS::instance()->leave(CS_DEVICECON_CS, "setviewproj");

}

void MeshInstanceds::setCBuf3(Graphics* g, unsigned int color_id) {

	D3D11_MAPPED_SUBRESOURCE subresource;
	cbuf3.color_id = color_id;
	cbuf3.offset1 = color_id;
	cbuf3.offset2 = color_id;
	cbuf3.offset3 = color_id;
//	CS::instance()->enter(CS_DEVICECON_CS, "setcolorid");
	g->getDeviceContext()->Map(cbuf3_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	memcpy( subresource.pData, &cbuf3, sizeof(MESHINSTANCED_CBUF3) );
	g->getDeviceContext()->Unmap(cbuf3_buffer, 0);
//	CS::instance()->enter(CS_DEVICECON_CS, "setcolorid");


}
void MeshInstanceds::_render(Graphics* g, RENDERINSTANCEINFOSTRUCT* st, int temp, Mesh* render_mesh) {


	if (render_mesh->VertexCount && render_mesh->FaceCount) {
	} else {
		return;
	}

	/*
	int size = render_mesh->Bones.size();
	MYMATRIX bone_combined_matrixs[KTROBO_MESH_BONE_MAX];
	memset(bone_combined_matrixs, 0, sizeof(MYMATRIX)* KTROBO_MESH_BONE_MAX);
	for (int i = 0; i < size; i++) {
		MeshBone* b = render_mesh->Bones[i];
		if (b->bone_index >= 0  && b->bone_index < KTROBO_MESH_BONE_MAX) {
			bone_combined_matrixs[b->bone_index] = b->combined_matrix;
		//	MyMatrixIdentity(bone_combined_matrixs[i]);
		}
	}
	*/
	
	//updateCBuf2(g, world, bone_combined_matrixs);

	CS::instance()->enter(CS_DEVICECON_CS, "render in meshinstanced");
	D3D11_MAPPED_SUBRESOURCE subresource;
	g->getDeviceContext()->Map(MeshInstanceds::render_instance_vertexbuffer,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	memcpy(subresource.pData, st, sizeof(RENDERINSTANCEINFOSTRUCT) * temp);//KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE);
	g->getDeviceContext()->Unmap(MeshInstanceds::render_instance_vertexbuffer,0);

	ID3D11RenderTargetView* rr = g->getRenderTargetView();
	g->getDeviceContext()->OMSetRenderTargets(1,&rr,Mesh::pDepthStencilView);//mss_for_render.depthstencilview);
	g->getDeviceContext()->RSSetViewports(1, g->getViewPort());
	g->getDeviceContext()->VSSetConstantBuffers(0,1, &cbuf2_buffer);
	g->getDeviceContext()->PSSetConstantBuffers(0,1, &cbuf2_buffer);
	//g->getDeviceContext()->VSSetConstantBuffers(1,1, &cbuf3_buffer);
	g->getDeviceContext()->IASetInputLayout( mss_for_render.vertexlayout );
	ID3D11Buffer* bufs[] = {render_mesh->p_vertexbuffer, render_instance_vertexbuffer};
	unsigned int stride[] = {sizeof(MESH_VERTEX), sizeof(RENDERINSTANCEINFOSTRUCT)};
	unsigned int offset[] = {0,0};

	g->getDeviceContext()->IASetVertexBuffers( 0, 2, bufs, stride, offset );
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	g->getDeviceContext()->IASetIndexBuffer (render_mesh->p_indexbuffer, DXGI_FORMAT_R32_UINT,0);
	g->getDeviceContext()->RSSetState(mss_for_render.rasterstate);

	float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

	g->getDeviceContext()->OMSetBlendState(mss_for_render.blendstate, blendFactor,0xFFFFFFFF/*0xFFFFFFFF*/);
	g->getDeviceContext()->VSSetShader(mss_for_render.vs, NULL, 0);
	g->getDeviceContext()->GSSetShader(NULL,NULL,0);
	g->getDeviceContext()->PSSetSamplers(1,1, &MeshInstanceds::combined_matrix_sampler_state);
	g->getDeviceContext()->PSSetSamplers(0,1, &MeshInstanceds::combined_matrix_sampler_state);
	g->getDeviceContext()->VSSetSamplers(0,1, &MeshInstanceds::combined_matrix_sampler_state);

	g->getDeviceContext()->PSSetShader(mss_for_render.ps, NULL, 0);
	
	g->getDeviceContext()->VSSetShaderResources(0,1, &combined_matrix_texture->view);
	g->getDeviceContext()->PSSetShaderResources(0,1, &combined_matrix_texture->view);
	g->getDeviceContext()->PSSetShaderResources(1,1,&color_texture->view);
	
	int subset_size = render_mesh->Subsets.size();
	int material_index = -1;
	MyTextureLoader::MY_TEXTURE_CLASS* tex_class = 0;
	for (int i=0;i<subset_size; i++) {
		MeshSubset* s = render_mesh->Subsets[i];

		if (material_index != s->MaterialIndex) {
			if (tex_class != render_mesh->Materials[s->MaterialIndex]->texture) {
				g->getDeviceContext()->PSSetShaderResources(2,1,&render_mesh->Materials[s->MaterialIndex]->texture->view);
				tex_class = render_mesh->Materials[s->MaterialIndex]->texture;
			}
			material_index = s->MaterialIndex;
		}
		
		setCBuf3(g, s->MaterialIndex);
		ID3D11Buffer* NUL = NULL;
		g->getDeviceContext()->PSSetConstantBuffers(1,1, &cbuf3_buffer);
		//g->getDeviceContext()->PSSetConstantBuffers(1,1, &cbuf3_buffer);
		g->getDeviceContext()->DrawIndexedInstanced(s->FaceCount*3,temp,s->FaceIndex*3,0, 0);
	}

	ID3D11ShaderResourceView* vi=NULL;
	g->getDeviceContext()->VSSetShaderResources(0,1, &vi);
	g->getDeviceContext()->PSSetShaderResources(1,1,&vi);
	CS::instance()->leave(CS_DEVICECON_CS, "render in mesh instance");
}


void MeshInstanceds::render(Graphics* g) {

	// 描画の手順
	// インスタンスのメッシュ→メッシュ
	// インスタンスごとの情報
	// world, instance_id
	// color はテクスチャに
	// インスタンスのメッシュに共通な情報
	// pos, normal, index weight (meshのvertexbufferと同じ)

	// cbuffer1 
	// view proj (変更された時点のみ）

	// cbuffer2 すごい頻繁に変更される
	// color_id
	
	//g->getDeviceContext()->ClearDepthStencilView(mss_for_render.depthstencilview, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f, 0 );
	// まず

//	if (getIsLoad()) {
		// ロード中なので描画せずにリターンする
	//	return;
//	}

	RENDERINSTANCEINFOSTRUCT stt[ KTROBO_MESH_INSTANCED_RENDER_INSTANCE_STRUCT_TEMPSIZE];
	memset(stt, 0 , sizeof(RENDERINSTANCEINFOSTRUCT)*KTROBO_MESH_INSTANCED_RENDER_INSTANCE_STRUCT_TEMPSIZE);
	int temp =0;
	int temp_max = KTROBO_MESH_INSTANCED_RENDER_INSTANCE_STRUCT_TEMPSIZE;
	// mesh ごとに計算する
	int meshsize = meshs.size();
	int isize = mesh_instanceds.size();
	for (int m = 0 ; m < meshsize; m++) {
		Mesh* mes = meshs[m];
		
		for (int i = 0 ; i < isize; i++) {
			MeshInstanced* mi = mesh_instanceds[i];
			if (mi->getIsRender() && mi->getMeshIndex() == m) {
				// 構造体に入れ込む
				stt[temp].instance_id = i;
//				stt[temp].world = *mi->getWorld();
				mi->getWorld(&stt[temp].world);
				temp++;
				if (temp >= temp_max) {
					_render(g, stt,temp, mes);
					temp = 0; 
					memset(stt, 0 , sizeof(RENDERINSTANCEINFOSTRUCT)*KTROBO_MESH_INSTANCED_RENDER_INSTANCE_STRUCT_TEMPSIZE);
	

				}
			}
		}
		if (temp > 0) {
			_render(g, stt,temp, mes);
			temp = 0;
			memset(stt, 0 , sizeof(RENDERINSTANCEINFOSTRUCT)*KTROBO_MESH_INSTANCED_RENDER_INSTANCE_STRUCT_TEMPSIZE);
	
		}

	}

	
}

