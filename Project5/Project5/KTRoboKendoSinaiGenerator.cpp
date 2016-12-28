#include "KTRoboKendoSinaiGenerator.h"
#include "KTRoboTexture.h"
#include "lua.h"
#include "KTRoboAtari.h"

using namespace KTROBO;

KendoSinaiGenerator::KendoSinaiGenerator(void)
{
	gui = 0;
	screen_window_id = 0;
	furu_play_toggle_button_id = 0;
	slider_sinai_dt_id = 0;
	slider_nigiruhito_anime_id = 0;
	nigiruhito_anime_set_button_id = 0;
	undo_button_id = 0;
	erase_anime_set_button_id = 0;
	sinai_anime_set_button_id = 0;
	erase_furu_sinai_button_id = 0;
	tex = 0;
	dpos = MYVECTOR3(0,0,0);
	drotx = 0;
	drotz = 0;
	dt = 0;
	for (int i = 0; i<KTROBO_MESH_BONE_MAX; i++) {
		anime[i] = 0;
	}
/*
	GUI* gui;
	int screen_window_id;
	int furu_play_toggle_button_id;
	int slider_sinai_dt_id;

	int slider_nigiruhito_anime_id;
	int nigiruhito_anime_set_button_id;

	int undo_button_id;
	int erase_anime_set_button_id;
	int erase_furu_sinai_button_id;

	Texture* tex;

	// sinai の位置と姿勢
	MYVECTOR3 dpos;
	float drotx;
	float drotz;
	
	vector<SinaiFuruPart> furu_parts;
	vector<int> furu_parts_tex;

	vector<KendoSinaiNigiruhitoPart> nigiruhito_parts;
	vector<int> nigiruhito_parts_tex;

	// 現在のパラメータ
	float dt;
	float anime[KTROBO_MESH_BONE_MAX];


*/
}


float KendoSinaiGenerator::getNigiruhitoAnimeTemp() {

	if (slider_nigiruhito_anime_id) {
		float now = gui->getNowFromSlider(slider_nigiruhito_anime_id);
		return now;

	}
	return 0;

}

KendoSinaiGenerator::~KendoSinaiGenerator(void)
{
}



SinaiFuruAnimeMaker::SinaiFuruAnimeMaker() {


}


SinaiFuruAnimeMaker::~SinaiFuruAnimeMaker() {




}




void KendoSinaiGenerator::Init(HWND hw, KTROBO::Texture* tex, lua_State* Ld, int screen_width, int screen_height) {


	gui = new GUI();
	gui->Init(hw, tex,Ld,screen_width,screen_height);

	screen_window_id = gui->makeWindow(0,0,screen_width,screen_height);
	gui->setEffect(screen_window_id,true);
	gui->setEnable(screen_window_id,true);
	gui->setRender(screen_window_id, true);
	
/*
	int furu_play_toggle_button_id;
	int slider_sinai_dt_id;

	int slider_nigiruhito_anime_id;
	int nigiruhito_anime_set_button_id;

	int undo_button_id;
	int erase_anime_set_button_id;
	int erase_furu_sinai_button_id;
*/
	
	furu_play_toggle_button_id  = gui->makeButton(100,200,50,50,"resrc/script/KSG_togglebutton.lua",33,"tog");
	gui->setEffect(furu_play_toggle_button_id,false);
	gui->setEnable(furu_play_toggle_button_id,true);
	gui->setRender(furu_play_toggle_button_id,true);
	gui->setPartToWindow(screen_window_id, furu_play_toggle_button_id);

	MYRECT zentai;
	zentai.left = 100;
	zentai.right = 800;
	zentai.top = 600;
	zentai.bottom = 630;

	slider_sinai_dt_id = gui->makeSliderH(&zentai,1000,0,0,"resrc/script/KSG_NONE.lua");
	gui->setEffect(slider_sinai_dt_id,true);
	gui->setEnable(slider_sinai_dt_id,true);
	gui->setRender(slider_sinai_dt_id,true);
	gui->setPartToWindow(screen_window_id, slider_sinai_dt_id);


	zentai.left = 100;
	zentai.right = 800;
	zentai.top = 670;
	zentai.bottom = 700;
	slider_nigiruhito_anime_id = gui->makeSliderH(&zentai, 512,0,0,"resrc/script/KSG_s_nigiruhitoupdate.lua");
	gui->setEffect(slider_nigiruhito_anime_id,true);
	gui->setEnable(slider_nigiruhito_anime_id,true);
	gui->setRender(slider_nigiruhito_anime_id,true);
	gui->setPartToWindow(screen_window_id, slider_nigiruhito_anime_id);

	nigiruhito_anime_set_button_id = gui->makeButton(810,670,20,20,"resrc/script/KSG_nh_setanime.lua",32,"nha");
	gui->setEffect(nigiruhito_anime_set_button_id,false);
	gui->setEnable(nigiruhito_anime_set_button_id,true);
	gui->setRender(nigiruhito_anime_set_button_id,true);
	gui->setPartToWindow(screen_window_id, nigiruhito_anime_set_button_id);

	sinai_anime_set_button_id = gui->makeButton(810,630,20,20,"resrc/script/KSG_s_setanime.lua",31,"sa");
	gui->setEffect(sinai_anime_set_button_id,false);
	gui->setEnable(sinai_anime_set_button_id,true);
	gui->setRender(sinai_anime_set_button_id,true);
	gui->setPartToWindow(screen_window_id, sinai_anime_set_button_id);


	gui->setRootWindowToInputMessageDispatcher(screen_window_id);

}

