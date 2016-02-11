#ifndef KTROBOANIMATIONBUILDER_H
#define KTROBOANIMATIONBUILDER_H

#pragma once
#include "KTRoboMesh.h"
#include "KTRoboScene.h"
#include "KTRoboInput.h"
namespace KTROBO {

interface IAnimationBuilder {
public:
	TO_LUA virtual int createAnimationBuilderImpl(char* hon_filepath)=0;
	TO_LUA virtual void setOnajiMesh(int impl_id, char* onaji_filepath)=0;
	TO_LUA virtual void setKoMesh(int impl_id, char* ko_filepath, char* oya_filepath, char* parent_bone_name, bool is_connect_without_material_local, YARITORI MYMATRIX* kakeru)=0;
	TO_LUA virtual int getHonMeshBoneNum(int impl_id)=0;
	TO_LUA virtual char* getHonMeshBoneName(int impl_id, int bone_index)=0;
	TO_LUA virtual void setHonMeshBoneRotX(int impl_id, int bone_index, float rotx)=0;
	TO_LUA virtual void setHonMeshBoneRotY(int impl_id, int bone_index, float roty)=0;
	TO_LUA virtual void setHonMeshBoneRotZ(int impl_id, int bone_index, float rotz)=0;
	TO_LUA virtual void toggleHonMeshBoneRotXIsChange(int impl_id, int bone_index)=0; // falseにすると回転が0になりIKでも回転しなくなる
	TO_LUA virtual void toggleHonMeshBoneRotYIsChange(int impl_id, int bone_index)=0;
	TO_LUA virtual void toggleHonMeshBoneRotZIsChange(int impl_id, int bone_index)=0;
	TO_LUA virtual void setHonMeshBoneTransX(int impl_id, int bone_index, float dx)=0;
	TO_LUA virtual void setHonMeshBoneTransY(int impl_id, int bone_index, float dy)=0;
	TO_LUA virtual void setHonMeshBoneTransZ(int impl_id, int bone_index, float dz)=0;
	TO_LUA virtual void setAnimePoseFrame(int impl_id, int frame)=0;// 現在のとっている姿勢を指定したアニメフレームとして保存する
	TO_LUA virtual int createFrameExe(int impl_id, char* frameexe_name, bool is_loop)=0;
	TO_LUA virtual void setFrameToExe(int impl_id, int frameexe_id, int pose_id,  int frame, float time)=0;
	TO_LUA virtual void changeFrameExe(int impl_id, int frameexe_id, int frame_id, int frame, float time)=0;
	TO_LUA virtual bool saveNowToFile(char* filename)=0;
	TO_LUA virtual bool loadFromFile(char* filename)=0;
	TO_LUA virtual bool forceLoadFromFile(char* filename)=0;
	TO_LUA virtual bool saveAnimeAndFrameToFile(int impl_id, char* filename)=0;
	TO_LUA virtual bool force_saveNowToFile(char* filename)=0;
	TO_LUA virtual bool force_saveAnimeAndFrameToFile(int impl_id, char* filename)=0;
	TO_LUA virtual void deleteAll()=0;
	TO_LUA virtual void setNowIMPLIndex(int index)=0;
	TO_LUA virtual int getNowIMPLIndex()=0;
	TO_LUA virtual int getNowBoneIndex()=0;
};

class AnimationMeshKakera {
public:
	vector<float> mesh_bone_rotx;
	vector<float> mesh_bone_roty;
	vector<float> mesh_bone_rotz;
	vector<float> mesh_bone_transx;
	vector<float> mesh_bone_transy;
	vector<float> mesh_bone_transz;
	vector<bool> mesh_bone_isrotx;
	vector<bool> mesh_bone_isroty;
	vector<bool> mesh_bone_isrotz;

	string mesh_filepathname;
	int frame;
	map<string,int> mesh_bone_name_index;

	vector<int> mesh_bone_default_anime_frame;

	vector<MYMATRIX> mesh_offset_matrix;
public:
	void setOffsetMatrixToMesh(Mesh* mesh); // 現在の姿勢を計算してmesh_offset_matrixに入れ込む
	void copy(AnimationMeshKakera* kakera_moto);// コピー元からコピーする
	void write(char* filename,int impl_id);
	void clear() {
		mesh_bone_rotx.clear();
		mesh_bone_roty.clear();
		mesh_bone_rotz.clear();
		mesh_bone_transx.clear();
		mesh_bone_transy.clear();
		mesh_bone_transz.clear();
		mesh_bone_isrotx.clear();
		mesh_bone_isroty.clear();
		mesh_bone_isrotz.clear();
		mesh_bone_name_index.clear();
		mesh_bone_default_anime_frame.clear();
		mesh_offset_matrix.clear();
	}
};


class AnimationMeshFrame {
public:
	AnimationMeshKakera* kakera;
	int kakera_index;
	int frame;
	float time;
};

class AnimationMesh {
public:
	vector<AnimationMeshFrame*> frames;
	string anime_name;
	int anime_index;
	float all_time;
	bool is_loop;

