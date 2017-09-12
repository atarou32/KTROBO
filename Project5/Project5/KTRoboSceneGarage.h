#ifndef KTROBOSCENEGARAGE_H
#define KTROBOSCENEGARAGE_H

#pragma once
#include "KTRoboScene.h"
#include "KTRoboGamen_GARAGE.h"
#ifndef KTROBO_TEXTURE_H
#include "KTRoboTexture.h"
#endif

namespace KTROBO {

class SceneGarage : public Scene, public INPUTSHORICLASS
{
private:
	Graphics* g;
	AtariHantei* hantei;
	Texture* tex;
	Texture* tex2;
	MyTextureLoader* loader;

	Gamen_GARAGE* gg;

public:
	SceneGarage(Graphics* g, AtariHantei* hantei, Texture* tex, Texture* tex2, MyTextureLoader* loader);
	~SceneGarage(void);

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



}


#endif