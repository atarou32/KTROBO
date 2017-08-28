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
#include "lua.hpp"
#include "lualib.h"
#include "lauxlib.h"
#include "KTRoboTexture.h"
#include "KTRoboInput.h"
#include "KTRoboText.h"
#include "vector"
#include "MySound.h"
#include "MyDefine.h"
#include "KTRoboMeshInstanced.h"
#include "KTRoboScene.h"
#include "KTRoboTextFromLua.h"
#include "KTRoboMessage.h"
#include "KTRoboAtari.h"
#include "KTRoboKendo.h"
#include "KTRoboKendoSinaiGenerator.h"
#include "KTRoboRobo.h"
#include "KTRoboArmPositioner.h"

namespace KTROBO {

class TempInputShori : public INPUTSHORICLASS {
public:
	TempInputShori(MySound* sound, Graphics* g) {
		this->sound = sound;
		sound_index =0;
		this->g = g;
		ray.dir = MYVECTOR3(0,0,1);
		ray.org = MYVECTOR3(0,0,0);
		testdayo = 0;
		testx = 0;
		testy = -0.90;
		testz = 2.0;
		roty = 0;
	}
	MySound* sound;
	int sound_index;
	MYMATRIX matd;
	MYMATRIX proj;
	MYMATRIX view;
	Graphics* g;
	RAY ray;
	int x;
	int y;
	float testdayo;
	float testx;
	float testz;
	float testy;
	float roty;
	void getPos(MYVECTOR3* p) {
			CS::instance()->enter(CS_MESSAGE_CS, "enter");
		p->float3.x = testx;
		p->float3.y = testy;
		p->float3.z = testz;
			CS::instance()->leave(CS_MESSAGE_CS, "leave");
	}

	void setMAT(MYMATRIX* mat,MYMATRIX* view, MYMATRIX* proj) {
		matd = *mat;
		this->proj = *proj;
		this->view = *view;
	}
	
	float getRotY() {
		return roty;
	}


	bool handleMessage(int msg, void * data, DWORD time);
};
class Game
{
private:
	Text* te;
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
	vector<UMeshUnit*> umesh_units;
	AtariHantei* hantei;

	TextFromLuas* cltf;
	CMeshs* cmeshs;
	Textures* texdayo;
	vector<Scene*> scenes;
	Messages* messages;
	MySound* sound;
	TempInputShori* temp_input_shori;
	Sinai* sinai;
	SinaiNigiru* ss;
	SinaiFuru* sfuru;
	KendoSinaiGenerator* ksgene;
	SinaiFuruAnimeMakers* makers;
	Robo* robodayo;
	Robo* roboaitedayo;

	ShudouArmPositioner* sap;
public:
	Game(void);
	~Game(void);

public:
	bool Init(HWND hwnd);
	void Del();
	void Run();

	MySound* getSound() {
		return sound;
	}

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
		if (scenes.size()) {
		Scene* s = scenes.back();
		scenes.pop_back();
		if (s) {
			s->leave();
			delete s;
			s = 0;
		}
		}
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