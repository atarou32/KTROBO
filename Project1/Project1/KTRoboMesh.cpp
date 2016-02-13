#include "KTRoboMesh.h"
#include "KTRoboGameError.h"
#include "KTRoboCS.h"
#include "MyTokenAnalyzer.h"

using namespace KTROBO;

ID3D11VertexShader* Mesh::vs=0;
ID3D11PixelShader* Mesh::ps=0;
ID3D11InputLayout* Mesh::p_vertexlayout=0;
ID3D11BlendState* Mesh::p_blendstate=0;
ID3D11SamplerState* Mesh::p_sampler=0;
ID3D11RasterizerState* Mesh::p_rasterstate=0;
ID3D11DepthStencilView* Mesh::pDepthStencilView = 0;
ID3D11Texture2D* Mesh::pDepthStencil=0;

MeshCBuf1 Mesh::cbuf1;
MeshCBuf2 Mesh::cbuf2;
MeshCBuf3 Mesh::cbuf3;
ID3D11Buffer* Mesh::cbuf1_buffer=0;
ID3D11Buffer* Mesh::cbuf2_buffer=0;
ID3D11Buffer* Mesh::cbuf3_buffer=0;









void Mesh::Init(Graphics* g) {

	HRESULT hr = S_OK;
	ID3DBlob* pblob = 0;
	p_blendstate = 0;
	p_sampler = 0;
	p_vertexlayout = 0;
	vs = 0;
	ps = 0;
	p_rasterstate = 0;
	try {
		CompileShaderFromFile(KTROBO_MESH_SHADER_FILENAME, KTROBO_MESH_SHADER_VS, "vs_4_0", &pblob);
		hr = g->getDevice()->CreateVertexShader(pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			NULL,
			&vs);
		if (FAILED(hr)) {
			pblob->Release();
			pblob = 0;
			vs = 0;
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vs make error");;
		}

		// ���̓��C�A�E�g�̒�`
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
	

		CompileShaderFromFile(KTROBO_MESH_SHADER_FILENAME, KTROBO_MESH_SHADER_PS, "ps_4_0", &pblob);
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
		BlendDesc.AlphaToCoverageEnable = TRUE;
		BlendDesc.IndependentBlendEnable = TRUE;
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
	descS.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	
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

	


	memset(&cbuf1, 0,  sizeof(MeshCBuf1));
	memset(&cbuf2, 0,  sizeof(MeshCBuf2));
	memset(&cbuf3, 0, sizeof(MeshCBuf3));
	D3D11_BUFFER_DESC des;
	des.ByteWidth = sizeof(MeshCBuf1);
	des.Usage = D3D11_USAGE_DEFAULT;
	des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	des.CPUAccessFlags = 0;
	des.MiscFlags = 0;
	des.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA idat;
	idat.pSysMem = &cbuf1;
	idat.SysMemPitch = 0;
	idat.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer(&des, &idat, &cbuf1_buffer);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
	}

	des.ByteWidth = sizeof(MeshCBuf2);
	des.Usage = D3D11_USAGE_DYNAMIC;
	des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	des.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	des.MiscFlags = 0;
	des.StructureByteStride = 0;

//	D3D11_SUBRESOURCE_DATA idat;
	idat.pSysMem = &cbuf2;
	idat.SysMemPitch = 0;
	idat.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer(&des, &idat, &cbuf2_buffer);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
	}

	des.ByteWidth = sizeof(MeshCBuf3);
	des.Usage = D3D11_USAGE_DYNAMIC;
	des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	des.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	des.MiscFlags = 0;
	des.StructureByteStride = 0;

//	D3D11_SUBRESOURCE_DATA idat;
	idat.pSysMem = &cbuf3;
	idat.SysMemPitch = 0;
	idat.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer(&des, &idat, &cbuf3_buffer);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
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

	
}



void Mesh::updateCBuf1(Graphics* g, MYMATRIX* view, MYMATRIX* proj) {
	cbuf1.proj = *proj;
	cbuf1.view = *view;

	g->getDeviceContext()->UpdateSubresource(cbuf1_buffer,0,NULL,&cbuf1,0,0);

}


