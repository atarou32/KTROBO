#include "KTRoboTextFromLua.h"
#include "stringconverter.h"
#include "KTRoboLuaCollection.h"

using namespace KTROBO;
TextFromLua::TextFromLua(Graphics* gg)
{
	g  = gg;
}


TextFromLua::~TextFromLua(void)
{
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