void KendoSinaiGenerator::Del() {

	if (gui) {
		gui->deleteAll();
		delete gui;
		gui = 0;
	}
}



void SinaiFuruAnimeMaker::togglePlayAnime() {
}

void SinaiFuruAnimeMaker::nigiruhitoSetAnime() {

}
void SinaiFuruAnimeMaker::nigiruhitoEraseAnime() {


}

void SinaiFuruAnimeMaker::sinaiEraseAnime() {


}
void SinaiFuruAnimeMaker::sinaiSetAnime() {


}

void SinaiFuruAnimeMaker::undo() {


}
void KendoSinaiGenerator::setSinaiPosRot(SinaiFuru* sf) {

	sf->setKAMAE(this->drotx,drotz,&dpos);

}



void SinaiFuruAnimeMaker::sinaiNigiruhitoUpdate() {
	float anime = this->generator->getNigiruhitoAnimeTemp();
//	bool bb = false;
//	this->nigiruhito->calcAnimeFrame(1,&anime,&bb);
//	this->nigiruhito->calcJyusinAndR();

}

void SinaiFuruAnimeMaker::update(float dt, SinaiFuru* furu) {
	generator->setSinaiPosRot(furu);
}


int SinaiFuruAnimeMakers::makeInst() {
	int bsize = makers.size();
	if (bsize ==0) {
		SinaiFuruAnimeMaker* mk = new SinaiFuruAnimeMaker();
		makers.push_back(mk);
		return 0;
	}
	return -1;
}

SinaiFuruAnimeMaker* SinaiFuruAnimeMakers::getInstance(int index) {

	makeInst();
	return makers[0];


}

ISinaiFuruAnimeMaker* SinaiFuruAnimeMakers::getInterface(int index) {
	makeInst();
	return makers[0];
}


bool KendoSinaiGenerator::handleMessage(int msg, void* data, DWORD time) {

	if (msg == KTROBO_INPUT_MESSAGE_ID_KEYDOWN) {
		MYINPUTMESSAGESTRUCT* input = (MYINPUTMESSAGESTRUCT*) data;

		if (input->getKEYSTATE()['Q'] & KTROBO_INPUT_BUTTON_PRESSED) {
				CS::instance()->enter(CS_MESSAGE_CS, "enter");
				this->dpos.float3.x += 0.1f;

				CS::instance()->leave(CS_MESSAGE_CS, "leave");
		}

		if (input->getKEYSTATE()['A'] & KTROBO_INPUT_BUTTON_PRESSED) {
				CS::instance()->enter(CS_MESSAGE_CS, "enter");
				dpos.float3.x -= 0.1f;

				CS::instance()->leave(CS_MESSAGE_CS, "leave");
		}

		if (input->getKEYSTATE()['W'] & KTROBO_INPUT_BUTTON_PRESSED) {
				CS::instance()->enter(CS_MESSAGE_CS, "enter");
				dpos.float3.y += 0.1f;

				CS::instance()->leave(CS_MESSAGE_CS, "leave");
		}

		if (input->getKEYSTATE()['S'] & KTROBO_INPUT_BUTTON_PRESSED) {
				CS::instance()->enter(CS_MESSAGE_CS, "enter");
				dpos.float3.y -= 0.1f;

				CS::instance()->leave(CS_MESSAGE_CS, "leave");
		}

		if (input->getKEYSTATE()['E'] & KTROBO_INPUT_BUTTON_PRESSED) {
				CS::instance()->enter(CS_MESSAGE_CS, "enter");
				dpos.float3.z += 0.1f;

				CS::instance()->leave(CS_MESSAGE_CS, "leave");
		}

		if (input->getKEYSTATE()['D'] & KTROBO_INPUT_BUTTON_PRESSED) {
				CS::instance()->enter(CS_MESSAGE_CS, "enter");
				dpos.float3.z -= 0.1f;

				CS::instance()->leave(CS_MESSAGE_CS, "leave");
		}

		if (input->getKEYSTATE()['R'] & KTROBO_INPUT_BUTTON_PRESSED) {
			drotx +=0.1f;
		}

		if (input->getKEYSTATE()['F'] & KTROBO_INPUT_BUTTON_PRESSED) {
			this->drotx -=0.1f;
		}

	}
	return true;
}

void SinaiFuruAnimeMaker::Init(KendoSinaiGenerator* generator, UMeshUnit* nigiruhito, Sinai* sinai) {
	this->generator = generator;
	this->nigiruhito = nigiruhito;
	this->sinai = sinai;





}