void Mesh::updateCBuf2(Graphics* g, MYMATRIX* world, MYMATRIX* bone_combined_matrixs) {
	
	D3D11_MAPPED_SUBRESOURCE msr;
	
	cbuf2.world = *world;
	for (int i = 0 ; i < KTROBO_MESH_BONE_MAX; i++) {
		cbuf2.bone_combined_matrixs[i] = bone_combined_matrixs[i];
	}

	g->getDeviceContext()->Map(cbuf2_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy( msr.pData, &cbuf2, sizeof(MeshCBuf2) );
	g->getDeviceContext()->Unmap(cbuf2_buffer, 0);
}

void Mesh::updateCBuf3(Graphics* g, MYVECTOR4* color, MYVECTOR4* lightdir, MYVECTOR4* viewdir) {
	D3D11_MAPPED_SUBRESOURCE msr;
	
	cbuf3.color = *color;
	cbuf3.lightdir = * lightdir;
	cbuf3.viewdir = * viewdir;
	g->getDeviceContext()->Map(cbuf3_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy( msr.pData, &cbuf3, sizeof(MeshCBuf3) );
	g->getDeviceContext()->Unmap(cbuf3_buffer, 0);
}


void Mesh::Del() {

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

	if (cbuf1_buffer) {
		cbuf1_buffer->Release();
		cbuf1_buffer = 0;
	}

	if (cbuf2_buffer) {
		cbuf2_buffer->Release();
		cbuf2_buffer = 0;
	}

	if (cbuf3_buffer) {
		cbuf3_buffer->Release();
		cbuf3_buffer = 0;
	}

	if( pDepthStencil )
	{
		pDepthStencil->Release();
	    pDepthStencil = 0;
	}

	if (pDepthStencilView) {
		pDepthStencilView->Release();
		pDepthStencilView = 0;
	}

}



void Mesh::readAnime(char* filename) {
	
	MyTokenAnalyzer a;
	if (!a.load(filename)) {
		return;
	}



	while (!a.enddayo()) {
		a.GetToken();
		if (strcmp(a.Toke(), "animsets")==0) {
			a.GetToken("{");
			int bone_size = a.GetIntToken();
			for (int i=0;i<bone_size; i++) {
				a.GetToken("anim");
				a.GetToken("{");
				a.GetToken();
				string bone_name = string(a.Toke());
				a.GetToken("keys");
				a.GetToken("{");
				int size = a.GetIntToken();
				for (int j=0;j<size;j++) {
					int frame = a.GetIntToken();
					a.GetToken("matrix");
					float matrix[16];
					for (int l=0;l<16;l++) {
						matrix[l] = a.GetFloatToken();// �擾���Ă��邯�ǎg��Ȃ�
					}
					a.GetToken("matrix_basis");
					float matrix_basis[16];
					for(int l=0;l<16;l++) {
						matrix_basis[l] = a.GetFloatToken();
					}
					MeshAnime* anime = new MeshAnime(BoneIndexes[bone_name],frame,matrix_basis);
					if (Bones.size() > 0) {
						(Bones[BoneIndexes[bone_name]])->animes.push_back(anime);
					}
				
				}
				a.GetToken("}");
				a.GetToken("}");
			}

		}
	}

	// ��ЂÂ�
	a.deletedayo();
	//animate(0,true);
	return;




}

void Mesh::Release() {

	if (VertexCount && FaceCount) {
	} else {
		return;
	}

	if (p_vertexbuffer) {
		p_vertexbuffer->Release();
		p_vertexbuffer = 0;
		VertexCount = 0;
	}

	if (p_indexbuffer) {
		p_indexbuffer->Release();
		p_indexbuffer = 0 ;
		FaceCount = 0;
	}

	vector<MeshBone*>::iterator bone_it;
	bone_it = Bones.begin();
	while (bone_it != Bones.end()) {
		MeshBone* bn = *bone_it;
		delete bn;
		bone_it = bone_it + 1;
	}
	Bones.clear();

	vector<MeshMaterial*>::iterator mate_it;
	mate_it = Materials.begin();
	while( mate_it != Materials.end()) {
		MeshMaterial* mm = *mate_it;
		delete mm;
		mate_it = mate_it + 1;
	}
	Materials.clear();

	vector<MeshSubset*>::iterator sub_it;
	sub_it = Subsets.begin();
	while(sub_it != Subsets.end()) {
		MeshSubset* ss = *sub_it;
		delete ss;
		sub_it = sub_it + 1;
	}
	Subsets.clear();
}

Mesh::~Mesh() {
	Release();
}

Mesh::Mesh() {
	p_vertexbuffer = 0;
	p_indexbuffer= 0;
	RootBone = 0;
	this->FaceCount = 0;
	this->VertexCount = 0;
	RootBone_connect_without_material_local = false;
	MyMatrixIdentity(rootbone_matrix_local_kakeru);
	bone_max_depth = 0;
}



void readBoneFrame(MyTokenAnalyzer* a,vector<MeshBone*>* BS, MeshBone* parent, map<string, int>* BI) {
	a->GetToken("bone_layer");
	a->GetToken("{");
	a->GetToken();
	int index = (*BI)[a->Toke()];
	((*BS)[index])->setParent(BS,parent->bone_index);
	parent->setChild(index);
	int childnum = a->GetIntToken();
	for (int i=0;i<childnum;i++) {
		readBoneFrame(a,BS,(*BS)[index],BI);
	}
	a->GetToken("}");
}




void Mesh::readMesh(Graphics* g, char* filename, MyTextureLoader* tex_loader) {



	MESH_VERTEX* vertexs=0;
	UINT* indexs = 0; 

	MyTokenAnalyzer a;
	if (!a.load(filename)) {
		throw new GameError(FATAL_ERROR, "no model");
	}
	try {

	while (!a.enddayo()) {
		a.GetToken();
		if (strcmp(a.Toke(), "verts")==0) {
			// vertices ��ݒ肷��
			//if (!loadMesh) continue;

			a.GetToken("{");
			VertexCount = a.GetIntToken();
			if (VertexCount > KTROBO_MESH_VERTEX_COUNT_MAX) {
				if (indexs) {
					delete[] indexs;
					indexs = 0;
				}
				throw new GameError(KTROBO::FATAL_ERROR, "too many vertex");
			}

			vertexs = new MESH_VERTEX[VertexCount];
			memset( vertexs , 0, sizeof(MESH_VERTEX) * VertexCount);
			
			MESH_VERTEX *v = vertexs;

			for(unsigned int i=0;i<VertexCount;i++) {
				int inde = a.GetIntToken();
				v[inde].pos.float3.x = a.GetFloatToken();
				v[inde].pos.float3.y = a.GetFloatToken();
				v[inde].pos.float3.z = a.GetFloatToken();
				v[inde].normal.float3.x = a.GetFloatToken();
				v[inde].normal.float3.y = a.GetFloatToken();
				v[inde].normal.float3.z = a.GetFloatToken();
				v[inde].UV.x = a.GetFloatToken();
				v[inde].UV.y = 1 - a.GetFloatToken();
				v[inde].Index[0]=0;
				v[inde].weight[0]=1;
				for (DWORD j=1; j<MODEL_BLEND_COUNT; j++) {
					v[inde].weight[j]=0;

					v[inde].Index[j] = 0;
					v[inde].Index[j+3] = 0;
				}
			}
//			CreateOBB();
			a.GetToken("}");
		}
		if (strcmp(a.Toke(), "polygons")==0) {
			
			a.GetToken("{");
			FaceCount=a.GetIntToken();
			if (3 * FaceCount > KTROBO_MESH_INDEX_COUNT_MAX) {
				if (vertexs) {
					delete[] vertexs;
					vertexs = 0;
				}
				throw new GameError(KTROBO::FATAL_ERROR, "too many indexs");
			}
			indexs = new UINT[FaceCount* 3];

			
			MeshSubset* subset= NULL;
			int prev_material_index= -1;
			for (DWORD i=0, n=FaceCount*3; i<n; i+=3) {
				//for (DWORD j=0; j<3; j++) {
				int face_inde = a.GetIntToken();
				a.GetFloatToken();//normalx
				a.GetFloatToken();//normaly
				a.GetFloatToken();//normalz
				int material_index = a.GetIntToken();
				if (material_index != prev_material_index) {
					subset = new MeshSubset();
					subset->FaceCount = 1;
					subset->FaceIndex = i / 3;
					subset->MaterialIndex = material_index;
					Subsets.push_back(subset);
					prev_material_index = material_index;
				} else {
					subset->FaceCount++;
				}
				indexs[i+0]=a.GetIntToken();
				indexs[i+1]=a.GetIntToken();
				indexs[i+2]=a.GetIntToken();
			}
			a.GetToken("}");
		}
		if (strcmp(a.Toke(), "materials")==0) {
			a.GetToken("{");
			int material_count = a.GetIntToken();
			for(int i=0;i<material_count;i++) {
				MeshMaterial* material = new MeshMaterial();
				Materials.push_back(material);
				a.GetToken();
				float x = a.GetFloatToken();
				float y = a.GetFloatToken();
				float z = a.GetFloatToken();
				material->color = MYVECTOR4(x,y,
					z,1.0f);
				a.GetToken();
				char buf[256];
				material->texture = tex_loader->loadClass(a.Toke());
			}
			a.GetToken("}");
		}
	}

	// �{�[�����
	
	a.resetPointer();
	while (!a.enddayo()) {
		a.GetToken();
		if (strcmp(a.Toke(), "boneinfo") ==0) {
			a.GetToken("{");
			int bone_size = a.GetIntToken();
			for (int i=0;i<bone_size; i++) {
				int bone_inde = a.GetIntToken();
				a.GetToken();
				string* bone_name = new string(a.Toke());
				float matrix[16];
				a.GetToken();
				for(int j=0;j<16;j++) {
					matrix[j] = a.GetFloatToken();
				}
				float matrix_local[16];
				a.GetToken();
				for(int j=0;j<16;j++) {
					matrix_local[j] = a.GetFloatToken();
				}
				MeshBone* b = new MeshBone(bone_inde, matrix_local, bone_name->c_str());
				if (Bones.size() <= bone_inde) {
					Bones.insert(Bones.begin()+bone_inde, b);
				} else {
					Bones[bone_inde] = b;
				}
			

				BoneIndexes.insert(pair<string,int>(*bone_name, bone_inde));
				delete bone_name;
			}
			a.GetToken("}");
		}
	}

	a.resetPointer();
	while (!a.enddayo()) {
		a.GetToken();

		// �{�[���K�w�\��
		if (strcmp(a.Toke(), "bone_layer") ==0) {
			a.GetToken("{");
			a.GetToken();
			int index = BoneIndexes[a.Toke()];
			Bones[index]->setParent(&Bones, KTROBO_MESH_BONE_NULL);
			this->RootBone = Bones[index];
			int childnum = a.GetIntToken();
			for (int i=0;i<childnum;i++) {
				readBoneFrame(&a,&Bones,Bones[index], &BoneIndexes);
			}
			a.GetToken("}");

			// �{�[���K�w�\�����ǂݏI������̂�
			// bone_max_depth ���v�Z����
			int bone_size = Bones.size();
			int max_dep = 0;
			for (int i = 0 ; i< bone_size; i++) {
				int dep = 0;
				MeshBone* bb = Bones[i];
				dep = bb->getDepth();

				if (dep > max_dep) {
					max_dep = dep;
				}
			}
			this->bone_max_depth = max_dep;
		}

		

		// �{�[���̊e���_�ɑ΂���d��
		if (strcmp(a.Toke(), "boneweights") ==0) {
			
			a.GetToken("{");
		

			int size = a.GetIntToken();
			if (size > KTROBO_MESH_VERTEX_COUNT_MAX) {
				if (vertexs) {
					delete[] vertexs;
					vertexs = 0;
				}
				if (indexs) {
					delete[] indexs;
					indexs = 0;
				}

				throw new GameError(FATAL_ERROR, "too many vertexs");
			}

			for (int i = 0;i<size;i++) {
				int vertice_index = a.GetIntToken();
				int group_size = a.GetIntToken();
				MESH_VERTEX* v = vertexs;
				/*
				for (int k = 0 ; k < MODEL_BLEND_COUNT+3; k++) {
					v[vertice_index].Index[k] = 0;
				}
				for (int k = 0 ; k < MODEL_BLEND_COUNT; k++) {
					v[vertice_index].weight[k] = 0;
				}
				*/

				if (vertice_index < size) {
					for (int j=0;j<group_size;j++) {
						unsigned int inde = a.GetUIntToken();//a.GetIntToken() << (8*j);
						float f = a.GetFloatToken();
						if (j < MODEL_BLEND_COUNT) {
							v[vertice_index].Index[j] = (unsigned char)inde;//0x00000001;//inde;
						}
						if (j < MODEL_BLEND_COUNT){
							v[vertice_index].weight[j] = f;
						}
					}
				}
			}
		}
	}
	} catch (GameError* err) {
		a.deletedayo();
		Release();

		char buf[4096];
		memset(buf,0,4096);
		sprintf_s(buf,4096, "%s %s",filename, err->getMessage());
		GameError* new_error = new GameError(err->getErrorCode(), string(buf));
		delete err;


		throw new_error;
	}
	// vertex���쐬����
	D3D11_BUFFER_DESC bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MESH_VERTEX)*VertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	memset(&initData, 0, sizeof(initData));
	initData.pSysMem = vertexs;

	HRESULT hr = g->getDevice()->CreateBuffer(&bd, &initData,&p_vertexbuffer);
	if (FAILED(hr)) {
		if (vertexs) {
			delete[] vertexs;
			vertexs = 0;
		}
		if (indexs ) { 
			delete[] indexs;
			indexs = 0;
		}

		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}

	
    D3D11_BUFFER_DESC hBufferDesc;
    hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.ByteWidth = sizeof( UINT) * FaceCount* 3;
    hBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    hBufferDesc.CPUAccessFlags = 0;
    hBufferDesc.MiscFlags = 0;
    hBufferDesc.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA hSubResourceData;
    hSubResourceData.pSysMem = indexs;
    hSubResourceData.SysMemPitch = 0;
    hSubResourceData.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer( &hBufferDesc, &hSubResourceData, &p_indexbuffer );
    if( FAILED( hr ) ) {
        if (vertexs) {
			delete[] vertexs;
			vertexs = 0;
		}
		if (indexs ) { 
			delete[] indexs;
			indexs = 0;
		}
		if (p_vertexbuffer) {
			p_vertexbuffer->Release();
			p_vertexbuffer = 0;
		}
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "index buffer make error");
	}

	delete[] vertexs;
	delete[] indexs;

	//edgeCollapse(100);
	// ��ЂÂ�
	a.deletedayo();
	//CS::instance()->leave(CS_MESH_CS, "load_mesh");
	return;

}




