#ifndef KTROBOMESHINSTANCED_H
#define KTROBOMESHINSTANCED_H


#pragma once
#include "KTRoboMesh.h"
#include <vector>
#include "MyDefine.h"
#include "MyLuaGlueMakeCommon.h"

namespace KTROBO {
#define KTROBO_MESH_INSTANCED_COLOR_MAX 12
#define KTROBO_MESH_INSTANCED_BONE_MAX 32

interface IMeshInstanced {
public:
	TO_LUA virtual int getParentIInstanceIndex()=0;
	TO_LUA virtual void setBoneIndexInfo(IN_ unsigned short* bones_anime_first_index, IN_ unsigned short* bones_anime_last_index, IN_ float* bones_anime_first_weight)=0;
	TO_LUA virtual void setWorld(IN_ YARITORI MYMATRIX* world)=0;
	TO_LUA virtual YARITORI MYMATRIX* getWorld()=0;
	TO_LUA virtual void setColor(IN_ YARITORI MYVECTOR4* colors)=0;
	TO_LUA virtual YARITORI MYVECTOR4* getColor(OUT_ int index)=0;
	TO_LUA virtual void setIsRender(IN_ bool t)=0;
	TO_LUA virtual bool getIsRender()=0;
	TO_LUA virtual int getInstanceIndex()=0;
};


class MeshInstanced : public IMeshInstanced
{
private:

	// 個々のインスタンスに共通な変数
	MYMATRIX world;
	MYMATRIX rootbone_matrix_local_kakeru; // rootbone のマトリックスにかけるマトリックス　親メッシュがある場合によく使用される 
	MYVECTOR4 colors[KTROBO_MESH_INSTANCED_COLOR_MAX];
	unsigned short bones_anime_first_index[KTROBO_MESH_INSTANCED_BONE_MAX];
	unsigned short bones_anime_last_index[KTROBO_MESH_INSTANCED_BONE_MAX];
	float bones_anime_first_weight[KTROBO_MESH_INSTANCED_BONE_MAX];

	int mesh_index; // もとになるメッシュのインデックス
	int skeleton_index;// もとになるスケルトンのインデックス
	int instance_index; // このインスタンスのインデックス

	MeshInstanced* parent_instance;
	int parent_bone_index;
	vector<MeshInstanced*> child_instances;

	bool connect_without_matrix_local;
	bool is_need_combined_matrix_load; // meshinstancedsのコンバインドメイトリクスのロードが必要かどうか
	bool is_need_color_texture_load;

	bool is_render;
public:
	bool getIsRender() {return is_render;}
	void setIsRender(bool t) {is_render = t;}
	void setIsNeedColorTextureLoad(bool t) {is_need_color_texture_load = t;}

	void setIsNeedCombinedMatrixLoad(bool t) {
		is_need_combined_matrix_load = t;
		if (t) {
			// true の場合はchildに伝播する
			vector<MeshInstanced*>::iterator it = child_instances.begin();
			while(it != child_instances.end()) {
				MeshInstanced* c = *it;
				c->setIsNeedCombinedMatrixLoad(t);
				it = it + 1;
			}
		}
	
	}

	bool getIsNeedCombinedMatrixLoad() {return is_need_combined_matrix_load;}
	bool getIsNeedColorTextureLoad() {return is_need_color_texture_load;}

	void setMeshIndex(int mesh_index) {
		this->mesh_index = mesh_index;
	}
	int getMeshIndex() {return mesh_index;}
	
	void setSkeletonIndex(int skeleton_index) {
		this->skeleton_index = skeleton_index;
	}
	int getSkeletonIndex() {return skeleton_index;}

	void setInstanceIndex(int instance_index) {
		this->instance_index = instance_index;
	}
	int getInstanceIndex() {return instance_index;}

	void setChildInstance(MeshInstanced* child) {
		child_instances.push_back(child);
	}

	void setParentInstance(MeshInstanced* parent, int parent_bone_index, bool connect_without_matrix_local) {
		this->parent_instance = parent;
		this->parent_bone_index = parent_bone_index;
		this->connect_without_matrix_local = connect_without_matrix_local;
	}
	IMeshInstanced* getParentIInstance() {return (IMeshInstanced*)parent_instance;}
	MeshInstanced* getParentInstance() {return parent_instance;}
	int getParentBoneIndex() {return parent_bone_index;}

