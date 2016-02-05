#include "KTRoboGUI.h"
#include "MyButukari.h"
#include "MyTokenAnalyzer.h"
#include "stringconverter.h"

using namespace KTROBO;

GUI::GUI(void)
{
}


GUI::~GUI(void)
{
}


bool GUI_BUTTON::handleMessage(int msg, void* data, DWORD time){

		MYINPUTMESSAGESTRUCT* d = (MYINPUTMESSAGESTRUCT*)data;

	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSEMOVE) {
		// 範囲に入っているかつ左マウスボタンが押されていない→focus
		// 範囲に入っているかつ左マウスボタンが押されている→press

		if (getIsMove()) {
			

			moveBox(d->getMOUSESTATE()->mouse_dx, d->getMOUSESTATE()->mouse_dy);
			texture->setRenderTexPos(box_tex_id, box.left, box.top);
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
		//	}
		}
	}
	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSERAWSTATE) {
		// 範囲に入っているかつ左マウスボタンが押されている→press
		unsigned int butukari = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &box);
		if ((butukari & BUTUKARIPOINT_IN) && (d->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_R_DOWN)) {
			this->setIsMove(true);
		} else if ((butukari & BUTUKARIPOINT_IN) && d->getMOUSESTATE()->mouse_l_button_pressed) {
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
				setIsEffect(false);

				// focusに戻す
				texture->setRenderTexTexPos(box_tex_id, KTROBO_GUI_BUTTON_FOCUS_LEFT,
					KTROBO_GUI_BUTTON_FOCUS_TOP,
					KTROBO_GUI_BUTTON_FOCUS_WIDTH,
					KTROBO_GUI_BUTTON_FOCUS_HEIGHT);
				return true;
			}
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
}
GUI_BUTTON::GUI_BUTTON(float x, float y, float width, float height, char* luaf, int len, char* info) : GUI_PART() {
	box.left = x;
	box.right = width + x;
	box.top = y;
	box.bottom = y + height;
	int tex_id = texture->getTexture(KTROBO_GUI_PNG);
	this->box_tex_id = texture->getRenderTex(tex_id, 0xFFFFFFFF,x,y,width,height, KTROBO_GUI_BUTTON_NORMAL_LEFT, KTROBO_GUI_BUTTON_NORMAL_TOP,
		KTROBO_GUI_BUTTON_NORMAL_WIDTH, KTROBO_GUI_BUTTON_NORMAL_HEIGHT);
	if (len < 128) {
		memcpy(this->l_str, luaf, len);
	} else {
		memset(l_str,0,128);
	}
	stringconverter sc;
	WCHAR buf[512];
	memset(buf,0,sizeof(WCHAR)*512);
	sc.charToWCHAR(info,buf);
	button_text = new Text(buf,wcslen(buf));
	
}
GUI_BUTTON::~GUI_BUTTON() {

	texture->lightdeleteRenderTex(box_tex_id);
	delete button_text;
}

void GUI_BUTTON::render(Graphics* g) {
	if (getIsRender()) {
		float width = button_text->getWidth(KTROBO_GUI_BUTTON_TEXT_HEIGHT);
		button_text->render(g,0xFFFFFFFF, (box.left + box.right)/2 - width/2, (box.top+box.bottom)/2 -KTROBO_GUI_BUTTON_TEXT_HEIGHT/2, KTROBO_GUI_BUTTON_TEXT_HEIGHT,
			width,KTROBO_GUI_BUTTON_TEXT_HEIGHT);
	}
}

GUI_INPUTTEXT::GUI_INPUTTEXT(float x, float y, float width, float height) : GUI_PART() {
	box.left = x;
	box.right = width + x;
	box.top = y;
	box.bottom = y + height;
	int tex_id = texture->getTexture(KTROBO_GUI_PNG);
	// ノーマルで！
	is_render = true;
	is_effect = false;
	this->box_tex_id_naka = texture->getRenderTex(tex_id, 0xFFFFFFFF,x + KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,y + KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,
		width- 2* KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,height- 2 * KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY,
		KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_NORMAL_TOP + KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
		KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH - 2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, 
		KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT-2 *KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);

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
}

