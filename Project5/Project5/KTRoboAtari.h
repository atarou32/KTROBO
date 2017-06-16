#ifndef KTROBOATARI_H
#define KTROBOATARI_H

#pragma once
#include "KTRoboMesh.h"
#include "MyDefine.h"
#include "KTRoboMeshInstanced.h"


namespace KTROBO {
#define KTROBO_MAX_TIKEI_HITOTU_INDEX 2048 // �ЂƂ̒n�`��face�̍ő吔 ����������������Ƃ��̓V�F�[�_�����������邱��
class UMesh {
public:
	Mesh* mesh;
private:
	static int id_max;
	int UMESH_ID;
	bool RootBone_connect_without_material_local;
	MYMATRIX RootBone_matrix_local_kakeru;
	MeshBone* rootbone_parent;
	int rootbone_parent_index;
	float anime_frame;
	bool is_use_vertexs_and_indexs;
	
public:
	int getUMESHID() {return UMESH_ID;}

	MESH_VERTEX* vertexs;
	UINT* indexs;
	OBB bone_obbs[KTROBO_MESH_BONE_MAX]; // �e�ʒu�A�p���A�A�j���̌v�Z��̂�����
	int bone_obbs_idx[KTROBO_MESH_BONE_MAX];
	bool is_bone_obbs_use[KTROBO_MESH_BONE_MAX];
public:
	void setInfo();
	void setAnimeFrame(float frame) {
		anime_frame = frame;
	}
	void Animate(bool calculateOffsetMatrix) {
		if (mesh) {
			mesh->animate(anime_frame, calculateOffsetMatrix);
		}
	}

	UMesh() {
		mesh = 0;
		RootBone_connect_without_material_local = false;
		MyMatrixIdentity(RootBone_matrix_local_kakeru);
		rootbone_parent = 0;
		rootbone_parent_index = KTROBO_MESH_BONE_NULL;
		is_use_vertexs_and_indexs = false;
		vertexs = 0;
		indexs = 0;
		anime_frame = 0;
		for (int i=0;i <KTROBO_MESH_BONE_MAX;i++) {
			is_bone_obbs_use[i] = false;
			bone_obbs_idx[i] = 0;
		}
		UMESH_ID = id_max;
		id_max++;
	}

	UMesh(Graphics* g, char* filename, MyTextureLoader* tex_loader, Mesh* mesh, bool RootBone_connect_without_material_local, MYMATRIX* RootBone_matrix_local_kakeru, MeshBone* parent_bone, int rootbone_parent_index, bool is_use_vertexs_and_indexs) {
		this->mesh = mesh;
		vertexs = 0;
		indexs = 0;
		this->RootBone_connect_without_material_local = RootBone_connect_without_material_local;
		this->RootBone_matrix_local_kakeru = *RootBone_matrix_local_kakeru;
		this->rootbone_parent = parent_bone;
		this->rootbone_parent_index = rootbone_parent_index;
		this->is_use_vertexs_and_indexs = is_use_vertexs_and_indexs;
		if (is_use_vertexs_and_indexs) {
			if (mesh) {
				// vertexs �Ɓ@indexs ���擾����
				mesh->readMeshOnlyForSaveVertexIndex(g, filename,tex_loader, &vertexs, &indexs);  
			}
		}
		for (int i=0;i<KTROBO_MESH_BONE_MAX;i++) {
			is_bone_obbs_use[i] = false;
			bone_obbs_idx[i] = 0;
		}
		UMESH_ID = id_max;
		id_max++;

	}

	~UMesh() {
		
		if (vertexs) {
			delete[] vertexs;
		}
		if (indexs) {
			delete[] indexs;
		}
	}

};

class UMeshUnit {
private:
	