	void setBoneIndexInfo(unsigned short* bones_anime_first_index, unsigned short* bones_anime_last_index, float* bones_anime_first_weight) {
		for (int i=0; i< KTROBO_MESH_INSTANCED_BONE_MAX; i++) {
			this->bones_anime_first_index[i] = bones_anime_first_index[i];
			this->bones_anime_last_index[i] = bones_anime_last_index[i];
			this->bones_anime_first_weight[i] = bones_anime_first_weight[i];
		}

		setIsNeedCombinedMatrixLoad(true);
	}
	void getBoneIndexInfo(unsigned short* bafi, unsigned short* bali, float* bafw) {
		for (int i=0; i< KTROBO_MESH_INSTANCED_BONE_MAX; i++) {
			bafi[i] = bones_anime_first_index[i];
			bali[i] = bones_anime_last_index[i];
			bafw[i] = bones_anime_first_weight[i];
		}
	}
	void setRootBoneMatrixLocalKakeru(MYMATRIX* m) {
		this->rootbone_matrix_local_kakeru = *m;
	}
	MYMATRIX* getRootBoneMatrixLocalKakeru() {return &this->rootbone_matrix_local_kakeru;}

	void setWorld(MYMATRIX* world) {
		this->world = *world;
	}
	MYMATRIX* getWorld() {return &world;}
	
	void setColor(MYVECTOR4* colors) {
		for (int i = 0;i<KTROBO_MESH_INSTANCED_COLOR_MAX; i++) {
			this->colors[i] = colors[i];
		}

		setIsNeedColorTextureLoad(true);
	}
	MYVECTOR4* getColor(int index) {
		if (index >= 0 && index < KTROBO_MESH_INSTANCED_COLOR_MAX) {
			return &colors[index];
		}
		return &colors[0];
	}

	void setConnectWithoutMatrixLocal(bool m) {
		connect_without_matrix_local = m;
	}
	bool getConnectWithoutMatrixLocal() {return connect_without_matrix_local;}

