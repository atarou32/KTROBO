#include "KTRoboScene.h"
#include "KTRoboGame.h"
#include "tolua_glue/tolua_glue.h"

using namespace KTROBO;

Scene::~Scene(void)
{
}

Graphics* Scene::gs[TASKTHREAD_NUM];
lua_State* Scene::Ls[TASKTHREAD_NUM];
Game* Scene::game=0;

void TCB_SCENE_AI(TCB* thisTCB) {
	Task* t = (Task*)thisTCB->Work[0];
	Scene* s = (Scene*)thisTCB->data;

	s->ai(t,thisTCB);
	


}

void TCB_SCENE_RENDERHOJYO(TCB* thisTCB) {
	Task* t = (Task*)thisTCB->Work[0];
	Scene* s = (Scene*)thisTCB->data;

	s->renderhojyo(t,thisTCB);

}

void TCB_SCENE_POSBUTUKARI(TCB* thisTCB) {
	Task* t = (Task*)thisTCB->Work[0];
	Scene* s = (Scene*)thisTCB->data;

	s->posbutukari(t,thisTCB);


}

void TCB_SCENE_LOAD(TCB* thisTCB) {
	Task* t = (Task*)thisTCB->Work[0];
	Scene* s = (Scene*)thisTCB->data;

	s->loaddestruct(t,thisTCB);

}


void Scene::enter() {
	// シーンに入るときに呼ばれる
	Task** t = game->getTask();	
	unsigned long work[TASK_WORK_SIZE];
	memset(work,0,sizeof(unsigned long)*TASK_WORK_SIZE);
	
	Task* ai_task = t[TASKTHREADS_AIDECISION];
	work[0] = (unsigned long)(ai_task);
	TCB* ai_tcb = ai_task->make(TCB_SCENE_AI, this,work,0x0000FFFF);

	Task* render_hojyo_task = t[TASKTHREADS_UPDATEANIMEFRAMENADO];
	work[0] = (unsigned long)(render_hojyo_task);
	TCB* render_hojyo_tcb = render_hojyo_task->make(TCB_SCENE_RENDERHOJYO, this, work, 0x0000FFFF);

	Task* pos_butukari_task = t[TASKTHREADS_UPDATEPOSBUTUKARI];
	work[0] = (unsigned long)(pos_butukari_task);
	TCB* pos_butukari_tcb = pos_butukari_task->make(TCB_SCENE_POSBUTUKARI, this, work, 0x0000FFFF);
	
	Task* load_task = t[TASKTHREADS_LOADDESTRUCT];
	work[0] = (unsigned long)(load_task);
	TCB* load_tcb = load_task->make(TCB_SCENE_LOAD, this, work, 0x0000FFFF);

	this->looptcbs[TASKTHREADS_AIDECISION] = ai_tcb;
	this->looptcbs[TASKTHREADS_LOADDESTRUCT] = load_tcb;
	this->looptcbs[TASKTHREADS_UPDATEANIMEFRAMENADO] = render_hojyo_tcb;
	this->looptcbs[TASKTHREADS_UPDATEPOSBUTUKARI] = pos_butukari_tcb;
	send_remove_message = false;
}

void Scene::leave() {
	// シーンから抜けるときに呼ばれる

	Task** t = game->getTask();
	if (looptcbs[TASKTHREADS_AIDECISION]) {
		t[TASKTHREADS_AIDECISION]->kill(looptcbs[TASKTHREADS_AIDECISION]);
		looptcbs[TASKTHREADS_AIDECISION] = 0;
	}

	if (looptcbs[TASKTHREADS_LOADDESTRUCT]) {
		t[TASKTHREADS_LOADDESTRUCT]->kill(looptcbs[TASKTHREADS_LOADDESTRUCT]);
		looptcbs[TASKTHREADS_LOADDESTRUCT] = 0;
	}

	if (looptcbs[TASKTHREADS_UPDATEANIMEFRAMENADO]) {
		t[TASKTHREADS_UPDATEANIMEFRAMENADO]->kill(looptcbs[TASKTHREADS_UPDATEANIMEFRAMENADO]);
		looptcbs[TASKTHREADS_UPDATEANIMEFRAMENADO] = 0;
	}

	if (looptcbs[TASKTHREADS_UPDATEPOSBUTUKARI]) {
		t[TASKTHREADS_UPDATEPOSBUTUKARI]->kill(looptcbs[TASKTHREADS_UPDATEPOSBUTUKARI]);
		looptcbs[TASKTHREADS_UPDATEPOSBUTUKARI] = 0;
	}
	send_remove_message = true;
}

