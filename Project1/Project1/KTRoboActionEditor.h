#ifndef KTROBOACTIONEDITOR_H
#define KTROBOACTIONEDITOR_H
#pragma once
#include "KTRoboScene.h"
#include "tolua_glue/MyLuaGlueMakeCommon.h"
#include "MyTextureLoader.h"
#include "KTRoboMesh.h"
#include "KTRoboInput.h"
#include "KTRoboTexture.h"
#include "KTRoboGameError.h"

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
	AtariHantei() {
		start_frame = 0;
		end_frame = 0;
		is_bone = true;
		memset(bone_name,0,128);
		memset(mesh_name,0,128);
		bone = 0;
		mesh = 0;
		radius = 0;
	}

};



class ActionToAction {
public:
	int moto_action_id;
	int saki_action_id;
	int command_id;// MyCommandのcommand_id
	ActionToAction() {
		moto_action_id = 0;
		saki_action_id = 0;
		command_id = 0;
	}
};

class CharacterActionCommand : public MyCommand{
public:
	CharacterActionCommand() : MyCommand() {
	}
	~CharacterActionCommand() {
		MyCommand::~MyCommand();
	}

};
class AkatFrame {
public:
	int mesh_instanced_frame;
	int akat_frame;
	AkatFrame* left;
	AkatFrame* right;
	void Release() {};
	AkatFrame() {
		mesh_instanced_frame = 0;
		akat_frame = 0;
		left = 0;
		right = 0;
	}

};
class Akat {
public:
	char name[32];
	int all_frame;
	AkatFrame* root_akat_frame;
	vector<AkatFrame*> all_akat_frame;

	Akat() {
		memset(name,0,32);
		all_frame = 0;
		root_akat_frame = 0;
	}
	~Akat() {
		Release();
	}

	void Release() {
		vector<AkatFrame*>::iterator it = all_akat_frame.begin();
		while(it != all_akat_frame.end()) {
			AkatFrame* ak = *it;
			if (ak) {
				ak->Release();
				delete ak;
				ak = 0;
			}
			it++;
		}
		all_akat_frame.clear();
	}

};

class CharacterMeshSkeleton {
public:
	Mesh* skeleton;
	vector<Akat*> akats;
};

class CharacterMesh {
public:
	vector<Mesh*> meshs;
	vector<bool> mesh_has_loaded;
	vector<string> mesh_filenames;// .MESHを含まない
	bool has_oya_mesh;
	char oya_meshfilename[128];
	bool is_connect_without_material_local;
	MYMATRIX matrix_kakeru;
	bool is_akat_loaded;
	vector<Akat*> akats;

	bool is_optional;
	bool is_render;


	char akat_filename[128];// .AKATを含む
	char mesh_animename[128]; // .ANIMEを含む



	CharacterMesh() {
		has_oya_mesh = false;
		is_connect_without_material_local = false;
		is_optional = false;
		is_render = false;
		MyMatrixIdentity(matrix_kakeru);
		memset(oya_meshfilename,0,128);
		memset(akat_filename, 0, 128);
		memset(mesh_animename,0,128);
		is_akat_loaded = false;
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

		vector<Akat*>::iterator itt = akats.begin();
		while(itt != akats.end()) {
			Akat* ak = *itt;
			if (ak) {
				ak->Release();
				delete ak;
				ak = 0;
			}
			itt = itt +1;
		}
		akats.clear();

	}
};


class Action {
public:
	int action_id;
	char action_name[32];
	int all_max_frame;
	int action_type;
	map<CharacterMesh*, Akat*> mesh_akat_pair;
	vector<AtariHantei*> hanteis;

	Action() {
		action_id = 0;
		memset(action_name,0,32);
		all_max_frame = 0;
		action_type = 0;
	}
	~Action() {Release();}

	void Release() {
		vector<AtariHantei*>::iterator it = hanteis.begin();
		while(it != hanteis.end()) {
			AtariHantei* ag = *it;
			if (ag) {
				delete ag;
				ag =0;
			}
			it++;
		}
		hanteis.clear();
	}

};

#define KTROBO_ACTIONEDITOR_TAB_TEXT_WIDTH_HEIGHT 16

