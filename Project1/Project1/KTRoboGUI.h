#ifndef KTROBOGUI_H
#define KTROBOGUI_H

#pragma once
#include "MyButukari.h"
#include "KTRoboInput.h"
#include "KTRoboTexture.h"
#include "KTRoboText.h"
#include "lua.hpp"
#include "stringconverter.h"
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

#define KTROBO_GUI_TAB_PNG "resrc/sample/none.png"
#define KTROBO_GUI_TAB_WIDTH 60
#define KTROBO_GUI_TAB_HEIGHT 40


#define KTROBO_GUI_SLIDERNOW_NORMAL_LEFT 0
#define KTROBO_GUI_SLIDERNOW_NORMAL_WIDTH 129
#define KTROBO_GUI_SLIDERNOW_NORMAL_TOP 0
#define KTROBO_GUI_SLIDERNOW_NORMAL_HEIGHT 129
#define KTROBO_GUI_SLIDERNOW_FOCUS_LEFT 131
#define KTROBO_GUI_SLIDERNOW_FOCUS_WIDTH 129
#define KTROBO_GUI_SLIDERNOW_FOCUS_TOP 0
#define KTROBO_GUI_SLIDERNOW_FOCUS_HEIGHT 129
#define KTROBO_GUI_SLIDERNOW_PRESS_LEFT 261
#define KTROBO_GUI_SLIDERNOW_PRESS_WIDTH 129
#define KTROBO_GUI_SLIDERNOW_PRESS_TOP 0
#define KTROBO_GUI_SLIDERNOW_PRESS_HEIGHT 129


#define KTROBO_GUI_SLIDERHMIN_NORMAL_LEFT 12
#define KTROBO_GUI_SLIDERHMIN_NORMAL_WIDTH 113
#define KTROBO_GUI_SLIDERHMIN_NORMAL_TOP 375
#define KTROBO_GUI_SLIDERHMIN_NORMAL_HEIGHT 134

#define KTROBO_GUI_SLIDERHMIN_FOCUS_LEFT 393
#define KTROBO_GUI_SLIDERHMIN_FOCUS_WIDTH 113
#define KTROBO_GUI_SLIDERHMIN_FOCUS_TOP 2
#define KTROBO_GUI_SLIDERHMIN_FOCUS_HEIGHT 134

#define KTROBO_GUI_SLIDERHMIN_PRESS_LEFT 270
#define KTROBO_GUI_SLIDERHMIN_PRESS_WIDTH 113
#define KTROBO_GUI_SLIDERHMIN_PRESS_TOP 377
#define KTROBO_GUI_SLIDERHMIN_PRESS_HEIGHT 134

#define KTROBO_GUI_SLIDERHMAX_NORMAL_LEFT 127
#define KTROBO_GUI_SLIDERHMAX_NORMAL_WIDTH 113
#define KTROBO_GUI_SLIDERHMAX_NORMAL_TOP 376
#define KTROBO_GUI_SLIDERHMAX_NORMAL_HEIGHT 134

#define KTROBO_GUI_SLIDERHMAX_FOCUS_LEFT 395
#define KTROBO_GUI_SLIDERHMAX_FOCUS_WIDTH 113
#define KTROBO_GUI_SLIDERHMAX_FOCUS_TOP 171
#define KTROBO_GUI_SLIDERHMAX_FOCUS_HEIGHT 134

#define KTROBO_GUI_SLIDERHMAX_PRESS_LEFT 390
#define KTROBO_GUI_SLIDERHMAX_PRESS_WIDTH 113
#define KTROBO_GUI_SLIDERHMAX_PRESS_TOP 375
#define KTROBO_GUI_SLIDERHMAX_PRESS_HEIGHT 134


#define KTROBO_GUI_SLIDERVMAX_NORMAL_LEFT 0
#define KTROBO_GUI_SLIDERVMAX_NORMAL_WIDTH 130
#define KTROBO_GUI_SLIDERVMAX_NORMAL_TOP 131
#define KTROBO_GUI_SLIDERVMAX_NORMAL_HEIGHT 113

#define KTROBO_GUI_SLIDERVMAX_FOCUS_LEFT 132
#define KTROBO_GUI_SLIDERVMAX_FOCUS_WIDTH 130
#define KTROBO_GUI_SLIDERVMAX_FOCUS_TOP 132
#define KTROBO_GUI_SLIDERVMAX_FOCUS_HEIGHT 113

