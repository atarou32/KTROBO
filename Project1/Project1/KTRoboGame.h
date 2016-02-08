#ifndef KTROBOGAME_H
#define KTROBOGAME_H


#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

#include "KTRoboGraphics.h"
#include "KTRoboClock.h"
#include "KTRoboDemoRender.h"
#include "KTRoboDebugText.h"
#include "KTRoboTelopText.h"
#include "KTRoboMesh.h"
#include "MyTokenAnalyzer.h"
#include "KTRoboTask.h"
#include "KTRoboMeshInstanced.h"
#include "MyTestForVertexTexture.h"
#include "lua.hpp"
#include "lualib.h"
#include "lauxlib.h"
#include "KTRoboLuaCMesh.h"
#include "KTRoboTextFromLua.h"
//#include "tolua_glue/tolua_glue.h"
#include "KTRoboTexture.h"
#include "KTRoboInput.h"
#include "KTRoboText.h"
#include "KTRoboGUI.h"
#include "KTRoboScene.h"
#include "KTRoboAnimationBuilder.h"
#include "vector"

namespace KTROBO {
#define RENDERTIME_IGNORETIME 1200
#define RENDERTIME_SETTIME 33


class Game
{
private:
	Text* te;
	kurukuru k;
	Graphics* g;
	lua_State* L;
	lua_State* Ls[TASKTHREAD_NUM];
	KTRoboDemoRender* demo;
	TelopTexts* telop_texts;
	void startWatch();
	double stopWatch();
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;
	int fps;
	float frameTime;
	long timestamp;
	long big_timestamp;
	int second;
	float dmsecond;
	float dt;
	Clock* c;
	Task* task_threads[TASKTHREAD_NUM];
	Graphics* g_for_task_threads[TASKTHREAD_NUM];

	Mesh* mesh;
	Mesh* mesh2;
	Mesh* mesh3[11];
	MeshInstanceds* mesh_instanceds;
	IMeshInstanced* mesh_i;
	IMeshInstanced* mesh_i2;
	IMeshInstanced* mesh_is[30];
	IMeshInstanced* mesh_is2[30];
	MyTestForVertexTexture* mytest_for_vt;
	TextFromLuas* cltf;
	CMeshs* cmeshs;
	Textures* texdayo;
	GUI_INPUTTEXT* inputtext;
	GUI_BUTTON* but;
	GUI_SLIDERH * slih;
	vector<Scene*> scenes;

public:
	Game(void);
	~Game(void);

public:
	bool Init(HWND hwnd);
	void Del();
	void Run();
	Task** getTask() {
		return task_threads;
	}
	// AIスレッドから呼ぶこと
	void setScene(Scene* scene) {

	// AI(TASK4) LOAD(TASK3) //INPUT(MAIN)inputはMESSAGEをロックする // MAINRENDER(TASK2) ANIME(TASK1) BUTUKARI(TASK0) DEVICE LOAD LOG MESSAGE RENDERDATA の順
		CS::instance()->enter(CS_TASK_CS, "ai lock", 4);
		CS::instance()->enter(CS_TASK_CS, "load lock", 3);
		CS::instance()->enter(CS_TASK_CS, "render lock",2);
		CS::instance()->enter(CS_TASK_CS, "anime lock", 1);
		CS::instance()->enter(CS_TASK_CS, "atari lock", 0);
		CS::instance()->enter(CS_MESSAGE_CS, "message lock");
		scenes.push_back(scene);
		scene->enter();
		CS::instance()->leave(CS_MESSAGE_CS, "message lock");
		CS::instance()->leave(CS_TASK_CS, "atari lock",0);
		CS::instance()->leave(CS_TASK_CS, "anime lock",1);
		CS::instance()->leave(CS_TASK_CS, "render lock",2);
		CS::instance()->leave(CS_TASK_CS, "load lock", 3);
		CS::instance()->leave(CS_TASK_CS, "ai lock", 4);
	}
	void removeScene() {
		CS::instance()->enter(CS_TASK_CS, "ai lock", 4);
		CS::instance()->enter(CS_TASK_CS, "load lock", 3);
		CS::instance()->enter(CS_TASK_CS, "render lock",2);
		CS::instance()->enter(CS_TASK_CS, "anime lock", 1);
		CS::instance()->enter(CS_TASK_CS, "atari lock", 0);
		CS::instance()->enter(CS_MESSAGE_CS, "message lock");
		Scene* s = scenes.back();
		scenes.pop_back();
		s->leave();
		delete s;
		s = 0;
	
		CS::instance()->leave(CS_MESSAGE_CS, "message lock");
		CS::instance()->leave(CS_TASK_CS, "atari lock",0);
		CS::instance()->leave(CS_TASK_CS, "anime lock",1);
		CS::instance()->leave(CS_TASK_CS, "render lock",2);
		CS::instance()->leave(CS_TASK_CS, "load lock", 3);
		CS::instance()->leave(CS_TASK_CS, "ai lock", 4);
	}
};

}

#endif