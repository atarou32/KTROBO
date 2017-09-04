#include "KTRoboSceneGarage.h"
#include "KTRoboGame.h"


using namespace KTROBO;

SceneGarage::SceneGarage(Graphics* g, AtariHantei* hantei, Texture* tex, MyTextureLoader* loader) : Scene("garage",6)
{
	gg = 0;
	this->g = g;
	this->hantei = hantei;
	this->tex = tex;
	this->loader = loader;

	
}


SceneGarage::~SceneGarage(void)
{
	
}


void SceneGarage::mainrenderIMPL(bool is_focused, Graphics* g, Game* game) {
	if (gg) {
		gg->byouga(g, NULL, game->getDMSecond(), game->getTimeStamp());
	}
}
void SceneGarage::renderhojyoIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {




}


void SceneGarage::aiIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game){

}

void SceneGarage::posbutukariIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game){

}

void SceneGarage::loaddestructIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game){
	if (gg) {
		gg->loadData(g,game->getDMSecond(), game->getTimeStamp());
	}
}

void SceneGarage::enter() {
	gg = new Gamen_GARAGE();
	gg->Init(g, hantei, tex,loader);
	INPUTGETBYMESSAGESTRUCT* ss  = InputMessageDispatcher::getRootInputGetStruct();
	while (ss->getParent()) {
		ss = ss->getParent();
	}
	InputMessageDispatcher::registerImpl(gg, NULL, ss->impl);

	Scene::enter();
	

}


void SceneGarage::leave() {

	Scene::leave();

	if (gg) {
		InputMessageDispatcher::unregisterImpl(gg);
		gg->Release();
		delete gg;
		gg = 0;
	}

	
}


bool SceneGarage::handleMessage(int msg, void* data, DWORD time) {
	if (gg) {
		gg->handleMessage(msg, data, time);
	}
	return false;
}