#include "KTRoboTask.h"
#include "KTRoboLog.h"
#include "KTRoboGameError.h"
#include "KTRoboCS.h"

using namespace KTROBO;

void MyTaskHead(TCB* TC) {
	
}

int Task::NOW_TASK_THREAD_NUM = 0;

void THREAD_TaskEXEC(void* data) {
	Task* t = (Task*)data;
	while(t->getIsExecTask()) {
		t->exec();
	}
	
}

void KTROBO::Task::init() {
	TCB* tmpTCB;
	
	is_exec_task = true;
	

	ZeroMemory(Tasks, sizeof(TCB)*MAX_TASK_COUNT);
	tmpTCB = Tasks;
	tmpTCB->Exec = MyTaskHead;
	tmpTCB->prio = TASK_HEAD_PRIO;
	tmpTCB->prev = NULL;
	tmpTCB->next = Tasks;
	tmpTCB->flag = TASK_USE;
	hd = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)THREAD_TaskEXEC,(void*)this,0,NULL);
};


void KTROBO::Task::exec() {
	TCB* execTCB;

	CS::instance()->enter(CS_TASK_CS,"", index);
	execTCB = Tasks;
	try {
	do {
		execTCB->Exec(execTCB);
		execTCB = execTCB->next;
	} while(execTCB->prio != TASK_HEAD_PRIO);
	} catch (GameError* err) {
		mylog::writelog(err->getErrorCode(),err->getMessage());
	//	MessageBoxA(hWnd,err->getMessage(),KTROBO::GameError::getErrorCodeString(err->getErrorCode()),MB_OK);
		if (err->getErrorCode() == KTROBO::FATAL_ERROR) {
			// 終了させる
	//		MessageBoxA(hWnd,"タスクスレッドを終了させます","FATAL",MB_OK);
			is_exec_task = false;
			delete err;
			CS::instance()->leave(CS_TASK_CS,"",index);
			return;
		}
		delete err;
		err_count++;
		if (err_count > TASK_ERR_COUNT_MAX) {


			is_exec_task = false;
			CS::instance()->leave(CS_TASK_CS, "", index);
			return;
		}
		Sleep(1); // そのまま続行
	}

	CS::instance()->leave(CS_TASK_CS,"",index);


	Sleep(1);
}


void KTROBO::Task::donow(void(*exec)(TCB*), void* data,  unsigned long* work,unsigned int prio) {



	TCB* newTCB;
	TCB* prevTCB;
	TCB* nextTCB;
	int id;
	CS::instance()->enter(CS_TASK_CS,"",index);

	for(id=0;id<MAX_TASK_COUNT;id++) {
		if (!(Tasks[id].flag & TASK_USE)) break;
	}

	if (id == MAX_TASK_COUNT) {
		CS::instance()->leave(CS_TASK_CS,"",index);
		throw new GameError(KTROBO::FATAL_ERROR, "to many tasks");
		//return NULL;
	}
	newTCB = &Tasks[id];
	

	ZeroMemory(newTCB, sizeof(TCB));
	newTCB->Exec = exec;
	newTCB->data = data;
	newTCB->prio = prio;

	
	for (int i=0;i<TASK_WORK_SIZE;i++) {
		newTCB->Work[i] = work[i];
	}

	newTCB->flag = 0;
	exec(newTCB);
	CS::instance()->leave(CS_TASK_CS,"",index);
	return;

}
TCB* KTROBO::Task::make(void(*exec)(TCB*),void* data, unsigned long* work, unsigned int prio) {
	TCB* newTCB;
	TCB* prevTCB;
	TCB* nextTCB;
	int id;
	CS::instance()->enter(CS_TASK_CS,"",index);

	for(id=0;id<MAX_TASK_COUNT;id++) {
		if (!(Tasks[id].flag & TASK_USE)) break;
	}

	if (id == MAX_TASK_COUNT) {
		CS::instance()->leave(CS_TASK_CS,"",index);
		throw new GameError(KTROBO::FATAL_ERROR, "to many tasks");
		//return NULL;
	}
	newTCB = &Tasks[id];
	prevTCB = Tasks;
	while (prevTCB->next->prio != TASK_HEAD_PRIO) {
		if (prevTCB->next->prio > prio) break;
		prevTCB = prevTCB->next;
	}
	nextTCB = prevTCB->next;

	ZeroMemory(newTCB, sizeof(TCB));
	newTCB->Exec = exec;
	newTCB->data = data;
	newTCB->prio = prio;

	newTCB->prev = prevTCB;
	newTCB->next = nextTCB;
	for (int i=0;i<TASK_WORK_SIZE;i++) {
		newTCB->Work[i] = work[i];
	}
	prevTCB->next = newTCB;
	nextTCB->prev = newTCB;
	newTCB->flag = TASK_USE;
	CS::instance()->leave(CS_TASK_CS,"",index);
	return newTCB;
}
void KTROBO::Task::kill(TCB* killtcb) {
	CS::instance()->enter(CS_TASK_CS,"", index);
	killtcb->prev->next = killtcb->next;
	killtcb->next->prev = killtcb->prev;
	killtcb->flag = 0;
	CS::instance()->leave(CS_TASK_CS,"", index);
}

void KTROBO::Task::change(TCB* thisTCB, void(*exec)(TCB*), void* data) {
	CS::instance()->enter(CS_TASK_CS,"",index);
	thisTCB->Exec = exec;
	thisTCB->data = data;
	CS::instance()->leave(CS_TASK_CS,"",index);
}
void KTROBO::Task::deleteTask() {
	is_exec_task = false;
	//%Sleep(10);
	WaitForSingleObject(hd,INFINITE);
	
}
