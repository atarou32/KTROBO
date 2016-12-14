#include "KTRoboKendoSinaiGenerator.h"
#include "KTRoboTexture.h"
#include "lua.h"
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

void SinaiFuruAnimeMaker::sinaiNigiruhitoUpdate() {



}

void SinaiFuruAnimeMaker::update(float dt) {


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


	return true;
}