#define KTROBO_GUI_SLIDERVMAX_PRESS_LEFT 264
#define KTROBO_GUI_SLIDERVMAX_PRESS_WIDTH 130
#define KTROBO_GUI_SLIDERVMAX_PRESS_TOP 132
#define KTROBO_GUI_SLIDERVMAX_PRESS_HEIGHT 113


#define KTROBO_GUI_SLIDERVMIN_NORMAL_LEFT 0
#define KTROBO_GUI_SLIDERVMIN_NORMAL_WIDTH 130
#define KTROBO_GUI_SLIDERVMIN_NORMAL_TOP 131+115
#define KTROBO_GUI_SLIDERVMIN_NORMAL_HEIGHT 113
						   
#define KTROBO_GUI_SLIDERVMIN_FOCUS_LEFT 132
#define KTROBO_GUI_SLIDERVMIN_FOCUS_WIDTH 130
#define KTROBO_GUI_SLIDERVMIN_FOCUS_TOP 132+115
#define KTROBO_GUI_SLIDERVMIN_FOCUS_HEIGHT 113
						   
#define KTROBO_GUI_SLIDERVMIN_PRESS_LEFT 264
#define KTROBO_GUI_SLIDERVMIN_PRESS_WIDTH 130
#define KTROBO_GUI_SLIDERVMIN_PRESS_TOP 132+115
#define KTROBO_GUI_SLIDERVMIN_PRESS_HEIGHT 113

#define KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT 20

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
		box.left = 0;
		box.right = 0;
		box.top = 0;
		box.bottom = 0;
	}
	virtual ~GUI_PART(){}
public:
	static void SetDefaultMaxBox(MYRECT* re) {
		max_default_box = *re;
	}
	virtual void moveBox(int dx, int dy);
	bool getIsRender() {return is_render;}
	bool getIsEffect() {return is_effect;}
	bool getIsMove() {return is_move;}
	void setIsMove(bool t) {is_move = t;}
	virtual void setIsRender(bool t)=0;
	virtual void setIsEffect(bool t)=0;
	MYRECT* getBox() {return &box;}
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
	void moveBox(int dx, int dy);
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

	void moveBox(int dx, int dy);


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



class GUI_TEXT : public GUI_PART, public HasRenderFunc
{
private:
	static Texture* tex;
	Text* text;
public:
	static void Init(Texture* te) {
		tex = te;
	}
	bool handleMessage(int msg, void* data, DWORD time){return false;};
	void setIsEffect(bool t) {is_effect = t;}
	void setIsRender(bool t) {is_render = t;}

	GUI_TEXT(float x, float y, float width, float height, WCHAR* tex, int len);
	~GUI_TEXT();
	void render(Graphics* g);

};

class GUI_TEX : public GUI_PART // GUIのテクスチャ
{
private:
	static Texture* tex;
	int tex_id;

public:
	static void Init(Texture* te) {
		tex = te;
	}
	void moveBox(int dx, int dy) {
		GUI_PART::moveBox(dx, dy);
		tex->setRenderTexPos(tex_id, box.left, box.top);
	}

	GUI_TEX(char* tex_name, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height) {
		int te = tex->getTexture(tex_name);
		tex_id = tex->getRenderTex(te,0xFFFFFFFF,(int)x,(int)y,(int)width,(int)height,(int)tex_x,(int)tex_y,(int)tex_width,(int)tex_height);
	}

	~GUI_TEX() {
		tex->lightdeleteRenderTex(tex_id);
	}

	bool handleMessage(int msg, void* data, DWORD time){return true;};
	void setIsEffect(bool t) {is_effect =t;}
	void setIsRender(bool t) {is_render =t;}
};

class GUI_WINDOW : public GUI_PART, public HasRenderFunc
{
private:

	vector<GUI_PART*> bodys; // bodys renders に関してはwindowのデストラクタが呼ばれてもデストラクトしない
	vector<HasRenderFunc*> renders;

	static Texture* tex;
public:

	static void Init(Texture* te) {
		tex = te;
	}
public:
	GUI_WINDOW(int x, int y, int width, int height) {
		box.left = x;
		box.top = y;
		box.right = x + width;
		box.bottom = y + height;
	}

	void setBody(GUI_PART* p) {
		bodys.push_back(p);
	}
	void setRender(HasRenderFunc* f) {
		renders.push_back(f);
	}
	