void GUI_PART::moveBox(int dx, int dy) {


	// 動かす
	box.left += dx;
	box.right += dx;
	box.top += dy;
	box.bottom += dy;
	if (box.left < max_box.left) {
		box.right += max_box.left-box.left;
		box.left = max_box.left;
	}
	if (box.top < max_box.top) {
		box.bottom += max_box.top-box.top;
		box.top = max_box.top;
	}
	if (box.right > max_box.right) {
		box.left += max_box.right - box.right;
		box.right = max_box.right;
	}
	if (box.bottom > max_box.bottom) {
		box.top += max_box.bottom - box.bottom;
		box.bottom = max_box.bottom;
	}
}

bool GUI_INPUTTEXT::handleMessage(int msg, void* data, DWORD time){

	MYINPUTMESSAGESTRUCT* d = (MYINPUTMESSAGESTRUCT*)data;

	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSEMOVE) {
		// 範囲に入っているかつ左マウスボタンが押されていない→focus
		// 範囲に入っているかつ左マウスボタンが押されている→press

		if (getIsMove()) {
			


			this->moveBox(d->getMOUSESTATE()->mouse_dx, d->getMOUSESTATE()->mouse_dy);

				texture->setRenderTexPos(box_tex_id_hidariue, box.left, box.top);
				texture->setRenderTexPos(box_tex_id_hidarinaka, box.left, box.top -1 + KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY);
				texture->setRenderTexPos(box_tex_id_hidarisita, box.left, box.bottom - KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY-3);
				texture->setRenderTexPos(box_tex_id_migiue, box.right-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, box.top);
				texture->setRenderTexPos(box_tex_id_miginaka, box.right-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, -1+box.top+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY);
				texture->setRenderTexPos(box_tex_id_migisita, box.right-KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, box.bottom -3- KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY);
				texture->setRenderTexPos(box_tex_id_uenaka, box.left+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, box.top);
				texture->setRenderTexPos(box_tex_id_naka, box.left+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, -1+box.top + KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY);
				texture->setRenderTexPos(box_tex_id_sitanaka, box.left +KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY, box.bottom -3- KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA_XY);

		}


		unsigned int butukari = getButukariStatusPoint(d->getMOUSESTATE()->mouse_x, d->getMOUSESTATE()->mouse_y, &box);
		if (butukari & BUTUKARIPOINT_IN) {

			

			if (!is_effect) { 
				texture->setRenderTexTexPos(box_tex_id_naka, KTROBO_GUI_INPUTTEXT_FOCUS_LEFT+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, KTROBO_GUI_INPUTTEXT_FOCUS_TOP+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
					KTROBO_GUI_INPUTTEXT_FOCUS_WIDTH-2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
					KTROBO_GUI_INPUTTEXT_FOCUS_HEIGHT-2 *KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);



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
				texture->setRenderTexTexPos(box_tex_id_naka, KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
					KTROBO_GUI_INPUTTEXT_NORMAL_TOP + KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
					KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH - 2 * KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
					KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT - 2 * KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);




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
			texture->setRenderTexTexPos(box_tex_id_naka, KTROBO_GUI_INPUTTEXT_PRESS_LEFT+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
				KTROBO_GUI_INPUTTEXT_PRESS_TOP+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA, 
				KTROBO_GUI_INPUTTEXT_PRESS_WIDTH-2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
				KTROBO_GUI_INPUTTEXT_PRESS_HEIGHT-2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);







			
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
			texture->setRenderTexTexPos(box_tex_id_naka, KTROBO_GUI_INPUTTEXT_NORMAL_LEFT+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
				KTROBO_GUI_INPUTTEXT_NORMAL_TOP+KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
				KTROBO_GUI_INPUTTEXT_NORMAL_WIDTH-2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA,
				KTROBO_GUI_INPUTTEXT_NORMAL_HEIGHT-2*KTROBO_GUI_INPUTTEXT_BOX_SOTOHABA);


			
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

void GUI_INPUTTEXT::render(Graphics* g) {
	if (text) {
		text->render(g, 0xFFFFFFFF,box.left+5,box.top+2, box.bottom-box.top-4, box.right-box.left-10,box.bottom - box.top);
	}
}

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