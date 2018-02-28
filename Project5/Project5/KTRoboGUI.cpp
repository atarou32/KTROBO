#include "KTRoboGUI.h"
#include "MyButukari.h"
#include "MyTokenAnalyzer.h"
#include "stringconverter.h"
#include "KTRoboLuaCollection.h"

using namespace KTROBO;

GUI::GUI(void)
{
	registered_rootwindow = 0;
}


GUI::~GUI(void)
{
	Release();
}


void GUI::Release() {
/*	
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
*/

	
	if (registered_rootwindow) {
		InputMessageDispatcher::unregisterImpl(registered_rootwindow);
	}


	CS::instance()->enter(CS_RENDERDATA_CS, "release renderguiclass");
	// ウィンドウを解除する
	int bsize = windows.size();
	CS::instance()->leave(CS_RENDERDATA_CS, "release renderguiclsass");
	for (int i=0;i<bsize;i++) {
		// bsizeは上下しない状況でdeleteallが呼ばれると考える
		InputMessageDispatcher::unregisterImpl(windows[i]);
		windows[i]->clearBody();
	}
	CS::instance()->enter(CS_RENDERDATA_CS, "release renderguiclass");
	registered_rootwindow = 0;
	



	buttons.clear();
	inputtexts.clear();
	sliderhs.clear();
	slidervs.clear();
	texts.clear();
	texs.clear();
	windows.clear();
	tabs.clear();

	p_buttons_index.clear();
	p_inputtexts_index.clear();
	p_sliderhs_index.clear();
	p_slidervs_index.clear();
	p_texts_index.clear();
	p_texs_index.clear();
	p_windows_index.clear();
	p_tabs_index.clear();

	vector<GUI_PART*>::iterator it = parts.begin();
	while(it != parts.end()) {
		GUI_PART* p = *it;
		if (p) {
		delete p;
		p = 0;
		}
		it++;
	}

	parts.clear();

	CS::instance()->leave(CS_RENDERDATA_CS, "release renderguiclsass");

}

int GUI::makeButton(float x, float y, float width, float height, char* luaf, int len, char* info) {
	GUI_BUTTON * but = new GUI_BUTTON(x,y,width,height,luaf,len,info);
	int size = parts.size();
	int bsize = buttons.size();
	parts.push_back(but);
	buttons.push_back(but);
	p_buttons_index.insert(pair<int,int>(size, bsize));
	return size;

}

int GUI::makeInputText(float x, float y, float width, float height) {
	GUI_INPUTTEXT * tex = new GUI_INPUTTEXT(x,y,width,height);
	int size = parts.size();
	int bsize = inputtexts.size();
	parts.push_back(tex);
	inputtexts.push_back(tex);
	p_inputtexts_index.insert(pair<int,int>(size,bsize));
	return size;
}

int GUI::makeText(float x, float y,float width, float height, char* textd) {
	GUI_TEXT* tex = new GUI_TEXT(x,y,width,height,textd);
	int size = parts.size();
	int bsize = texts.size();
	parts.push_back(tex);
	texts.push_back(tex);
	p_texts_index.insert(pair<int,int>(size,bsize));
	return size;
}


int GUI::makeTex(char* tex_name, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height) {

	GUI_TEX* tex = new GUI_TEX(tex_name, x, y, width, height, tex_x, tex_y, tex_width, tex_height);
	int size = parts.size();
	int bsize = texs.size();
	parts.push_back(tex);
	texs.push_back(tex);
	p_texs_index.insert(pair<int,int>(size,bsize));
	return size;
}
int GUI::makeWindow(int x, int y, int width, int height) {
	GUI_WINDOW* win = new GUI_WINDOW(x,y,width,height);
	int size = parts.size();
	int bsize = windows.size();
	parts.push_back(win);
	windows.push_back(win);
	p_windows_index.insert(pair<int,int>(size,bsize));
	return size;
}

int GUI::makeTab(int tab_index,char *s) {
	GUI_TAB* t = new GUI_TAB(tab_index,s );
	int size = parts.size();
	int bsize = tabs.size();
	parts.push_back(t);
	tabs.push_back(t);
	p_tabs_index.insert(pair<int,int>(size,bsize));
	return size;


}
int GUI::makeSliderV(YARITORI MYRECT* zentai, float max, float min, float now, char* l_str) {

	GUI_SLIDERV* s = new GUI_SLIDERV(*zentai,max,min,now,l_str);
	int size = parts.size();
	int bsize = slidervs.size();
	parts.push_back(s);
	slidervs.push_back(s);
	p_slidervs_index.insert(pair<int,int>(size,bsize));
	return size;
}

int GUI::makeSliderH(YARITORI MYRECT* zentai, float max, float min, float now, char* l_str) {
	GUI_SLIDERH* s = new GUI_SLIDERH(*zentai, max,min,now,l_str);
	int size = parts.size();
	int bsize = sliderhs.size();
	parts.push_back(s);
	sliderhs.push_back(s);
	p_sliderhs_index.insert(pair<int,int>(size,bsize));
	return size;
}

void GUI::setEffect(int gui_id, bool t) {
	CS::instance()->enter(CS_MESSAGE_CS, "seteffect");
	int size = parts.size();
	if (gui_id >=0 && size > gui_id) {
		parts[gui_id]->setIsEffect(t);
	}
	CS::instance()->leave(CS_MESSAGE_CS, "seteffect");
}

void GUI::setRender(int gui_id, bool t) {
	CS::instance()->enter(CS_MESSAGE_CS, "seteffect");
	int size = parts.size();
	if (gui_id >=0 && size > gui_id) {
		parts[gui_id]->setIsRender(t);
	}
	CS::instance()->leave(CS_MESSAGE_CS, "seteffect");
}
void GUI::setNOWMAXMINToSlider(int gui_id, float max, float min, float now) {

	

	CS::instance()->enter(CS_MESSAGE_CS, "getmin");
	if (p_sliderhs_index.find(gui_id) != p_sliderhs_index.end()) {

		GUI_SLIDERH* s = sliderhs[p_sliderhs_index[gui_id]];
		s->setMAXMINNOW(max, min, now);
		CS::instance()->leave(CS_MESSAGE_CS, "getmin");
		return;
	}
	if (p_slidervs_index.find(gui_id) != p_slidervs_index.end()) {

		GUI_SLIDERV* s = slidervs[p_slidervs_index[gui_id]];
		s->setMAXMINNOW(max, min, now);
		CS::instance()->leave(CS_MESSAGE_CS, "getmin");
		return;
	}


	CS::instance()->leave(CS_MESSAGE_CS, "getmin");
}


void GUI_SLIDERH::setMAXMINNOW(float max, float min, float now) {

	this->max = max;
	this->min = min;
	this->now = now;
	setMINMAXNOWFROMZENTAI();

}


void GUI_SLIDERV::setMAXMINNOW(float max, float min, float now) {

	this->max = max;
	this->min = min;
	this->now = now;
	setMINMAXNOWFROMZENTAI();

}

void GUI::setPartToWindow(int window_gui_id, int part_gui_id) {

	if (window_gui_id == part_gui_id) return;
	CS::instance()->enter(CS_MESSAGE_CS, "push part to window");
	GUI_PART* part;
	if (part_gui_id < parts.size() && part_gui_id >=0) {
		part = parts[part_gui_id];
	} else {
		CS::instance()->leave(CS_MESSAGE_CS, "push part to window");
		return;
	}

	if (p_windows_index.find(window_gui_id) != p_windows_index.end()) {
		
		
		windows[p_windows_index[window_gui_id]]->setBody(part);
	
	}
	CS::instance()->leave(CS_MESSAGE_CS, "push part to window");
}

int GUI::setWindowToTab(int tab_gui_id, int window_gui_id, char* window_name) {
	int ans=0;
	if (tab_gui_id == window_gui_id) return 0;
	CS::instance()->enter(CS_MESSAGE_CS, "push part to tab");
	if (p_windows_index.find(window_gui_id) != p_windows_index.end()) {
		if (p_tabs_index.find(tab_gui_id) != p_tabs_index.end()) {
		
			GUI_TAB* t = tabs[p_tabs_index[tab_gui_id]];
			GUI_WINDOW* w = windows[p_windows_index[window_gui_id]];
			ans = t->setWindow(w,string(window_name));

		
		}
	}
	CS::instance()->leave(CS_MESSAGE_CS, "push part to tab");
	return ans;
}

char* GUI::getStrFromInput(int gui_id) {

	CS::instance()->enter(CS_MESSAGE_CS, "setstr");
	char* s;
	int size = parts.size();
	if (gui_id >=0 && size > gui_id) {
		if (p_inputtexts_index.find(gui_id) != p_inputtexts_index.end()) {
			s = inputtexts[p_inputtexts_index[gui_id]]->getStr();
			CS::instance()->leave(CS_MESSAGE_CS, "setstr");
			return s;
		}
	}
	CS::instance()->leave(CS_MESSAGE_CS, "setstr");
	return "none";
}

float GUI::getNowFromSlider(int gui_id) {
	CS::instance()->enter(CS_MESSAGE_CS, "getnow");
	if (p_sliderhs_index.find(gui_id) != p_sliderhs_index.end()) {

		GUI_SLIDERH* s = sliderhs[p_sliderhs_index[gui_id]];
		float ans = s->getNow();
		CS::instance()->leave(CS_MESSAGE_CS, "getnow");
		return ans;
	}
	if (p_slidervs_index.find(gui_id) != p_slidervs_index.end()) {

		GUI_SLIDERV* s = slidervs[p_slidervs_index[gui_id]];
		float ans = s->getNow();
		CS::instance()->leave(CS_MESSAGE_CS, "getnow");
		return ans;
	}


	CS::instance()->leave(CS_MESSAGE_CS, "getnow");
	return 0;

}
float GUI::getMaxFromSlider(int gui_id) {
	CS::instance()->enter(CS_MESSAGE_CS, "getmax");
	if (p_sliderhs_index.find(gui_id) != p_sliderhs_index.end()) {

		GUI_SLIDERH* s = sliderhs[p_sliderhs_index[gui_id]];
		float ans = s->getMax();
		CS::instance()->leave(CS_MESSAGE_CS, "getmax");
		return ans;
	}
	if (p_slidervs_index.find(gui_id) != p_slidervs_index.end()) {

		GUI_SLIDERV* s = slidervs[p_slidervs_index[gui_id]];
		float ans = s->getMax();
		CS::instance()->leave(CS_MESSAGE_CS, "getmax");
		return ans;
	}


	CS::instance()->leave(CS_MESSAGE_CS, "getmax");
	return 0;

}



void GUI::setCallLuaToButton(int gui_id, bool t) {
	CS::instance()->enter(CS_MESSAGE_CS, "setcalllua");
	if (p_buttons_index.find(gui_id) != p_buttons_index.end()) {

		GUI_BUTTON* s = buttons[p_buttons_index[gui_id]];
		s->setIsCallLua(t);
		CS::instance()->leave(CS_MESSAGE_CS, "setcalllua");
		return;
	}

	CS::instance()->leave(CS_MESSAGE_CS, "setcalllua");
	return;
}

