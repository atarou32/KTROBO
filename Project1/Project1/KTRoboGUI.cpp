#include "KTRoboGUI.h"
#include "MyButukari.h"

using namespace KTROBO;
GUI::GUI(void)
{
}


GUI::~GUI(void)
{
}

GUI_INPUTTEXT::GUI_INPUTTEXT(float x, float y, float width, float height, Texture* tex) {
	box.left = x;
	box.right = width + x;
	box.top = y;
	box.bottom = y + height;
	texture = tex;
	int tex_id = tex->getTexture(KTROBO_GUI_PNG);
	// ノーマルで！
	is_render = true;
	is_effect = false;
	this->box_tex_id = tex->getRenderTex(tex_id, 0xFFFFFFFF,x,y,width,height, KTROBO_GUI_INPUTTEXT_NORMAL_LEFT, KTROBO_GUI_INPUTTEXT_NORMAL_TOP, KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH, KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT);

	text = new Text(L"",0);
}

GUI_INPUTTEXT::~GUI_INPUTTEXT() {
	if (text) {
		delete text;
		text = 0;
	}
}

bool GUI_INPUTTEXT::handleMessage(int msg, void* data, DWORD time){

	MYINPUTMESSAGESTRUCT* d = (MYINPUTMESSAGESTRUCT*)data;

	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSEMOVE) {
		// 範囲に入っているかつ左マウスボタンが押されていない→focus
		// 範囲に入っているかつ左マウスボタンが押されている→press
		unsigned int butukari = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &box);
		if (butukari & BUTUKARIPOINT_IN) {
			if (!is_effect) { 
				texture->setRenderTexTexPos(box_tex_id, KTROBO_GUI_INPUTTEXT_FOCUS_LEFT, KTROBO_GUI_INPUTTEXT_FOCUS_TOP, KTROBO_GUI_INPUTTEXT_FOCUS_WIDTH,
					KTROBO_GUI_INPUTTEXT_FOCUS_HEIGHT);
			}
		} else {

		// 範囲に入ってなければnormal
			if (!is_effect) {
			texture->setRenderTexTexPos(box_tex_id, KTROBO_GUI_INPUTTEXT_NORMAL_LEFT, KTROBO_GUI_INPUTTEXT_NORMAL_TOP, KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH,
				KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT);
			}
		}
		


	}

	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSERAWSTATE) {
		// 範囲に入っているかつ左マウスボタンが押されている→press
		unsigned int butukari = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &box);
		if ((butukari & BUTUKARIPOINT_IN) && d->getMOUSESTATE()->mouse_l_button_pressed) {
				texture->setRenderTexTexPos(box_tex_id, KTROBO_GUI_INPUTTEXT_PRESS_LEFT, KTROBO_GUI_INPUTTEXT_PRESS_TOP, 
					KTROBO_GUI_INPUTTEXT_PRESS_WIDTH,
					KTROBO_GUI_INPUTTEXT_PRESS_HEIGHT);
				this->setIsEffect(true);
		} else if(!(butukari & BUTUKARIPOINT_IN) && d->getMOUSESTATE()->mouse_l_button_pressed) {
			this->setIsEffect(false);
			// 範囲に入ってなければnormal
			texture->setRenderTexTexPos(box_tex_id, KTROBO_GUI_INPUTTEXT_NORMAL_LEFT, KTROBO_GUI_INPUTTEXT_NORMAL_TOP, KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH,
				KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT);
		}	
	}

	if (msg == KTROBO_INPUT_MESSAGE_ID_KEYDOWN) {
		if (is_effect) {
			text->changeText(L"rr",2);

		}
	}

	return true;



};
void GUI_INPUTTEXT::setIsEffect(bool t) {
	is_effect = t;
}
void GUI_INPUTTEXT::setIsRender(bool t) {
	is_render = t;
	texture->setRenderTexIsRender(box_tex_id,t);
}