void KTROBO::WAgetScaleFromMatrix(MYMATRIX* mat, MYVECTOR3* p) {
	MYVECTOR3 v;
	v.float3.x = mat->_11;
	v.float3.y = mat->_12;
	v.float3.z = mat->_13;
	float x = MyVec3Length(v);

	v.float3.x = mat->_21;
	v.float3.y = mat->_22;
	v.float3.z = mat->_23;
	float y = MyVec3Length(v);

	v.float3.x = mat->_31;
	v.float3.y = mat->_32;
	v.float3.z = mat->_33;
	float z = MyVec3Length(v);

	//mylog::writelog("x,y,z = %f,%f,%f\n",x,y,z);
	//static D3DXVECTOR3 ans;
	p->float3.x = x;
	p->float3.y = y;
	p->float3.z = z;
	//return &ans;
}

void KTROBO::WAsetScaleToMatrix(MYMATRIX* mat, MYVECTOR3* v) {

	MYVECTOR3 now;
	WAgetScaleFromMatrix(mat,&now);
	float bairitux = v->float3.x / now.float3.x;
	float bairituy = v->float3.y / now.float3.y;
	float bairituz = v->float3.z / now.float3.z;

	mat->_11 = mat->_11 * bairitux;
	mat->_12 = mat->_12 * bairitux;
	mat->_13 = mat->_13 * bairitux;
	mat->_21 = mat->_21 * bairituy;
	mat->_22 = mat->_22 * bairituy;
	mat->_23 = mat->_23 * bairituy;
	mat->_31 = mat->_31 * bairituz;
	mat->_32 = mat->_32 * bairituz;
	mat->_33 = mat->_33 * bairituz;
}

