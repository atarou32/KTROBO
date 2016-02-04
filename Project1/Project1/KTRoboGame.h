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


namespace KTROBO {
#define RENDERTIME_IGNORETIME 1200
#define RENDERTIME_SETTIME 33


class kurukuru : public INPUTSHORICLASS{
public:
	MYMATRIX view;
	MYVECTOR3 at;
	MYVECTOR3 from;
	MYVECTOR3 up;
	float a;
    bool handleMessage(int msg, void* data, DWORD time);
	kurukuru() {
		at.float3.x = 0;
		at.float3.y = 0;
		at.float3.z = 0;
		up = MYVECTOR3(0,0,1);
		from = MYVECTOR3(3,-15,14);
		a = 0;
		MyMatrixLookAtRH(view,from,at,up);
	}
};
class Game
{
private:
	Text* te;
	kurukuru k;
	Graphics* g;
	lua_State* L;
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
};

}

#endif