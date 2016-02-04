#ifndef KTROBOANIMATIONBUILDER_H
#define KTROBOANIMATIONBUILDER_H

#pragma once
#include "KTRoboMesh.h"
#include "KTRoboScene.h"

namespace KTROBO {
class AnimationBuilder : Scene
{
public:
	AnimationBuilder(char* c, int len);
	virtual ~AnimationBuilder(void) {
		Scene::~Scene();
	}
	void enter() {
		Scene::enter();
	}
	void leave() {
		Scene::leave();
	}

	void mainrenderIMPL(bool is_focused, Graphics* g, Game* game) {};
	void renderhojyoIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {};
	void aiIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {};
	void posbutukariIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {};
	void loaddestructIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {};

};


}






#endif