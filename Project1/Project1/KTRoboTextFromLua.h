#ifndef KTROBOTEXTFROMLUA_H
#define KTROBOTEXTFROMLUA_H

#pragma once
#include "KTRoboDebugText.h"
#include "tolua_glue/MyLuaGlueMakeCommon.h"
#include "KTRoboGraphics.h"

namespace KTROBO {

class Game;
interface ITextFromLua {
public:
	TO_LUA virtual void setDebugText(char* str)=0;
	TO_LUA virtual int add(int a, int b)=0;
	TO_LUA virtual void enterABScene(COLLECTED AnimationBuilder* a)=0;
	TO_LUA virtual void enterABSceneWithoutLeave(COLLECTED AnimationBuilder* a)=0;
	TO_LUA virtual void makeAILua(bool is_lock_sita, char* lua_filename)=0;
	TO_LUA virtual void makeButukariLua(bool is_lock, char* lua_filename)=0;
	TO_LUA virtual void makeLoadLua(bool is_lock, char* lua_filename)=0;
	TO_LUA virtual void makeAnimeLua(bool is_lock, char* lua_filename)=0;
	TO_LUA virtual void makeRenderLua(bool is_lock, char* lua_filename)=0;
};

class TextFromLua : public ITextFromLua
{
private:
	Graphics* g;
	Game* gdayo;
public:
	TextFromLua(Graphics* gg, Game* ggg);
	~TextFromLua(void);

	void setDebugText(char* str);
	int add(int a, int b) {return a+b;}
	void makeAILua(bool is_lock_sita, char* lua_filename);
	void makeButukariLua(bool is_lock, char* lua_filename);
	void makeLoadLua(bool is_lock, char* lua_filename);
	void makeAnimeLua(bool is_lock, char* lua_filename);
	void makeRenderLua(bool is_lock, char* lua_filename);
	void enterABScene(COLLECTED AnimationBuilder* a);
	void enterABSceneWithoutLeave(COLLECTED AnimationBuilder* a);


};


class TextFromLuas {
public:
	Graphics* g;
	Game* gdayo;
	vector<TextFromLua*> instances;
	
	TextFromLuas(Graphics* gg, Game* ggdayo) {
		g = gg;
		gdayo = ggdayo;
	}
	~TextFromLuas() {
		vector<TextFromLua*>::iterator it = instances.begin();
		while(it != instances.end()) {
			TextFromLua* k =*it;
			delete k;

			it = it + 1;
		}
		instances.clear();
	}

	ITextFromLua* getInterface(int index) {
		return instances[index];
	}

	TextFromLua* getInstance(int index) {
		return instances[index];
	}

	int makeInstance() {
		TextFromLua* l = new TextFromLua(g, gdayo);
		int ans = instances.size();
		instances.push_back(l);
		return ans;
	}
};


}
#endif