	bool handleMessage(int msg, void* data, DWORD time) {
		MYINPUTMESSAGESTRUCT* d = (MYINPUTMESSAGESTRUCT*)data;
		if (!is_effect) {return true;}

		if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSEMOVE) {
			if (getIsMove()) {
				// 動かす
				moveBox(d->getMOUSESTATE()->mouse_dx, d->getMOUSESTATE()->mouse_dy);
				vector<GUI_PART*>::iterator it = bodys.begin();
				while(it != bodys.end()) {
					GUI_PART* p = *it;
					p->moveBox(d->getMOUSESTATE()->mouse_dx, d->getMOUSESTATE()->mouse_dy);
					it++;
				}
				return true;
			}
		}
		if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSERAWSTATE) {
			if (d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_R_DOWN) {
				unsigned int butukari = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &box);
				if (butukari & BUTUKARIPOINT_IN) {
					setIsMove(true);
				}
			}
			if (d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_R_UP) {
				setIsMove(false);
			}
		}

		vector<GUI_PART*>::iterator it = bodys.begin();
		while (it != bodys.end()) {

			GUI_PART* par = *it;
			
			if (par->handleMessage(msg, data, time)) {
				return true;
			}

			it = it + 1;
		}
		
		return true;// 次の子のウィンドウには渡さない
	};

	void setIsEffect(bool t) {
		is_effect =t;
		vector<GUI_PART*>::iterator it = bodys.begin();
		while (it != bodys.end()) {
			GUI_PART* p = *it;
			p->setIsEffect(t);
			it = it+1;
		}
	}
	void setIsRender(bool t) {is_render =t;
	
		vector<GUI_PART*>::iterator it = bodys.begin();
		while (it != bodys.end()) {

			GUI_PART* p = *it;
			p->setIsRender(t);
			it = it+1;
		}
	
	
	}
	void render(Graphics* g) {
		if (is_render) {
			vector<HasRenderFunc*>::iterator it = renders.begin();
			while (it != renders.end()) {
				
				HasRenderFunc* f = *it;
				f->render(g);
				it = it + 1;
			}
		}
	}
};


class GUI_TAB : public GUI_PART, public HasRenderFunc
{
private:
	int tab_index; //　ルートから何番目のタブかということ
	vector<Text*> window_names;// デストラクタが呼ばれる
	vector<GUI_WINDOW*> child_windows;// tabのデストラクトが呼ばれてもwindowをデストラクトはされない
	vector<int> tex_rects; 
	vector<MYRECT> tex_rect_boxs;

	int now_index; // 現在注目されているウィンドウのインデックス

	static Texture* tex;
	static unsigned int colors[8];


	static unsigned int f_colors[8];

public:
	GUI_TAB(int tab_index) {
		this->tab_index = tab_index;
		now_index = 0;
	}
	~GUI_TAB() {
		vector<Text*>::iterator it = window_names.begin();
		while(it != window_names.end()) {
			Text* t = *it;
			if (t) {
				delete t;
				t = 0;
			}
			it = it + 1;
		}
	}
	void setWindow(GUI_WINDOW* c, string name) {
		c->setIsRender(false);
		c->setIsEffect(false);
		
		child_windows.push_back(c);
		stringconverter sc;
		WCHAR buf[512];
		memset(buf,0,sizeof(WCHAR)*512);
		sc.charToWCHAR(name.c_str(), buf);
		Text* t = new Text(buf, wcslen(buf));
		window_names.push_back(t);
		
		// tex_rectsにMYRECT int を入れる
		int tsize = tex_rects.size();
		int tex_i = tex->getTexture(KTROBO_GUI_PNG);
		int inde = tsize % 8;

		int tex_id = tex->getRenderTex(tex_i,colors[inde], KTROBO_GUI_TAB_WIDTH* tsize, KTROBO_GUI_TAB_HEIGHT*2*tab_index, KTROBO_GUI_TAB_WIDTH,
			KTROBO_GUI_TAB_HEIGHT, 0, 0, 1 , 1);
		MYRECT r;
		r.left = KTROBO_GUI_TAB_WIDTH*tsize;
		r.right = r.left + KTROBO_GUI_TAB_WIDTH;
		r.top = KTROBO_GUI_TAB_HEIGHT*2*tab_index;
		r.bottom = r.top + KTROBO_GUI_TAB_HEIGHT;
		tex_rects.push_back(tex_id);
		tex_rect_boxs.push_back(r);
	}