#define KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT 30

void ONEMESSAGE::enter() {
	Scene::enter();
	Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
	WCHAR buf[512];
	memset(buf, 0, sizeof(buf));
	stringconverter sc;
	sc.charToWCHAR(message_str.c_str(), buf);
	int len = wcslen(buf);
	char str[512];
	memset(str,0,sizeof(str));
	hmystrcpy(str,512,0,message_str.c_str());
	message = tex->getRenderText(str, gs[TASKTHREADS_AIDECISION]->getScreenWidth()/2 - len*KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT/2,
		gs[TASKTHREADS_AIDECISION]->getScreenHeight()/2 - KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT/2,KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT,len*KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT,KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT);
	tex->setRenderTextIsRender(message, true);

	INPUTGETBYMESSAGESTRUCT* ss  = InputMessageDispatcher::getRootInputGetStruct();
	while (ss->getParent()) {
		ss = ss->getParent();
	}
	InputMessageDispatcher::registerImpl(this, NULL, ss->impl);
	
}

void ONEMESSAGE::leave() {

	InputMessageDispatcher::unregisterImpl(this);
	if (message) {
		Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
		tex->lightdeleteRenderText(message);
		message = 0;
	}

	Scene::leave();
}

ONEMESSAGE::ONEMESSAGE() : Scene("one_message",11){
	message = 0;

}
void ONEMESSAGE::changeText(char* message) {
	message_str = string(message);
	if (this->message) {
		Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
		tex->setRenderTextChangeText(this->message, message);
	}
}
bool ONEMESSAGE::handleMessage(int msg, void* data, DWORD time) {

	MYINPUTMESSAGESTRUCT* input = (MYINPUTMESSAGESTRUCT*)data;



	if (msg == KTROBO_INPUT_MESSAGE_ID_KEYDOWN) {
		if (input->getKEYSTATE()[VK_RETURN] & KTROBO_INPUT_BUTTON_DOWN) {
			if (!send_remove_message) {
			LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION,true, "resrc/script/SCENE_remove.lua");
			send_remove_message = true;
			}
			return true;

		}
	}
	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSERAWSTATE) {
		if (input->getMOUSESTATE()->mouse_button & (KTROBO_MOUSESTATE_R_DOWN | KTROBO_MOUSESTATE_L_DOWN)) {
			MYRECT re;
			re.left = 0;
			re.right = gs[TASKTHREADS_AIDECISION]->getScreenWidth();
			re.top = 0;
			re.bottom = gs[TASKTHREADS_AIDECISION]->getScreenHeight();
			unsigned int butu = getButukariStatusPoint(input->getMOUSESTATE()->mouse_x, input->getMOUSESTATE()->mouse_y, &re);
			if (butu & BUTUKARIPOINT_IN) {
				if (!send_remove_message) {
					LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION,true, "resrc/script/SCENE_remove.lua");
					send_remove_message = true;
				}
			}
			return true;

		}
	}

	return true;
}





bool TWOTAKU::handleMessage(int msg, void* data, DWORD time){
	return true;
}

