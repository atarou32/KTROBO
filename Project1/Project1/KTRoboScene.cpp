#include "KTRoboScene.h"
#include "KTRoboGame.h"

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
	
	Task* load_task = t[TASKTHREADS_UPDATEPOSBUTUKARI];
	work[0] = (unsigned long)(load_task);
	TCB* load_tcb = load_task->make(TCB_SCENE_LOAD, this, work, 0x0000FFFF);

	this->looptcbs[TASKTHREADS_AIDECISION] = ai_tcb;
	this->looptcbs[TASKTHREADS_LOADDESTRUCT] = load_tcb;
	this->looptcbs[TASKTHREADS_UPDATEANIMEFRAMENADO] = render_hojyo_tcb;
	this->looptcbs[TASKTHREADS_UPDATEPOSBUTUKARI] = pos_butukari_tcb;
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
}