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
#include "memory.h"
#include "MyTokenAnalyzer.h"

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
#define KTROBO_GUI_TAB_WIDTH 24
#define KTROBO_GUI_TAB_HEIGHT 16


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

#define KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT 16



class GUI_PART : public INPUTSHORICLASS{
protected:
	bool is_render;
	bool is_effect;
	bool is_move; // マウス右ボタンをクリックしながら動かすとなんでも移動させられる
	bool is_disabled;
protected:
	MYRECT box;
	MYRECT max_boxdayo;
	static MYRECT max_default_box;
	GUI_PART() {
		is_render = false;
		is_effect = false;
		is_move = false;
		is_disabled = false;
		max_boxdayo = max_default_box;
		box.left = 0;
		box.right = 0;
		box.top = 0;
		box.bottom = 0;
	}
	
public:
	virtual ~GUI_PART(){}
	static void SetDefaultMaxBox(MYRECT* re) {
		max_default_box = *re;
	}
	bool moveBoxSitei(MYRECT* rect, MYRECT* maxb, int dx, int dy) {
		bool ans = false;
		rect->left += dx;
		rect->right += dx;
		rect->top += dy;
		rect->bottom += dy;

		if (rect->left < maxb->left) {
			rect->right += maxb->left-rect->left;
			rect->left = maxb->left;
			ans = true;
		}
		if (rect->top < maxb->top) {
			rect->bottom += maxb->top-rect->top;
			rect->top = maxb->top;
			ans = true;
		}
		if (rect->right > maxb->right) {
			rect->left += maxb->right - rect->right;
			rect->right = maxb->right;
			ans = true;
		}
		if (rect->bottom > maxb->bottom) {
			rect->top += maxb->bottom - rect->bottom;
			rect->bottom = maxb->bottom;
			ans = true;
		}
		return ans;
	}
	virtual bool moveBox(int dx, int dy); // 境界線にいっているかどうか
	bool getIsRender() {return is_render;}
	bool getIsEffect() {return is_effect;}
	bool getIsMove() {return is_move;}
	bool getIsDisabled() {return is_disabled;}
	void setIsDisabled(bool t) {is_disabled = t;}
	virtual void setIsMove(bool t) {is_move = t;}
	virtual void setIsRender(bool t)=0;
	virtual void setIsEffect(bool t)=0;
	MYRECT* getBox() {return &box;}
	virtual bool handleMessage(int msg, void* data, DWORD time) =0;
	
};


interface IGUI {
public:
	TO_LUA virtual int makeButton(float x, float y, float width, float height, char* luaf, int len, char* info)=0;
	TO_LUA virtual int makeInputText(float x, float y, float width, float height)=0;
	TO_LUA virtual int makeText(float x, float y,float width, float height, char* textd)=0;
	TO_LUA virtual int makeTex(char* tex_name, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height)=0;
	TO_LUA virtual int makeWindow(int x, int y, int width, int height)=0;
	TO_LUA virtual int makeTab(int tab_index, char* s)=0;
	TO_LUA virtual int makeSliderV(YARITORI MYRECT* zentai, float max, float min, float now, char* l_str)=0;
	TO_LUA virtual int makeSliderH(YARITORI MYRECT* zentai, float max, float min, float now, char* l_str)=0;

	TO_LUA virtual void setEffect(int gui_id, bool t)=0;
	TO_LUA virtual void setRender(int gui_id, bool t)=0;
	TO_LUA virtual void setEnable(int gui_id, bool t)=0;

	TO_LUA virtual void setPartToWindow(int window_gui_id, int part_gui_id)=0;
	TO_LUA virtual int setWindowToTab(int tab_gui_id, int window_gui_id, char* window_name)=0;
	TO_LUA virtual void setTabIndex(int tab_gui_id, int index)=0;
	TO_LUA virtual int getTabIndex(int tab_gui_id)=0;

