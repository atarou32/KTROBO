#ifndef KTROBOGUI_H
#define KTROBOGUI_H

#pragma once
#include "MyButukari.h"
#include "KTRoboInput.h"
#include "KTRoboTexture.h"
#include "KTRoboText.h"

#include "vector"
using namespace std;
namespace KTROBO {

#define KTROBO_GUI_PNG "resrc/sample/gui.png"

#define KTROBO_GUI_INPUTTEXT_NORMAL_LEFT 0
#define KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH 128
#define KTROBO_GUI_INPUTTEXT_NORMAL_TOP 0
#define KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT 128
#define KTROBO_GUI_INPUTTEXT_FOCUS_LEFT 129
#define KTROBO_GUI_INPUTTEXT_FOCUS_WIDTH 128
#define KTROBO_GUI_INPUTTEXT_FOCUS_TOP 0
#define KTROBO_GUI_INPUTTEXT_FOCUS_HEIGHT 128
#define KTROBO_GUI_INPUTTEXT_PRESS_LEFT 257
#define KTROBO_GUI_INPUTTEXT_PRESS_WIDTH 128
#define KTROBO_GUI_INPUTTEXT_PRESS_TOP 0
#define KTROBO_GUI_INPUTTEXT_PRESS_HEIGHT 128
#define KTROBO_GUI_INPUTSENTENCE_IMM_MODE_HANKAKU IME_CMODE_ALPHANUMERIC
#define KTROBO_GUI_INPUTSENTENCE_IMM_MODE_HANKAKU2 IME_CMODE_ROMAN


interface HasRenderFunc {
	virtual void render(Graphics* g)=0;
};

class GUI
{
public:
	GUI(void);
	~GUI(void);
};

class GUI_PART {
protected:
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

/*
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
*/
class GUI_INPUTTEXT : public INPUTSHORICLASS, public GUI_PART, public HasRenderFunc
{
private:
//	RENDEREFFECT_Object* render_effect;
//	RENDERTEX_Object* render_frame;
//	RENDERTEX_CObject* render_text; // テキストはテクスチャの複数の集合なのでこうする
	Text* text;
	Texture* texture;
	int box_tex_id;




	char sentencestring[512];
	char inputstring[256];
	char kouhostring[256];
	DWORD now_mode;
	int cursor_x;
	int string_max_x;
	bool is_focused;
	void setIME(bool t);
	void copyIMESTRINGToInputString();
	void copyIMESTRINGToKouhoString(int msg_id, void* data, DWORD time);
	void copyStringFromAction(int msg_id, void* data, DWORD time);
	void copyInputStringToMyText();
	void copyKouhoStringToMyText();
	void setCursorX(int msg_id, void* data, DWORD time);
	void eraseSentenceString();
	char* getInputStr(unsigned char* keys);
	bool is_render_and_update;
	HWND hwnd;





public:
	GUI_INPUTTEXT(float x, float y, float width, float height, Texture* tex, HWND hwnd);
	~GUI_INPUTTEXT();

	bool handleMessage(int msg, void* data, DWORD time);
	void setIsEffect(bool t);
	void setIsRender(bool t);
	void render(Graphics* g);

};


/*
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
*/


}

#endif