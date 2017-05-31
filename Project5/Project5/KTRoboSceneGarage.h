#ifndef KTROBOSCENEGARAGE_H
#define KTROBOSCENEGARAGE_H

#pragma once
#include "KTRoboScene.h"

namespace KTROBO {

class SceneGarage : public Scene
{
public:
	SceneGarage(void);
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