void GUI::setCallLuaToSlider(int gui_id, bool t) {
	CS::instance()->enter(CS_MESSAGE_CS, "setcalllua");
	if (p_sliderhs_index.find(gui_id) != p_sliderhs_index.end()) {

		GUI_SLIDERH* s = sliderhs[p_sliderhs_index[gui_id]];
		s->setIsCallLua(t);
		CS::instance()->leave(CS_MESSAGE_CS, "setcalllua");
		return;
	}
	if (p_slidervs_index.find(gui_id) != p_slidervs_index.end()) {

		GUI_SLIDERV* s = slidervs[p_slidervs_index[gui_id]];
		s->setIsCallLua(t);
		CS::instance()->leave(CS_MESSAGE_CS, "setcalllua");
		return;
	}


	CS::instance()->leave(CS_MESSAGE_CS, "setcalllua");
	return;
}

bool GUI::getTriedToSendFromSlider(int gui_id) {
		CS::instance()->enter(CS_MESSAGE_CS, "getmin");
	if (p_sliderhs_index.find(gui_id) != p_sliderhs_index.end()) {

		GUI_SLIDERH* s = sliderhs[p_sliderhs_index[gui_id]];
		bool ans = s->getIsLuaTriedToSend();
		CS::instance()->leave(CS_MESSAGE_CS, "getmin");
		return ans;
	}
	if (p_slidervs_index.find(gui_id) != p_slidervs_index.end()) {

		GUI_SLIDERV* s = slidervs[p_slidervs_index[gui_id]];
		bool ans = s->getIsLuaTriedToSend();
		CS::instance()->leave(CS_MESSAGE_CS, "getmin");
		return ans;
	}


	CS::instance()->leave(CS_MESSAGE_CS, "getmin");
	return false;



}



bool GUI::getTriedToSendFromButton(int gui_id) {
		CS::instance()->enter(CS_MESSAGE_CS, "getmin");
	if (p_buttons_index.find(gui_id) != p_buttons_index.end()) {

		GUI_BUTTON* s = buttons[p_buttons_index[gui_id]];
		bool ans = s->getIsLuaTriedToSend();
		CS::instance()->leave(CS_MESSAGE_CS, "getmin");
		return ans;
	}

	CS::instance()->leave(CS_MESSAGE_CS, "getmin");
	return false;



}

float GUI::getMinFromSlider(int gui_id) {
	CS::instance()->enter(CS_MESSAGE_CS, "getmin");
	if (p_sliderhs_index.find(gui_id) != p_sliderhs_index.end()) {

		GUI_SLIDERH* s = sliderhs[p_sliderhs_index[gui_id]];
		float ans = s->getMin();
		CS::instance()->leave(CS_MESSAGE_CS, "getmin");
		return ans;
	}
	if (p_slidervs_index.find(gui_id) != p_slidervs_index.end()) {

		GUI_SLIDERV* s = slidervs[p_slidervs_index[gui_id]];
		float ans = s->getMin();
		CS::instance()->leave(CS_MESSAGE_CS, "getmin");
		return ans;
	}


	CS::instance()->leave(CS_MESSAGE_CS, "getmin");
	return 0;



}

void GUI::setLeafWindowToInputMessageDispatcher(int gui_window_id) {
	// 一番下につっこむ
	CS::instance()->enter(CS_MESSAGE_CS, "inputmessage");

	if (p_windows_index.find(gui_window_id) != p_windows_index.end()) {
		GUI_WINDOW* w = windows[p_windows_index[gui_window_id]];
		INPUTGETBYMESSAGESTRUCT* c = InputMessageDispatcher::getRootInputGetStruct();
		INPUTGETBYMESSAGESTRUCT* cc = c;
		cc = c->getParent();
		if (cc) {
		InputMessageDispatcher::registerImpl(w,cc->impl,c->impl);
		} else {
			InputMessageDispatcher::registerImpl(w, NULL,c->impl);
		}
	}
	CS::instance()->leave(CS_MESSAGE_CS, "inputmessage");

}


void GUI::setRootWindowToInputMessageDispatcher(int gui_window_id) {


	CS::instance()->enter(CS_MESSAGE_CS, "inputmessage");

	if (p_windows_index.find(gui_window_id) != p_windows_index.end()) {
		GUI_WINDOW* w = windows[p_windows_index[gui_window_id]];
		INPUTGETBYMESSAGESTRUCT* c = InputMessageDispatcher::getRootInputGetStruct();
		INPUTGETBYMESSAGESTRUCT* cc = c;
		while(c->getParent() && (cc != c->getParent())) {
			c = c->getParent();
		}
		InputMessageDispatcher::registerImpl(w,NULL,c->impl);
	}
	CS::instance()->leave(CS_MESSAGE_CS, "inputmessage");

}



void GUI::deleteAll() {
	Release();
}
















