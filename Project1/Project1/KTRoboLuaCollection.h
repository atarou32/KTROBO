#ifndef KTROBOLUACOLLECTION_H
#define KTROBOLUACOLLECTION_H

#pragma once
#include "KTRoboCS.h"
#include "KTRoboGameError.h"

namespace KTROBO {
// ロード処理を持つクラスで他のスレッドとの競合が発生しそうなクラスに使う
// そのクラス内のコレクトされるクラスのコンストラクタまたはデストラクタでexecConstructOrDestruct() を呼ぶようにする
// is_loadをfalseにするのは生成処理破棄処理スレッドのみ
// is_loadをtrueにするのは他のスレッドということにしよう

class Loadable
{
protected:
	bool is_load; // collectされるクラスを生成中かどうか

public:
//	virtual int getInstance(int index)=0;
//	virtual int getInterface(int index)=0;
//  construct
//  destruct

	Loadable() {
		is_load = true;
	}

	void setIsLoad(bool t) {
		if (t != is_load) {
			CS::instance()->enter(CS_LOAD_CS, "set isload");
			is_load = t;
			CS::instance()->leave(CS_LOAD_CS, "set isload");
		}
	}

	void execConstructOrDestruct() {

		if (is_load) {
			// 大丈夫
		} else {

			CS::instance()->enter(CS_LOAD_CS, "set isload");
			if (is_load) {
				// 大丈夫
			} else {
				// 例外を投げる
				CS::instance()->leave(CS_LOAD_CS, "set isload");
				throw new GameError(KTROBO::WARNING, "exception in execConstructOrDestruct");
			}
			CS::instance()->leave(CS_LOAD_CS, "set isload");
		}
	}


	bool getIsLoad() {
		return is_load;
	}
};


}

#endif