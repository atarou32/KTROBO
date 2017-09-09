#ifndef KTROBOMISSION_H
#define KTROBOMISSION_H

/*
	ミッションの概要

	ミッションを選択し、ミッションを開始すると
	１．機体を選ぶ
	２．ミッションブリーフィング
	３．ミッション開始
	４．ミッション終了（達成、失敗）
	５．ミッション報酬の取得

	ミッションの種類
	１．通常ミッション
	２．ストーリーミッション
	３．緊急ミッション
	
	ミッションの達成条件
	１．特定の敵の撃破
	２．特定ポイントの取得
	３．特定時間の生存

	ミッションの属性
	
	ミッションID
	ミッション種類タイプ
	ミッション名
	ミッションコードネーム
	ミッション説明
	ミッション達成条件
	ミッション地形ID

	


*/

namespace KTROBO {
#pragma once
class Mission
{
public:
	Mission(void);
	~Mission(void);

	void start();
	void end();
	void success();
	void failure();
	void gift();
	int get_now_state();

};

class MissionBreefing {
public:
	void waiting();
	void ready(); 
	void startMission();
	void breakMission();
	void roboChange();
	int get_now_state();
};

}
#endif