bool GUI_BUTTON::handleMessage(int msg, void* data, DWORD time){

		MYINPUTMESSAGESTRUCT* d = (MYINPUTMESSAGESTRUCT*)data;
		if (getIsDisabled()) return false;
	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSEMOVE) {
		// 範囲に入っているかつ左マウスボタンが押されていない→focus
		// 範囲に入っているかつ左マウスボタンが押されている→press

		if (getIsMove()) {
			

			moveBox(d->getMOUSESTATE()->mouse_dx, d->getMOUSESTATE()->mouse_dy);
		//	texture->setRenderTexPos(box_tex_id, box.left, box.top);
		}


		unsigned int butukari = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &box);
		if (butukari & BUTUKARIPOINT_IN) {

			

			if (!is_effect) { 
				texture->setRenderTexTexPos(box_tex_id, KTROBO_GUI_BUTTON_FOCUS_LEFT, KTROBO_GUI_BUTTON_FOCUS_TOP,
					KTROBO_GUI_BUTTON_FOCUS_WIDTH,KTROBO_GUI_BUTTON_FOCUS_HEIGHT);
			}
		} else {

		// 範囲に入ってなければnormal
		//	if (!is_effect) {
				texture->setRenderTexTexPos(box_tex_id, KTROBO_GUI_BUTTON_NORMAL_LEFT,
					KTROBO_GUI_BUTTON_NORMAL_TOP,
					KTROBO_GUI_BUTTON_NORMAL_WIDTH,
					KTROBO_GUI_BUTTON_NORMAL_HEIGHT);
				setIsEffect(false);
		//	}
		}
	}
	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSERAWSTATE) {
		// 範囲に入っているかつ左マウスボタンが押されている→press
		unsigned int butukari = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &box);
		if ((butukari & BUTUKARIPOINT_IN) && (d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_R_DOWN)) {
			this->setIsMove(true);
		} else if ((butukari & BUTUKARIPOINT_IN) && (d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_L_DOWN)) {
			texture->setRenderTexTexPos(box_tex_id, KTROBO_GUI_BUTTON_PRESS_LEFT,
				KTROBO_GUI_BUTTON_PRESS_TOP, 
				KTROBO_GUI_BUTTON_PRESS_WIDTH,
				KTROBO_GUI_BUTTON_PRESS_HEIGHT);

				this->setIsEffect(true);

		}

		if (d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_R_UP) {
			this->setIsMove(false);
		}

		if ((butukari & BUTUKARIPOINT_IN) && (d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_L_UP)) {
			// ボタンが離されたので
			if (getIsEffect()) {
				
				// ボタンが実行可能になっているので実行する
				// lua ファイル実行
				if (is_call_lua) {
				LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, this->l_str);
				} else {
					is_lua_tried_to_send = true;
				}

				setIsEffect(false);

				// focusに戻す
				texture->setRenderTexTexPos(box_tex_id, KTROBO_GUI_BUTTON_FOCUS_LEFT,
					KTROBO_GUI_BUTTON_FOCUS_TOP,
					KTROBO_GUI_BUTTON_FOCUS_WIDTH,
					KTROBO_GUI_BUTTON_FOCUS_HEIGHT);
				return true;
			}
		} else if(!(butukari & BUTUKARIPOINT_IN) && (d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_L_UP)) {


			setIsEffect(false);
			texture->setRenderTexTexPos(box_tex_id, KTROBO_GUI_BUTTON_NORMAL_LEFT,
					KTROBO_GUI_BUTTON_NORMAL_TOP,
					KTROBO_GUI_BUTTON_NORMAL_WIDTH,
					KTROBO_GUI_BUTTON_NORMAL_HEIGHT);
			return false;



		}
	}

	

	return false;// 次のストラクトに渡す





}
void GUI_BUTTON::setIsEffect(bool t) {
	is_effect = t;
}
void GUI_BUTTON::setIsRender(bool t) {
	is_render = t;
	texture->setRenderTexIsRender(box_tex_id,t);
	texture->setRenderTextIsRender(this->button_text,t);
}
GUI_BUTTON::GUI_BUTTON(float x, float y, float width, float height, char* luaf, int len, char* info) : GUI_PART() {
	box.left = x;
	box.right = width + x;
	box.top = y;
	box.bottom = y + height;
	int tex_id = texture->getTexture(KTROBO_GUI_PNG,1024);
	this->box_tex_id = texture->getRenderTex(tex_id, 0xFFFFFFFF,x,y,width,height, KTROBO_GUI_BUTTON_NORMAL_LEFT, KTROBO_GUI_BUTTON_NORMAL_TOP,
		KTROBO_GUI_BUTTON_NORMAL_WIDTH, KTROBO_GUI_BUTTON_NORMAL_HEIGHT);
	if (len < 128 && len > 0) {
		memcpy(this->l_str, luaf, len);
		l_str[len] = '\0';
	} else {
		memset(l_str,0,128);
	}
	stringconverter sc;
	WCHAR buf[512];
	memset(buf,0,sizeof(WCHAR)*512);
	sc.charToWCHAR(info,buf);
	float wid = wcslen(buf) * KTROBO_GUI_BUTTON_TEXT_HEIGHT;
	button_text = texture->getRenderText(info, (box.left + box.right)/2 - wid/2, (box.top+box.bottom)/2 -KTROBO_GUI_BUTTON_TEXT_HEIGHT/2, KTROBO_GUI_BUTTON_TEXT_HEIGHT,
			wid,KTROBO_GUI_BUTTON_TEXT_HEIGHT);
//		button_text->render(g,0xFFFFFFFF, (box.left + box.right)/2 - width/2, (box.top+box.bottom)/2 -KTROBO_GUI_BUTTON_TEXT_HEIGHT/2, KTROBO_GUI_BUTTON_TEXT_HEIGHT,
//			width,KTROBO_GUI_BUTTON_TEXT_HEIGHT);

}
GUI_BUTTON::~GUI_BUTTON() {

	texture->lightdeleteRenderTex(box_tex_id);
	texture->lightdeleteRenderText(button_text);
}
/*
void GUI_BUTTON::render(Graphics* g) {
	if (getIsRender()) {
		float width = button_text->getWidth(KTROBO_GUI_BUTTON_TEXT_HEIGHT);
		button_text->render(g,0xFFFFFFFF, (box.left + box.right)/2 - width/2, (box.top+box.bottom)/2 -KTROBO_GUI_BUTTON_TEXT_HEIGHT/2, KTROBO_GUI_BUTTON_TEXT_HEIGHT,
			width,KTROBO_GUI_BUTTON_TEXT_HEIGHT);
	}
}
*/
GUI_INPUTTEXT::GUI_INPUTTEXT(float x, float y, float width, float height) : GUI_PART() {
	box.left = x;
	box.right = width + x;
	box.top = y;
	box.bottom = y + height;
	int tex_id = texture->getTexture(KTROBO_GUI_PNG,1024);
	// ノーマルで！
	is_render = true;
	is_effect = false;
	this->box_tex_id_naka = texture->getRenderTex(tex_id, 0xFFFFFFFF,
		x + KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY-1,
		y + KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY-1,
		width- 2* KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY+2,
		height- 2 * KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY+2,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, 
		KTROBO_GUI_INPUTTEXT_NORMAL_TOP +2* KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH - 3*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, 
		KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT-3 *KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);
	
	box_tex_id_hidariue = texture->getRenderTex(tex_id, 0xFFFFFFFF, x,y,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT,KTROBO_GUI_INPUTTEXT_NORMAL_TOP, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

	box_tex_id_hidarinaka  = texture->getRenderTex(tex_id, 0xFFFFFFFF, x,y+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,height - 2 * KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT,KTROBO_GUI_INPUTTEXT_NORMAL_TOP+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT - 2 *KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

	box_tex_id_hidarisita = texture->getRenderTex(tex_id, 0xFFFFFFFF, x,y+height - KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT,KTROBO_GUI_INPUTTEXT_NORMAL_TOP+KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT - KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

	box_tex_id_migiue = texture->getRenderTex(tex_id, 0xFFFFFFFF,x+width-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,y,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH-
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_NORMAL_TOP,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

	box_tex_id_miginaka = texture->getRenderTex(tex_id, 0xFFFFFFFF, x+width-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,y+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,height- KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY*2,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH- KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_NORMAL_TOP+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT-
		2 * KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

	box_tex_id_migisita =  texture->getRenderTex(tex_id, 0xFFFFFFFF, x+width-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,
		y+height - KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_NORMAL_TOP+KTROBO_GUI_INPUTTEXT_PRESS_HEIGHT - KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

	box_tex_id_sitanaka = texture->getRenderTex(tex_id, 0xFFFFFFFF, x+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,
		y,width - 2 * KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_NORMAL_TOP,KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH-2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

	box_tex_id_uenaka = texture->getRenderTex(tex_id, 0xFFFFFFFF, x+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,
		y+height - KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,width - 2 * KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, 
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_NORMAL_TOP+KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH-2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

	text = new Text(L"",0);
	
//	text->render(g, 0xFFFFFFFF,box.left+5,box.top+2, box.bottom-box.top-4, box.right-box.left-10,box.bottom - box.top);

	input_text = texture->getRenderText("", box.left+5,box.top+2, box.bottom-box.top-4, box.right- box.left-10, box.bottom-box.top);

	now_mode = 0;
	cursor_x = 0;
	string_max_x = 0;
	memset(sentencestring,0,sizeof(char)*512);
	memset(kouhostring,0,sizeof(char)*256);
	memset(inputstring,0,sizeof(char)*256);
}

GUI_INPUTTEXT::~GUI_INPUTTEXT() {
	if (text) {
		delete text;
		text = 0;
	}
	texture->lightdeleteRenderTex(box_tex_id_hidarinaka);
	texture->lightdeleteRenderTex(box_tex_id_hidarisita);
	texture->lightdeleteRenderTex(box_tex_id_hidariue);
	texture->lightdeleteRenderTex(box_tex_id_miginaka);
	texture->lightdeleteRenderTex(box_tex_id_migisita);
	texture->lightdeleteRenderTex(box_tex_id_migiue);
	texture->lightdeleteRenderTex(box_tex_id_naka);
	texture->lightdeleteRenderTex(box_tex_id_sitanaka);
	texture->lightdeleteRenderTex(box_tex_id_uenaka);
	texture->lightdeleteRenderText(input_text);
}

bool GUI_PART::moveBox(int dx, int dy) {

	bool ans = false;
	// 動かす
	MYRECT temp_box = box;
	box.left += dx;
	box.right += dx;
	box.top += dy;
	box.bottom += dy;
	if (box.left < max_boxdayo.left) {
		box.right += max_boxdayo.left-box.left;
		box.left = max_boxdayo.left;
		box = temp_box;
		ans = true;
		return true;
	}
	if (box.top < max_boxdayo.top) {
		box.bottom += max_boxdayo.top-box.top;
		box.top = max_boxdayo.top;
		box = temp_box;
		return true;
	}
	if (box.right > max_boxdayo.right) {
		box.left += max_boxdayo.right - box.right;
		box.right = max_boxdayo.right;
		box = temp_box;
		return true;
	}
	if (box.bottom > max_boxdayo.bottom) {
		box.top += max_boxdayo.bottom - box.bottom;
		box.bottom = max_boxdayo.bottom;
		box = temp_box;
		return true;
	}

	return ans;
}
bool GUI_INPUTTEXT::moveBox(int dx, int dy) {

	bool ans = GUI_PART::moveBox(dx, dy);

	texture->setRenderTexPos(box_tex_id_hidariue, box.left, box.top);
	texture->setRenderTexPos(box_tex_id_hidarinaka, box.left, box.top -1 + KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY);
	texture->setRenderTexPos(box_tex_id_hidarisita, box.left, box.bottom - KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY-3);
	texture->setRenderTexPos(box_tex_id_migiue, box.right-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, box.top);
	texture->setRenderTexPos(box_tex_id_miginaka, box.right-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, -1+box.top+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY);
	texture->setRenderTexPos(box_tex_id_migisita, box.right-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, box.bottom -3- KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY);
	texture->setRenderTexPos(box_tex_id_uenaka, box.left+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, box.top);
	texture->setRenderTexPos(box_tex_id_naka, box.left+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, -1+box.top + KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY);
	texture->setRenderTexPos(box_tex_id_sitanaka, box.left +KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, box.bottom -3- KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY);

	//	text->render(g, 0xFFFFFFFF,box.left+5,box.top+2, box.bottom-box.top-4, box.right-box.left-10,box.bottom - box.top);
	texture->setRenderTextPos(input_text, box.left+5, box.top+2);
	return ans;
}

bool GUI_WINDOW::moveBox(int dx, int dy) {

	bool ans = GUI_PART::moveBox(dx, dy);


	return ans;
}
bool GUI_BUTTON::moveBox(int dx, int dy) {

	bool ans = GUI_PART::moveBox(dx, dy);
	texture->setRenderTexPos(box_tex_id, box.left, box.top);
	// (box.left + box.right)/2 - width/2, (box.top+box.bottom)/2 -KTROBO_GUI_BUTTON_TEXT_HEIGHT/2, KTROBO_GUI_BUTTON_TEXT_HEIGHT,
	float width = texture->getRenderTextWidth(button_text, KTROBO_GUI_BUTTON_TEXT_HEIGHT);
	texture->setRenderTextPos(this->button_text, (box.left + box.right)/2 - width/2, (box.top+box.bottom)/2 -KTROBO_GUI_BUTTON_TEXT_HEIGHT/2);
	return ans;
}

bool GUI_INPUTTEXT::handleMessage(int msg, void* data, DWORD time){

	MYINPUTMESSAGESTRUCT* d = (MYINPUTMESSAGESTRUCT*)data;
	if (getIsDisabled()) return false;
	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSEMOVE) {
		// 範囲に入っているかつ左マウスボタンが押されていない→focus
		// 範囲に入っているかつ左マウスボタンが押されている→press

		if (getIsMove()) {
			


			this->moveBox(d->getMOUSESTATE()->mouse_dx, d->getMOUSESTATE()->mouse_dy);

			/*	texture->setRenderTexPos(box_tex_id_hidariue, box.left, box.top);
				texture->setRenderTexPos(box_tex_id_hidarinaka, box.left, box.top -1 + KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY);
				texture->setRenderTexPos(box_tex_id_hidarisita, box.left, box.bottom - KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY-3);
				texture->setRenderTexPos(box_tex_id_migiue, box.right-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, box.top);
				texture->setRenderTexPos(box_tex_id_miginaka, box.right-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, -1+box.top+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY);
				texture->setRenderTexPos(box_tex_id_migisita, box.right-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, box.bottom -3- KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY);
				texture->setRenderTexPos(box_tex_id_uenaka, box.left+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, box.top);
				texture->setRenderTexPos(box_tex_id_naka, box.left+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, -1+box.top + KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY);
				texture->setRenderTexPos(box_tex_id_sitanaka, box.left +KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, box.bottom -3- KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY);
				*/
		}


		unsigned int butukari = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &box);
		if (butukari & BUTUKARIPOINT_IN) {

			

			if (!is_effect) { 
				texture->setRenderTexTexPos(box_tex_id_naka, KTROBO_GUI_INPUTTEXT_FOCUS_LEFT+2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_FOCUS_TOP+2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
					KTROBO_GUI_INPUTTEXT_FOCUS_WIDTH-3*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
					KTROBO_GUI_INPUTTEXT_FOCUS_HEIGHT-3 *KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);



					texture->setRenderTexTexPos(box_tex_id_hidariue,
		KTROBO_GUI_INPUTTEXT_FOCUS_LEFT,KTROBO_GUI_INPUTTEXT_FOCUS_TOP,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

	                texture->setRenderTexTexPos(box_tex_id_hidarinaka,
		KTROBO_GUI_INPUTTEXT_FOCUS_LEFT,KTROBO_GUI_INPUTTEXT_FOCUS_TOP+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_FOCUS_HEIGHT - 2 *KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_hidarisita,
		KTROBO_GUI_INPUTTEXT_FOCUS_LEFT,KTROBO_GUI_INPUTTEXT_FOCUS_TOP+KTROBO_GUI_INPUTTEXT_FOCUS_HEIGHT - KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_migiue,KTROBO_GUI_INPUTTEXT_FOCUS_LEFT+KTROBO_GUI_INPUTTEXT_FOCUS_WIDTH-
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_FOCUS_TOP,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_miginaka,
		KTROBO_GUI_INPUTTEXT_FOCUS_LEFT+KTROBO_GUI_INPUTTEXT_FOCUS_WIDTH- KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_FOCUS_TOP+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_FOCUS_HEIGHT-
		2 * KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_migisita,
		KTROBO_GUI_INPUTTEXT_FOCUS_LEFT+KTROBO_GUI_INPUTTEXT_FOCUS_WIDTH-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_FOCUS_TOP+KTROBO_GUI_INPUTTEXT_FOCUS_HEIGHT - KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_sitanaka, KTROBO_GUI_INPUTTEXT_FOCUS_LEFT+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_FOCUS_TOP,KTROBO_GUI_INPUTTEXT_FOCUS_WIDTH-2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_uenaka, KTROBO_GUI_INPUTTEXT_FOCUS_LEFT+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_FOCUS_TOP+KTROBO_GUI_INPUTTEXT_FOCUS_HEIGHT-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_FOCUS_WIDTH-2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);








			}
		} else {

		// 範囲に入ってなければnormal
			if (!is_effect) {
				texture->setRenderTexTexPos(box_tex_id_naka, KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
					KTROBO_GUI_INPUTTEXT_NORMAL_TOP + 2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
					KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH - 3 * KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
					KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT - 3 * KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);




					texture->setRenderTexTexPos(box_tex_id_hidariue,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT,KTROBO_GUI_INPUTTEXT_NORMAL_TOP,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

	                texture->setRenderTexTexPos(box_tex_id_hidarinaka,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT,KTROBO_GUI_INPUTTEXT_NORMAL_TOP+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT - 2 *KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_hidarisita,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT,KTROBO_GUI_INPUTTEXT_NORMAL_TOP+KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT - KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_migiue,KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH-
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_NORMAL_TOP,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_miginaka,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH- KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_NORMAL_TOP+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT-
		2 * KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_migisita,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_NORMAL_TOP+KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT - KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_sitanaka, KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_NORMAL_TOP,KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH-2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_uenaka, KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_NORMAL_TOP+KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH-2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

			}
		}
		


	}

	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSERAWSTATE) {
		// 範囲に入っているかつ左マウスボタンが押されている→press
		unsigned int butukari = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &box);
		if ((butukari & BUTUKARIPOINT_IN) && (d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_R_DOWN)) {
			this->setIsMove(true);
		} else if ((butukari & BUTUKARIPOINT_IN) && d->getMOUSESTATE()->mouse_l_button_pressed) {
			texture->setRenderTexTexPos(box_tex_id_naka, KTROBO_GUI_INPUTTEXT_PRESS_LEFT+2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
				KTROBO_GUI_INPUTTEXT_PRESS_TOP+2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, 
				KTROBO_GUI_INPUTTEXT_PRESS_WIDTH-3*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
				KTROBO_GUI_INPUTTEXT_PRESS_HEIGHT-3*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);







			
					texture->setRenderTexTexPos(box_tex_id_hidariue,
		KTROBO_GUI_INPUTTEXT_PRESS_LEFT,KTROBO_GUI_INPUTTEXT_PRESS_TOP,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

	                texture->setRenderTexTexPos(box_tex_id_hidarinaka,
		KTROBO_GUI_INPUTTEXT_PRESS_LEFT,KTROBO_GUI_INPUTTEXT_PRESS_TOP+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_PRESS_HEIGHT - 2 *KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_hidarisita,
		KTROBO_GUI_INPUTTEXT_PRESS_LEFT,KTROBO_GUI_INPUTTEXT_PRESS_TOP+KTROBO_GUI_INPUTTEXT_PRESS_HEIGHT - KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_migiue,KTROBO_GUI_INPUTTEXT_PRESS_LEFT+KTROBO_GUI_INPUTTEXT_PRESS_WIDTH-
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_PRESS_TOP,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_miginaka,
		KTROBO_GUI_INPUTTEXT_PRESS_LEFT+KTROBO_GUI_INPUTTEXT_PRESS_WIDTH- KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_PRESS_TOP+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_PRESS_HEIGHT-
		2 * KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_migisita,
		KTROBO_GUI_INPUTTEXT_PRESS_LEFT+KTROBO_GUI_INPUTTEXT_PRESS_WIDTH-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_PRESS_TOP+KTROBO_GUI_INPUTTEXT_PRESS_HEIGHT - KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_sitanaka, KTROBO_GUI_INPUTTEXT_PRESS_LEFT+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_PRESS_TOP,KTROBO_GUI_INPUTTEXT_PRESS_WIDTH-2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_uenaka, KTROBO_GUI_INPUTTEXT_PRESS_LEFT+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_PRESS_TOP+KTROBO_GUI_INPUTTEXT_PRESS_HEIGHT-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_PRESS_WIDTH-2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

				this->setIsEffect(true);
				this->setIME(true);
				this->setCursorX(msg, data,time);

		} else if(!(butukari & BUTUKARIPOINT_IN) && d->getMOUSESTATE()->mouse_l_button_pressed) {
			this->setIsEffect(false);
			// 範囲に入ってなければnormal
			texture->setRenderTexTexPos(box_tex_id_naka, KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
				KTROBO_GUI_INPUTTEXT_NORMAL_TOP+2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
				KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH-3*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
				KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT-3*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);


			
					texture->setRenderTexTexPos(box_tex_id_hidariue,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT,KTROBO_GUI_INPUTTEXT_NORMAL_TOP,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

	                texture->setRenderTexTexPos(box_tex_id_hidarinaka,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT,KTROBO_GUI_INPUTTEXT_NORMAL_TOP+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT - 2 *KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_hidarisita,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT,KTROBO_GUI_INPUTTEXT_NORMAL_TOP+KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT - KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_migiue,KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH-
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_NORMAL_TOP,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_miginaka,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH- KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_NORMAL_TOP+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT-
		2 * KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_migisita,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_NORMAL_TOP+KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT - KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_sitanaka, KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_NORMAL_TOP,KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH-2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

					texture->setRenderTexTexPos(box_tex_id_uenaka, KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_NORMAL_TOP+KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH-2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);









			this->setIME(false);
		}

		if (d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_R_UP) {
			// ボタンが離されたので
			this->setIsMove(false);
		}
	}

	if (msg == KTROBO_INPUT_MESSAGE_ID_KEYDOWN) {
		if (is_effect) {
				HIMC him = ImmGetContext(hwnd);
				HideCaret(hwnd);
				if (ImmGetOpenStatus(him)) {
					DWORD henkan;
					DWORD mode;
					if (ImmGetConversionStatus(him, &mode, &henkan)) {
						if (now_mode != mode) {
							// モードが変わったので
							ImmNotifyIME(him, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
							now_mode = mode;
						}
								
						if (now_mode == KTROBO_GUI_INPUTSENTENCE_IMM_MODE_HANKAKU || now_mode == KTROBO_GUI_INPUTSENTENCE_IMM_MODE_HANKAKU2) {
							copyStringFromAction(msg,data,time);
						} else {
						
							if ((d->getKEYSTATE()[VK_RETURN] & KTROBO_INPUT_BUTTON_DOWN) != 0) {
								this->copyIMESTRINGToInputString();
							} else {
								this->copyIMESTRINGToKouhoString(msg, data,time);
							}
						}
					}
				} else {
					
					copyStringFromAction(msg,data,time);
					
				}
			ImmReleaseContext(hwnd, him);

		}
	}

	return false;// 次のストラクトに渡す



};
void GUI_INPUTTEXT::setIsEffect(bool t) {
	is_effect = t;
}
void GUI_INPUTTEXT::setIsRender(bool t) {
	is_render = t;
	texture->setRenderTexIsRender(box_tex_id_naka,t);
	texture->setRenderTexIsRender(box_tex_id_hidarinaka,t);
	texture->setRenderTexIsRender(box_tex_id_hidarisita,t);
	texture->setRenderTexIsRender(box_tex_id_hidariue, t);
	texture->setRenderTexIsRender(box_tex_id_miginaka,t);
	texture->setRenderTexIsRender(box_tex_id_migiue, t);
	texture->setRenderTexIsRender(box_tex_id_migisita, t);
	texture->setRenderTexIsRender(box_tex_id_sitanaka,t);
	texture->setRenderTexIsRender(box_tex_id_uenaka,t);
	texture->setRenderTextIsRender(input_text, t);
}













void GUI_INPUTTEXT::eraseSentenceString() {
	int sin_cursor_x = 0;
	int ss_cursor_x = 0;
	int sin_cursor_maxx = strlen(sentencestring);
	unsigned char* temp = (unsigned char*)sentencestring;
	bool mae_moji_multi = false;
	for (sin_cursor_x = 0; sin_cursor_x < sin_cursor_maxx;) {
		if (SJISMultiCheck(*temp)) {
			mae_moji_multi = true;
			temp += 2;
			sin_cursor_x+=2;
			ss_cursor_x += 1;
		} else {
			mae_moji_multi = false;
			temp += 1;
			sin_cursor_x+=1;
			ss_cursor_x += 1;
		}
		if (ss_cursor_x >= cursor_x) {
			break;
		}
	}
	if (ss_cursor_x > 0) {
		if (mae_moji_multi) {
			if (sin_cursor_x > 1) {
				for (int t = sin_cursor_x-2;t < sin_cursor_maxx-2;t++) {
					sentencestring[t] = sentencestring[t+2];
				}
				sentencestring[sin_cursor_maxx-2] = '\0';
			}
		} else {
			if (sin_cursor_x > 0) {
				for (int t = sin_cursor_x-1;t<sin_cursor_maxx-1;t++) {
					sentencestring[t] = sentencestring[t+1];
				}
				sentencestring[sin_cursor_maxx-1] = '\0';
			}
		}
	}
	return;
}
void GUI_INPUTTEXT::copyInputStringToMyText() {
	/*if (text) {
		text->release(true);
		delete text;
	}*/
	WCHAR s[512];
	memset(s,0,512*sizeof(WCHAR));
	stringconverter sc;
	char usiro_string[512];
	memset(usiro_string,0,512);

	unsigned char* temp = (unsigned char*)sentencestring;
	int sin_cursor_x=0;
	int ss_cursor_x=0;
	int ss_cursor_maxx = SJISlength((unsigned char*)sentencestring);
	int sin_cursor_maxx = strlen(sentencestring);
	while(*temp != '\0' && ss_cursor_x < cursor_x && sin_cursor_x < sin_cursor_maxx) {
		if (SJISMultiCheck(*temp)) {
			sin_cursor_x += 2;
			ss_cursor_x += 1;
			temp += 2;
		} else {
			sin_cursor_x += 1;
			ss_cursor_x += 1;
			temp += 1;
		}
	}
	if (ss_cursor_x != cursor_x) {
		cursor_x = ss_cursor_x;
	}
	// inputstring の長さの分カーソルを動かす
	temp = (unsigned char*)inputstring;
	cursor_x += SJISlength(temp);
	// 後ろのストリングを今のうちにコピーしておく
	int t_x = 0;
	for (int t=sin_cursor_x;t<sin_cursor_maxx;t++) {
		usiro_string[t_x] = sentencestring[t];
		t_x++;
	}
	// コピー完了

	int inputstring_x=0;
	int inputstring_maxx = strlen(inputstring);
	while (sin_cursor_x < 511 && inputstring_x < inputstring_maxx) {
		sentencestring[sin_cursor_x] = inputstring[inputstring_x];
		sin_cursor_x++;
		inputstring_x++;
	}
	
	// 後ろの文字をコピーする

	for (int t=0;t<t_x && sin_cursor_x < 511 ;t++) {
		sentencestring[sin_cursor_x] = usiro_string[t];
		sin_cursor_x++;
	}
	if (sin_cursor_x > 511) {
		sin_cursor_x = 511;
	}
	sentencestring[sin_cursor_x] = '\0';
	
	sc.charToWCHAR(sentencestring,s);
	//text = new MyText(s,0);
	if (text) {
		text->changeText(s, wcslen(s));
		texture->setRenderTextChangeText(input_text,sentencestring);
	}

}
void GUI_INPUTTEXT::copyKouhoStringToMyText() {

/*	if (text) {
		text->release(true);
		delete text;
	}*/
	char allstring[512];
	memset(allstring,0,512);

		WCHAR s[512];
		memset(s,0,512*sizeof(WCHAR));
		stringconverter sc;

			char usiro_string[512];
	memset(usiro_string,0,512);
	strcpy_s(allstring,512,sentencestring);
	unsigned char* temp = (unsigned char*)allstring;
	int sin_cursor_x=0;
	int ss_cursor_x = 0;
	int ss_cursor_maxx = SJISlength((unsigned char*)allstring);
	int sin_cursor_maxx = strlen(allstring);
	while(*temp != '\0' && ss_cursor_x < cursor_x && sin_cursor_x < sin_cursor_maxx) {
		if (SJISMultiCheck(*temp)) {
			sin_cursor_x += 2;
			ss_cursor_x += 1;
			temp += 2;
		} else {
			sin_cursor_x += 1;
			ss_cursor_x += 1;
			temp += 1;
		}
	}
	if (ss_cursor_x != cursor_x) {
		cursor_x = ss_cursor_x;
	}
	// 後ろのストリングを今のうちにコピーしておく
	int t_x = 0;
	for (int t=sin_cursor_x;t<sin_cursor_maxx;t++) {
		usiro_string[t_x] = allstring[t];
		t_x++;
	}
	// コピー完了

	int inputstring_x=0;
	int inputstring_maxx = strlen(kouhostring);
	while (sin_cursor_x < 511 && inputstring_x < inputstring_maxx) {
		allstring[sin_cursor_x] = kouhostring[inputstring_x];
		sin_cursor_x++;
		inputstring_x++;
	}
	if (sin_cursor_x > 511) {
		sin_cursor_x = 511;
	}
	allstring[sin_cursor_x] = '\0';
	
	// 後ろの文字をコピーする

	for (int t=0;t<t_x && sin_cursor_x < 511 ;t++) {
		allstring[sin_cursor_x] = usiro_string[t];
		sin_cursor_x++;
	}
	if (sin_cursor_x > 511) {
		sin_cursor_x = 511;
	}
	allstring[sin_cursor_x] = '\0';
	
	sc.charToWCHAR(allstring,s);
	//text = new MyText(s,0);
	if (text) {
		text->changeText(s, wcslen(s));
		texture->setRenderTextChangeText(input_text, allstring);
	}
}
void GUI_INPUTTEXT::copyStringFromAction(int msg_id, void* data, DWORD time) {

	MYINPUTMESSAGESTRUCT* d = (MYINPUTMESSAGESTRUCT*)data;
	if (msg_id == KTROBO_INPUT_MESSAGE_ID_KEYDOWN) {

		inputstring[0] = '\0';
		strcpy_s(inputstring,255,this->getInputStr(d->getKEYSTATE()));
		if (strlen(inputstring) != 0) {
			cursor_x++;
		}
		if ((d->getKEYSTATE()[VK_BACK] & KTROBO_INPUT_BUTTON_DOWN) != 0) {
			eraseSentenceString();
		}
	}else {
		inputstring[0] = '\0';
		
	}


	copyInputStringToMyText();
/*	HIMC him = ImmGetContext(hwnd);
	ImmSetCompositionStringA(him, SCS_SETSTR, "", 0, "", 0);
	ImmNotifyIME(him, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
	ImmReleaseContext(hwnd, him);*/
}

void GUI_INPUTTEXT::setIME(bool t) {
	HIMC him = ImmGetContext(hwnd);
	ImmSetOpenStatus(him, t);
	DWORD mode;
	DWORD henkan;
	if (ImmGetConversionStatus(him, &mode, &henkan)) {
		if (now_mode != mode) {
			// モードが変わったので
			ImmNotifyIME(him, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
			now_mode = mode;
		}

	}
	if (is_focused != t) {
		ImmNotifyIME(him, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		//ImmSetCompositionStringA(him, SCS_SETSTR, "", 0, "", 0);
		is_focused = t;
	}
	ImmReleaseContext(hwnd, him);
}
void GUI_INPUTTEXT::copyIMESTRINGToInputString() {
	HIMC him = ImmGetContext(hwnd);
	memset(inputstring,0,256);
	ImmGetCompositionStringA(him,GCS_RESULTSTR,(void*)inputstring,256);
	inputstring[255] = '\0';
	copyInputStringToMyText();
	
	kouhostring[0] = '\0';
	inputstring[0] = '\0';
	ImmSetCompositionStringA(him, SCS_SETSTR, "", 0, "", 0);
	ImmNotifyIME(him, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
	ImmReleaseContext(hwnd, him);
}
void GUI_INPUTTEXT::copyIMESTRINGToKouhoString(int msg_id, void* data, DWORD time) {

	MYINPUTMESSAGESTRUCT* d = (MYINPUTMESSAGESTRUCT*)data;

	HIMC him = ImmGetContext(hwnd);
	int strl = strlen(kouhostring);
	bool mae_nagasa = false;
	if (strl) {
		mae_nagasa = true;
	}
	memset(kouhostring,0,256);
	ImmGetCompositionStringA(him,GCS_COMPSTR,(void*)kouhostring,256);
	kouhostring[255] = '\0';
	if (msg_id == KTROBO_INPUT_MESSAGE_ID_KEYDOWN) {
		
		if ((d->getKEYSTATE()[VK_BACK] & KTROBO_INPUT_BUTTON_DOWN) != 0) {
			WCHAR st[512];
			memset(st, 0, 512*sizeof(WCHAR));
			stringconverter sc;
			sc.charToWCHAR(kouhostring,st);
			// 一文字削る
			bool has_kezuru = false;
			int wlen = wcslen(st);
			if (wlen > 0) {
				st[wlen-1] = L'\0';
				wlen = wlen -1;
				has_kezuru = true;
			} else {
				if (!mae_nagasa) {
					eraseSentenceString();
				}
			}
			/*
			strcpy_s(kouhostring,256,sc.wcharTochar(st));
			ImmNotifyIME(him, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
			ImmSetCompositionStringA(him, SCS_SETSTR, kouhostring, strlen(kouhostring), "", 0);
			if (has_kezuru) {
				cursor_x--;
			}
			
		}*/	
		}
	}

	copyKouhoStringToMyText();
	
	ImmReleaseContext(hwnd, him);



}


void GUI_INPUTTEXT::setCursorX(int msg_id, void* data, DWORD time) {
	MYRECT r = box;
	MYINPUTMESSAGESTRUCT* input = (MYINPUTMESSAGESTRUCT*)data;
	int X = input->getMOUSESTATE()->mouse_x;
	if (r.bottom - r.top > 0) {
		
		if (text) {
			if (text->getWidth(r.bottom-r.top) +r.bottom-r.top > X -r.left) {
				cursor_x = (X -r.left)/ (r.bottom-r.top);
			}
		}
	}

}

/*
void GUI_INPUTTEXT::changeText(char* new_text) {
	
	stringconverter sc;
	WCHAR ch[512];
	memset(ch,0,sizeof(WCHAR)*512);
	sc.charToWCHAR(new_text, ch);
	if (text) {
		text->changeText(ch, wcslen(ch));
	}

	MYRECT r = *getBOX();
	float width = text->getWidth(r.bottom - r.top);
	r.right = r.left + width;
	this->changeBOX(&r);
}
*/

char* GUI_INPUTTEXT::getInputStr(unsigned char* keys) {

// is_shift
	bool is_shift = keys[VK_RSHIFT] || keys[VK_LSHIFT];
	int down_index = 0;
	bool is_down = false;
	for (int i = 0 ; i < 256; i++) {
		if ((keys[i] & KTROBO_INPUT_BUTTON_DOWN) != 0) {
			down_index = i;
			is_down = true;
		}
	}
	if (is_shift) {
	switch (down_index) {
	case '0':
		return "";
	case '1':
		return "!";
	case '2':
		return "\"";
	case '3':
		return "#";
	case '4':
		return "$";
	case '5':
		return "%";
	case '6':
		return "&";
	case '7':
		return "\'";
	case '8':
		return "(";
	case '9':
		return ")";
	case VK_OEM_1:
		return "*";
	case VK_OEM_PLUS:
		return "+";
	case VK_OEM_MINUS:
		return "=";
	case VK_OEM_2:
		return "?";
	case VK_SPACE:
		return " ";
	case VK_OEM_102:
		return "_";
	case 'Q':
		return "Q";
	case 'W':
		return "W";
	case 'E':
		return "E";
	case 'R':
		return "R";
	case 'T':
		return "T";
	case 'Y':
		return "Y";
	case 'U':
		return "U";
	case 'I':
		return "I";
	case 'O':
		return "O";
	case 'P':
		return "P";
	case 'A':
		return "A";
	case 'S':
		return "S";
	case 'D':
		return "D";
	case 'F':
		return "F";
	case 'G':
		return "G";
	case 'H':
		return "H";
	case 'J':
		return "J";
	case 'K':
		return "K";
	case 'L':
		return "L";
	case 'Z':
		return "Z";
	case 'X':
		return "X";
	case 'C':
		return "C";
	case 'V':
		return "V";
	case 'B':
		return "B";
	case 'N':
		return "N";
	case 'M':
		return "M";
	case VK_OEM_COMMA:
		return "<";
	case VK_OEM_PERIOD:
		return ">";
	case VK_OEM_4:
		return "{";
	case VK_OEM_5:
		return "|";
	case VK_OEM_6:
		return "}";
	case VK_OEM_7:
		return "~";
	}

	} else {
		switch(down_index) {
	case '0':
		return "0";
	case '1':
		return "1";
	case '2':
		return "2";
	case '3':
		return "3";
	case '4':
		return "4";
	case '5':
		return "5";
	case '6':
		return "6";
	case '7':
		return "7";
	case '8':
		return "8";
	case '9':
		return "9";
	case VK_OEM_1:
		return ":";
	case VK_OEM_PLUS:
		return ";";
	case VK_OEM_MINUS:
		return "-";
	case VK_OEM_2:
		return "/";
	case VK_SPACE:
		return " ";
	
	case 'Q':
		return "q";
	case 'W':
		return "w";
	case 'E':
		return "e";
	case 'R':
		return "r";
	case 'T':
		return "t";
	case 'Y':
		return "y";
	case 'U':
		return "u";
	case 'I':
		return "i";
	case 'O':
		return "o";
	case 'P':
		return "p";
	case 'A':
		return "a";
	case 'S':
		return "s";
	case 'D':
		return "d";
	case 'F':
		return "f";
	case 'G':
		return "g";
	case 'H':
		return "h";
	case 'J':
		return "j";
	case 'K':
		return "k";
	case 'L':
		return "l";
	case 'Z':
		return "z";
	case 'X':
		return "x";
	case 'C':
		return "c";
	case 'V':
		return "v";
	case 'B':
		return "b";
	case 'N':
		return "n";
	case 'M':
		return "m";
	case VK_OEM_COMMA:
		return ",";
	case VK_OEM_PERIOD:
		return ".";
	case VK_OEM_102:
		return "\\";
	case VK_OEM_3:
		return "@";
	case VK_OEM_4:
		return "[";
	case VK_OEM_5:
		return "\\";
	case VK_OEM_6:
		return "]";
	case VK_OEM_7:
		return "^";
		}
	}
	return "";
}

/*
void GUI_INPUTTEXT::render(Graphics* g) {
	if (text) {
		text->render(g, 0xFFFFFFFF,box.left+5,box.top+2, box.bottom-box.top-4, box.right-box.left-10,box.bottom - box.top);
	}
}
*/
HWND GUI_INPUTTEXT::hwnd=0;
Texture* GUI_INPUTTEXT::texture=0;

lua_State* GUI_BUTTON::L=0; // handlemessageが呼ばれるのは AIスレッドなのでAIスレッドのLを呼ぶ
Texture* GUI_BUTTON::texture=0;

Texture* GUI_TEXT::tex=0;
Texture* GUI_TEX::tex=0;
Texture* GUI_WINDOW::tex=0;
Texture* GUI_TAB::tex=0;
Texture* GUI_SLIDERH::tex =0;
Texture* GUI_SLIDERV::tex =0;
MYRECT GUI_PART::max_default_box;





GUI_TEXT::GUI_TEXT(float x, float y, float width, float height, char* texc) : GUI_PART(){
	
	stringconverter sc;
	WCHAR buf[512];
	memset(buf,0,sizeof(WCHAR)*512);
	sc.charToWCHAR(texc,buf);
	//text = new Text(buf, wcslen(buf));
	text= GUI_TEXT::tex->getRenderText(texc, x, y, height, width,height);
	box.left = x;
	box.top = y;
	box.right = x + width;
	box.bottom = y + height;



}


GUI_TEXT::~GUI_TEXT() {
	/*if (text) {
		delete text;
		text = 0;
	}*/
	tex->lightdeleteRenderText(text);
}

/*
void GUI_TEXT::render(Graphics* g) {
	if (getIsRender()) {
		text->render(g, 0xFFFFFFFF,box.left,box.right, box.bottom- box.top, box.right- box.left, box.bottom - box.top);
	}
}
*/

bool GUI_TAB::handleMessage(int msg, void* data, DWORD time) {
	MYINPUTMESSAGESTRUCT* d = (MYINPUTMESSAGESTRUCT*)data;
	if (getIsDisabled()) return false;
	//is_effect = true;
	//is_render =true;
	if ((msg == KTROBO_INPUT_MESSAGE_ID_MOUSERAWSTATE) && getIsEffect()) {
		unsigned int butukari;
		int size = tex_rect_boxs.size();
		for (int i=0;i<size;i++) {
			MYRECT r = tex_rect_boxs[i];
			butukari = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &r);
			if (butukari & BUTUKARIPOINT_IN) {
				if (d->getMOUSESTATE()->mouse_l_button_pressed) {
					now_index = i;
					setIsEffect(true);//is_effect);
					setIsRender(true);//is_render);
					LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION,true, this->l_str);

					return true;
				}
			}
		}
	}

	int size = child_windows.size();
	if (size) {
		if (child_windows[now_index]->handleMessage(msg, data,time)) {
			return true;
		}
	}


	return false;
}

unsigned int GUI_TAB::colors[8];


unsigned int GUI_TAB::f_colors[8];


int GUI::getTabIndex(int tab_gui_id) {
	int inde = 0;
	CS::instance()->enter(CS_MESSAGE_CS, "push part to tab");
	if (p_tabs_index.find(tab_gui_id) != p_tabs_index.end()) {
		
		GUI_TAB* t = tabs[p_tabs_index[tab_gui_id]];
		inde = t->getNowIndex();
	
	}
	CS::instance()->leave(CS_MESSAGE_CS, "push part to tab");
	return inde;
}
void GUI::setTabIndex(int tab_gui_id, int index) {
	CS::instance()->enter(CS_MESSAGE_CS, "push part to tab");
	if (p_tabs_index.find(tab_gui_id) != p_tabs_index.end()) {
		
		GUI_TAB* t = tabs[p_tabs_index[tab_gui_id]];
		t->setNowIndex(index);
	
	}
	CS::instance()->leave(CS_MESSAGE_CS, "push part to tab");
}


GUI_SLIDERV::GUI_SLIDERV(MYRECT zentai, float max, float min, float now, char* l_str) : GUI_PART() {
	this->max = max;
	this->min = min;
	this->now = now;
	this->zentai_box = zentai;
	memset(this->l_str,0 ,64);
	if (strlen(l_str) < 64) {
	strcpy_s(this->l_str,64,l_str);
	}
	this->is_box_moved = false;
	this->is_min_pressed = false;
	this->is_max_pressed = false;

	int tex_id = tex->getTexture(KTROBO_GUI_PNG,1024);

	this->tex_id_max = tex->getRenderTex(tex_id, 0xFFFFFFFF, zentai.left,zentai.top,
		zentai.right - zentai.left, KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT, KTROBO_GUI_SLIDERVMAX_NORMAL_LEFT,
		KTROBO_GUI_SLIDERVMAX_NORMAL_TOP, KTROBO_GUI_SLIDERVMAX_NORMAL_WIDTH, KTROBO_GUI_SLIDERVMAX_NORMAL_HEIGHT);
	this->tex_id_min = tex->getRenderTex(tex_id, 0xFFFFFFFF,zentai.left, zentai.bottom - KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT,
		zentai.right- zentai.left,
		KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT, KTROBO_GUI_SLIDERVMIN_NORMAL_LEFT, KTROBO_GUI_SLIDERVMIN_NORMAL_TOP,
		KTROBO_GUI_SLIDERVMIN_NORMAL_WIDTH, KTROBO_GUI_SLIDERVMIN_NORMAL_HEIGHT);

	float tex_id_now_y = getTexIdNowY();

	this->tex_id_now = tex->getRenderTex(tex_id, 0xFFFFFFFF,zentai.left, tex_id_now_y, zentai.right-zentai.left, KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT, 
		KTROBO_GUI_SLIDERNOW_NORMAL_LEFT, KTROBO_GUI_SLIDERNOW_NORMAL_TOP, KTROBO_GUI_SLIDERNOW_NORMAL_WIDTH, KTROBO_GUI_SLIDERNOW_NORMAL_HEIGHT);

	setMINMAXNOWFROMZENTAI();
/*	max_boxdayo.left = zentai.left;// + KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	max_boxdayo.top = zentai.top+KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	max_boxdayo.bottom = zentai.bottom - KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	max_boxdayo.right = zentai.right;// - KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
*/
	is_call_lua = true;
	is_lua_tried_to_send = false;
}

GUI_SLIDERV::~GUI_SLIDERV() {
	tex->lightdeleteRenderTex(tex_id_now);
	tex->lightdeleteRenderTex(tex_id_max);
	tex->lightdeleteRenderTex(tex_id_min);

}

	
bool GUI_SLIDERV::moveBox(int dx, int dy) {
	
	MYRECT r;
	r.left = zentai_box.left;
	r.right =zentai_box.right;
	r.top = zentai_box.top + KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	r.bottom = zentai_box.bottom - KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;

	bool ans = GUI_PART::moveBoxSitei(&box, &r,dx,dy);
	// 移動した後に位置からnowを計算する
//		tex_id_now_y= zentai_box.bottom - 2*KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT - (zentai_box.bottom - zentai_box.top - KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT*3)* (now-  min)/m;

	now = min + (min - max ) * (box.top + 2* KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT - zentai_box.bottom)/(float)(zentai_box.bottom - zentai_box.top - KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT*3);

	tex->setRenderTexPos(tex_id_now, box.left,box.top);
	return ans;
}
bool GUI_SLIDERV::handleMessage(int msg, void* data, DWORD time) {
	
	MYINPUTMESSAGESTRUCT* d = (MYINPUTMESSAGESTRUCT*)data;
	if (getIsDisabled()) return false;
	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSEMOVE) {
		// 範囲に入っているかつ左マウスボタンが押されていない→focus
		// 範囲に入っているかつ左マウスボタンが押されている→press

		if (getIsMove()) {
			

			moveAllBox(d->getMOUSESTATE()->mouse_dx, d->getMOUSESTATE()->mouse_dy);
			
		}

		if (is_min_pressed && getIsEffect()) {
			//moveBox(-1,-1);
			moveBox(-abs(d->getMOUSESTATE()->mouse_dx), -abs(d->getMOUSESTATE()->mouse_dy));
			if (is_call_lua) {
			LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, this->l_str);
			// lua ファイル実行
			} else {
				is_lua_tried_to_send = true;
			}
		}

		if (is_max_pressed && getIsEffect()) {
			//moveBox(1,1);
			moveBox(abs(d->getMOUSESTATE()->mouse_dx), abs(d->getMOUSESTATE()->mouse_dy));
			// lua ファイル実行
			if (is_call_lua) {
			LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, this->l_str);
			}else {
				is_lua_tried_to_send = true;
			}
		}

		if (is_box_moved && getIsEffect() && d->getMOUSESTATE()->mouse_l_button_pressed) {

			moveBox(d->getMOUSESTATE()->mouse_dx, d->getMOUSESTATE()->mouse_dy);
			// lua ファイル実行
			if (is_call_lua) {
			LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, this->l_str);
			} else {
				is_lua_tried_to_send = true;
			}
		}
	

		unsigned int butukari = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &zentai_box);
		if (butukari & BUTUKARIPOINT_IN) {

			

			if (!is_effect) { 
				tex->setRenderTexTexPos(tex_id_min, KTROBO_GUI_SLIDERVMIN_FOCUS_LEFT, KTROBO_GUI_SLIDERVMIN_FOCUS_TOP,
					KTROBO_GUI_SLIDERVMIN_FOCUS_WIDTH,KTROBO_GUI_SLIDERVMIN_FOCUS_HEIGHT);
				tex->setRenderTexTexPos(tex_id_max, KTROBO_GUI_SLIDERVMAX_FOCUS_LEFT, KTROBO_GUI_SLIDERVMAX_FOCUS_TOP,
					KTROBO_GUI_SLIDERVMAX_FOCUS_WIDTH,KTROBO_GUI_SLIDERVMAX_FOCUS_HEIGHT);
				tex->setRenderTexTexPos(tex_id_now, KTROBO_GUI_SLIDERNOW_FOCUS_LEFT, KTROBO_GUI_SLIDERNOW_FOCUS_TOP,
					KTROBO_GUI_SLIDERNOW_FOCUS_WIDTH,KTROBO_GUI_SLIDERNOW_FOCUS_HEIGHT);
			}
		} else {

		// 範囲に入ってなければnormal
			if (!is_effect) {
					tex->setRenderTexTexPos(tex_id_min, KTROBO_GUI_SLIDERVMIN_NORMAL_LEFT, KTROBO_GUI_SLIDERVMIN_NORMAL_TOP,
					KTROBO_GUI_SLIDERVMIN_NORMAL_WIDTH,KTROBO_GUI_SLIDERVMIN_NORMAL_HEIGHT);
				tex->setRenderTexTexPos(tex_id_max, KTROBO_GUI_SLIDERVMAX_NORMAL_LEFT, KTROBO_GUI_SLIDERVMAX_NORMAL_TOP,
					KTROBO_GUI_SLIDERVMAX_NORMAL_WIDTH,KTROBO_GUI_SLIDERVMAX_NORMAL_HEIGHT);
				tex->setRenderTexTexPos(tex_id_now, KTROBO_GUI_SLIDERNOW_NORMAL_LEFT, KTROBO_GUI_SLIDERNOW_NORMAL_TOP,
					KTROBO_GUI_SLIDERNOW_NORMAL_WIDTH,KTROBO_GUI_SLIDERNOW_NORMAL_HEIGHT);
	
			}
		}
	}
	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSERAWSTATE) {










		// 範囲に入っているかつ左マウスボタンが押されている→press
		unsigned int butukari = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &zentai_box);
		if ((butukari & BUTUKARIPOINT_IN) && (d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_R_DOWN)) {
			this->setIsMove(true);
		} else if ((butukari & BUTUKARIPOINT_IN) && (d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_L_DOWN)/* d->getMOUSESTATE()->mouse_l_button_pressed*/) {
			
			unsigned int butu_max = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &max_box);
			unsigned int butu_min = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &min_box);
			unsigned int butu_now = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &box);
			if (butu_max & BUTUKARIPOINT_IN) {
				is_max_pressed = true;
				this->setIsEffect(true);
				moveBox(1,1);
				if (is_call_lua) {
				LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, this->l_str);
				} else {
					is_lua_tried_to_send = true;
				}
				tex->setRenderTexTexPos(tex_id_max, KTROBO_GUI_SLIDERVMAX_PRESS_LEFT, KTROBO_GUI_SLIDERVMAX_PRESS_TOP,
					KTROBO_GUI_SLIDERVMAX_PRESS_WIDTH,KTROBO_GUI_SLIDERVMAX_PRESS_HEIGHT);
			} else if (butu_min & BUTUKARIPOINT_IN) {
				is_min_pressed = true;
				moveBox(-1,-1);
				if (is_call_lua) {
				LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, this->l_str);
				} else {
					is_lua_tried_to_send = true;
				}
				this->setIsEffect(true);
				tex->setRenderTexTexPos(tex_id_min, KTROBO_GUI_SLIDERVMIN_PRESS_LEFT, KTROBO_GUI_SLIDERVMIN_PRESS_TOP,
					KTROBO_GUI_SLIDERVMIN_PRESS_WIDTH,KTROBO_GUI_SLIDERVMIN_PRESS_HEIGHT);
			} else if (butu_now & BUTUKARIPOINT_IN) {
				is_box_moved = true;
				this->setIsEffect(true);
				tex->setRenderTexTexPos(tex_id_now, KTROBO_GUI_SLIDERNOW_PRESS_LEFT, KTROBO_GUI_SLIDERNOW_PRESS_TOP,
					KTROBO_GUI_SLIDERNOW_PRESS_WIDTH,KTROBO_GUI_SLIDERNOW_PRESS_HEIGHT);
			}

		}

		if (d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_R_UP) {
			this->setIsMove(false);
		}

		if ((d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_L_UP)) {
			// ボタンが離されたので
			if (getIsEffect()) {
			
				setIsEffect(false);

				// focusに戻す
				tex->setRenderTexTexPos(tex_id_min, KTROBO_GUI_SLIDERVMIN_FOCUS_LEFT, KTROBO_GUI_SLIDERVMIN_FOCUS_TOP,
					KTROBO_GUI_SLIDERVMIN_FOCUS_WIDTH,KTROBO_GUI_SLIDERVMIN_FOCUS_HEIGHT);
				tex->setRenderTexTexPos(tex_id_max, KTROBO_GUI_SLIDERVMAX_FOCUS_LEFT, KTROBO_GUI_SLIDERVMAX_FOCUS_TOP,
					KTROBO_GUI_SLIDERVMAX_FOCUS_WIDTH,KTROBO_GUI_SLIDERVMAX_FOCUS_HEIGHT);
				tex->setRenderTexTexPos(tex_id_now, KTROBO_GUI_SLIDERNOW_FOCUS_LEFT, KTROBO_GUI_SLIDERNOW_FOCUS_TOP,
					KTROBO_GUI_SLIDERNOW_FOCUS_WIDTH,KTROBO_GUI_SLIDERNOW_FOCUS_HEIGHT);
				is_min_pressed = false;
				is_max_pressed = false;
				is_box_moved =false;
				return false;
			}
				setIsEffect(false);
				is_min_pressed = false;
				is_max_pressed = false;
				is_box_moved =false;
		}
	}

	

	return false;// 次のストラクトに渡す





}
lua_State* GUI_SLIDERH::l=0;
lua_State* GUI_SLIDERV::l=0;

float GUI_SLIDERH::getTexIdNowX() {
		float m = max - min;
	float tex_id_now_x = zentai_box.left+KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	if (m > 0.0001f) {

		tex_id_now_x = zentai_box.left + KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT + (zentai_box.right - zentai_box.left - KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT*3)* (now-  min)/m;
	}

	return tex_id_now_x;
}
void GUI_SLIDERH::setMINMAXNOWFROMZENTAI() {

	float tex_id_now_x = getTexIdNowX();
	box.left = tex_id_now_x;
	box.right = tex_id_now_x + KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	box.top = zentai_box.top;
	box.bottom = zentai_box.bottom;
	min_box.left = zentai_box.left;
	min_box.right = zentai_box.left + KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	min_box.top = zentai_box.top;
	min_box.bottom = zentai_box.bottom;
	max_box.left = zentai_box.right - KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	max_box.right = zentai_box.right;
	max_box.top = zentai_box.top;
	max_box.bottom = zentai_box.bottom;
	tex->setRenderTexPos(tex_id_now, box.left,box.top);
	tex->setRenderTexPos(tex_id_max, max_box.left, max_box.top);
	tex->setRenderTexPos(tex_id_min, min_box.left, min_box.top);
}





float GUI_SLIDERV::getTexIdNowY() {
		float m = max - min;
	float tex_id_now_y= zentai_box.bottom-KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	if (m > 0.0001f) {

		tex_id_now_y= zentai_box.bottom - 2*KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT - (zentai_box.bottom - zentai_box.top - KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT*3)* (now-  min)/m;
	}

	return tex_id_now_y;
}
void GUI_SLIDERV::setMINMAXNOWFROMZENTAI() {
	float tex_id_now_y = getTexIdNowY();
	box.left = zentai_box.left;
	box.right = zentai_box.right;//tex_id_now_x + KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	box.top = tex_id_now_y;
	box.bottom = tex_id_now_y+KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	min_box.left = zentai_box.left;
	min_box.right = zentai_box.right;//zentai.left + KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	min_box.top = zentai_box.bottom - KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	min_box.bottom = zentai_box.bottom;
	max_box.left = zentai_box.left;// - KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	max_box.right = zentai_box.right;
	max_box.top = zentai_box.top;
	max_box.bottom = zentai_box.top + KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;//.bottom;
	tex->setRenderTexPos(tex_id_now, box.left,box.top);
	tex->setRenderTexPos(tex_id_max, max_box.left, max_box.top);
	tex->setRenderTexPos(tex_id_min, min_box.left, min_box.top);
}




GUI_SLIDERH::GUI_SLIDERH(MYRECT zentai, float max, float min, float now, char* l_str) : GUI_PART() {
	this->max = max;
	this->min = min;
	this->now = now;
	this->zentai_box = zentai;
	memset(this->l_str,0 ,64);
	if (strlen(l_str)< 64) {
	strcpy_s(this->l_str,64,l_str);
	}
	this->is_box_moved = false;
	this->is_min_pressed = false;
	this->is_max_pressed = false;


	int tex_id = tex->getTexture(KTROBO_GUI_PNG,1024);

	this->tex_id_max = tex->getRenderTex(tex_id, 0xFFFFFFFF,zentai.right- KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT,zentai.top,
		KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT,zentai.bottom-zentai.top, KTROBO_GUI_SLIDERHMAX_NORMAL_LEFT,
		KTROBO_GUI_SLIDERHMAX_NORMAL_TOP, KTROBO_GUI_SLIDERHMAX_NORMAL_WIDTH, KTROBO_GUI_SLIDERHMAX_NORMAL_HEIGHT);
	this->tex_id_min = tex->getRenderTex(tex_id, 0xFFFFFFFF,zentai.left, zentai.top,KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT,
		zentai.bottom - zentai.top, KTROBO_GUI_SLIDERHMIN_NORMAL_LEFT, KTROBO_GUI_SLIDERHMIN_NORMAL_TOP,
		KTROBO_GUI_SLIDERHMIN_NORMAL_WIDTH, KTROBO_GUI_SLIDERHMIN_NORMAL_HEIGHT);

	float tex_id_now_x = getTexIdNowX();

	this->tex_id_now = tex->getRenderTex(tex_id, 0xFFFFFFFF,tex_id_now_x,zentai.top,KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT, zentai.bottom-zentai.top, 
		KTROBO_GUI_SLIDERNOW_NORMAL_LEFT, KTROBO_GUI_SLIDERNOW_NORMAL_TOP, KTROBO_GUI_SLIDERNOW_NORMAL_WIDTH, KTROBO_GUI_SLIDERNOW_NORMAL_HEIGHT);
	
	setMINMAXNOWFROMZENTAI();
/*	max_boxdayo.left = zentai.left + KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	max_boxdayo.top = zentai.top;
	max_boxdayo.bottom = zentai.bottom;
	max_boxdayo.right = zentai.right - KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	*/
	is_call_lua = true;
	is_lua_tried_to_send = false;
}


GUI_SLIDERH::~GUI_SLIDERH() {

	tex->lightdeleteRenderTex(tex_id_now);
	tex->lightdeleteRenderTex(tex_id_max);
	tex->lightdeleteRenderTex(tex_id_min);



}
	
bool GUI_SLIDERH::moveBox(int dx, int dy) {
	MYRECT r;
	r.left = zentai_box.left+KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	r.right =zentai_box.right- KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT;
	r.top = zentai_box.top;
	r.bottom = zentai_box.bottom;

	bool ans = GUI_PART::moveBoxSitei(&box, &r,dx,dy);
	// 移動した後に位置からnowを計算する
//			tex_id_now_x = zentai_box.left + KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT + (zentai_box.right - zentai_box.left - KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT*3)* (now-  min)/m;
	now = (box.left - zentai_box.left - KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT)*(max-min) / (float)(zentai_box.right- zentai_box.left - KTROBO_GUI_SLIDERMINMAX_WIDTH_HEIGHT*3)+min;
	tex->setRenderTexPos(tex_id_now, box.left,box.top);
	return ans;



}


void GUI_SLIDERH::moveAllBox(int dx, int dy) {

	GUI_PART::moveBoxSitei(&zentai_box,&max_boxdayo, dx, dy);
	// 移動したので
	setMINMAXNOWFROMZENTAI();

	




}


void GUI_SLIDERV::moveAllBox(int dx, int dy) {
	GUI_PART::moveBoxSitei(&zentai_box, &max_boxdayo, dx, dy);
	// 移動したので
	setMINMAXNOWFROMZENTAI();


}


void GUI::setEnable(int gui_id, bool t) {


	CS::instance()->enter(CS_MESSAGE_CS, "seteffect");
	int size = parts.size();
	if (gui_id >=0 && size > gui_id) {
		parts[gui_id]->setIsDisabled(!t);
	}
	CS::instance()->leave(CS_MESSAGE_CS, "seteffect");


}

bool GUI_SLIDERH::handleMessage(int msg, void* data, DWORD time) {


	MYINPUTMESSAGESTRUCT* d = (MYINPUTMESSAGESTRUCT*)data;
	if (getIsDisabled()) return false;

	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSEMOVE) {
		// 範囲に入っているかつ左マウスボタンが押されていない→focus
		// 範囲に入っているかつ左マウスボタンが押されている→press

		if (getIsMove()) {
			

			moveAllBox(d->getMOUSESTATE()->mouse_dx, d->getMOUSESTATE()->mouse_dy);
			
		}

		if (is_min_pressed && getIsEffect()) {
			//moveBox(-1,-1);
			moveBox(-abs(d->getMOUSESTATE()->mouse_dx), -abs(d->getMOUSESTATE()->mouse_dy));
			if (is_call_lua) {
			LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, this->l_str);
			
			// lua ファイル実行
			} else {
				is_lua_tried_to_send = true;
			}
		}

		if (is_max_pressed && getIsEffect()) {
			//moveBox(1,1);
			moveBox(abs(d->getMOUSESTATE()->mouse_dx), abs(d->getMOUSESTATE()->mouse_dy));
			if (is_call_lua) {
			LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, this->l_str);
			
			// lua ファイル実行
			} else {
				is_lua_tried_to_send = true;
			}
		}

		if (is_box_moved && getIsEffect() &&(d->getMOUSESTATE()->mouse_l_button_pressed)) {
			moveBox(d->getMOUSESTATE()->mouse_dx, d->getMOUSESTATE()->mouse_dy);
			// lua ファイル実行
			if (is_call_lua) {
			LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, this->l_str);
			} else {
			is_lua_tried_to_send = true;
			}
		}
	

		unsigned int butukari = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &zentai_box);
		if (butukari & BUTUKARIPOINT_IN) {

			

			if (!is_effect) { 
				tex->setRenderTexTexPos(tex_id_min, KTROBO_GUI_SLIDERHMIN_FOCUS_LEFT, KTROBO_GUI_SLIDERHMIN_FOCUS_TOP,
					KTROBO_GUI_SLIDERHMIN_FOCUS_WIDTH,KTROBO_GUI_SLIDERHMIN_FOCUS_HEIGHT);
				tex->setRenderTexTexPos(tex_id_max, KTROBO_GUI_SLIDERHMAX_FOCUS_LEFT, KTROBO_GUI_SLIDERHMAX_FOCUS_TOP,
					KTROBO_GUI_SLIDERHMAX_FOCUS_WIDTH,KTROBO_GUI_SLIDERHMAX_FOCUS_HEIGHT);
				tex->setRenderTexTexPos(tex_id_now, KTROBO_GUI_SLIDERNOW_FOCUS_LEFT, KTROBO_GUI_SLIDERNOW_FOCUS_TOP,
					KTROBO_GUI_SLIDERNOW_FOCUS_WIDTH,KTROBO_GUI_SLIDERNOW_FOCUS_HEIGHT);
			}
		} else {

		// 範囲に入ってなければnormal
			if (!is_effect) {
					tex->setRenderTexTexPos(tex_id_min, KTROBO_GUI_SLIDERHMIN_NORMAL_LEFT, KTROBO_GUI_SLIDERHMIN_NORMAL_TOP,
					KTROBO_GUI_SLIDERHMIN_NORMAL_WIDTH,KTROBO_GUI_SLIDERHMIN_NORMAL_HEIGHT);
				tex->setRenderTexTexPos(tex_id_max, KTROBO_GUI_SLIDERHMAX_NORMAL_LEFT, KTROBO_GUI_SLIDERHMAX_NORMAL_TOP,
					KTROBO_GUI_SLIDERHMAX_NORMAL_WIDTH,KTROBO_GUI_SLIDERHMAX_NORMAL_HEIGHT);
				tex->setRenderTexTexPos(tex_id_now, KTROBO_GUI_SLIDERNOW_NORMAL_LEFT, KTROBO_GUI_SLIDERNOW_NORMAL_TOP,
					KTROBO_GUI_SLIDERNOW_NORMAL_WIDTH,KTROBO_GUI_SLIDERNOW_NORMAL_HEIGHT);
	
			}
		}
	}
	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSERAWSTATE) {








		// 範囲に入っているかつ左マウスボタンが押されている→press
		unsigned int butukari = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &zentai_box);
		if ((butukari & BUTUKARIPOINT_IN) && (d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_R_DOWN)) {
			this->setIsMove(true);
		} else if ((butukari & BUTUKARIPOINT_IN) && (d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_L_DOWN)) {//l_button_pressed) {
			
			unsigned int butu_max = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &max_box);
			unsigned int butu_min = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &min_box);
			unsigned int butu_now = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &box);
			if (butu_max & BUTUKARIPOINT_IN) {
				is_max_pressed = true;
				this->setIsEffect(true);
				tex->setRenderTexTexPos(tex_id_max, KTROBO_GUI_SLIDERHMAX_PRESS_LEFT, KTROBO_GUI_SLIDERHMAX_PRESS_TOP,
					KTROBO_GUI_SLIDERHMAX_PRESS_WIDTH,KTROBO_GUI_SLIDERHMAX_PRESS_HEIGHT);
				moveBox(1,1);
		//	moveBox(-abs(d->getMOUSESTATE()->mouse_dx), -abs(d->getMOUSESTATE()->mouse_dy));
				if (is_call_lua) {
			LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, this->l_str);
			
				} else {
					is_lua_tried_to_send = true;
				}
			} else if (butu_min & BUTUKARIPOINT_IN) {
				is_min_pressed = true;
				this->setIsEffect(true);
				tex->setRenderTexTexPos(tex_id_min, KTROBO_GUI_SLIDERHMIN_PRESS_LEFT, KTROBO_GUI_SLIDERHMIN_PRESS_TOP,
					KTROBO_GUI_SLIDERHMIN_PRESS_WIDTH,KTROBO_GUI_SLIDERHMIN_PRESS_HEIGHT);
				moveBox(-1,-1);
		//	moveBox(-abs(d->getMOUSESTATE()->mouse_dx), -abs(d->getMOUSESTATE()->mouse_dy));
				if (is_call_lua) {
			LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, this->l_str);
				} else {
					is_lua_tried_to_send = true;
				}
			} else if (butu_now & BUTUKARIPOINT_IN) {
				is_box_moved = true;
				this->setIsEffect(true);
				tex->setRenderTexTexPos(tex_id_now, KTROBO_GUI_SLIDERNOW_PRESS_LEFT, KTROBO_GUI_SLIDERNOW_PRESS_TOP,
					KTROBO_GUI_SLIDERNOW_PRESS_WIDTH,KTROBO_GUI_SLIDERNOW_PRESS_HEIGHT);
			}

		}

		if (d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_R_UP) {
			this->setIsMove(false);
		}

		if ((d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_L_UP)) {
			// ボタンが離されたので
			
			if (getIsEffect()) {
			
				setIsEffect(false);

				// focusに戻す
				tex->setRenderTexTexPos(tex_id_min, KTROBO_GUI_SLIDERHMIN_FOCUS_LEFT, KTROBO_GUI_SLIDERHMIN_FOCUS_TOP,
					KTROBO_GUI_SLIDERHMIN_FOCUS_WIDTH,KTROBO_GUI_SLIDERHMIN_FOCUS_HEIGHT);
				tex->setRenderTexTexPos(tex_id_max, KTROBO_GUI_SLIDERHMAX_FOCUS_LEFT, KTROBO_GUI_SLIDERHMAX_FOCUS_TOP,
					KTROBO_GUI_SLIDERHMAX_FOCUS_WIDTH,KTROBO_GUI_SLIDERHMAX_FOCUS_HEIGHT);
				tex->setRenderTexTexPos(tex_id_now, KTROBO_GUI_SLIDERNOW_FOCUS_LEFT, KTROBO_GUI_SLIDERNOW_FOCUS_TOP,
					KTROBO_GUI_SLIDERNOW_FOCUS_WIDTH,KTROBO_GUI_SLIDERNOW_FOCUS_HEIGHT);


				if (is_call_lua) {
				LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, this->l_str);
				} else {
					is_lua_tried_to_send = true;
				}
			
			is_min_pressed = false;
			is_max_pressed = false;
			is_box_moved =false;
				return false;
			}
			
			is_min_pressed = false;
			is_max_pressed = false;
			is_box_moved =false;
			setIsEffect(false);
			
		}
	}

	

	return false;// 次のストラクトに渡す





}