class ActionCharacter {
private:
	char character_name[32];
	vector<CharacterMesh*> meshs;
	vector<Action*> actions;
	vector<ActionToAction*> action_to_actions;
	vector<CharacterActionCommand*> commands;
	Texture* tex;
	int now_akat;
	int now_action;
	int now_akat_text;
	int now_action_text;
public:
	vector<CharacterMesh*>* getMeshs() {return &meshs;};
	void setNowAkat(int now_character_index, int index) {
		if (index >=0 && index < meshs[now_character_index]->akats.size()) {
			now_akat = index;
			tex->setRenderTextChangeText(now_akat_text, meshs[now_character_index]->akats[index]->name);
		} else {
			throw new GameError(KTROBO::WARNING, "out side vector of now akat");
		}
	}
	void setNowAction(int now_character_index, int index) {
		if (index >= 0 && index < actions.size()) {
			now_action = index;
			tex->setRenderTextChangeText(now_action_text, actions[now_action]->action_name);
		} else {
			throw new GameError(KTROBO::WARNING, "out side vector of now action");
		}
	}
	ActionCharacter(char* name, Texture* tex) {
		this->tex = tex;
		memset(character_name,0,32);
		hmystrcpy(character_name,31,0,name);
		now_akat = 0;
		now_action = 0;
		now_akat_text = tex->getRenderText(name,0,KTROBO_ACTIONEDITOR_TAB_TEXT_WIDTH_HEIGHT*2,KTROBO_ACTIONEDITOR_TAB_TEXT_WIDTH_HEIGHT,
			KTROBO_ACTIONEDITOR_TAB_TEXT_WIDTH_HEIGHT*18,KTROBO_ACTIONEDITOR_TAB_TEXT_WIDTH_HEIGHT);
		now_action_text = tex->getRenderText(name, 0,KTROBO_ACTIONEDITOR_TAB_TEXT_WIDTH_HEIGHT*2,KTROBO_ACTIONEDITOR_TAB_TEXT_WIDTH_HEIGHT,
			KTROBO_ACTIONEDITOR_TAB_TEXT_WIDTH_HEIGHT*18,KTROBO_ACTIONEDITOR_TAB_TEXT_WIDTH_HEIGHT);

	}
	~ActionCharacter() {
		Release();

		if (now_akat_text) {
			tex->lightdeleteRenderText(now_akat_text);
			now_akat_text = 0;
		}
		if (now_action_text) {
			tex->lightdeleteRenderText(now_action_text);
			now_action_text = 0;
		}


	}

	void setFocus(Texture* tex) {
		tex->setRenderTextIsRender(now_akat_text, true);
		tex->setRenderTextIsRender(now_action_text, true);
	}

	void setUnFocus(Texture* tex) {
		tex->setRenderTextIsRender(now_akat_text, false);
		tex->setRenderTextIsRender(now_action_text, false);
	}

	void Release() {

	

		vector<CharacterMesh*>::iterator mesh_it = meshs.begin();
		while( mesh_it != meshs.end()) {
			CharacterMesh* cm = *mesh_it;
			if (cm) {
				delete cm;
				cm = 0;
			}
			mesh_it++;
		}
		meshs.clear();

		vector<Action*>::iterator a_it = actions.begin();
		while(a_it != actions.end()) {
			Action* ac = *a_it;
			if (ac) {
				delete ac;
				ac = 0;
			}
			a_it++;
		}
		actions.clear();

		vector<ActionToAction*>::iterator atoa_it = action_to_actions.begin();
		while(atoa_it != action_to_actions.end()) {
		
			ActionToAction* a = *atoa_it;
			if (a) {
			delete a;
			a = 0;
			}
			atoa_it++;
		}
		action_to_actions.clear();
		
		vector<CharacterActionCommand*>::iterator ittt = commands.begin();
		while(ittt != commands.end()) {
			CharacterActionCommand* cac = *ittt;
			if (cac) {
				delete cac;
				cac = 0;
			}

			ittt++;
		}
		commands.clear();
	}
};

class ActionEditor : public Scene, public IActionEditor
{
private:
	vector<ActionCharacter*> characters;
	int now_character_index;
	int now_character_text;
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