void Mesh::getOffsetMatrixToGetMinMaxAndWeightIndex(MeshBone* bone, float frame, unsigned short* ans_minmax_index, unsigned short* ans_maxmin_index, float* weight) {
	// �܂���ԋ߂��A�j���t���[�����擾����
	float minmax = 511;
	float maxmin = 0;
	int minmaxindex = bone->animes.size()-1;
	int maxminindex = 0;
	int siz = bone->animes.size();
	for (int i=0;i<siz; i++) {
		MeshAnime* animebone = bone->animes[i];
		if (animebone->frame <= minmax && animebone->frame >= frame) {
			minmax = (float)animebone->frame;
			minmaxindex = i;
		}

		if (animebone->frame >= maxmin && animebone->frame <= frame) {
			maxmin = (float)animebone->frame;
			maxminindex = i;
		}
	}


	float r=1;
	if (minmax != maxmin) {
		r = (minmax - frame)/(minmax - maxmin);
	}

	*ans_minmax_index = (unsigned short)minmaxindex;
	*ans_maxmin_index = (unsigned short)maxminindex;
	*weight = r;
}

void Mesh::calculateOffsetMatrixToGetMinMaxAndWeight(MeshBone* bone, float frame, unsigned short* ans_minmax, unsigned short* ans_maxmin, float* weight) {
	// �܂���ԋ߂��A�j���t���[�����擾����
	float minmax = 1000000;
	float maxmin = -1;
	int minmaxindex = -1;
	int maxminindex = -1;
	int siz = bone->animes.size();
	for (int i=0;i<siz; i++) {
		MeshAnime* animebone = bone->animes[i];
		if (animebone->frame <= minmax && animebone->frame >= frame) {
			minmax = (float)animebone->frame;
			minmaxindex = i;
		}

		if (animebone->frame >= maxmin && animebone->frame <= frame) {
			maxmin = (float)animebone->frame;
			maxminindex = i;
		}
	}


	float r=1;
	if (minmax != maxmin) {
		r = (minmax - frame)/(minmax - maxmin);
	}

	*ans_minmax = (unsigned short)minmax;
	*ans_maxmin = (unsigned short)maxmin;
	*weight = r;
}