	static void Init(Texture* te) {
		tex = te;
		
		colors[0] =	0xFFFFFF77;
		colors[1] =	0xFFFF0077;
		colors[2] =	0xFF00FF77;
		colors[3] =	0x00FFFF77;
		colors[4] =	0xFF000077;
		colors[5] =	0x00FF0077;
		colors[6] =	0x0000FF77;
		colors[7] =	0x00000077;

		f_colors[0] =	0xFFFFFFFF;
		f_colors[1] =	0xFFFF00FF;
		f_colors[2] =	0xFF00FFFF;
		f_colors[3] =	0x00FFFFFF;
		f_colors[4] =	0xFF0000FF;
		f_colors[5] =	0x00FF00FF;
		f_colors[6] =	0x0000FFFF;
		f_colors[7] =	0x000000FF;
		
	}

	bool handleMessage(int msg, void* data, DWORD time);
	void setIsEffect(bool t) {
		int wsize = child_windows.size();
		for (int i=0;i<wsize;i++) {
			child_windows[i]->setIsEffect(false);
			tex->setRenderTexColor(tex_rects[i], colors[i%8]);
		}
		child_windows[now_index]->setIsEffect(t);
		tex->setRenderTexColor(tex_rects[now_index], f_colors[now_index%8]);
	}
	void setIsRender(bool t) {
		int wsize = child_windows.size();
		for (int i=0;i<wsize;i++) {
			child_windows[i]->setIsRender(false);
		}
		child_windows[now_index]->setIsRender(t);
	}
	void render(Graphics* g) {
		// now_indexのwindow_nameを呼ぶ
		if (window_names.size()) {
			Text* te = window_names[now_index];
			MYRECT r = tex_rect_boxs[now_index];
			if (is_render) {
				te->render(g, 0xFFFFFFFF,r.left, r.right,r.bottom-r.top, te->getWidth(r.bottom-r.top) , r.bottom - r.top);
						
			}
		}
	}
};

class GUI_SLIDERV : public GUI_PART {

private:
	MYRECT zentai_box;
	MYRECT min_box;
	MYRECT max_box;
	float max;
	float min;
	float now;
	char l_str[64];
	static lua_State* l;
	static Texture* tex;
	bool is_min_pressed;
	bool is_max_pressed;
	bool is_box_moved;
	int tex_id_min;
	int tex_id_max;
	int tex_id_now;
public:
	GUI_SLIDERV(MYRECT zentai, float max, float min, float now, char* l_str);
	~GUI_SLIDERV();
	
	void moveBox(int dx, int dy);
	bool handleMessage(int msg, void* data, DWORD time);
	void setIsEffect(bool t) {is_effect = t;}
	void setIsRender(bool t) {
		is_render = t;
		tex->setRenderTexIsRender(tex_id_min,t);
		tex->setRenderTexIsRender(tex_id_max,t);
		tex->setRenderTexIsRender(tex_id_now,t);
	}

	static void Init(Texture* te,lua_State* ls) {
		tex = te;
		l = ls;
	}
};

class GUI_SLIDERH : public GUI_PART {

private:
	MYRECT zentai_box;
	MYRECT min_box;
	MYRECT max_box;
	float max;
	float min;
	float now;
	char l_str[64];
	static Texture* tex;
	static lua_State* l;
	bool is_min_pressed;
	bool is_max_pressed;
	bool is_box_moved;
	int tex_id_min;
	int tex_id_max;
	int tex_id_now;
public:
	GUI_SLIDERH(MYRECT zentai, float max, float min, float now, char* l_str);
	~GUI_SLIDERH();
	void moveBox(int dx, int dy);
	

	bool handleMessage(int msg, void* data, DWORD time) { return true;};
	void setIsEffect(bool t) {
		is_effect = t;
	}

	void setIsRender(bool t) {
		is_render = t;
		tex->setRenderTexIsRender(tex_id_min,t);
		tex->setRenderTexIsRender(tex_id_max,t);
		tex->setRenderTexIsRender(tex_id_now,t);
	}
	static void Init(Texture* te, lua_State* ls) {
		tex = te;
		l = ls;

	}

};



class GUI
{
public:
	GUI(void);
	~GUI(void);

	static void Init(HWND hw, Texture* tex, lua_State* Ld, int screen_width, int screen_height) {
		GUI_BUTTON::Init(Ld, tex);
		GUI_SLIDERH::Init(tex, Ld);
		GUI_SLIDERV::Init(tex, Ld);
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