#include "KTRoboSceneGarage.h"

using namespace KTROBO;

SceneGarage::SceneGarage(void) : Scene("garage",6)
{

}


SceneGarage::~SceneGarage(void)
{
}


void SceneGarage::mainrenderIMPL(bool is_focused, Graphics* g, Game* game) {

}
void SceneGarage::renderhojyoIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {




}


void SceneGarage::aiIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game){

}

void SceneGarage::posbutukariIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game){

}

void SceneGarage::loaddestructIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game){

}

void SceneGarage::enter() {
	Scene::enter();
}


void SceneGarage::leave() {
	Scene::leave();
}


bool SceneGarage::handleMessage(int msg, void* data, DWORD time) {
	return false;
}






