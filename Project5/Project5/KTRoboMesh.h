#ifndef KTROBOMESH_H
#define KTROBOMESH_H


#pragma once
#include "MyDefine.h"
#include <vector>
#include <string>
#include <map>
#include "MyTextureLoader.h"
#include "MyButukari3D.h"
#include "KTRoboGraphics.h"
#include "KTRoboGameError.h"
#include "tolua_glue/MyLuaGlueMakeCommon.h"
#include "MyTokenAnalyzer.h"

using namespace std;

namespace KTROBO {

#define KTROBO_MESH_BONE_NULL 0xFFFF
#define KTROBO_MESH_SHADER_FILENAME "resrc/shader/meshsimple.fx"

#define KTROBO_MESH_SHADER_VS "VSFunc"
#define KTROBO_MESH_SHADER_PS "PSFunc"
#define KTROBO_MESH_VERTEX_COUNT_MAX 0xFFFF
#define KTROBO_MESH_INDEX_COUNT_MAX 0xFFFF
#define KTROBO_MESH_BONE_MAX 32

#define MODEL_BLEND_COUNT 5
struct MESH_VERTEX{
	MYVECTOR3 pos;
	unsigned char Index[MODEL_BLEND_COUNT+3];// 4byte alignment のため
	float weight[MODEL_BLEND_COUNT];
	MYVECTOR3 normal;
	MYVECTOR2 UV;
};


class MeshAnime
{
public:
	int bone_index;
	MYMATRIX matrix_basis;
	int frame;
	MeshAnime(MeshAnime* an) {
		this->matrix_basis = an->matrix_basis;
		this->bone_index = an->bone_index;
		this->frame = an->frame;
	}

	MeshAnime(int bone_index, int frame, float* matrix_basis) {
		this->matrix_basis._11 = matrix_basis[0];
		this->matrix_basis._21 = matrix_basis[1];
		this->matrix_basis._31 = matrix_basis[2];
		this->matrix_basis._41 = matrix_basis[3];
		this->matrix_basis._12 = matrix_basis[4];
		this->matrix_basis._22 = matrix_basis[5];
		this->matrix_basis._32 = matrix_basis[6];
		this->matrix_basis._42 = matrix_basis[7];
		this->matrix_basis._13 = matrix_basis[8];
		this->matrix_basis._23 = matrix_basis[9];
		this->matrix_basis._33 = matrix_basis[10];
		this->matrix_basis._43 = matrix_basis[11];
		this->matrix_basis._14 = matrix_basis[12];
		this->matrix_basis._24 = matrix_basis[13];
		this->matrix_basis._34 = matrix_basis[14];
		this->matrix_basis._44 = matrix_basis[15];
	/*	
		this->matrix_basis._11 = matrix_basis[0];
		this->matrix_basis._12 = matrix_basis[1];
		this->matrix_basis._13 = matrix_basis[2];
		this->matrix_basis._14 = matrix_basis[3];
		this->matrix_basis._21 = matrix_basis[4];
		this->matrix_basis._22 = matrix_basis[5];
		this->matrix_basis._23 = matrix_basis[6];
		this->matrix_basis._24 = matrix_basis[7];
		this->matrix_basis._31 = matrix_basis[8];
		this->matrix_basis._32 = matrix_basis[9];
		this->matrix_basis._33 = matrix_basis[10];
		this->matrix_basis._34 = matrix_basis[11];
		this->matrix_basis._41 = matrix_basis[12];
		this->matrix_basis._42 = matrix_basis[13];
		this->matrix_basis._43 = matrix_basis[14];
		this->matrix_basis._44 = matrix_basis[15];
*/		

		this->bone_index = bone_index;
		this->frame = frame;
	}
};


class MeshBone
{
public:
	int bone_index;
	MYMATRIX matrix_local;
	MYMATRIX combined_matrix;
	MYMATRIX offset_matrix;
	char bone_name[64];
	MeshBone* parent_bone;
	int parent_bone_index;
	vector<int> child_bone_indexs;
	vector<MeshAnime*> animes;

	OBB houkatuobb;
	bool houkatuobbuse;
	MeshBone(MeshBone* b) {// 親関係はコピーされない
		this->bone_index = b->bone_index;
		memset(this->bone_name, 0, 64);
		strcpy_s(this->bone_name,64,b->bone_name);
		memset(&combined_matrix, 0, sizeof(MYMATRIX));
		memset(&offset_matrix, 0, sizeof(MYMATRIX));
		this->matrix_local = b->matrix_local;
		parent_bone = 0;
		parent_bone_index = b->parent_bone_index;
		child_bone_indexs = b->child_bone_indexs;
		vector<MeshAnime*>::iterator itt = b->animes.begin();
		while(itt != b->animes.end()) {
			MeshAnime* m = *itt;
			MeshAnime* mm = new MeshAnime(m);
			this->animes.push_back(mm);
			itt++;
		}
		houkatuobbuse = false;
	}