	MeshInstanced(void);
	~MeshInstanced(void);
};

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

// デストラクタはmeshinstancedのみ呼ばれる
// Mesh, skeleton については外部でデストラクタを呼ぶ

#define KTROBO_MESH_INSTANCED_SHADER_FILENAME_MATRIX_LOCAL "resrc/shader/simplemesh_i_ml.fx"
#define KTROBO_MESH_INSTANCED_SHADER_FILENAME_MATRIX_BASIS "resrc/shader/simplemesh_i_mb.fx"
#define KTROBO_MESH_INSTANCED_SHADER_FILENAME_COMBINED_MATRIX "resrc/shader/simplemesh_i_cm.fx"
#define KTROBO_MESH_INSTANCED_SHADER_FILENAME_RENDER "resrc/shader/simplemesh_i_render.fx"
#define KTROBO_MESH_INSTANCED_SHADER_FILENAME_COMBINED_MATRIX_COMPUTE "resrc/shader/simplemesh_i_compute_cm.ps"
#define KTROBO_MESH_INSTANCED_SHADER_FILENAME_COLOR "resrc/shader/simplemesh_i_color.fx"
#define KTROBO_MESH_INSTANCED_SHADER_VS "VSFunc"
#define KTROBO_MESH_INSTANCED_SHADER_GS "GSFunc"
#define KTROBO_MESH_INSTANCED_SHADER_PS "PSFunc"
#define KTROBO_MESH_INSTANCED_MATRIX_LOCAL_TEXTURE_WIDTH_HEIGHT 512
#define KTROBO_MESH_INSTANCED_MATRIX_BASIS_TEXTURE_WIDTH_HEIGHT 4096
#define KTROBO_MESH_INSTANCED_MATRIX_COMBINED_TEXTURE_WIDTH_HEIGHT 512
#define KTROBO_MESH_INSTANCED_ANIME_MATRIX_BASIS_NUM_MAX 512
#define KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE 49152
#define KTROBO_MESH_INSTANCED_COMBINED_MATRIX_CALC_STRUCT_TEMPSIZE 64*6
#define KTROBO_MESH_INSTANCED_COMBINED_MATRIX_WATASU_STRUCT_TEMPSIZE 64*6
#define KTROBO_MESH_INSTANCED_COLOR_TEXTURE_STRUCT_TEMPSIZE KTROBO_MESH_INSTANCED_COLOR_MAX*6*8
#define KTROBO_MESH_INSTANCED_RENDER_INSTANCE_STRUCT_TEMPSIZE 64 
#define KTROBO_MESH_INSTANCED_COMBINED_MATRIX_INSTANCE_SIZE 512
#define KTROBO_MESH_INSTANCED_COLOR_TEXTURE_WIDTH_HEIGHT 512

struct ANIMETEXTURELOADSTRUCT {
	MYVECTOR3 pos;
//	unsigned char matrix_offset; // 0 ~ 15 matrix内の場所
//	unsigned char offset_nouse;
//	unsigned short skeleton_index; // skeletonのインデックス
//	unsigned short bone_index; // ボーンのインデックス
//	unsigned short anime_index; // アニメのインデックス 0 ~ 511
	float matrix_value;
};



#define KTROBO_COMBINED_MATRIX_CONTROL_VALUE_ROOT_BONE 0x01
#define KTROBO_COMBINED_MATRIX_CONTROL_VALUE_CONNECT_WITHOUT_MATRIX_LOCAL 0x02


struct COMBINEDMATRIXCALC_CBUF{
	unsigned int depth;
	unsigned int vertex_num;
	unsigned int omoteura;
	unsigned int matrix_offset;
};

struct COMBINEDMATRIXCALCSTRUCT {
	unsigned int depth;
	unsigned int instance_index;
	unsigned int skeleton_index;
	unsigned int parent_instance_index;
	unsigned int parent_skeleton_index;	
	unsigned int anime_first_index;
	unsigned int anime_last_index;
	unsigned int control_value;
	unsigned int bone_index;
	unsigned int parent_bone_index;
	float anime_first_weight;
	MYMATRIX matrix_kakeru;
};

struct COMBINEDMATRIXWATASUSTRUCT {
	unsigned int instance_index;
	unsigned int bone_index;
	unsigned int offset1;
	unsigned int offset2;
	MYMATRIX combined_matrix;
};


struct SHADERCOLORTEXTUREINPUTSTRUCT {
	unsigned int instance_id;
	unsigned int color_index;
	unsigned int vertex_index;
	unsigned int offset;
	MYVECTOR4 color;
};

struct RENDERINSTANCEINFOSTRUCT {
	unsigned int instance_id;
	MYMATRIX world;
};

struct MESHINSTANCED_CBUF2 {
		MYMATRIX view;
		MYMATRIX proj;
		MYVECTOR4 lightdir;
};

struct MESHINSTANCED_CBUF3 {
		unsigned int color_id;
		unsigned int offset1;
		unsigned int offset2;
		unsigned int offset3;
};

#define KTROBO_MESH_INSTANCED_BONE_DEPTH_NULL 0xFFFF


class MeshInstanceds{
public:
	void clearLoadState() {
		int sml_size = skeletons.size();
		for (int i=0;i<sml_size;i++) {
			skeleton_matrix_local_is_loaded[i] = false;
			skeleton_anime_is_loaded[i]= false;
		}
		int msize = mesh_instanceds.size();
		for (int m = 0; m < msize;m++) {
			mesh_instanceds[m]->setIsNeedColorTextureLoad(true);
			mesh_instanceds[m]->setIsNeedCombinedMatrixLoad(true);
		}
	}
private:
	vector<MeshInstanced*> mesh_instanceds;
	vector<Mesh*> meshs;
	vector<Mesh*> skeletons;
	vector<bool> skeleton_matrix_local_is_loaded;
	vector<bool> skeleton_anime_is_loaded;
	static ID3D11Buffer* matrix_local_load_vertexbuffer;// スケルトンのマトリックスローカルをテクスチャに保存するために使用するバッファ
	static ID3D11Buffer* anime_matrix_load_vertexbuffer;// スケルトンのアニメ情報をテクスチャに保存するために使用するバッファ
	static ID3D11Buffer* combined_matrix_calc_vertexbuffer;
	static ID3D11Buffer* combined_matrix_watasu_vertexbuffer;
	static ID3D11UnorderedAccessView* combined_matrix_calc_vertexbuffer_view;
	static ID3D11UnorderedAccessView* combined_matrix_watasu_vertexbuffer_view;
	static ID3D11Buffer* combined_matrix_watasu_offsetbuffer;
	static ID3D11Buffer* combined_matrix_watasu_indexbuffer;
	static COMBINEDMATRIXCALC_CBUF cbuf1;
	static ID3D11Buffer* cbuf1_buffer;
	static ID3D11Buffer* color_texture_vertexbuffer;
	static ID3D11SamplerState* combined_matrix_sampler_state;
	static ID3D11Buffer* render_instance_vertexbuffer;
public:
	MyTextureLoader::MY_TEXTURE_CLASS* matrix_local_texture;
	MyTextureLoader::MY_TEXTURE_CLASS* anime_matrix_basis_texture;
	MyTextureLoader::MY_TEXTURE_CLASS* combined_matrix_texture;
	MyTextureLoader::MY_TEXTURE_CLASS* color_texture;
private:
	
