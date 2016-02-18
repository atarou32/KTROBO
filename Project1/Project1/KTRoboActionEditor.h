#ifndef KTROBOACTIONEDITOR_H
#define KTROBOACTIONEDITOR_H
#pragma once
#include "KTRoboScene.h"
#include "tolua_glue/MyLuaGlueMakeCommon.h"
#include "MyTextureLoader.h"
#include "KTRoboMesh.h"
#include "KTRoboInput.h"

#include "vector"
using namespace std;
namespace KTROBO {
interface IActionEditor {
TO_LUA virtual int createActionCharacter(char* name)=0;
TO_LUA virtual int setHonMesh(int character_id, char* mesh_filename, char* oya_mesh_filename, bool is_connect_without_matrial_local, YARITORI MYMATRIX* mat)=0;
TO_LUA virtual int setOnajiMesh(int character_id, int hon_mesh_id, char* mesh_filename)=0;
TO_LUA virtual void toggleMeshOptional(int character_id, int hon_mesh_id)=0;// そのキャラクターにとってメッシュが特定の状態でつけられるかどうかをトグルする
TO_LUA virtual void toggleMeshRender(int character_id, int hon_mesh_id)=0;
TO_LUA virtual int setAnimeAndAkatFile(int character_id, int hon_mesh_id, char* anime_filename, char* akat_filename)=0;
TO_LUA virtual int getAkatID(int character_id, int hon_mesh_id, char* anime_filename, char* akat_filename)=0;
TO_LUA virtual int getAkatNum(int character_id, int akat_id)=0;
TO_LUA virtual char* getAkatName(int character_id, int akat_id, int akat_index)=0;
TO_LUA virtual int makeAction(int character_id, char* action_name)=0;
TO_LUA virtual int setAkatToAction(int character_id, int action_id, int akat_id, int akat_index)=0;
TO_LUA virtual void setNowCharacterId(int character_id)=0;
TO_LUA virtual int getNowCharacterId()=0;
TO_LUA virtual void setNowAkat(int character_id, int akat_id, int akat_index)=0;
TO_LUA virtual void getNowAkatIndex(int character_id, OUT_ int* akat_id, OUT_ int* akat_index)=0;
TO_LUA virtual void setNowAction(int character_id, int action_id)=0;
TO_LUA virtual void togglePlayNowAction()=0;
TO_LUA virtual void togglePlayNowAkat()=0;
TO_LUA virtual void setCommandToCharacter(int character_id, int command_id, char* command, char* name, int priority)=0;
TO_LUA virtual int makeActiontoAction(int character_id, char* action_name_moto, char* action_name_saki, int command_id, bool is_modoru)=0;
TO_LUA virtual void clearActionToAction(int character_id, int action_to_action_id)=0;
TO_LUA virtual void deleteAll()=0;
TO_LUA virtual void saveNowToFile(char* filename)=0;
TO_LUA virtual void forceSaveNowToFile(char* filename)=0;
TO_LUA virtual void loadFromFile(char* filename)=0;
TO_LUA virtual void forceLoadFromFile(char* filename)=0;
TO_LUA virtual void saveNowCharacterToFile(char* filename)=0;
TO_LUA virtual void forceSaveNowCharacterToFile(char* filename)=0;
};


class AtariHantei {
	// 当たり判定の位置を計算するにはどうしたらいいのか
	// 方法その１GPUの計算結果をもってくる
	// 方法その２CPUで別途計算する
	// 方法その３あらかじめ場所を計算しておく その３だと限定的だけど計算パワーは使用せずにすむ
public:
	int start_frame;
	int end_frame;
	bool is_bone;
	char bone_name[128];
	char mesh_name[128];
	MeshBone* bone;
	Mesh* mesh;
	float radius;
	vector<MYMATRIX> combined_matrixs;
};


class Action {
	int action_id;
	char action_name[32];
	int all_max_frame;
	int action_type;
	map<CharacterMesh*, Akat*> mesh_akat_pair;
	vector<AtariHantei*> hanteis;
};

class ActionToAction {
	int moto_action_id;
	int saki_action_id;
	int command_id;// MyCommandのcommand_id
};

class CharacterActionCommand : public MyCommand{
	CharacterActionCommand() : MyCommand() {
	}
	~CharacterActionCommand() {
		~MyCommand();
	}

};
class AkatFrame {
	int mesh_instanced_frame;
	int akat_frame;
	AkatFrame* left;
	AkatFrame* right;
};
class Akat {
	char name[32];
	int all_frame;
	AkatFrame* root_akat_frame;
	vector<AkatFrame*> all_akat_frame;
};
class CharacterMesh {
public:
	vector<Mesh*> meshs;
	vector<string> mesh_filenames;
	bool has_oya_mesh;
	char oya_meshfilename[128];
	bool is_connect_without_material_local;
	MYMATRIX matrix_kakeru;