	TO_LUA virtual char* getStrFromInput(int gui_id)=0;
	TO_LUA virtual float getNowFromSlider(int gui_id)=0;
	TO_LUA virtual float getMaxFromSlider(int gui_id)=0;
	TO_LUA virtual float getMinFromSlider(int gui_id)=0;
	TO_LUA virtual void setNOWMAXMINToSlider(int gui_id, float max, float min, float now)=0;

	TO_LUA virtual void setRootWindowToInputMessageDispatcher(int gui_window_id)=0; // 一番上につっこむ
	TO_LUA virtual void setLeafWindowToInputMessageDispatcher(int gui_window_id)=0; // 一番下につっこむ
	TO_LUA virtual void unregisterWindowToInputMessageDispatcher(int gui_window_id)=0;
	TO_LUA virtual void deleteAll()=0; // ウィンドウのみunregisterする

};

class GUI_BUTTON : public GUI_PART
{
private:

	bool is_call_lua;
	bool is_lua_tried_to_send; // 変更タイミング


	int box_tex_id;
	char l_str[128]; // pressされたときによばれるLuaファイル
	int button_text;
static lua_State* L; // handlemessageが呼ばれるのは AIスレッドなのでAIスレッドのLを呼ぶ
static Texture* texture;

public:

	void setIsCallLua(bool t) {
		is_call_lua = t;
	}
	bool getIsLuaTriedToSend() {
		if (is_lua_tried_to_send) {
			is_lua_tried_to_send = false;
			return true;
		}
		return false;//is_lua_tried_to_send;
	}


	static void Init(lua_State* Ld, Texture* tex) {
		L = Ld;
		texture = tex;
	}
	bool moveBox(int dx, int dy);
	bool handleMessage(int msg, void* data, DWORD time);
	void setIsEffect(bool t);
	void setIsRender(bool t);
	
	GUI_BUTTON(float x, float y, float width, float height, char* luaf, int len, char* info);
	~GUI_BUTTON();
};


class GUI_INPUTTEXT : public GUI_PART
{
private:

	Text* text;
	int input_text;

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

	bool moveBox(int dx, int dy);


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
	
	char* getStr() {
		return text->getStr();
	}
};



class GUI_TEXT : public GUI_PART
{
private:
	static Texture* tex;
	int text;
public:
	static void Init(Texture* te) {
		tex = te;
	}
	bool handleMessage(int msg, void* data, DWORD time){return false;};
	void setIsEffect(bool t) {is_effect = t;}
	void setIsRender(bool t) {is_render = t;}

	GUI_TEXT(float x, float y, float width, float height, char* tex);
	~GUI_TEXT();
	

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
	bool moveBox(int dx, int dy) {
		bool t = GUI_PART::moveBox(dx, dy);
		tex->setRenderTexPos(tex_id, box.left, box.top);
		return t;
	}

	GUI_TEX(char* tex_name, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height) : GUI_PART() {
		int te = tex->getTexture(tex_name,1024);
		tex_id = tex->getRenderTex(te,0xFFFFFFFF,(int)x,(int)y,(int)width,(int)height,(int)tex_x,(int)tex_y,(int)tex_width,(int)tex_height);
	}

	~GUI_TEX() {
		tex->lightdeleteRenderTex(tex_id);
	}

	bool handleMessage(int msg, void* data, DWORD time){return false;};
	void setIsEffect(bool t) {is_effect =t;}
	void setIsRender(bool t) {is_render =t;}
};

class GUI_WINDOW : public GUI_PART
{
private:

	vector<GUI_PART*> bodys; // bodys に関してはwindowのデストラクタが呼ばれてもデストラクトしない

	static Texture* tex;
public:

	static void Init(Texture* te) {
		tex = te;
	}
public:
	GUI_WINDOW(int x, int y, int width, int height):  GUI_PART() {
		box.left = x;
		box.top = y;
		box.right = x + width;
		box.bottom = y + height;
	}