	MeshBone(int bone_index, float* matrix_local, const char* bone_name) {
		this->matrix_local._11 = matrix_local[0];
		this->matrix_local._21 = matrix_local[1];
		this->matrix_local._31 = matrix_local[2];
		this->matrix_local._41 = matrix_local[3];
		this->matrix_local._12 = matrix_local[4];
		this->matrix_local._22 = matrix_local[5];
		this->matrix_local._32 = matrix_local[6];
		this->matrix_local._42 = matrix_local[7];
		this->matrix_local._13 = matrix_local[8];
		this->matrix_local._23 = matrix_local[9];
		this->matrix_local._33 = matrix_local[10];
		this->matrix_local._43 = matrix_local[11];
		this->matrix_local._14 = matrix_local[12];
		this->matrix_local._24 = matrix_local[13];
		this->matrix_local._34 = matrix_local[14];
		this->matrix_local._44 = matrix_local[15];
		/*
		this->matrix_local._11 = matrix_local[0];
		this->matrix_local._12 = matrix_local[1];
		this->matrix_local._13 = matrix_local[2];
		this->matrix_local._14 = matrix_local[3];
		this->matrix_local._21 = matrix_local[4];
		this->matrix_local._22 = matrix_local[5];
		this->matrix_local._23 = matrix_local[6];
		this->matrix_local._24 = matrix_local[7];
		this->matrix_local._31 = matrix_local[8];
		this->matrix_local._32 = matrix_local[9];
		this->matrix_local._33 = matrix_local[10];
		this->matrix_local._34 = matrix_local[11];
		this->matrix_local._41 = matrix_local[12];
		this->matrix_local._42 = matrix_local[13];
		this->matrix_local._43 = matrix_local[14];
		this->matrix_local._44 = matrix_local[15];*/
		memset(this->bone_name, 0, 64);
		strcpy_s(this->bone_name,64,bone_name);
		this->bone_index = bone_index;
		
		memset(&combined_matrix, 0, sizeof(MYMATRIX));
		memset(&offset_matrix, 0, sizeof(MYMATRIX));
		
		parent_bone = 0;
		parent_bone_index = KTROBO_MESH_BONE_NULL;

		houkatuobbuse = false;
	}

	~MeshBone() {
		vector<MeshAnime*>::iterator it;
		it = animes.begin();
		while(it != animes.end()) {
			MeshAnime* a = *it;
			delete a;
			it = it +  1;
		}
		animes.clear();
	}

	void setParent(vector<MeshBone*>* bones, int parent_bone_index) {

		int size = bones->size();
		if (size > parent_bone_index && 0 <= parent_bone_index && parent_bone_index != KTROBO_MESH_BONE_NULL) {
			this->parent_bone = (*bones)[parent_bone_index];
		}
		if ((size > parent_bone_index && 0 <= parent_bone_index) || (parent_bone_index == KTROBO_MESH_BONE_NULL)) {
			this->parent_bone_index = parent_bone_index;
		}else {
			throw new GameError(KTROBO::FATAL_ERROR, "parent bone index okasii");
		}
	}
	void setChild(int child_index) {
		child_bone_indexs.push_back(child_index);
	}

	void setAnime(MeshAnime* anim) {
		if (anim->bone_index != bone_index) {
			throw new GameError(KTROBO::WARNING, "dont fit bone index");
		}
		animes.push_back(anim);
	}

	int getDepth() {
		int dep = 0;
		MeshBone* bb = this;
		while(bb) {
			bb = bb->parent_bone;
			dep++;
		}
		return dep-1;
	}
};

class MeshSubset
{
public:
	unsigned int MaterialIndex;
	unsigned int FaceIndex;
	unsigned int FaceCount;
};

class MeshMaterial
{
public:
	MYVECTOR4 color;
	MyTextureLoader::MY_TEXTURE_CLASS* texture;
};

struct MeshCBuf1 {
	
	MYMATRIX view;
	MYMATRIX proj;
};


struct MeshCBuf2 {
	MYMATRIX world;
	MYMATRIX bone_combined_matrixs[KTROBO_MESH_BONE_MAX];
};

struct MeshCBuf3 {
	
	MYVECTOR4 color;
	MYVECTOR4 lightdir;
	MYVECTOR4 viewdir;
};

interface IWrappedMesh {
public:
	TO_LUA virtual void readAnime(char* filename)=0;
	TO_LUA virtual void readMesh(char* filename)=0;
};


class Mesh
{
public:
	static void Init(Graphics* g);
	static void Del();
	void Release();
	Mesh(void);
	virtual ~Mesh(void);