void Mesh::calculateOffsetMatrix(MeshBone* bone,float frame) {
	// �܂���ԋ߂��A�j���t���[�����擾����
	float minmax = 1000000;
	float maxmin = -1;
	int minmaxindex = -1;
	int maxminindex = -1;
	int siz = bone->animes.size();
	for (int i=0;i<siz; i++) {
		MeshAnime* animebone = bone->animes[i];
		if (animebone->frame <= minmax && animebone->frame >= frame) {
			minmax = (float)animebone->frame;
			minmaxindex = i;
		}

		if (animebone->frame >= maxmin && animebone->frame <= frame) {
			maxmin = (float)animebone->frame;
			maxminindex = i;
		}
	}

	if (minmaxindex == -1 && maxminindex == -1) {
		MyMatrixIdentity(bone->offset_matrix);
	} else if( minmaxindex == -1) {
		bone->offset_matrix = bone->animes[maxminindex]->matrix_basis;
	} else if( maxminindex == -1) {
		bone->offset_matrix = bone->animes[minmaxindex]->matrix_basis;
	} else if (bone->animes[minmaxindex]->frame == frame) {
		bone->offset_matrix = bone->animes[minmaxindex]->matrix_basis;
	} else if( bone->animes[maxminindex]->frame == frame) {
		bone->offset_matrix = bone->animes[maxminindex]->matrix_basis;
	} else {
		float r=(minmax - frame)/(minmax - maxmin);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
			bone->offset_matrix.m[i][j] = ((
			 bone->animes[maxminindex]->matrix_basis.m[i][j]) * (r)
							+ ((1-r) 
							* bone->animes[minmaxindex]->matrix_basis.m[i][j]))
							;
			}
		}
		static MYVECTOR3 v(1,1,1);
		WAsetScaleToMatrix(&bone->offset_matrix,&v);
	}
	//MyMatrixIdentity(bone->matrix_local);
	//MyMatrixRotationZ(bone->offset_matrix, 13);//frame);
	//bone->offset_matrix._41= 7;
	//MyMatrixTranslation(bone->offset_matrix, 0,1,0);
	vector<int>::iterator it;
	it = (bone->child_bone_indexs).begin();
	vector<int>::iterator en = bone->child_bone_indexs.end();
	while(it != en) {
		MeshBone* child = Bones[*it];
		calculateOffsetMatrix(child,frame);
		it = it + 1;
	}



}