	void setBody(GUI_PART* p) {
		bodys.push_back(p);
	}
	void clearBody() {
		bodys.clear();
	}
	bool moveBox(int dx, int dy);
	bool handleMessage(int msg, void* data, DWORD time) {
		MYINPUTMESSAGESTRUCT* d = (MYINPUTMESSAGESTRUCT*)data;
		if (getIsDisabled()) return false;
		if (!is_effect) {return false;}

		if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSEMOVE) {
			if (getIsMove()) {
				// 動かす
				if (!moveBox(d->getMOUSESTATE()->mouse_dx, d->getMOUSESTATE()->mouse_dy)) {
				//bool t = moveBox(d->getMOUSESTATE()->mouse_dx, d->getMOUSESTATE()->mouse_dy);
				vector<GUI_PART*>::iterator it = bodys.begin();
				while(it != bodys.end()) {
					GUI_PART* p = *it;
					p->moveBox(d->getMOUSESTATE()->mouse_dx, d->getMOUSESTATE()->mouse_dy);
					it++;
				}
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
			// すべてのパートに渡す
			
			if (par->handleMessage(msg, data, time)) {
			//	return true;
			}

			it = it + 1;
		}
		
		return true;// 次の子のウィンドウには渡さない
	};

	void setIsEffect(bool t) {
		CS::instance()->enter(CS_RENDERDATA_CS, "setiseffect window");
		is_effect =t;
		vector<GUI_PART*>::iterator it = bodys.begin();
		while (it != bodys.end()) {
			GUI_PART* p = *it;
			p->setIsEffect(t);
			it = it+1;
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "setiseffect window");
	}
	void setIsRender(bool t) {
		
		CS::instance()->enter(CS_RENDERDATA_CS, " setisrender window");
		is_render =t;
	
		vector<GUI_PART*>::iterator it = bodys.begin();
		while (it != bodys.end()) {

			GUI_PART* p = *it;
			p->setIsRender(t);
			it = it+1;
		}
	
		CS::instance()->leave(CS_RENDERDATA_CS, " setisrender window");
	}
	/*
	void render(Graphics* g) {
		CS::instance()->enter(CS_RENDERDATA_CS, "render window");
		if (is_render) {
			vector<GUI_PART*>::iterator it = bodys.begin();
			while (it != bodys.end()) {
				
				GUI_PART* f = *it;
				f->render(g);
				it = it + 1;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "render window");
	}*/
};


class GUI_TAB : public GUI_PART
{
private:
	int tab_index; //　ルートから何番目のタブかということ
	vector<int> window_names;// デストラクタが呼ばれる
	vector<GUI_WINDOW*> child_windows;// tabのデストラクトが呼ばれてもwindowをデストラクトはされない
	vector<int> tex_rects; //デストラクトを呼ぶ
	vector<MYRECT> tex_rect_boxs;

	int now_index; // 現在注目されているウィンドウのインデックス
	char l_str[128];
	static Texture* tex;
	static unsigned int colors[8];


	static unsigned int f_colors[8];

public:
	void setNowIndex(int i) {
		if (i >=0 && i < tex_rect_boxs.size()) {
			now_index = i;
			/*setIsEffect(true);*/setIsEffect(is_effect);
			/*setIsRender(true);*/setIsRender(is_render);
		}
	}
	int getNowIndex() {
		return now_index;
	}