	vector<Akat*> akats;

	bool is_optional;
	bool is_render;

	CharacterMesh() {
		has_oya_mesh = false;
		is_connect_without_material_local = false;
		is_optional = false;
		is_render = false;
		MyMatrixIdentity(matrix_kakeru);
		memset(oya_meshfilename,0,128);
	}

	~CharacterMesh() {
		vector<Mesh*>::iterator it = meshs.begin();
		while( it != meshs.end()) {
			Mesh* mm = *it;
			if (mm) {
			mm->Release();
			delete mm;
			mm = 0;
			}
			it = it + 1;
		}
		meshs.clear();
	}
};
class ActionCharacter {
	char character_name[32];
	vector<CharacterMesh*> meshs;
	vector<Action*> actions;
	vector<ActionToAction*> action_to_actions;
	vector<CharacterActionCommand*> commands;

};

class ActionEditor : public Scene, public IActionEditor
{
private:
	vector<ActionCharacter*> characters;
	int now_character_index;
public:

	ActionEditor(void);
	~ActionEditor(void);

	void enter();
	void leave();

	void mainrenderIMPL(bool is_focused, Graphics* g, Game* game);
	void renderhojyoIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game);
	void aiIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game);
	void posbutukariIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game);
	void loaddestructIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game);

	int createActionCharacter(char* name);
	int setHonMesh(int character_id, char* mesh_filename, char* oya_mesh_filename, bool is_connect_without_matrial_local, YARITORI MYMATRIX* mat);
	int setOnajiMesh(int character_id, int hon_mesh_id, char* mesh_filename);
	void toggleMeshOptional(int character_id, int hon_mesh_id);// そのキャラクターにとってメッシュが特定の状態でつけられるかどうかをトグルする
	int setAnimeAndAkatFile(int character_id, int hon_mesh_id, char* anime_filename, char* akat_filename);
	int getAkatID(int character_id, int hon_mesh_id, char* anime_filename, char* akat_filename);
	int getAkatNum(int character_id, int akat_id);
	char* getAkatName(int character_id, int akat_id, int akat_index);
	int makeAction(int character_id, char* action_name);
	int setAkatToAction(int character_id, int action_id, int akat_id, int akat_index);
	void setNowCharacterId(int character_id);
	int getNowCharacterId();
	void setNowAkat(int character_id, int akat_id, int akat_index);
	void getNowAkatIndex(int character_id, OUT_ int* akat_id, OUT_ int* akat_index);
	void setNowAction(int character_id, int action_id);
	void togglePlayNowAction();
	void togglePlayNowAkat();
	void setCommandToCharacter(int character_id, int command_id, char* command, char* name, int priority);
	int makeActiontoAction(int character_id, char* action_name_moto, char* action_name_saki, int command_id, bool is_modoru);
	void clearActionToAction(int character_id, int action_to_action_id);
	void deleteAll();
	void saveNowToFile(char* filename);
	void forceSaveNowToFile(char* filename);
	void loadFromFile(char* filename);
	void forceLoadFromFile(char* filename);
	void saveNowCharacterToFile(char* filename);
	void forceSaveNowCharacterToFile(char* filename);
	void toggleMeshRender(int character_id, int hon_mesh_id);
};




class ActionEditors {
private:
	vector<ActionEditor*> aes;
	MyTextureLoader* loader;
public:
	int makeInst();
	
	

	ActionEditor* getInstance(int index);
	

	IActionEditor* getInterface(int index);
	

	ActionEditors(MyTextureLoader* lo) {
	 loader = lo;
	}
	~ActionEditors() {
		Release();
	}

	void Release() {
		int size = aes.size();
		for (int i= 0; i<size;i++) {
			delete aes[i];
			aes[i] = 0;
		}
		aes.clear();
	}
};




}

#endif