	void animate(float frame, bool calculate_offsetmatrix);
	void readAnime(char* filename);
	void readMesh(Graphics* g, char* filename, MyTextureLoader* tex_loader);
	void readMeshWithoutVertex(Graphics* g, char* filename, MyTextureLoader* tex_loader);
	void readMeshOnlyForSaveVertexIndex(Graphics* g, char* filename, MyTextureLoader* tex_loader, MESH_VERTEX** vertexs, UINT** indexs);
	void draw(Graphics* g, MYMATRIX* world, MYMATRIX* view, MYMATRIX* proj);
	void drawWithObbs(Graphics* g, MYMATRIX* world, MYMATRIX* view, MYMATRIX* proj);

	bool RootBone_connect_without_material_local;
public:
	unsigned int VertexCount;
	unsigned int FaceCount;
	ID3D11Buffer* p_vertexbuffer;
	ID3D11Buffer* p_indexbuffer;
private:
	void readBoneInfo(MyTokenAnalyzer* a, bool is_read_weight, MESH_VERTEX* vertexs, UINT* indexs);
	void calcHoukatuOBB(MESH_VERTEX* vertexs, UINT* indexs);
	static MeshCBuf1 cbuf1;
	static MeshCBuf2 cbuf2;
	static MeshCBuf3 cbuf3;
	static ID3D11Buffer* cbuf1_buffer;
	static ID3D11Buffer* cbuf2_buffer;
	static ID3D11Buffer* cbuf3_buffer;

private:
	static void updateCBuf1(Graphics* g, MYMATRIX* view, MYMATRIX* proj);
	static void updateCBuf2(Graphics* g, MYMATRIX* world, MYMATRIX* bone_combined_matrixs);
	static void updateCBuf3(Graphics* g, MYVECTOR4* color, MYVECTOR4* lightdir, MYVECTOR4* viewdir);
public:
	MeshBone* RootBone;
	int bone_max_depth;
	char filename[256];

	OBB houkatuobb;
	MYMATRIX rootbone_matrix_local_kakeru;
	vector<MeshSubset*> Subsets;
	vector<MeshMaterial*> Materials;
	vector<MeshBone*> Bones;
	map<string, int> BoneIndexes;
	Mesh* clone();
private:
	bool is_cloned;
	
	void animateBoneFrame(MeshBone* root_bone);


	static ID3D11VertexShader* vs;
	static ID3D11PixelShader* ps;
	static ID3D11InputLayout* p_vertexlayout;
	static ID3D11BlendState* p_blendstate;

	static ID3D11RasterizerState* p_rasterstate;
	static ID3D11Texture2D* pDepthStencil;
public:
	void Mesh::getOffsetMatrixToGetMinMaxAndWeightIndex(MeshBone* bone, float frame, unsigned short* ans_minmax_index, unsigned short* ans_maxmin_index, float* weight);
	void calculateOffsetMatrix(MeshBone* root_bone,float frame);
	void calculateOffsetMatrixToGetMinMaxAndWeight(MeshBone* bone, float frame, unsigned short* ans_minmax, unsigned short* ans_maxmin, float* weight);
	static ID3D11DepthStencilView* pDepthStencilView;
	static ID3D11SamplerState* p_sampler;
};


class WrappedMesh : public Mesh, public IWrappedMesh{
private:
	Graphics* g;
	MyTextureLoader* loader;
public:
	WrappedMesh(Graphics* g, MyTextureLoader* loader) {
		Mesh();
		this->g = g;
		this->loader = loader;
	}
	~WrappedMesh() {
	//	Mesh::~Mesh();
		Release();
	}
    void readMesh(char* filename) {
		Mesh::readMesh(g, filename, loader);
	}
	void readAnime(char* filename) {
		Mesh::readAnime(filename);
	}
};

// Meshs に関しては　他のスレッドでMeshInstanceが参照されるので　MeshInstance でLoadableを実装しておけばMeshsに関しても守られる？
class WrappedMeshs {
private:
	vector<WrappedMesh*> wrapped_meshs;
	Graphics* g;
	MyTextureLoader* loader;
public:
	WrappedMeshs(Graphics* g, MyTextureLoader* loader) {
		this->g = g;
		this->loader = loader;
	}
	~WrappedMeshs() {
		vector<WrappedMesh*>::iterator it = wrapped_meshs.begin();
		while(it != wrapped_meshs.end()) {
			WrappedMesh* wrapped = *it;
			delete wrapped;
			it = it + 1;
		}
		wrapped_meshs.clear();
	}
	int makeInstanceID() {
		WrappedMesh* m = new WrappedMesh(g, loader);
		int size = wrapped_meshs.size();
		wrapped_meshs.push_back(m);
		return size;
	}
	WrappedMesh* getInstance(int index) {
		return wrapped_meshs[index];
	}
	IWrappedMesh* getInterface(int index) {
		return wrapped_meshs[index];
	}

};

void WAgetScaleFromMatrix(MYMATRIX* mat, MYVECTOR3* p);
void WAsetScaleToMatrix(MYMATRIX* mat, MYVECTOR3* v);

}

#endif