	bool is_updated;
	// �p���ƈʒu�̏��
public:
	float x;
	float y;
	float z;
	float rotx; // zxy �̏�
	float roty;
	float rotz;
	float scalex;
	float scaley;
	float scalez;
	
	

	
public:
	MYVECTOR3 v;
	MYMATRIX world;
	MYVECTOR3 jyusin;// ����̏d�S(xyz rotx roty rotz ����) �����蔻��Ɏg�p����
	float r;// ����̂�
	float dt; // �o�ߎ���
	vector<UMesh*> meshs;
	void setUMesh(UMesh* mm) {
		meshs.push_back(mm);
	}

public:
	UMeshUnit() {
		x = 0;
		y = 0;
		z = 0;
		rotx = 0;
		roty = 0;
		rotz = 0;
		v.float3.x = 0;
		v.float3.y = 0;
		v.float3.z = 0;
		jyusin.float3.x = 0;
		jyusin.float3.y = 0;
		jyusin.float3.z = 0;
		scalex = 1;
		scaley = 1;
		scalez = 1;
		r = 1;
		MyMatrixIdentity(world);
		is_updated = true;
		dt = 0;
	}
	void setXYZ(float x, float y, float z);
	void setROTXYZ(float rotx, float roty, float rotz);
	void setSCALEXYZ(float scalex,float scaley,float scalez) {
		this->scalex = scalex;
		this->scaley = scaley;
		this->scalez = scalez;
		is_updated = true;
	}
	void setDT(float d) {
		dt = d;
	}

	void setV(MYVECTOR3* v);
	void calcAnimeFrame(int meshnum, float* frames, bool* calculateoffsetmatrixs);
	void calcJyusinAndR();

	~UMeshUnit() {
		vector<UMesh*>::iterator it = meshs.begin();
		// UMesh ���d�����ēo�^���邱�Ƃ̂Ȃ��悤�ɂ���

		while (it != meshs.end()) {
			UMesh* em = *it;
			delete em;
			em = 0;
			it++;
		}
		meshs.clear();
	}

	void draw(Graphics* g,  MYMATRIX* view, MYMATRIX* proj, int meshnum, float* frames, bool* calculateoffsetmatrixs,
		bool is_draw_mesh, bool is_calc_animeframe, bool is_draw_jyusinR, bool is_draw_boneobb);

};

class AtariUnit {
public:
	enum AtariType {
		ATARI_TIKEI=0, // �n�`�͂��ׂĂ̖ʂƓ����蔻����s��
		ATARI_OBJECT=1, // �I�u�W�F�N�g�͕������(�{�[������)�Ɠ����蔻����s��
		ATARI_CHARA=2, // �L�����N�^�[�͕������(�{�[�����Ɓj�Ɠ����蔻����s��
		ATARI_WAZA=3 // �L�����N�^�[�̍U���̋Z�̔���
	};

	AtariType type;
	// �����蔻��̎d�g��
	// �n�`�Ƃ��ēo�^���ꂽ���b�V�����m�͓����蔻����s��Ȃ�
	// �n�`�ƃI�u�W�F�N�g�@�I�u�W�F�N�g���m�@�n�`�ƃL�����N�^�[�@�I�u�W�F�N�g�ƃL�����N�^�[�@�L�����N�^�[���m
	// �Z�ƃL�����N�^�[�@�Z�ƃI�u�W�F�N�g�@�Z�ƒn�`�i�e�̏ꍇ����������s��) 
	// �͍s��

	int atariidx; // �V�F�[�_�ɓn��idx
	UMeshUnit* umesh_unit; // ���b�V���������郆�j�b�g
	UMesh* umesh; // ���b�V��
	AtariUnit() {
		type = ATARI_TIKEI;
		atariidx = 0;
		umesh_unit = 0;
		umesh = 0;
	}
};



#define KTROBO_MAX_TIKEI_VERTEXS_COUNT 65535
#define KTROBO_MAX_TIKEI_INDEXS_COUNT 65535*3
#define KTROBO_MAX_ATARI_HANTEI_UNIT_NUM 4048

struct AtariUnitInfo{
	int atari_idx;
	int vertexs_place;
	int indexs_place;
	int vertex_count;
	int index_count;
	MYMATRIX world;
	MYVECTOR3 jyusin;
	MYVECTOR3 v;
	float r;
	float dt;
};

struct AtariUnitVertexs {
	MYVECTOR4 pos;
	MYVECTOR4 normal;
};
struct AtariUnitIndexs {
	unsigned int index;
};

struct AtariUnitKumi {
	int atari_idx;
	int atari_idx2;
};

struct AtariUnitAns {
	int atari_idx;
	int obbidx;
	int atari_idx2;
	int obbidx2;
	MYVECTOR3 kouten_jyusin;
	MYVECTOR3 kouten_housen;
	int is_use;
};

struct AtariUnitAnsKWSK {

