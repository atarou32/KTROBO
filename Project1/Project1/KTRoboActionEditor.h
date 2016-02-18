#ifndef KTROBOACTIONEDITOR_H
#define KTROBOACTIONEDITOR_H

#pragma once
#include "KTRoboScene.h"
#include "tolua_glue/MyLuaGlueMakeCommon.h"

namespace KTROBO {


interface IActionEditor {

TO_LUA virtual int createActionCharacter(char* name)=0;
TO_LUA virtual int setHonMesh(int character_id, char* mesh_filename, char* oya_mesh_filename, bool is_connect_without_matrial_local, YARITORI MYMATRIX* mat)=0;
TO_LUA virtual int setOnajiMesh(int character_id, int hon_mesh_id, char* mesh_filename)=0;
TO_LUA virtual void toggleMeshOptional(int character_id, int hon_mesh_id)=0;// そのキャラクターにとってメッシュが特定の状態でつけられるかどうかをトグルする
TO_LUA virtual int setAnimeAndAkatFile(int character_id, int hon_mesh_id, char* anime_filename, char* akat_filename)=0;
TO_LUA virtual int getAkatID(int character_id, int hon_mesh_id, char* anime_filename, char* akat_filename)=0;
TO_LUA virtual int getAkatNum(int character_id, int akat_id)=0;
TO_LUA virtual char* getAkatName(int character_id, int akat_id, int akat_index)=0;
TO_LUA virtual int makeAction(int character_id, char* action_name)=0;
TO_LUA virtual int setAkatToAction(int character_id, int action_id, int akat_id, int akat_index)=0;
TO_LUA virtual void setNowCharacterId(int character_id);
TO_LUA virtual int getNowCharacterId();
TO_LUA virtual void setNowAkat(int character_id, int akat_id, int akat_index)=0;
TO_LUA virtual void getNowAkatIndex(int character_id, OUT_ int* akat_id, OUT_ int* akat_index)=0;
TO_LUA virtual void setNowAction(int character_id, int action_id)=0;
TO_LUA virtual void togglePlayNowAction()=0;
TO_LUA virtual void togglePlayNowAkat()=0;
TO_LUA virtual void setCommandToCharacter(int character_id, int command_id, char* command)=0;
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


class Action {




};

class ActionToAction {





};

class ActionCharacter {






};

class ActionEditor : public Scene
{
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
	void setCommandToCharacter(int character_id, int command_id, char* command);
	int makeActiontoAction(int character_id, char* action_name_moto, char* action_name_saki, int command_id, bool is_modoru);
	void clearActionToAction(int character_id, int action_to_action_id);
	void deleteAll();
	void saveNowToFile(char* filename);
	void forceSaveNowToFile(char* filename);
	void loadFromFile(char* filename);
	void forceLoadFromFile(char* filename);
	void saveNowCharacterToFile(char* filename);
	void forceSaveNowCharacterToFile(char* filename);

};





}

#endif