#ifndef KTROBOGUI_H
#define KTROBOGUI_H

#pragma once
#include "MyButukari.h"
#include "KTRoboInput.h"
#include "KTRoboTexture.h"
#include "KTRoboText.h"
#include "lua.hpp"

#include "vector"

// GUI のデストラクトはテクスチャのデストラクトの前に行うこと
using namespace std;
namespace KTROBO {

#define KTROBO_GUI_PNG "resrc/sample/gui.png"

#define KTROBO_GUI_INPUTTEXT_NORMAL_LEFT 0
#define KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH 129
#define KTROBO_GUI_INPUTTEXT_NORMAL_TOP 0
#define KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT 129
#define KTROBO_GUI_INPUTTEXT_FOCUS_LEFT 131
#define KTROBO_GUI_INPUTTEXT_FOCUS_WIDTH 129
#define KTROBO_GUI_INPUTTEXT_FOCUS_TOP 0
#define KTROBO_GUI_INPUTTEXT_FOCUS_HEIGHT 129
#define KTROBO_GUI_INPUTTEXT_PRESS_LEFT 261
#define KTROBO_GUI_INPUTTEXT_PRESS_WIDTH 129
#define KTROBO_GUI_INPUTTEXT_PRESS_TOP 0
#define KTROBO_GUI_INPUTTEXT_PRESS_HEIGHT 129
#define KTROBO_GUI_INPUTSENTENCE_IMM_MODE_HANKAKU IME_CMODE_ALPHANUMERIC
#define KTROBO_GUI_INPUTSENTENCE_IMM_MODE_HANKAKU2 IME_CMODE_ROMAN
#define KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA 16
#define KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY 4



#define KTROBO_GUI_BUTTON_NORMAL_LEFT 0
#define KTROBO_GUI_BUTTON_NORMAL_WIDTH 129
#define KTROBO_GUI_BUTTON_NORMAL_TOP 0
#define KTROBO_GUI_BUTTON_NORMAL_HEIGHT 129
#define KTROBO_GUI_BUTTON_FOCUS_LEFT 131
#define KTROBO_GUI_BUTTON_FOCUS_WIDTH 129
#define KTROBO_GUI_BUTTON_FOCUS_TOP 0
#define KTROBO_GUI_BUTTON_FOCUS_HEIGHT 129
#define KTROBO_GUI_BUTTON_PRESS_LEFT 261
#define KTROBO_GUI_BUTTON_PRESS_WIDTH 129
#define KTROBO_GUI_BUTTON_PRESS_TOP 0
#define KTROBO_GUI_BUTTON_PRESS_HEIGHT 129
#define KTROBO_GUI_BUTTON_TEXT_HEIGHT 20

interface HasRenderFunc {
	virtual void render(Graphics* g)=0;
};


class GUI_PART : public INPUTSHORICLASS {
protected:
	bool is_render;
	bool is_effect;
	bool is_move; // マウス右ボタンをクリックしながら動かすとなんでも移動させられる
protected:
	MYRECT box;
	MYRECT max_box;
	static MYRECT max_default_box;
	GUI_PART() {
		is_render = false;
		is_effect = false;
		is_move = false;
		max_box = max_default_box;
	}
	virtual ~GUI_PART(){}
public:
	static void SetDefaultMaxBox(MYRECT* re) {
		max_default_box = *re;
	}
	void moveBox(int dx, int dy);
	bool getIsRender() {return is_render;}
	bool getIsEffect() {return is_effect;}
	bool getIsMove() {return is_move;}
	void setIsMove(bool t) {is_move = t;}
	virtual void setIsRender(bool t)=0;
	virtual void setIsEffect(bool t)=0;
	
