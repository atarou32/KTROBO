#ifndef KTROBOLUACOLLECTION_H
#define KTROBOLUACOLLECTION_H

#pragma once
#include "KTRoboCS.h"
#include "KTRoboGameError.h"
#include "KTRoboTask.h"
#include "lua.hpp"

namespace KTROBO {

class LuaTCBStruct {
public:
	char lua_filename[256];
	bool is_lock_sita;
	int task_index;
	bool is_use;
	bool is_send;
};

#define KTROBO_LUAEXEC_STRUCT_SIZE 32
class LuaTCBMaker {
private:
	static LuaTCBStruct structs[KTROBO_LUAEXEC_STRUCT_SIZE];	
	static lua_State* ls[TASKTHREAD_NUM];
	static Task* ts[TASKTHREAD_NUM];
public:
	static void Init(Task** t, lua_State** l);
	static void doTCBnow(int task_index, bool is_lock_sita, char* lua_filename);
	static void makeTCB(int task_index, bool is_lock_sita, char* lua_filename);
	static void makeTCBExec();
	
};



// ロード処理を持つクラスで他のスレッドとの競合が発生しそうなクラスに使う
// そのクラス内のコレクトされるクラスのコンストラクタまたはデストラクタでexecConstructOrDestruct() を呼ぶようにする
// is_loadをfalseにするのは生成処理破棄処理スレッドのみ
// is_loadをtrueにするのは他のスレッドのどれかひとつのみ
// is_load を頻繁に変えられることは考えられていない

class Loadable
{
protected:
	volatile bool is_load; // collectされるクラスを生成中かどうか

public:
//	virtual int getInstance(int index)=0;
//	virtual int getInterface(int index)=0;
//  construct
//  destruct

	Loadable() {
		is_load = true;
	}

	void setIsLoad(bool t) {
		
			CS::instance()->enter(CS_LOAD_CS, "set isload");
			is_load = t;
			CS::instance()->leave(CS_LOAD_CS, "set isload");
	}

	void execConstructOrDestruct() { 
		// このメソッドはis_loadをfalseにするのが生成破棄処理スレッドだけしかしないことを前提としている
		// しかし、現在のところ他のスレッドでもis_loadをfalseにできるこれには注意しなければならない
		CS::instance()->enter(CS_LOAD_CS, "set isload");
		if (is_load) {
			// 大丈夫
		} else {

			
			if (is_load) {
				// 大丈夫
			} else {
				// 例外を投げる
				CS::instance()->leave(CS_LOAD_CS, "set isload");
				throw new GameError(KTROBO::WARNING, "exception in execConstructOrDestruct");
			}
			
		}
		CS::instance()->leave(CS_LOAD_CS, "set isload");
	}


	bool getIsLoad() {

		CS::instance()->enter(CS_LOAD_CS, "get isload");
		bool t;
		t = is_load;
		CS::instance()->leave(CS_LOAD_CS, "get isload");
		return t;
	}
};


}

#endif