void Mesh::animateBoneFrame(MeshBone* bone) {

	MYMATRIX restinv;
	MyMatrixInverse(restinv,NULL,bone->matrix_local);
	//MyMatrixIdentity(bone->matrix_local);
	//MyMatrixScaling(bone->matrix_local,2,2,3);

	if (bone->parent_bone && (bone != this->RootBone && RootBone->parent_bone == NULL)) {
		MYMATRIX par_rest;
		MYMATRIX tempmatrix;
/*
		MyMatrixMultiply(tempmatrix, bone->parent_bone->combined_matrix, bone->matrix_local);
		MyMatrixInverse(par_rest,NULL, tempmatrix);
		MyMatrixMultiply(bone->combined_matrix, tempmatrix, bone->offset_matrix);
		MyMatrixMultiply(bone->combined_matrix, bone->combined_matrix, par_rest);
		MyMatrixMultiply(bone->combined_matrix, bone->combined_matrix, bone->parent_bone->combined_matrix);
	*/	
		
//		MyMatrixIdentity(bone->matrix_local);
		MyMatrixMultiply(tempmatrix, bone->matrix_local, bone->parent_bone->combined_matrix);
		MyMatrixInverse(par_rest,NULL, tempmatrix);
		MyMatrixMultiply(bone->combined_matrix, bone->offset_matrix, tempmatrix);
		MyMatrixMultiply(bone->combined_matrix, par_rest, bone->combined_matrix);
		MyMatrixMultiply(bone->combined_matrix, bone->parent_bone->combined_matrix, bone->combined_matrix);
		
	//	MyMatrixTranslation(bone->combined_matrix,0,0,0);

	} else if (bone->parent_bone && bone!= RootBone && RootBone->parent_bone != NULL) {
		MYMATRIX par_rest;
		MYMATRIX tempmatrix;
	
		/*MyMatrixMultiply(tempmatrix,bone->parent_bone->combined_matrix, bone->matrix_local);
		MyMatrixInverse(par_rest,NULL, tempmatrix);
		MyMatrixMultiply(bone->combined_matrix, tempmatrix, bone->offset_matrix);
		MyMatrixMultiply(bone->combined_matrix, bone->combined_matrix, par_rest);
		MyMatrixMultiply(bone->combined_matrix, bone->combined_matrix, bone->parent_bone->combined_matrix);*/
	
		MyMatrixMultiply(tempmatrix,bone->matrix_local, bone->parent_bone->combined_matrix);
		MyMatrixInverse(par_rest,NULL, tempmatrix);
		MyMatrixMultiply(bone->combined_matrix, bone->offset_matrix, tempmatrix);
		MyMatrixMultiply(bone->combined_matrix, par_rest, bone->combined_matrix);
		MyMatrixMultiply(bone->combined_matrix, bone->parent_bone->combined_matrix, bone->combined_matrix);
	

	}else if (bone->parent_bone && bone == RootBone && !RootBone_connect_without_material_local) {
		
		MYMATRIX par_rest;
		MYMATRIX parent_inverse;
		MYMATRIX p;
		MYMATRIX pp;
		MyMatrixIdentity(pp);
		MYMATRIX tempmatrix;
		
		MeshBone* bb = bone->parent_bone;
		while(bb->parent_bone) {
			bb = bb->parent_bone;
		}

	

	//	MyMatrixMultiply(p,bone->matrix_local,bb->matrix_local);
	//	MyMatrixInverse(parent_inverse, NULL, p);
	//	MYMATRIX tempmatrix;
	//	MyMatrixMultiply(tempmatrix,bone->parent_bone->combined_matrix, bone->matrix_local);
	//	MyMatrixInverse(par_rest, NULL, tempmatrix);
		// RootBone ��parent ������ꍇ��offset �̌��ʂ𖳂���
	//	MyMatrixMultiply(bone->combined_matrix, bone->parent_bone->combined_matrix, bone->parent_bone->matrix_local);
	//	MyMatrixMultiply(bone->combined_matrix, bone->combined_matrix, bb->matrix_local);
		//MyMatrixMultiply(bone->combined_matrix, bone->combined_matrix, parent_inverse);
	//	*/
		MYMATRIX tem;
		MyMatrixMultiply(tem, bone->matrix_local, this->rootbone_matrix_local_kakeru);
		MyMatrixInverse(p, NULL, tem);
	//	p = bone->matrix_local;
//		MyMatrixInverse(pp, NULL, bone->parent_bone->matrix_local);
		MyMatrixMultiply(bone->combined_matrix, bone->parent_bone->matrix_local, bone->parent_bone->combined_matrix);
		MyMatrixMultiply(bone->combined_matrix, p, bone->combined_matrix);
//		MyMatrixMultiply(bone->combined_matrix,bone->combined_matrix, bone->matrix_local);

		//MyMatrixMultiply(bone->combined_matrix, bone->parent_bone->matrix_local, bb->matrix_local);// combined_matrix;
		//bone->combined_matrix = bb->matrix_local;

	} else if (bone->parent_bone && bone == RootBone && RootBone_connect_without_material_local) {
		MYMATRIX par_rest;
		MYMATRIX tempmatrix;
	/*	MyMatrixMultiply(tempmatrix, bone->parent_bone->combined_matrix, bone->matrix_local);
		MyMatrixInverse(par_rest,NULL, tempmatrix);
		MyMatrixMultiply(bone->combined_matrix, tempmatrix, bone->offset_matrix);
		MyMatrixMultiply(bone->combined_matrix, bone->combined_matrix, par_rest);
		MyMatrixMultiply(bone->combined_matrix, bone->combined_matrix, bone->parent_bone->combined_matrix);*/
		MYMATRIX tem;
		MyMatrixMultiply(tem, bone->matrix_local, this->rootbone_matrix_local_kakeru);
		
		MyMatrixMultiply(tempmatrix, tem, bone->parent_bone->combined_matrix);
		MyMatrixInverse(par_rest,NULL, tempmatrix);
		MyMatrixMultiply(bone->combined_matrix, bone->offset_matrix, tempmatrix);
		MyMatrixMultiply(bone->combined_matrix, par_rest, bone->combined_matrix);
		MyMatrixMultiply(bone->combined_matrix, bone->parent_bone->combined_matrix, bone->combined_matrix);

	} else {
		static MYMATRIX mat_inv;
	
	//	MyMatrixIdentity(bone->matrix_local); 
		MyMatrixInverse(mat_inv,NULL,bone->matrix_local);
//		MYMATRIX par_inv;
//		MyMatrixMultiply(bone->combined_matrix, bone->matrix_local, bone->offset_matrix);
//	MyMatrixMultiply(bone->combined_matrix, bone->combined_matrix, mat_inv);


		
		MyMatrixMultiply(bone->combined_matrix, bone->offset_matrix, bone->matrix_local);
		MyMatrixMultiply(bone->combined_matrix, mat_inv, bone->combined_matrix);
	
		//MyMatrixTranslation(bone->combined_matrix, 0,0,1);
	//MyMatrixTranslation(bone->combined_matrix, 0,0,0);
	//	MyMatrixIdentity(bone->combined_matrix);
	}

	vector<int>::iterator it;
	it = (bone->child_bone_indexs).begin();
	vector<int>::iterator en = bone->child_bone_indexs.end();
	while(it != en) {
		MeshBone* child = Bones[*it];
		animateBoneFrame(child);
		it = it + 1;
	}
}