	virtual bool handleMessage(int msg, void* data, DWORD time) =0;
};

class GUI_BUTTON : public GUI_PART, public HasRenderFunc
{
private:
	int box_tex_id;
	char l_str[128]; // pressされたときによばれるLuaファイル
	Text* button_text;
static lua_State* L; // handlemessageが呼ばれるのは AIスレッドなのでAIスレッドのLを呼ぶ
static Texture* texture;

public:
	static void Init(lua_State* Ld, Texture* tex) {
		L = Ld;
		texture = tex;
	}
	bool handleMessage(int msg, void* data, DWORD time);
	void setIsEffect(bool t);
	void setIsRender(bool t);
	void render(Graphics* g);
	GUI_BUTTON(float x, float y, float width, float height, char* luaf, int len, char* info);
	~GUI_BUTTON();
};


class GUI_INPUTTEXT : public GUI_PART, public HasRenderFunc
{
private:

	Text* text;
	
	int box_tex_id_naka;
	int box_tex_id_hidariue;
	int box_tex_id_hidarinaka;
	int box_tex_id_hidarisita;
	int box_tex_id_migiue;
	int box_tex_id_miginaka;
	int box_tex_id_migisita;
	int box_tex_id_sitanaka;
	int box_tex_id_uenaka;



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




	static HWND hwnd;
	static Texture* texture;




public:
	GUI_INPUTTEXT(float x, float y, float width, float height);
	~GUI_INPUTTEXT();

	static void Init(HWND hw, Texture* tex) {
		hwnd = hw;
		texture = tex;
	}

	bool handleMessage(int msg, void* data, DWORD time);
	void setIsEffect(bool t);
	void setIsRender(bool t);
	void render(Graphics* g);

};



class GUI_TEXT : public GUI_PART
{
private:
	static Texture* tex;
public:
	static void Init(Texture* te) {
		tex = te;
	}
	bool handleMessage(int msg, void* data, DWORD time){return true;};
	void setIsEffect(bool t);
	void setIsRender(bool t);
};

class GUI_TEX : public GUI_PART // GUIのテクスチャ
{
private:
	static Texture* tex;


public:
	static void Init(Texture* te) {
		tex = te;
	}

	bool handleMessage(int msg, void* data, DWORD time){return true;};
	void setIsEffect(bool t);
	void setIsRender(bool t);
};

class GUI_WINDOW : public GUI_PART
{
private:
	GUI_BUTTON* close_button;
	GUI_TEXT* title;
	bool has_button_and_title;
	vector<GUI_PART*> bodys;
	static Texture* tex;
public:
	static void Init(Texture* te) {
		tex = te;
	}
public:
	bool handleMessage(int msg, void* data, DWORD time){return true;};
	void setIsEffect(bool t);
	void setIsRender(bool t);
};


class GUI_TAB : public GUI_PART
{
private:
	int tab_index;
	GUI_PART* child_window;
	static Texture* tex;
public:
	static void Init(Texture* te) {
		tex = te;
	}

	bool handleMessage(int msg, void* data, DWORD time){return true;};
	void setIsEffect(bool t);
	void setIsRender(bool t);
};

class GUI_SLIDERV : public GUI_PART {
private:

	MYRECT zentai_box;
	static Texture* tex;
public:
	static void Init(Texture* te) {
		tex = te;
	}
	bool handleMessage(int msg, void* data, DWORD time){return true;};
	void setIsEffect(bool t);
	void setIsRender(bool t);
};

class GUI_SLIDERH : public GUI_PART {
private:

	MYRECT zentai_box;
	static Texture* tex;
public:
	static void Init(Texture* te) {
		tex = te;
	}

	bool handleMessage(int msg, void* data, DWORD time) { return true;};
	void setIsEffect(bool t);
	void setIsRender(bool t);
};



class GUI
{
public:
	GUI(void);
	~GUI(void);

	static void Init(HWND hw, Texture* tex, lua_State* Ld, int screen_width, int screen_height) {
		GUI_BUTTON::Init(Ld, tex);
		GUI_SLIDERH::Init(tex);
		GUI_SLIDERV::Init(tex);
		GUI_TEX::Init(tex);
		GUI_TEXT::Init(tex);
		GUI_WINDOW::Init(tex);
		GUI_TAB::Init(tex);
		GUI_INPUTTEXT::Init(hw, tex);
		MYRECT r;
		r.left = 0;
		r.right = screen_width;
		r.top = 0;
		r.bottom = screen_height;
		GUI_PART::SetDefaultMaxBox(&r);
	}



};

}

#endif