	AnimationMesh() {
		anime_index = 0;
		all_time = 0;
		is_loop = 0;

	}

	~AnimationMesh() {
		vector<AnimationMeshFrame*>::iterator it = frames.begin();
		while(it != frames.end()) {
			AnimationMeshFrame* ff = *it;
			if (ff) {
				delete ff;
				ff = 0;
			}

			it++;
		}
		frames.clear();
	}
	void write(char* filename, int impl_id);
};

class AnimationBuilderMesh {
public:
	Mesh* mesh;
	bool mesh_loaded;
	char mesh_filepath[128];
	char mesh_meshpath[128];
	char mesh_animepath[128];
	bool is_animated;
	char oya_filepath[128];
	bool is_connect_without_material_local;
	MYMATRIX matrix_kakeru;
	char oya_mesh_bone_name[128];
public:
	bool isOyaFile(char* oya_filepath) {
		if (strlen(oya_filepath)==0) return false;
		if(strcmp(mesh_filepath, oya_filepath)==0) {
			return true;
		}
		return false;
	}
	AnimationBuilderMesh(char* dmesh_filepath,char* dmesh_meshpath, char* dmesh_animepath) {
		mesh = new Mesh();
		mesh_loaded = false;
		memset(mesh_meshpath,0,128);
		memset(mesh_animepath,0,128);
		memset(mesh_filepath,0,128);
		strcpy_s(mesh_filepath,128,dmesh_filepath);
		strcpy_s(mesh_meshpath, 128,dmesh_meshpath);
		strcpy_s(mesh_animepath,128,dmesh_animepath);
		memset(oya_filepath,0,128);
		is_connect_without_material_local = true;
		memset(oya_mesh_bone_name,0,128);
		MyMatrixIdentity(matrix_kakeru);
		is_animated = false;
	}

	void setOyaMesh(char* doya_filepath, char* obname, bool is_c, MYMATRIX* matrix_kakeru) {
		strcpy_s(oya_filepath,128,doya_filepath);
		is_connect_without_material_local = is_c;
		this->matrix_kakeru = *matrix_kakeru;
		strcpy_s(oya_mesh_bone_name, 128, obname);
	}

	void write(char* filename, int impl_id);

	~AnimationBuilderMesh() {
		if (mesh) {
			mesh->Release();
			delete mesh;
			mesh = 0;
		}
	}
};

class AnimationBuilderImpl {
public:

	AnimationBuilderMesh* hon_mesh;
	vector<AnimationBuilderMesh*> onaji_mesh;
	vector<AnimationBuilderMesh*> ko_mesh;

	vector<AnimationMeshKakera*> kakeras;
	map<int,int> frame_to_kakera_index;

	vector<AnimationMesh*> animes;
	AnimationMeshKakera* now_kakera;
	AnimationBuilderMesh* getOyaMesh(char* oya_filepath) {
		if (hon_mesh) {
			if (hon_mesh->isOyaFile(oya_filepath)) {
				return hon_mesh;
			}
		}
		int oms = onaji_mesh.size();
		int kms = ko_mesh.size();
		if (oms) {
			for (int i=0;i<oms;i++) {
			AnimationBuilderMesh* mm = onaji_mesh[i];
			if (mm->isOyaFile(oya_filepath)) {
				return mm;
			}
			}
		}

		if (kms) {
			for (int i=0;i<kms;i++) {
				AnimationBuilderMesh* mm = ko_mesh[i];
				if (mm->isOyaFile(oya_filepath)) {
					return mm;
				}
			}
		}
		return 0;
	}

	void setIsAnimate(bool t);

	AnimationBuilderImpl(char* hon_filepath) {
		char hon_mesh_meshpath[128];
		char hon_mesh_animepath[128];
		memset(hon_mesh_meshpath,0,128);
		memset(hon_mesh_animepath,0,128);
		sprintf_s(hon_mesh_meshpath,128,"%s.MESH", hon_filepath);
		sprintf_s(hon_mesh_animepath,128,"%s.ANIME", hon_filepath);
		hon_mesh = new AnimationBuilderMesh(hon_filepath, hon_mesh_meshpath, hon_mesh_animepath);
		now_kakera = 0;
	}
	void Release() {

		if (hon_mesh) {
			delete hon_mesh;
			hon_mesh = 0;
		}
		vector<AnimationBuilderMesh*>::iterator it = onaji_mesh.begin();
		while(it != onaji_mesh.end()) {
			AnimationBuilderMesh* mm = *it;
			if (mm) {
				delete mm;
				mm =0;
			}

			it++;
		}
		onaji_mesh.clear();

		it = ko_mesh.begin();
		while( it != ko_mesh.end()) {
			AnimationBuilderMesh* mm = *it;
			if (mm) {
				delete mm;
				mm = 0;
			}
			it++;
		}
		ko_mesh.clear();

		// kakeraの消去
		vector<AnimationMeshKakera*>::iterator itk = kakeras.begin();
		while( itk != kakeras.end()) {
			AnimationMeshKakera* kk = *itk;
			if (kk) {
				delete kk;
				kk = 0;
			}
			itk++;
		}
		kakeras.clear();
		if (now_kakera) {
			delete now_kakera;
			now_kakera = 0;
		}
		vector<AnimationMesh*>::iterator itm = animes.begin();
		while(itm != animes.end()) {
			AnimationMesh* mmm = *itm;
			if (mmm) {
				delete mmm;
				mmm = 0;
			}

			itm++;
		}
		animes.clear();
	}

