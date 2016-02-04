#ifndef KTROBOSCENE_H
#define KTROBOSCENE_H

#pragma once
#include "KTRoboGraphics.h"
//#include "KTRoboGame.h"
#include "lua.hpp"
#include "MyDefine.h"
#include "KTRoboGameError.h"
#include "KTRoboTask.h"
#include "MyTokenAnalyzer.h"

namespace KTROBO {

class Game;

// �V�[�����m�̃X�g���N�g�󂯓n���Ȃǂ͂ǂ�����̂�
// tolua�̃V���O���g���ɑS�Ď�������
// tolua�Ƃ͕ʂɃV���O���g�������Ƃ�����������Ȃ�


class Scene
{
private:
	char scene_name[32];
	static Graphics* gs[TASKTHREAD_NUM];
	static lua_State* Ls[TASKTHREAD_NUM];

	static Game* game;
	TCB* looptcbs[TASKTHREAD_NUM];
	
public:	
	virtual void mainrenderIMPL(bool is_focused, Graphics* g, Game* game)=0;
	virtual void renderhojyoIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game)=0;
	virtual void aiIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game)=0;
	virtual void posbutukariIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game)=0;
	virtual void loaddestructIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game)=0;
public:
	virtual void enter(); // �V�[���ɓ���Ƃ��ɌĂ΂�� enter leave ���Ă΂��̂�AI�X���b�h����
	//�i��ԏ�ʂ̃X���b�h�j�ő��̃X���b�h�i�C���v�b�g�ȊO�H�j�����b�N���čs��
	virtual void leave(); // �V�[�����甲����Ƃ��ɌĂ΂��

	static void Init(Graphics** mgs, lua_State** mls, Game* mgame) {
		for (int i=0;i<TASKTHREAD_NUM;i++) {
			gs[i] = mgs[i];
			Ls[i] = mls[i];
		}
		Scene::game = mgame; // gs ls game �̓o�^�Ɏg��
	}

	static void Del() {};

	Scene(char* n, int len) {
		if (len >= 31) {
			memcpy(scene_name,n,31);
			scene_name[31] = 0;
		} else {
			memcpy(scene_name,n, len);
		}
		for (int i=0;i<TASKTHREAD_NUM;i++) {
			looptcbs[i] = 0;
		}
		
	}
	~Scene(void);

	void mainrender(bool is_focused){
		mainrenderIMPL(is_focused, gs[TASKTHREADS_UPDATEMAINRENDER], game);
	};
	void renderhojyo(Task* task, TCB* thisTCB){
		renderhojyoIMPL(task, thisTCB, gs[TASKTHREADS_UPDATEANIMEFRAMENADO], Ls[TASKTHREADS_UPDATEANIMEFRAMENADO], game);
	
	};
	void ai(Task* task, TCB* thisTCB){
		aiIMPL(task, thisTCB, gs[TASKTHREADS_AIDECISION], Ls[TASKTHREADS_AIDECISION], game);
	
	};
	void posbutukari(Task* task,TCB* thisTCB){
		posbutukariIMPL(task,thisTCB, gs[TASKTHREADS_UPDATEPOSBUTUKARI], Ls[TASKTHREADS_UPDATEPOSBUTUKARI], game);
	
	};
	void loaddestruct(Task* task, TCB* thisTCB){
		loaddestructIMPL(task,thisTCB,gs[TASKTHREADS_LOADDESTRUCT], Ls[TASKTHREADS_LOADDESTRUCT], game);
	};
};
}




#endif