void GUI::unregisterWindowToInputMessageDispatcher(int gui_window_id) {


	CS::instance()->enter(CS_MESSAGE_CS, "inputmessage");

	if (p_windows_index.find(gui_window_id) != p_windows_index.end()) {
		GUI_WINDOW* w = windows[p_windows_index[gui_window_id]];
		InputMessageDispatcher::unregisterImpl(w);
	}
	CS::instance()->leave(CS_MESSAGE_CS, "inputmessage");




}

int GUIs::makeInst() {
		GUI* g = new GUI();
		guis.push_back(g);
		return guis.size()-1;
	}

	GUI* GUIs::getInstance(int index) {
		if (index >= 0 && index < guis.size()) {
			return guis[index];
		}

		if (index == 1) {
			int s = makeInst();
			if (s !=1) {
				makeInst();
				return getInstance(1);
			}
			return getInstance(1);
		}


		if (index ==0) {
			makeInst();
			return getInstance(0);
		}
		
		throw new GameError(KTROBO::WARNING, "no gui");
	}

	IGUI* GUIs::getInterface(int index) {
		if (index >= 0 && index < guis.size()) {
			return guis[index];
		}

		if (index == 1) {
			int s = makeInst();
			if (s !=1) {
				makeInst();
				return getInterface(1);
			}
			return getInterface(1);
		}


		if (index ==0) {
			makeInst();
			return getInterface(0);
		}
		throw new GameError(KTROBO::WARNING, "no gui");
	}