	~AnimationBuilderImpl(){
		Release();
	}
};


class AnimationBuilder;
class kurukuru : public INPUTSHORICLASS{
public:
	MYMATRIX view;
	MYVECTOR3 at;
	MYVECTOR3 from;
	MYVECTOR3 up;
	float a;
	AnimationBuilder* ab;

    bool handleMessage(int msg, void* data, DWORD time);
	kurukuru() {
		at.float3.x = 0;
		at.float3.y = 0;
		at.float3.z = 0;
		up = MYVECTOR3(0,0,1);
		from = MYVECTOR3(3,-15,14);
		a = 0;
		MyMatrixLookAtRH(view,from,at,up);
		ab = 0;
	}
	void setAB(AnimationBuilder* ad) {
		ab = ad;
	}

};


class AnimationBuilder : public Scene, public IAnimationBuilder
{
private:
	vector<AnimationBuilderImpl*> impls;
	int now_index;
	MyTextureLoader* loader;
	MYMATRIX view;
public:
	MYMATRIX proj;
private:
	kurukuru* kuru;

	int bone_index;
public:
	MYVECTOR3 bone_poss[KTROBO_MESH_BONE_MAX];
private:
	int bone_bills[KTROBO_MESH_BONE_MAX];

public:
	AnimationBuilder(char* c, int len, MyTextureLoader* loader);
	virtual ~AnimationBuilder(void) {
		Scene::~Scene();
		deleteAll();
		if (kuru) {
			delete kuru;
			kuru = 0;
		}
	}
	void enter();
	
	void leave();
	AnimationBuilderImpl* getNowImpl() {
		if (impls.size()) {
			return impls[now_index];
		}
		return NULL;
	}

	void mainrenderIMPL(bool is_focused, Graphics* g, Game* game);
	void renderhojyoIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game);
	void aiIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game);
	void posbutukariIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game);
	void loaddestructIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game);

	int createAnimationBuilderImpl(char* hon_filepath);
	void setOnajiMesh(int impl_id, char* onaji_filepath);
	void setKoMesh(int impl_id, char* ko_filepath, char* oya_filepath,char* parent_bone_name, bool is_connect_without_material_local, YARITORI MYMATRIX* kakeru);
	int getHonMeshBoneNum(int impl_id);
	char* getHonMeshBoneName(int impl_id, int bone_index);

	void setHonMeshBoneRotX(int impl_id, int bone_index, float rotx);
	void setHonMeshBoneRotY(int impl_id, int bone_index, float roty);
	void setHonMeshBoneRotZ(int impl_id, int bone_index, float rotz);
	void toggleHonMeshBoneRotXIsChange(int impl_id, int bone_index); // falseにすると回転が0になりIKでも回転しなくなる
	void toggleHonMeshBoneRotYIsChange(int impl_id, int bone_index);
	void toggleHonMeshBoneRotZIsChange(int impl_id, int bone_index);
	void setHonMeshBoneTransX(int impl_id, int bone_index, float dx);
	void setHonMeshBoneTransY(int impl_id, int bone_index, float dy);
	void setHonMeshBoneTransZ(int impl_id, int bone_index, float dz);
	void setAnimePoseFrame(int impl_id, int frame);// 現在のとっている姿勢を指定したアニメフレームとして保存する
	int createFrameExe(int impl_id, char* frameexe_name, bool is_loop);
	void setFrameToExe(int impl_id, int frameexe_id, int pose_id, int frame, float time);
	void changeFrameExe(int impl_id, int frameexe_id, int frame_id, int frame, float time);

	bool saveNowToFile(char* filename);
	bool loadFromFile(char* filename);
	bool saveAnimeAndFrameToFile(int impl_id, char* filename);
	bool force_saveNowToFile(char* filename);
	bool forceLoadFromFile(char* filename);
	bool force_saveAnimeAndFrameToFile(int impl_id, char* filename);
	void deleteAll();
	void setNowIMPLIndex(int index);
	int getNowBoneIndex();
	void setNowBoneIndex(int index);
	int getNowIMPLIndex();

};













class AnimationBuilders {
private:
	vector<AnimationBuilder*> animebs;
	MyTextureLoader* loader;
public:
	int makeInst();
	
	

	AnimationBuilder* getInstance(int index);
	

	IAnimationBuilder* getInterface(int index);
	

	AnimationBuilders(MyTextureLoader* lo) {
	 loader = lo;
	}
	~AnimationBuilders() {
		Release();
	}

	void Release() {
		int size = animebs.size();
		for (int i= 0; i<size;i++) {
			delete animebs[i];
			animebs[i] = 0;
		}
		animebs.clear();
	}
};


}

#endif