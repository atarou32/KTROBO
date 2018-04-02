#ifndef KTROBOMISSION_H
#define KTROBOMISSION_H

/*
	ミッションの概要

	ミッションを選択し、ミッションを開始すると
	１．機体を選ぶ
	２．ミッションブリーフィング
	３．ミッション開始
	４．ミッション終了（達成、失敗）
	５．ミッション報酬の取得

	ミッションの種類
	１．通常ミッション
	２．ストーリーミッション
	３．緊急ミッション
	
	ミッションの達成条件
	１．特定の敵の撃破
	２．特定ポイントの取得
	３．特定時間の生存

	ミッションの属性
	
	ミッションID
	ミッション種類タイプ
	ミッション名
	ミッションコードネーム
	ミッション説明
	ミッション達成条件
	ミッション地形ID

	


*/

#pragma once
#ifndef KTROBO_BULLET_H
#include "KTRoboBullet.h"
#endif

#ifndef KTROBO_ROBO_H
#include "KTRoboRobo.h"
#endif

#ifndef KTROBO_INPUT_H
#include "KTRoboInput.h"
#endif

#ifndef KTROBO_GAMEN_H
#include "KTRoboGamen.h"
#endif

#ifndef KTROBO_SCENE_H
#include "KTRoboScene.h"
#endif

#ifndef KTROBO_GAMEN_GARAGE_H
#include "KTRoboGamen_GARAGE.h"
#endif

#ifndef KTROBO_GRAPHICS_H
#include "KTRoboGraphics.h"
#endif

namespace KTROBO {

#define KTROBO_MISSION_GAMEN_PILOT_PNG "resrc/sample/pilot.png"

#define KTROBO_MISSION_MESH_DUMMY_FILENAME "resrc/model/cube/pkcube.mesh"
#define KTROBO_MISSION_MESH_ANIME_DUMMY_FILENAME "resrc/model/cube/pkcube.anime"


class Mission
{
private:
	int nokori_second;
public:
	Mission(void);
	~Mission(void);

	void start();
	void end();
	void success();
	void failure();
	void gift();
	int get_now_state();
	void decSecond() {
		nokori_second--;
	}
	int getSecondSita() {
		return nokori_second % 60;
	}
	int getSecondUe() {
		return nokori_second / 60;
	}

};

class Gamen_MISSION : public Gamen {
private:
	Texture* t;
	Texture* t2;
	int timer_suuji_tex[4];
	int ap_suuji_tex[5];
	int houi_tex;
	int houi_tex2;
	int a_tex;
	int p_tex;
	int timer_tex;
	int energy_tex;
	int n_tex;
	int e_tex;
	int s_tex;
	int w_tex;

	float lookatspeed;
	float lookfromspeed;

	int screen_tex2;

	BulletController* bullet_c;

public:
	MYVECTOR3 lookat;
	MYVECTOR3 lookfrom;

	MYVECTOR3 tempp_lookfrom;
public:
	MYMATRIX view;
public:
	Robo* robo;
	Mission mis;
	Mesh* cubemesh;
	UMeshUnit* cameraunit;
	bool is_atari_tikei_camera;
public:
	Gamen_MISSION(Texture* t, Texture* t2) {
		this->t = t;
		this->t2 = t2;
		for (int i=0;i<4;i++) {
		timer_suuji_tex[i] = 0;
		}
		for (int i=0;i<5;i++) {
			ap_suuji_tex[i] = 0;
		}
		houi_tex = 0;
		houi_tex2 = 0;
		a_tex = 0;
		p_tex=0;
		timer_tex = 0;
		energy_tex = 0;
		n_tex = 0;
		e_tex = 0;
		s_tex = 0;
		w_tex = 0;
		lookat = MYVECTOR3(0,0,0);
		lookfrom = MYVECTOR3(25,25,12);
		MyMatrixIdentity(view);
		lookatspeed = 0;
		lookfromspeed = 0;
		screen_tex2=0;
		bullet_c = 0;
		cubemesh = 0;
		is_atari_tikei_camera = false;
	}
	~Gamen_MISSION() {
	}
	void Init(Graphics* g, AtariHantei* hantei, MyTextureLoader* loader);
	void Release() {
		if (bullet_c) {
			bullet_c->Release();
			delete bullet_c;
			bullet_c = 0;
		}

		if (cameraunit) {
			delete cameraunit;
			cameraunit = 0;
		}

		if (cubemesh) {
			cubemesh->Release();
			delete cubemesh;
			cubemesh = 0;

		}
	}

	void byouga(Graphics* g, GUI* gui, float dsecond, int stamp);
	void clickedShori(int id);
	void clickedShoriWithData(int id, void* data);

	void setTimerTex(int second_sita, int second_ue);
	void setTexSuuji(int suuji,int tex_id);
	void setHoui(float rotz);
	void setView(MYMATRIX* world, float R, float dsecond);
	void atariShori(Graphics* g, AtariHantei* hantei, float dsecond, int stamp);
	void posButukari(Graphics* g, Scene* scene, Game* game, AtariHantei* hantei, float dsecond, int stamp);
	void renderhojyo(Graphics* g, float dsecond, int stamp);

};


class Game_SCENE : public Scene, public INPUTSHORICLASS {

private:
	Graphics* g;
	AtariHantei* hantei;
	Texture* tex;
	Texture* tex2;
	MyTextureLoader* loader;

	Gamen_GARAGE* gg;
	Gamen_MISSION* gm;
	Clock butukari_clock;
	ClockWatches watches;
public:
	Game_SCENE(Graphics* g, AtariHantei* hantei, Texture* tex, Texture* tex2, MyTextureLoader* loader);
	~Game_SCENE(void);

public:
	void mainrenderIMPL(bool is_focused, Graphics* g, Game* game);
	void renderhojyoIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game);
	void aiIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game);
	void posbutukariIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game);
	void loaddestructIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game);

	void enter();
	void leave();
	bool handleMessage(int msg, void* data, DWORD time);
};








class MissionBreefing {
public:
	void waiting();
	void ready(); 
	void startMission();
	void breakMission();
	void roboChange();
	int get_now_state();
};

}
#endif