void Mesh::animate(float frame, bool calculate_offsetmatrix) {
	if (!this->RootBone) {
		throw new GameError(KTROBO::FATAL_ERROR, "no model");
	}
	if (!this->VertexCount || !this->FaceCount) {
		throw new GameError(KTROBO::FATAL_ERROR, "no count");
	}

	if (calculate_offsetmatrix) {
		calculateOffsetMatrix(this->RootBone,frame);
	}
	animateBoneFrame(this->RootBone);
}


void Mesh::draw(Graphics* g, MYMATRIX* world, MYMATRIX* view, MYMATRIX* proj) {
		
	if (VertexCount && FaceCount) {
	} else {
		return;
	}

	int size = Bones.size();
	MYMATRIX bone_combined_matrixs[KTROBO_MESH_BONE_MAX];
	memset(bone_combined_matrixs, 0, sizeof(MYMATRIX)* KTROBO_MESH_BONE_MAX);
	for (int i = 0; i < size; i++) {
		MeshBone* b = Bones[i];
		if (b->bone_index >= 0  && b->bone_index < KTROBO_MESH_BONE_MAX) {
			bone_combined_matrixs[b->bone_index] = b->combined_matrix;
			//MyMatrixIdentity(bone_combined_matrixs[i]);
		}
	}
	Mesh::updateCBuf1(g, view, proj);
	Mesh::updateCBuf2(g, world, bone_combined_matrixs);

	unsigned int stride = sizeof(MESH_VERTEX);
	unsigned int offset = 0;
	
	//g->getDeviceContext()->IASetInputLayout(p_vertexlayout);


	g->getDeviceContext()->VSSetConstantBuffers(0,1, &cbuf1_buffer);
	g->getDeviceContext()->VSSetConstantBuffers(1,1, &cbuf2_buffer);
	g->getDeviceContext()->IASetInputLayout( p_vertexlayout );
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, &p_vertexbuffer, &stride, &offset );
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	g->getDeviceContext()->IASetIndexBuffer (p_indexbuffer, DXGI_FORMAT_R32_UINT,0);
	g->getDeviceContext()->RSSetState(p_rasterstate);
	
	float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

	g->getDeviceContext()->OMSetBlendState(p_blendstate, blendFactor,0xFFFFFFFF/*0xFFFFFFFF*/);
		
	g->getDeviceContext()->VSSetShader(this->vs, NULL, 0);
	g->getDeviceContext()->GSSetShader(NULL,NULL,0);
	g->getDeviceContext()->PSSetSamplers(0,1, &p_sampler);
		
	g->getDeviceContext()->PSSetShader(this->ps, NULL, 0);
	

	
	int subset_size = Subsets.size();
	int material_index = -1;
	MyTextureLoader::MY_TEXTURE_CLASS* tex_class = 0;
	for (int i=0;i<subset_size; i++) {
		MeshSubset* s = Subsets[i];

		if (material_index != s->MaterialIndex) {
			if (tex_class != Materials[s->MaterialIndex]->texture) {
				g->getDeviceContext()->PSSetShaderResources(0,1,&Materials[s->MaterialIndex]->texture->view);
				tex_class = Materials[s->MaterialIndex]->texture;
			}
			material_index = s->MaterialIndex;
		}
		MYVECTOR4 lightdir = MYVECTOR4(1,1,-10,1);
		MYVECTOR4 viewdir = MYVECTOR4(0,1,0,1);
		updateCBuf3(g, &Materials[s->MaterialIndex]->color, &lightdir, &viewdir);
	
		g->getDeviceContext()->PSSetConstantBuffers(0,1, &cbuf3_buffer);
		
		g->getDeviceContext()->DrawIndexed( s->FaceCount*3, s->FaceIndex*3, 0);
		
	}

}