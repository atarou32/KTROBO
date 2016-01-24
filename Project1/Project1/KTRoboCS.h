#ifndef KTROBOCS_H
#define KTROBOCS_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "stdlib.h"
#include "stdio.h"
#include "KTRoboTask.h"

// 複数ロックするときの順番
// MAIN TASK0 TASK1 TASK2 TASK3 LOAD DEVICECON LOG

namespace KTROBO {
#define CS_LOG_CS 2
#define CS_TASK_CS 3
#define CS_DEVICECON_CS 4
#define CS_MAINTHREAD_CS 5
#define CS_LOAD_CS 6

class CS
{
private:
	CRITICAL_SECTION LOG_CS;
	CRITICAL_SECTION TASK_CS[TASKTHREAD_NUM];
	CRITICAL_SECTION DEVICECON_CS;
	CRITICAL_SECTION MAINTHREAD_CS; // メインのスレッドが止まっているかどうかに使う
	CRITICAL_SECTION LOAD_CS;
	bool is_main_enter;
private:
	CS(void);

	bool is_log;
	char* getCSName(int index);

public:
	static CS* instance() {
		static CS a;
		return &a;
	}
	void setLogMode(bool is_log) {
		this->is_log = is_log;
	}

	void log(int index, char* p, int length);
	
	
	void Init() {
		InitializeCriticalSection(&LOG_CS);
		for (int i = 0 ; i <TASKTHREAD_NUM; i++) {
			InitializeCriticalSection(&TASK_CS[i]);
		}
		InitializeCriticalSection(&DEVICECON_CS);
		InitializeCriticalSection(&MAINTHREAD_CS);
		InitializeCriticalSection(&LOAD_CS);
	}

	void Del() {
		DeleteCriticalSection(&LOG_CS);
		for (int i=0;i<TASKTHREAD_NUM; i++) {
			DeleteCriticalSection(&TASK_CS[i]);
		}
		DeleteCriticalSection(&DEVICECON_CS);
		DeleteCriticalSection(&MAINTHREAD_CS);
		DeleteCriticalSection(&LOAD_CS);
	}

	~CS(void);
	void enter(int index, char* buf, int offset = 0) {
		if (index == CS_LOG_CS) {
			EnterCriticalSection(&LOG_CS);
			int length = strlen(buf);
			log(index, buf, length);
		} else if(index == CS_TASK_CS) {

			if (offset > 0 && offset < TASKTHREAD_NUM) {

				EnterCriticalSection(&TASK_CS[offset]);
			}
			int length = strlen(buf);
			log(index, buf, length);
		} else if(index == CS_DEVICECON_CS) {
			EnterCriticalSection(&DEVICECON_CS);
			// いらなくなったわけではないみたいだ
			int length = strlen(buf);
			log(index, buf, length);
		} else if(index == CS_MAINTHREAD_CS) {
			EnterCriticalSection(&MAINTHREAD_CS);
			is_main_enter = true;
			int length = strlen(buf);
			log(index, buf, length);
		} else if(index == CS_LOAD_CS) {
			EnterCriticalSection(&LOAD_CS);
			int length = strlen(buf);
			log(index, buf, length);
		}
	}

	void leave(int index, char* buf, int offset = 0) {
		if (index == CS_LOG_CS) {
			LeaveCriticalSection(&LOG_CS);
		} else if(index == CS_TASK_CS) {
			if (offset >0 && offset < TASKTHREAD_NUM) {
				LeaveCriticalSection(&TASK_CS[offset]);
			}
		} else if(index == CS_DEVICECON_CS) {
			LeaveCriticalSection(&DEVICECON_CS);
			// いらなくなったわけではないみたいだ
		} else if(index == CS_MAINTHREAD_CS) {
			if (is_main_enter) {
			is_main_enter = false;
			LeaveCriticalSection(&MAINTHREAD_CS);
			int length = strlen(buf);
			log(index, buf, length);
			}
		} else if(index == CS_LOAD_CS) {
			LeaveCriticalSection(&LOAD_CS);
			int length = strlen(buf);
			log(index, buf, length);
		}
	}
};

}

#endif