	AtariUnit::AtariType aite_type;
	AtariUnitAns* ans;
	UMeshUnit* aite;
	UMesh* aite_umesh;
};

// ����OBB�̓A�j���[�V�����ƈʒu�p�����K�p���ꂽOBB
// ���̓K�p��GPU�ł͂Ȃ�CPU�ōs���Ă���

struct AtariUnitOBB {
	MYVECTOR3 c; // ���S�̍��W
	MYVECTOR3 u[3];
	MYVECTOR3 e; // ���S�_����ʂ܂ł̒���
};

struct AtariUnitTikeiToSoreigai {
	int atariidx;
	int obbidx;
	int atariidx2;
	int offset;
};

struct AtariUnitTikeiIgaiDousi {
	int atariidx;
	int obbidx;
	int atariidx2;
	int obbidx2;
};

// ���������ǂꂭ�炢�m�ۂ��Ă��邩�z��̐�����ۑ����Ă���
struct AtariHanteiTempCount {
	int vertexs_count;
	int indexs_count;
	int auinfo_count;
	int obbs_count;
	int kumi_count;
	int ans_count;
	int igaidousi_count;
	int soreigai_count;
};


#define KTROBO_ATARI_SHADER_COMPUTE "resrc/shader/simple_atari_compute.cso"
#define KTROBO_ATARI_SHADER_COMPUTE2 "resrc/shader/simple_atari_compute_kuwasiku.cso"

class AtariHantei {
private:
	static const int kakuho_max = 7;
	static const int kakuho_counts[]; 
	static int getKakuhoCountsFromCount(int count);
	bool is_updated;
public:
	AtariUnit units[KTROBO_MAX_ATARI_HANTEI_UNIT_NUM];
private:
	AtariHanteiTempCount max_count;
	AtariHanteiTempCount temp_count;
	int au_count;
	int au_tikei_count;
	int au_object_count;
	int au_chara_count;
	int au_waza_count;
	int au_obbs_count;
	int atatta_count;
public:
	bool getIsUpdated() {return is_updated;}

	vector<UMeshUnit*> umesh_units;
private:
	vector<AtariUnit::AtariType> umesh_unit_types;

	AtariUnitVertexs* max_tikei_vertexs;
	AtariUnitIndexs* max_tikei_indexs;
	AtariUnitInfo* au_info;
	AtariUnitKumi* kumi;
	AtariUnitAns* ans;
	AtariUnitOBB* obbs;
	AtariUnitTikeiIgaiDousi* autid;
	AtariUnitTikeiToSoreigai* autts;
public:

	void setUMeshUnit(UMeshUnit* u, AtariUnit::AtariType type) {
		umesh_units.push_back(u);
		umesh_unit_types.push_back(type);
	}

	void maecalcdayo(Graphics* g);
	void calcKumi(Graphics* g);
	void calcKumiKuwasiku(Graphics* g);
	void calcAuInfo(Graphics* g, bool calc_vertex_and_index);
	void calcObb(Graphics* g);
	void drawKekka(Graphics* g, MYMATRIX* view, MYMATRIX* proj);
	int getAns(AtariUnitAnsKWSK* out_ans, UMeshUnit* oya,UMesh* oya2, int out_ans_num);
private:
	static MYSHADERSTRUCT mss;
	static MYSHADERSTRUCT mss2;
	static ID3D11Buffer* buffer_count;

	ID3D11Buffer* buffer_vertexs;
	ID3D11Buffer* buffer_indexs;
	ID3D11Buffer* buffer_au_info;
	ID3D11Buffer* buffer_obbs;
	ID3D11Buffer* buffer_kumi;
	ID3D11Buffer* buffer_ans;
	ID3D11Buffer* buffer_ans2;
	ID3D11Buffer* buffer_ans_copy;
	ID3D11Buffer* buffer_ans2_copy;
	ID3D11Buffer* buffer_autid;
	ID3D11Buffer* buffer_autts;

	ID3D11ShaderResourceView* buffer_vertexs_view;
	ID3D11ShaderResourceView* buffer_indexs_view;
	ID3D11ShaderResourceView* buffer_au_info_view;
	ID3D11ShaderResourceView* buffer_obbs_view;
	ID3D11ShaderResourceView* buffer_kumi_view;
	ID3D11ShaderResourceView* buffer_autid_view;
	ID3D11ShaderResourceView* buffer_autts_view;