	GUI_TAB(int tab_index, char* l_s) : GUI_PART() {
		this->tab_index = tab_index;
		memset(l_str,0,128);
		hmystrcpy(l_str,128,0,l_s);
		now_index = 0;
		is_effect = true;
		is_render =true;
	}
	~GUI_TAB() {
		vector<int>::iterator it = window_names.begin();
		while(it != window_names.end()) {
			int t = *it;
			/*if (t) {
				delete t;
				t = 0;
			}*/
			tex->lightdeleteRenderText(t);
			it = it + 1;
		}

		it = tex_rects.begin();
		while(it != tex_rects.end()) {
			int t = *it;
			tex->lightdeleteRenderTex(t);
			it = it + 1;
		}
		tex_rects.clear();
		window_names.clear();
	}
	int setWindow(GUI_WINDOW* c, string name) {
		//c->setIsRender(false);
		//c->setIsEffect(false);
		
		child_windows.push_back(c);
		
		
		// tex_rectsにMYRECT int を入れる
		int tsize = tex_rects.size();
		int tex_i = tex->getTexture("resrc/sample/none.png",4096);
		int inde = tsize % 8;
		int place_x_max  = tex->loader->g->getScreenWidth()/ KTROBO_GUI_TAB_WIDTH;
		int place = tsize;
		int place_x =  place % place_x_max;
		int place_y = place / place_x_max;
		int tex_id = tex->getRenderTex(tex_i,colors[inde], KTROBO_GUI_TAB_WIDTH* place_x, KTROBO_GUI_TAB_HEIGHT*2*tab_index+ place_y*KTROBO_GUI_TAB_HEIGHT, KTROBO_GUI_TAB_WIDTH,
			KTROBO_GUI_TAB_HEIGHT, 0, 0, 1 , 1);
		tex->setRenderTexIsRender(tex_id,true);
		MYRECT r;
		r.left = place_x * KTROBO_GUI_TAB_WIDTH;
		r.right = r.left + KTROBO_GUI_TAB_WIDTH;
		r.top = KTROBO_GUI_TAB_HEIGHT*2*tab_index+place_y*KTROBO_GUI_TAB_HEIGHT;
		r.bottom = r.top + KTROBO_GUI_TAB_HEIGHT;
		tex_rects.push_back(tex_id);
		tex_rect_boxs.push_back(r);

		char namet[512];
		strcpy_s(namet,512,name.c_str());
		namet[strlen(name.c_str())] = '\0';
		int t = tex->getRenderText(namet, r.left,r.top,r.bottom-r.top, r.right-r.left, r.bottom - r. top); 
		window_names.push_back(t);
		//setNowIndex(window_names.size()-1);
		return window_names.size()-1;
		
	}

	static void Init(Texture* te) {
		tex = te;
		
		colors[0] =	0xFFFFFF33;
		colors[1] =	0xFFFF0033;
		colors[2] =	0xFF00FF33;
		colors[3] =	0x00FFFF33;
		colors[4] =	0xFF000033;
		colors[5] =	0x00FF0033;
		colors[6] =	0x0000FF33;
		colors[7] =	0x00000033;

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
		if (wsize) {
		child_windows[now_index]->setIsEffect(t);
		tex->setRenderTexColor(tex_rects[now_index], f_colors[now_index%8]);
		}
	}
	void setIsRender(bool t) {
		int wsize = child_windows.size();
		for (int i=0;i<wsize;i++) {
			child_windows[i]->setIsRender(false);
			tex->setRenderTextIsRender(window_names[i],true);
		}
		if (wsize) {
		child_windows[now_index]->setIsRender(t);
		tex->setRenderTextIsRender(window_names[now_index], t);
		int tsize = tex_rects.size();
		for (int i=0;i<tsize;i++) {
			tex->setRenderTexIsRender(tex_rects[i],t);
		}
		}
	}
	/*
	void render(Graphics* g) {
		// now_indexのwindow_nameを呼ぶ
		if (window_names.size()) {
			Text* te = window_names[now_index];
			MYRECT r = tex_rect_boxs[now_index];
			if (is_render) {
				te->render(g, 0xFFFFFFFF,r.left, r.right,r.bottom-r.top, te->getWidth(r.bottom-r.top) , r.bottom - r.top);
						
			}
		}
	}*/
};

class GUI_SLIDERV : public GUI_PART {

private:
	bool is_call_lua;
	bool is_lua_tried_to_send; // 変更タイミング


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
	float getTexIdNowY();
	void setMINMAXNOWFROMZENTAI();
public:
	void setIsCallLua(bool t) {
		is_call_lua = t;
	}
	bool getIsLuaTriedToSend() {
		if (is_lua_tried_to_send) {
			is_lua_tried_to_send = false;
			return true;
		}
		return false;//is_lua_tried_to_send;
	}