	static MESHINSTANCED_CBUF2 cbuf2;
	static ID3D11Buffer* cbuf2_buffer;

	
	static MESHINSTANCED_CBUF3 cbuf3;
	static ID3D11Buffer* cbuf3_buffer;
	void setCBuf3(Graphics* g, unsigned int color_id);

public:
	MeshInstanceds(Graphics* g, MyTextureLoader* tex_loader) {
		matrix_local_texture = tex_loader->makeClass(KTROBO_MESH_INSTANCED_MATRIX_LOCAL_TEXTURE_WIDTH_HEIGHT, KTROBO_MESH_INSTANCED_MATRIX_LOCAL_TEXTURE_WIDTH_HEIGHT);
		anime_matrix_basis_texture = tex_loader->makeClass(KTROBO_MESH_INSTANCED_MATRIX_BASIS_TEXTURE_WIDTH_HEIGHT, KTROBO_MESH_INSTANCED_MATRIX_BASIS_TEXTURE_WIDTH_HEIGHT);
		combined_matrix_texture = tex_loader->makeClass(KTROBO_MESH_INSTANCED_MATRIX_COMBINED_TEXTURE_WIDTH_HEIGHT, KTROBO_MESH_INSTANCED_MATRIX_COMBINED_TEXTURE_WIDTH_HEIGHT);
		color_texture = tex_loader->makeClass(KTROBO_MESH_INSTANCED_COLOR_TEXTURE_WIDTH_HEIGHT, KTROBO_MESH_INSTANCED_COLOR_TEXTURE_WIDTH_HEIGHT);
		//MyMatrixIdentity(view);
		//MyMatrixIdentity(proj);
	}

	~MeshInstanceds() {
		Release();
	}
	void Release() {
		vector<MeshInstanced*>::iterator it;
		it = mesh_instanceds.begin();
		while (it != mesh_instanceds.end()) {
			MeshInstanced* i = *it;
			delete i;
			i = 0;



			it = it + 1;
		}
		mesh_instanceds.clear();
	}

	static MYSHADERSTRUCT mss_for_matrix_local;
	static MYSHADERSTRUCT mss_for_matrix_basis;
	static MYSHADERSTRUCT mss_for_combined_matrix;
	static MYSHADERSTRUCT mss_for_render;
	static MYSHADERSTRUCT mss_for_color;


	static void loadShader(Graphics* g, MYSHADERSTRUCT* s, char* shader_filename,
		char* vs_func_name, char* gs_func_name, char* ps_func_name, 
		unsigned int ds_width, unsigned int ds_height,D3D11_INPUT_ELEMENT_DESC* layout, int numoflayout,
		bool blend_enable);