	ID3D11UnorderedAccessView* buffer_ans_view;
	ID3D11UnorderedAccessView* buffer_ans2_view;
	void releaseBufferAndView();

public:
	static void compileShader(Graphics* g);
	static void del() {
		mss.Del();
		mss2.Del();
		if (buffer_count) {
			buffer_count->Release();
			buffer_count = 0;
		}
	}

	static HRESULT createStructuredBuffer(Graphics* g, UINT element_size, UINT count, void* pInitData, ID3D11Buffer** ppBufferOut);
	static HRESULT createShaderResourceView(Graphics* g, ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSrvOut);
	static HRESULT createBufferUnorderedAccessView(Graphics* g, ID3D11Buffer* pBuffer, ID3D11UnorderedAccessView** ppUavOut);
	static HRESULT createBufferForCopy(Graphics* g, ID3D11Buffer* pBuffer, ID3D11Buffer** ppBufOut);
	HRESULT copyKekkaToBufferForCopy(Graphics* g,bool isans1);
	void clearKekkaOfBuffer(Graphics* g);

	// �v�Z����̂͂��������ɍs��
	void runComputeShader(Graphics* g);
	void runComputeShaderKuwasiku(Graphics* g);

	~AtariHantei() {
		// UMeshUnit�͊O�ŏ���
		/*
		vector<UMeshUnit*>::iterator it = umesh_units.begin();
		while(it != umesh_units.end()) {
			UMeshUnit* uu = *it;
			delete uu;
			uu = 0;
			it++;
		}
		*/
		umesh_unit_types.clear();
		umesh_units.clear();

		if (max_tikei_vertexs) {
			delete[] max_tikei_vertexs;
			max_tikei_vertexs = 0;
		}

		if (max_tikei_indexs) {
			delete[] max_tikei_indexs;
			max_tikei_indexs = 0;
		}

		if (au_info) {
			delete[] au_info;
			au_info = 0;
		}
		if (kumi) {
			delete[] kumi;
			kumi = 0;
		}

		if (ans) {
			delete[] ans;
			ans = 0;
		}

		if (obbs) {
			delete[] obbs;
			obbs = 0;
		}

		if (autid) {
			delete[] autid;
			autid =0;
		}

		if (autts) {
			delete[] autts;
			autts = 0;
		}
		releaseBufferAndView();
	}

