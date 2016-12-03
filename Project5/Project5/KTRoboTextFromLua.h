#ifndef KTROBOTEXTFROMLUA_H
#define KTROBOTEXTFROMLUA_H

#pragma once
#include "KTRoboDebugText.h"
#include "tolua_glue/MyLuaGlueMakeCommon.h"
#include "KTRoboGraphics.h"
//#include "KTRoboAnimationBuilder.h"
#include "KTRoboScene.h"
//#include "KTRoboActionEditor.h"

namespace KTROBO {

class Game;
interface ITextFromLua {
public:
	TO_LUA virtual void setDebugText(char* str)=0;
	TO_LUA virtual int add(int a, int b)=0;

	TO_LUA virtual void removeScene()=0;
	TO_LUA virtual void enterTWOTAKU(char* yes_str, char* no_str, char* render_text)=0; // 二つのTWOTAKUがでていないようにすること
	TO_LUA virtual void enterONEMESSAGE(char* mes)=0; // ふたつのメッセージがでていないようにすること
	TO_LUA virtual void enterLOADTYUU()=0; 

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
	TWOTAKU* two;
	ONEMESSAGE* one;
	LOADTYUU* load;
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
//	void enterABScene(COLLECTED AnimationBuilder* a);
//	void enterABSceneWithoutLeave(COLLECTED AnimationBuilder* a);
//	void enterAEScene(COLLECTED ActionEditor* a);
	void enterTWOTAKU(char* yes_str, char* no_str, char* render_text);
	void enterONEMESSAGE(char* mes);
	void removeScene();
	void enterLOADTYUU(); 

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

	ITextFromLua* getInterface(int index);
	

	TextFromLua* getInstance(int index);
	

	int makeInstance();
	
	
};


}
#endif