	static void loadShaderForMatrixLocal(Graphics* g);
	static void loadShaderForMatrixBasis(Graphics* g);
	static void loadShaderForCombinedMatrix(Graphics* g);
	static void loadShaderForRender(Graphics* g);
	static void loadShaderForColor(Graphics* g);
private:
	void _loadColorToTexture(Graphics* g, SHADERCOLORTEXTUREINPUTSTRUCT* st, int temp);
	void _loadAnimeMatrixBasisToTexture(Graphics* g, ANIMETEXTURELOADSTRUCT* st, int temp);
	void _loadMatrixLocalToTexture(Graphics* g, ANIMETEXTURELOADSTRUCT* st, int temp);
	void _calcCombinedMatrixToTexture(Graphics* g, COMBINEDMATRIXCALCSTRUCT* stt, int temp, unsigned int depth);
	void _render(Graphics* g, RENDERINSTANCEINFOSTRUCT* st, int temp, Mesh* render_mesh);
public:
	static void Init(Graphics* g);
	static void Del() {
		mss_for_matrix_local.Del();
		mss_for_matrix_basis.Del();
		mss_for_combined_matrix.Del();
		mss_for_render.Del();
		mss_for_color.Del();
		if (matrix_local_load_vertexbuffer) {
			matrix_local_load_vertexbuffer->Release();
			matrix_local_load_vertexbuffer = 0;
		}// スケルトンのマトリックスローカルをテクスチャに保存するために使用するバッファ
		if (anime_matrix_load_vertexbuffer) {
			anime_matrix_load_vertexbuffer->Release();
			anime_matrix_load_vertexbuffer = 0;
		}// スケルトンのアニメ情報をテクスチャに保存するために使用するバッファ
		if (combined_matrix_calc_vertexbuffer) {
			combined_matrix_calc_vertexbuffer->Release();
			combined_matrix_calc_vertexbuffer = 0;
		}
		if (combined_matrix_watasu_vertexbuffer) {
			combined_matrix_watasu_vertexbuffer->Release();
			combined_matrix_watasu_vertexbuffer = 0;
		}
		if (combined_matrix_calc_vertexbuffer_view) {
			combined_matrix_calc_vertexbuffer_view->Release();
			combined_matrix_calc_vertexbuffer_view = 0;
		}
		if (combined_matrix_watasu_vertexbuffer_view) {
			combined_matrix_watasu_vertexbuffer_view->Release();
			combined_matrix_watasu_vertexbuffer_view = 0;
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

		if (combined_matrix_watasu_indexbuffer) {
			combined_matrix_watasu_indexbuffer->Release();
			combined_matrix_watasu_indexbuffer = 0;
		}

		if (combined_matrix_watasu_offsetbuffer) {
			combined_matrix_watasu_offsetbuffer->Release();
			combined_matrix_watasu_offsetbuffer = 0;
		}

		if (color_texture_vertexbuffer) {
			color_texture_vertexbuffer->Release();
			color_texture_vertexbuffer = 0;
		}
		if (combined_matrix_sampler_state) {
			combined_matrix_sampler_state->Release();
			combined_matrix_sampler_state = 0;
		}
		if (render_instance_vertexbuffer) {
			render_instance_vertexbuffer->Release();
			render_instance_vertexbuffer = 0;
		}
	};

	//void setMesh(Mesh* m);
	void setSkeleton(Mesh* m);

	void setViewProj(Graphics* g, MYMATRIX* view, MYMATRIX* proj, MYVECTOR4* lightdir);

	void loadMatrixLocalToTexture(Graphics* g); 
	void loadAnimeMatrixBasisToTexture(Graphics* g);
	void calcCombinedMatrixToTexture(Graphics* g);
	void loadColorToTexture(Graphics* g);

	void render(Graphics* g);

	int getMeshIndexOrSet(Mesh* mesh) {
		int mesh_size = meshs.size();
		for (int i = 0;i<mesh_size;i++) {
			if (meshs[i] == mesh) {
				return i;
			}
		}
		meshs.push_back(mesh);
		return mesh_size;
	}
	int getSkeletonIndexOrSet(Mesh* skeleton) {
		int skeleton_size = skeletons.size();
		for (int i = 0; i < skeleton_size; i++) {
			if (skeletons[i] == skeleton) {
				return i;
			}
		}
		setSkeleton(skeleton);
		return skeleton_size;
	}
	int makeInstanced(COLLECTED Mesh* mesh, COLLECTED Mesh* skeleton,  COLLECTED MeshInstanced* parent_instance, int parent_bone_index, bool connect_without_matrix_local, YARITORI MYMATRIX* matrix_local_kakeru)
	{
		MeshInstanced* mm = new MeshInstanced();
		mm->setMeshIndex(this->getMeshIndexOrSet(mesh));
		mm->setSkeletonIndex(this->getSkeletonIndexOrSet(skeleton));
		MYVECTOR4 colors[KTROBO_MESH_INSTANCED_COLOR_MAX];
		memset(colors, 0, sizeof(colors));

		int msize = mesh->Materials.size();
		for (int h = 0; h < msize && h < KTROBO_MESH_INSTANCED_COLOR_MAX; h++) {
			colors[h] = mesh->Materials[h]->color;
		}
		mm->setColor(colors);

		if (iparent_instance) {
			int pinstance_index = iparent_instance->getInstanceIndex();
			MeshInstanced* parent_instance = mesh_instanceds[pinstance_index];
			mm->setParentInstance(parent_instance, parent_bone_index, connect_without_matrix_local);
			mm->setRootBoneMatrixLocalKakeru(matrix_local_kakeru);
			parent_instance->setChildInstance(mm);
		}

		//mm->setMeshIndex(mesh_instanceds.size());
		int instance_id = mesh_instanceds.size();
		mm->setInstanceIndex(instance_id);
		mesh_instanceds.push_back(mm);
		return instance_id;
	}

	int getDepth(MeshInstanced* instance, int bone_index) {

		int skeleton_index = instance->getSkeletonIndex();
		Mesh* m = skeletons[skeleton_index];
		if (m) {
			if (bone_index >= 0 && bone_index < m->Bones.size()) {
				MeshBone* bon = m->Bones[bone_index];
				int depth = bon->getDepth();
				int parent_depth = 0;
				if (instance->getParentInstance()) {
					parent_depth = this->getDepth(instance->getParentInstance(), instance->getParentBoneIndex());
					parent_depth += 1;
				}
				depth = parent_depth + depth;
				return depth;
			}
		}
		return KTROBO_MESH_INSTANCED_BONE_DEPTH_NULL;
	}


};
}

#endif