	AtariHantei() {
		max_count.ans_count = 0;//kakuho_counts[0];
		max_count.auinfo_count = 0;//kakuho_counts[0];
		max_count.igaidousi_count =0;// kakuho_counts[0];
		max_count.indexs_count = 0;//kakuho_counts[0];
		max_count.kumi_count = 0;//kakuho_counts[0];
		max_count.obbs_count = 0;//kakuho_counts[0];
		max_count.soreigai_count =0;// kakuho_counts[0];
		max_count.vertexs_count = 0;//kakuho_counts[0];

		temp_count.ans_count = 0;
		temp_count.auinfo_count = 0;
		temp_count.igaidousi_count = 0;
		temp_count.indexs_count = 0;
		temp_count.kumi_count = 0;
		temp_count.obbs_count = 0;
		temp_count.soreigai_count = 0;
		temp_count.vertexs_count = 0;

		max_tikei_vertexs = new AtariUnitVertexs[kakuho_counts[0]];
		max_tikei_indexs = new AtariUnitIndexs[kakuho_counts[0]];
		au_info = new AtariUnitInfo[kakuho_counts[0]];
		autid = new AtariUnitTikeiIgaiDousi[kakuho_counts[0]];
		autts = new AtariUnitTikeiToSoreigai[kakuho_counts[0]];
		kumi = new AtariUnitKumi[kakuho_counts[0]];
		ans = new AtariUnitAns[kakuho_counts[0]];
		obbs = new AtariUnitOBB[kakuho_counts[0]];
		au_count = 0;
		au_tikei_count = 0;
		au_chara_count = 0;
		au_waza_count = 0;
		au_object_count = 0;
		au_obbs_count=0;

		is_updated = false;
		/*
		ID3D11Buffer* buffer_vertexs;
		ID3D11Buffer* buffer_indexs;
		ID3D11Buffer* buffer_au_info;
		ID3D11Buffer* buffer_obbs;
		ID3D11Buffer* buffer_kumi;
		ID3D11Buffer* buffer_ans;
		ID3D11Buffer* buffer_ans2;
		ID3D11Buffer* buffer_autid;
		ID3D11Buffer* buffer_autts;
		*/

		buffer_vertexs = 0;
		buffer_indexs = 0;
		buffer_au_info = 0;
		buffer_obbs = 0;
		buffer_kumi = 0;
		buffer_ans = 0;
		buffer_ans2 = 0;
		buffer_autid = 0;
		buffer_autts = 0;

		buffer_vertexs_view = 0;
		buffer_indexs_view = 0;
		buffer_au_info_view = 0;
		buffer_obbs_view = 0;
		buffer_kumi_view = 0;
		buffer_ans_view = 0;
		buffer_ans2_view = 0;
		buffer_autid_view = 0;
		buffer_autts_view = 0;
		buffer_ans_copy = 0;
		buffer_ans2_copy  =0;
		atatta_count = 0;
	}
};


// ���l���Ă��邱�Ƃ͒n�`�̒��_�����ǂ��Ɏ������邩�Ƃ�������
// �n�`�̒��ɂ��������b�V���𕡐��o�^����ꍇ�͑��݂��Ă���

// �n�`�ƒn�`�͓����蔻����s��Ȃ�
// �n�`�ȊO�̂��̓��m�̓����蔻���OBB���m�ł�GPU���g���čs��
// �n�`�ƒn�`�ȊO�̓����蔻���vertexindex����OBB����GPU�ɑ����čs��
// �n�`���͕�����UMesh��vertex���m����������vertexs�Ƃ��� UMesh��indexs���m����������indexs�Ƃ���
// UmeshUnit �̏��i�ʒu�p��)���R���s���[�g�V�F�[�_�ɓn��
// OBB�����R���s���[�g�V�F�[�_�ɓn��
// �����蔻��̒i���
// 1. �S�Ă�UMeshUnit�̑g�ɑ΂��ďd�S�Ƃ����g���ē����蔻����s��
// 2. ��������UMeshUnit�̑g�Ɋւ���
// 2-a. �n�`���m�̏ꍇ�@��������
// 2-b. �n�`�ƒn�`�ȊO�̏ꍇ
//      vertexs �Ɓ@indexs �̏��(�n�`�Ȃ̂œ����Ȃ��̂ň��) �Ɓ@OBB�i�����������đ傫������) �œ����蔻����s��
// 2-c. �n�`�ȊO���m�̏ꍇ
//		OBB (v���������đ傫������)���m�œ����蔻����s��
// vertexs �Ɓ@indexs �̏�񂨂��UMeshUnit�̏����ǂ��i�[���邩
// 1.vertexs �̔z��
// 2.indexs �̔z��
// 3.AtariUnit�̏��, UMesh, UMeshUnit
/*
	int UMeshID, int UMeshUnitIDX, int atariidx, int vertexs_place, int indexs_place , int vertex_count, int index_count,
	MYMATRIX world, MYVECTOR3 v, MYVECTOR3 jyusin, float r
*/
// 4. �ǂ̓����胆�j�b�g���m�̔�����s�����̏��
// 5. ���ʂ̏�� atariidx umeshid umeshunitidx �̑g
// 5�̌��ʂ��g���Ă���ɍi�荞��ł���
// OBB�̏������͂��Ȃ���΂Ȃ�Ȃ�
// 1. OBB�̔z��œ��͂���
// 2. OBBUnit�݂����Ȃ��̂������atariidx�̌��ʏ�����������
// obbidx, atariidx, obbidx2,atariidx2�̓��͏��łQ�|���̔�����s��
// 
// 3. ���ʂ̏��@atariidx �̑g�ƌ�_�H

// 1.OBBUnit2�݂����Ȃ��̂������atariidx�̌��ʏ�����������
// obbidx atariidx atariidx2, vertexs_place,indexs_place, vertex_count, index_count �̓��͏���2-b�̔�����s��
// 2-b, 2-c�̏o�͂Ƃ��Ă�atariidx �̑g���o�͂����@�����CPU���œǂݍ���ŏ����Ɏg��
// 2. OBB�̔z��
// 3.vertexs�̔z��
// 4.indexs�̔z��
// 5. atariidx�̑g�ƒn�`�ʂ̏d�S�Ɩ@���H

// ���́iCPU���j�̃f�[�^���������̂�
// 1.��̓R���s���[�g�V�F�[�_�ɓn����`���ɂ���
// 2.�V�F�[�_������
// 3.�V�F�[�_����������

}

#endif