	GUI_SLIDERV(MYRECT zentai, float max, float min, float now, char* l_str);
	~GUI_SLIDERV();
	float getMax() {return max;}
	float getMin() {return min;}
	float getNow() {return now;}
	void setMAXMINNOW(float max, float min, float now);
	bool moveBox(int dx, int dy);
	void moveAllBox(int dx, int dy);
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
	bool is_call_lua;
	bool is_lua_tried_to_send; // 変更タイミング

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
	float getTexIdNowX();
	void setMINMAXNOWFROMZENTAI();
public:
	void setIsCallLua(bool t) {
		is_call_lua = t;
	}
	bool getIsLuaTriedToSend() {
		if (is_lua_tried_to_send) {
			is_lua_tried_to_send = false;
			return true;
		}
		return false;//is_lua_tried_to_send;
	}

	GUI_SLIDERH(MYRECT zentai, float max, float min, float now, char* l_str);
	~GUI_SLIDERH();
	bool moveBox(int dx, int dy);
	void moveAllBox(int dx, int dy);
	float getMax() {return max;}
	float getMin() {return min;}
	float getNow() {return now;}
	void setMAXMINNOW(float max, float min, float now);
	bool handleMessage(int msg, void* data, DWORD time);
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



class GUI : public IGUI
{
private:
	vector<GUI_PART*> parts;
	
	vector<GUI_BUTTON*> buttons;
	vector<GUI_INPUTTEXT*> inputtexts;
	vector<GUI_SLIDERH*> sliderhs;
	vector<GUI_SLIDERV*> slidervs;
	vector<GUI_TEXT*> texts;
	vector<GUI_TEX*> texs;
	vector<GUI_WINDOW*> windows;
	vector<GUI_TAB*> tabs;

	map<int,int> p_buttons_index;
	map<int,int> p_inputtexts_index;
	map<int,int> p_sliderhs_index;
	map<int,int> p_slidervs_index;
	map<int,int> p_texts_index;
	map<int,int> p_texs_index;
	map<int,int> p_windows_index;
	map<int,int> p_tabs_index;
	
	GUI_WINDOW* registered_rootwindow;
public:
	void Release();

	int makeButton(float x, float y, float width, float height, char* luaf, int len, char* info);
	int makeInputText(float x, float y, float width, float height);
	int makeText(float x, float y,float width, float height, char* textd);
	int makeTex(char* tex_name, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height);
	int makeWindow(int x, int y, int width, int height);
	int makeTab(int tab_index, char* s);
	int makeSliderV(YARITORI MYRECT* zentai, float max, float min, float now, char* l_str);
	int makeSliderH(YARITORI MYRECT* zentai, float max, float min, float now, char* l_str);

	void setEffect(int gui_id, bool t);
	void setRender(int gui_id, bool t);

	void setPartToWindow(int window_gui_id, int part_gui_id);
	int setWindowToTab(int tab_gui_id, int window_gui_id, char* window_name);
	int getTabIndex(int tab_gui_id);

	char* getStrFromInput(int gui_id);
	float getNowFromSlider(int gui_id);
	float getMaxFromSlider(int gui_id);
	float getMinFromSlider(int gui_id);
	bool getTriedToSendFromSlider(int gui_id);
	bool getTriedToSendFromButton(int gui_id);
	void setCallLuaToSlider(int gui_id, bool t);
	void setCallLuaToButton(int gui_id, bool t);

	void setNOWMAXMINToSlider(int gui_id, float max, float min, float now);
	void setTabIndex(int tab_gui_id, int index);
	void setRootWindowToInputMessageDispatcher(int gui_window_id);
	void unregisterWindowToInputMessageDispatcher(int gui_window_id);
	void setLeafWindowToInputMessageDispatcher(int gui_window_id); // 一番下につっこむ
	void deleteAll();
	void setEnable(int gui_id, bool t);
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



class GUIs {
private:
	vector<GUI*> guis;
public:
	int makeInst();
	

	GUI* getInstance(int index);

	IGUI* getInterface(int index);

	GUIs() {}
	~GUIs() {
		Release();
	}

	void Release() {
		int size = guis.size();
		for (int i= 0; i<size;i++) {
			guis[i]->Release();
			delete guis[i];
			guis[i] = 0;
		}
		guis.clear();
	}
};
}

#endif