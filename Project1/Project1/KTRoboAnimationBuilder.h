#ifndef KTROBOANIMATIONBUILDER_H
#define KTROBOANIMATIONBUILDER_H

#pragma once
#include "KTRoboMesh.h"
#include "KTRoboScene.h"

namespace KTROBO {

interface IAnimationBuilder {
	TO_LUA virtual int createAnimationBuilderImpl(char* hon_filepath)=0;
	TO_LUA virtual void setOnajiMesh(int impl_id, char* onaji_filepath)=0;
	TO_LUA virtual void setKoMesh(int impl_id, char* ko_filepath, char* parent_bone_name, bool is_connect_without_material_local)=0;
	TO_LUA virtual int getHonMeshBoneNum(int impl_id)=0;
	TO_LUA virtual char* getHonMeshBoneName(int impl_id, int bone_index)=0;
	TO_LUA virtual void setHonMeshBoneRotX(int impl_id, int bone_index, float rotx)=0;
	TO_LUA virtual void setHonMeshBoneRotY(int impl_id, int bone_index, float roty)=0;
	TO_LUA virtual void setHonMeshBoneRotZ(int impl_id, int bone_index, float rotz)=0;
	TO_LUA virtual void setHonMeshBoneRotXIsChange(int impl_id, int bone_index, bool t)=0; // falseにすると回転が0になりIKでも回転しなくなる
	TO_LUA virtual void setHonMeshBoneRotYIsChange(int impl_id, int bone_index, bool t)=0;
	TO_LUA virtual void setHonMeshBoneRotZIsChange(int impl_id, int bone_index, bool t)=0;
	TO_LUA virtual void setHonMeshBoneTransX(int impl_id, int bone_index, float dx)=0;
	TO_LUA virtual void setHonMeshBoneTransY(int impl_id, int bone_index, float dy)=0;
	TO_LUA virtual void setHonMeshBoneTransZ(int impl_id, int bone_index, float dz)=0;
	TO_LUA virtual void setAnimePoseFrame(int impl_id, int frame)=0;// 現在のとっている姿勢を指定したアニメフレームとして保存する
	TO_LUA virtual int createFrameExe(int impl_id, char* frameexe_name, bool is_loop)=0;
	TO_LUA virtual void setFrameToExe(int impl_id, int frameexe_id, int pose_id,  int frame, float time)=0;
	TO_LUA virtual void changeFrameExe(int impl_id, int frameexe_id, int frame_id, int frame, float time)=0;
	TO_LUA virtual void saveNowToFile(char* filename)=0;
	TO_LUA virtual void loadFromFile(char* filename)=0;
	TO_LUA virtual void saveAnimeAndFrameToFile(int impl_id, char* filename)=0;
	TO_LUA virtual void deleteAll()=0;
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

	vector<MYMATRIX*> mesh_offset_matrix;
public:
	void setOffsetMatrixToMesh(Mesh* mesh);
	void copy(AnimationMeshKakera* kakera_moto);// コピー元からコピーする
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
	int all_time;
	bool is_loop;
};

class AnimationBuilderMesh {
public:
	Mesh* mesh;
	bool mesh_loaded;
	char mesh_meshpath[128];
	char mesh_animepath[128];

	AnimationBuilderMesh* oya_mesh;
	bool is_connect_without_material_local;
	char oya_mesh_bone_name[128];
public:
	AnimationBuilderMesh(char* dmesh_meshpath, char* dmesh_animepath) {
		mesh = new Mesh();
		mesh_loaded = false;
		memset(mesh_meshpath,0,128);
		memset(mesh_animepath,0,128);
		strcpy_s(mesh_meshpath, 128,dmesh_meshpath);
		strcpy_s(mesh_animepath,128,dmesh_animepath);
		oya_mesh = 0;
		is_connect_without_material_local = true;
		memset(oya_mesh_bone_name,0,128);
	}

	void setOyaMesh(AnimationBuilderMesh* o, char* obname, bool is_c) {
		oya_mesh = o;
		is_connect_without_material_local = is_c;
		strcpy_s(oya_mesh_bone_name, 128, obname);
	}


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

	AnimationBuilderImpl(char* hon_filepath) {
		char hon_mesh_meshpath[128];
		char hon_mesh_animepath[128];
		memset(hon_mesh_meshpath,0,128);
		memset(hon_mesh_animepath,0,128);
		sprintf_s(hon_mesh_meshpath,128,"%s.MESH", hon_filepath);
		sprintf_s(hon_mesh_animepath,128,"%s.ANIME", hon_filepath);
		hon_mesh = new AnimationBuilderMesh(hon_mesh_meshpath, hon_mesh_animepath);
		now_kakera = 0;
	}

};

class AnimationBuilder : Scene
{
private:
	vector<AnimationBuilderImpl*> impls;
	int now_index;

public:
	AnimationBuilder(char* c, int len);
	virtual ~AnimationBuilder(void) {
		Scene::~Scene();
	}
	void enter() {
		Scene::enter();
	}
	void leave() {
		Scene::leave();
	}

	void mainrenderIMPL(bool is_focused, Graphics* g, Game* game) {};
	void renderhojyoIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {};
	void aiIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {};
	void posbutukariIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {};
	void loaddestructIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {};

	int createAnimationBuilderImpl(char* hon_filepath);
	void setOnajiMesh(int impl_id, char* onaji_filepath);
	void setKoMesh(int impl_id, char* ko_filepath, char* parent_bone_name, bool is_connect_without_material_local);
	int getHonMeshBoneNum(int impl_id);
	char* getHonMeshBoneName(int impl_id, int bone_index);
	void setHonMeshBoneRotX(int impl_id, int bone_index, float rotx);
	void setHonMeshBoneRotY(int impl_id, int bone_index, float roty);
	void setHonMeshBoneRotZ(int impl_id, int bone_index, float rotz);
	void setHonMeshBoneRotXIsChange(int impl_id, int bone_index, bool t); // falseにすると回転が0になりIKでも回転しなくなる
	void setHonMeshBoneRotYIsChange(int impl_id, int bone_index, bool t);
	void setHonMeshBoneRotZIsChange(int impl_id, int bone_index, bool t);
	void setHonMeshBoneTransX(int impl_id, int bone_index, float dx);
	void setHonMeshBoneTransY(int impl_id, int bone_index, float dy);
	void setHonMeshBoneTransZ(int impl_id, int bone_index, float dz);
	void setAnimePoseFrame(int impl_id, int frame);// 現在のとっている姿勢を指定したアニメフレームとして保存する
	int createFrameExe(int impl_id, char* frameexe_name, bool is_loop);
	void setFrameToExe(int impl_id, int frameexe_id, int pose_id, int frame, float time);
	void changeFrameExe(int impl_id, int frameexe_id, int frame_id, int frame, float time);
	void saveNowToFile(char* filename);
	void loadFromFile(char* filename);
	void saveAnimeAndFrameToFile(int impl_id, char* filename);
	void deleteAll();

};
}

#endif