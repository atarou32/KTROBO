#ifndef KTROBOEVENTITEM_H
#define KTROBOEVENTITEM_H

/*
	イベントアイテムの概要
	個数の概念がなく、持っているか持っていないかのどちらか
	プレイヤーはイベントアイテムにアクセスする方法を持たない
	イベントアイテムの種類
	１．クエスト許可証
	２．ミッション許可証
	３．特定のフラグをオンにするアイテム
	４．アクセス証（システムとしてアクセスしているかユーザとしてアクセスしているか）
*/
#pragma once
class KTROBOEventItem
{
public:
	KTROBOEventItem(void);
	~KTROBOEventItem(void);
};

#endif