#include "KTRoboSceneGarage.h"
#include "KTRoboGame.h"


using namespace KTROBO;

SceneGarage::SceneGarage(Graphics* g, AtariHantei* hantei, Texture* tex, Texture* tex2, MyTextureLoader* loader) : Scene("garage",6)
{
	gg = 0;
	this->g = g;
	this->hantei = hantei;
	this->tex = tex;
	this->tex2 = tex2;
	this->loader = loader;

	
}


SceneGarage::~SceneGarage(void)
{
	
}


void SceneGarage::mainrenderIMPL(bool is_focused, Graphics* g, Game* game) {
	if (gg && is_focused) {
		gg->byouga(g, NULL, game->getDMSecond(), game->getTimeStamp());
		MYVECTOR3 from(22,22,10);
		MYVECTOR3 at(0,0,0);
		MYVECTOR3 up(0,0,1);
		MyMatrixLookAtRH(game->view,from,at,up);

		tex->setViewProj(g,&game->view,g->getProj(),&from,&at);
		tex2->setViewProj(g,&game->view, g->getProj(),&from,&at);
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
		try { 
		gg->loadData(g,game->getDMSecond(), game->getTimeStamp());
		} catch (GameError* err) {
			KTROBO::mylog::writelog(KTROBO::FATAL_ERROR,err->getMessage());
			throw err;
		}
	}
}

void SceneGarage::enter() {
	gg = new Gamen_GARAGE();
	gg->Init(g, hantei, tex, tex2, loader);
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