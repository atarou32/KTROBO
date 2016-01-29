#ifndef KTROBOGUI_H
#define KTROBOGUI_H

#pragma once
#include "MyButukari.h"
#include "KTRoboInput.h"
#include "KTRoboTexture.h"

#include "vector"
using namespace std;
namespace KTROBO {
class GUI
{
public:
	GUI(void);
	~GUI(void);
};

class GUI_PART {
private:
	bool is_render;
	bool is_effect;
protected:
	MYRECT box;
	GUI_PART() {
		is_render = true;
		is_effect = true;
	}
	bool getIsRender() {return is_render;}
	bool getIsEffect() {return is_effect;}
	virtual void setIsRender(bool t)=0;
	virtual void setIsEffect(bool t)=0;
};

class GUI_BUTTON : public INPUTSHORICLASS, public GUI_PART
{
private:
//	RENDEREFFECT_Object* render_effect; // たとえばボタンが押されたときにエフェクト？を発生させるときに使用する？
//	RENDERTEX_Object* render_obj;
public:
		
	bool handleMessage(int msg, void* data, DWORD time){return true;};
	void setIsEffect(bool t);
	void setIsRender(bool t);
};

class GUI_INPUTTEXT : public INPUTSHORICLASS, public GUI_PART
{
private:
//	RENDEREFFECT_Object* render_effect;
//	RENDERTEX_Object* render_frame;
//	RENDERTEX_CObject* render_text; // テキストはテクスチャの複数の集合なのでこうする

public:
	bool handleMessage(int msg, void* data, DWORD time){return true;};
	void setIsEffect(bool t);
	void setIsRender(bool t);

};

class GUI_TEXT : public INPUTSHORICLASS, public GUI_PART
{
private:
//	RENDEREFFECT_Object* render_effect;
//	RENDERTEX_Object* render_frame;
//	RENDERTEX_CObject* render_text; // テキストはテクスチャの複数の集合なのでこうする
public:
	bool handleMessage(int msg, void* data, DWORD time){return true;};
	void setIsEffect(bool t);
	void setIsRender(bool t);
};

class GUI_TEX : public INPUTSHORICLASS , public GUI_PART // GUIのテクスチャ
{
private:
//	RENDEREFFECT_Object* render_effect;
//	RENDERTEX_Object* render_obj;
public:
	bool handleMessage(int msg, void* data, DWORD time){return true;};
	void setIsEffect(bool t);
	void setIsRender(bool t);
};

class GUI_WINDOW : public INPUTSHORICLASS, public GUI_PART
{
private:
//	RENDEREFFECT_Object* render_effect;
//	RENDERTEX_Object* render_obj;
	GUI_BUTTON* close_button;
	GUI_TEXT* title;
	bool has_button_and_title;
	vector<GUI_PART*> bodys;
public:
	bool handleMessage(int msg, void* data, DWORD time){return true;};
	void setIsEffect(bool t);
	void setIsRender(bool t);
};


class GUI_TAB : public INPUTSHORICLASS, public GUI_PART
{
private:
//	RENDEREFFECT_Object* render_effect;
//	RENDERTEX_Object* render_obj;
	int tab_index;
	GUI_PART* child_window;
public:
	bool handleMessage(int msg, void* data, DWORD time){return true;};
	void setIsEffect(bool t);
	void setIsRender(bool t);
};

class GUI_SLIDERV : public INPUTSHORICLASS, public GUI_PART {
private:
//	RENDEREFFECT_Object* render_effect;
//	RENDERTEX_Object* render_obj;
	MYRECT zentai_box;
public:
	bool handleMessage(int msg, void* data, DWORD time){return true;};
	void setIsEffect(bool t);
	void setIsRender(bool t);
};

class GUI_SLIDERH : public INPUTSHORICLASS, public GUI_PART {
private:
//	RENDEREFFECT_Object* render_effect;
//	RENDERTEX_Object* render_obj;
	MYRECT zentai_box;
public:
	bool handleMessage(int msg, void* data, DWORD time) { return true;};
	void setIsEffect(bool t);
	void setIsRender(bool t);
};



}

#endif