#include "KTRoboTextFromLua.h"
#include "stringconverter.h"
#include "KTRoboLuaCollection.h"
#include "KTRoboGame.h"

using namespace KTROBO;
TextFromLua::TextFromLua(Graphics* gg, Game* ggg)
{
	g  = gg;
	gdayo = ggg;
}


TextFromLua::~TextFromLua(void)
{
}

void TextFromLua::enterABScene(COLLECTED AnimationBuilder* a) {
	gdayo->removeScene();
	this->gdayo->setScene(a);
}


void TextFromLua::enterABSceneWithoutLeave(COLLECTED AnimationBuilder* a) {
	
	gdayo->setScene(a);
}

void TextFromLua::setDebugText(char* str) {
	WCHAR test[512];
	memset(test,0,sizeof(WCHAR)*512);
	stringconverter sc;
	sc.charToWCHAR(str, test);
	DebugTexts::instance()->setText(g, wcslen(test), test);

}


void TextFromLua::makeAILua(bool is_lock_sita, char* lua_filename) {
	LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION,is_lock_sita,lua_filename);
}


void TextFromLua::makeButukariLua(bool is_lock, char* lua_filename) {
	LuaTCBMaker::makeTCB(TASKTHREADS_UPDATEPOSBUTUKARI, is_lock, lua_filename);
}


void TextFromLua::makeLoadLua(bool is_lock, char* lua_filename) {
	LuaTCBMaker::makeTCB(TASKTHREADS_LOADDESTRUCT, is_lock, lua_filename);
}

void TextFromLua::makeAnimeLua(bool is_lock, char* lua_filename) {
	LuaTCBMaker::makeTCB(TASKTHREADS_UPDATEANIMEFRAMENADO, is_lock, lua_filename);
}


void TextFromLua::makeRenderLua(bool is_lock, char* lua_filename) {
	LuaTCBMaker::makeTCB(TASKTHREADS_UPDATEMAINRENDER,is_lock, lua_filename);
}

ITextFromLua* TextFromLuas::getInterface(int index) {
	if (instances.size()) {
		return instances[index];
	} else {
		makeInstance();
		return instances[0];
	}

}

TextFromLua* TextFromLuas::getInstance(int index) {
	if (instances.size()) {	
	return instances[index];
	} else {
		makeInstance();
		return instances[0];
	}
}

int TextFromLuas::makeInstance() {
	TextFromLua* l = new TextFromLua(g, gdayo);
	int ans = instances.size();
	instances.push_back(l);
	return ans;
}