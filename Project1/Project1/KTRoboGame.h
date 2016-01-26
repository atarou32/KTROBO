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


namespace KTROBO {
#define RENDERTIME_IGNORETIME 1200
#define RENDERTIME_SETTIME 33

#define TASKTHREADS_UPDATEANIMEFRAMENADO 1
#define TASKTHREADS_UPDATEMAINRENDER 2
#define TASKTHREADS_UPDATEPOSBUTUKARI 0
#define TASKTHREADS_LOADDESTRUCT 3
#define TASKTHREADS_AIDECISION 4


class Game
{
private:
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
public:
	Game(void);
	~Game(void);

public:
	bool Init(HWND hwnd);
	void Del();
	void Run();
};

}

#endif