void TWOTAKU::enter() {
	Scene::enter();
	Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
//	GUI* gui = MyLuaGlueSingleton::getInstance()->getColGUIs(0)->getInstance(1); // メッセージ表示関連は1を使用する
	WCHAR buf[512];
	memset(buf, 0, sizeof(buf));
	stringconverter sc;
	sc.charToWCHAR(srender_text.c_str(), buf);
	int len = wcslen(buf);
	char str[512];
	memset(str,0,sizeof(str));
	hmystrcpy(str,512,0,srender_text.c_str());

	render_text = tex->getRenderText(str, gs[TASKTHREADS_AIDECISION]->getScreenWidth()/2 - len*KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT/2,
		gs[TASKTHREADS_AIDECISION]->getScreenHeight()/2 - KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT/2*3,KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT,len*KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT,KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT);
	tex->setRenderTextIsRender(render_text, true);

	memset(str,0,sizeof(str));
	hmystrcpy(str,512,0,yes_str.c_str());
	/*
	yes_button = gui->makeButton(gs[TASKTHREADS_AIDECISION]->getScreenWidth()/4, gs[TASKTHREADS_AIDECISION]->getScreenHeight()/2,
		KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT*2,KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT, str, strlen(str), "はい");

	memset(str,0,sizeof(str));
	hmystrcpy(str,512,0,no_str.c_str());
	no_button = gui->makeButton(gs[TASKTHREADS_AIDECISION]->getScreenWidth()*3/4, gs[TASKTHREADS_AIDECISION]->getScreenHeight()/2,
		KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT*2,KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT, str, strlen(str), "いいえ");
	window_id = gui->makeWindow(-10,-10,1,1);
	gui->setPartToWindow(window_id, yes_button);
	gui->setPartToWindow(window_id, no_button);
	gui->setRender(window_id, true);
	gui->setEffect(window_id, true);
	gui->setRender(yes_button, true);
	gui->setEffect(yes_button, true);
	gui->setRender(no_button, true);
	gui->setEffect(no_button, true);



	// 下のクラスにいかせないために先に登録しておく　setrootwindowtoinputmessagedispatcherよりも先に登録すること
	INPUTGETBYMESSAGESTRUCT* ss  = InputMessageDispatcher::getRootInputGetStruct();
	while (ss->getParent()) {
		ss = ss->getParent();
	}
	*/
	//InputMessageDispatcher::registerImpl(this, NULL, ss->impl);
//	gui->setRootWindowToInputMessageDispatcher(window_id);



}


void TWOTAKU::leave() {
	Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
//	GUI* gui = MyLuaGlueSingleton::getInstance()->getColGUIs(0)->getInstance(1); // メッセージ表示関連は1を使用する
	if (window_id) {
	//	gui->unregisterWindowToInputMessageDispatcher(window_id);
		window_id = 0;
	}
	if(yes_button) {
		
		yes_button = 0;
	}
	if (no_button) {
		
		no_button = 0;
	}
	InputMessageDispatcher::unregisterImpl(this);
	if (render_text) {
		tex->lightdeleteRenderText(render_text);
		render_text = 0;
	}
	//gui->deleteAll();
	
	Scene::leave();
}
void TWOTAKU::setYesStr(char* y) {
		yes_str = string(y);
}
void TWOTAKU::setNoStr(char* n) {
		no_str = string(n);
}
void TWOTAKU::setRenderText(char* t) {
		srender_text = string(t);
}


TWOTAKU::TWOTAKU ( char* dyes_str, char* dno_str, char* dtext) : Scene("twotaku",7) {
	this->yes_str = string(dyes_str);
	this->no_str = string(dno_str);
	this->srender_text = string(dtext);
}









void LOADTYUU::enter() {
	Scene::enter();

	Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
	this->string_id = tex->getRenderText("セーブロード中・・・", gs[TASKTHREADS_AIDECISION]->getScreenWidth()/2 - 12*KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT/2,
		gs[TASKTHREADS_AIDECISION]->getScreenHeight()/2 - KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT/2,KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT,12*KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT,KTROBO_SCENE_ONEMESSAGE_STR_HEIGHT);
	tex->setRenderTextIsRender(string_id, true);

	INPUTGETBYMESSAGESTRUCT* ss  = InputMessageDispatcher::getRootInputGetStruct();
	while (ss->getParent()) {
		ss = ss->getParent();
	}
	InputMessageDispatcher::registerImpl(this, NULL, ss->impl);
}

void LOADTYUU::renderhojyoIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game)
	{
		time =timeGetTime();
		Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
		static char* strs[] = {
			"セーブロード中・・・",
			"セーブロード中・・",
			"セーブロード中・"
		};

		int ddtime = time % 1000;
		int dddtime = 0;
		if (ddtime < 333) {
			dddtime = 0;
		} else if(ddtime < 667) {
			dddtime = 1;
		} else {
			dddtime = 2;
		}
		if (dtime != dddtime) {
			dtime = dddtime;
			tex->setRenderTextChangeText(string_id, strs[dtime % 3]);

	}
}

void LOADTYUU::leave() {
	Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);

	if (string_id) {
		tex->lightdeleteRenderText(string_id);
		string_id = 0;
	}
	InputMessageDispatcher::unregisterImpl(this);
	Scene::leave();
}


bool LOADTYUU::handleMessage(int msg, void* data, DWORD time) {
	return true;
}
LOADTYUU::LOADTYUU(): Scene("loadtyuu", 8) {
	dtime = 0;
	time = 0;
	string_id = 0;
}