#ifndef KTROBO_EFFECT_SUUJI_H
#define KTROBO_EFFECT_SUUJI_H

#pragma once
#ifndef KTROBO_EFFECT_H
#include "KTRoboEffect.h"
#endif





namespace KTROBO {

#define KTROBO_EFFECT_NAME_SUUJI_ZERO "s_zero"
#define KTROBO_EFFECT_NAME_SUUJI_ONE "s_one"
#define KTROBO_EFFECT_NAME_SUUJI_TWO "s_two"
#define KTROBO_EFFECT_NAME_SUUJI_THREE "s_three"
#define KTROBO_EFFECT_NAME_SUUJI_FOUR "s_four"
#define KTROBO_EFFECT_NAME_SUUJI_FIVE "s_five"
#define KTROBO_EFFECT_NAME_SUUJI_SIX "s_six"
#define KTROBO_EFFECT_NAME_SUUJI_SEVEN "s_seven"
#define KTROBO_EFFECT_NAME_SUUJI_EIGHT "s_eight"
#define KTROBO_EFFECT_NAME_SUUJI_NINE "s_nine"

#define KTROBO_EFFECT_SUUJI_NUM_ZERO 64
#define KTROBO_EFFECT_SUUJI_NUM 32


class EffectSuujiPart {
public:
	int effect_impl_id;
	MYVECTOR3 pos; // 該当するビルボードのposではなくまとまりのpos
	float offset;
	int jyunban;
	int keta;
	EffectSuujiPart() {
		effect_impl_id = 0;
		pos = MYVECTOR3(0,0,0);
		offset = 0;
		keta = 0;
		jyunban = 0;
	}
};

class EffectSuuji
{
private:
	EffectManager* manager;
	EffectSuujiPart zeros[KTROBO_EFFECT_SUUJI_NUM_ZERO];
	EffectSuujiPart ones[KTROBO_EFFECT_SUUJI_NUM];
	EffectSuujiPart twos[KTROBO_EFFECT_SUUJI_NUM];
	EffectSuujiPart threes[KTROBO_EFFECT_SUUJI_NUM];
	EffectSuujiPart fours[KTROBO_EFFECT_SUUJI_NUM];
	EffectSuujiPart fives[KTROBO_EFFECT_SUUJI_NUM];
	EffectSuujiPart sixs[KTROBO_EFFECT_SUUJI_NUM];
	EffectSuujiPart sevens[KTROBO_EFFECT_SUUJI_NUM];
	EffectSuujiPart eights[KTROBO_EFFECT_SUUJI_NUM];
	EffectSuujiPart nines[KTROBO_EFFECT_SUUJI_NUM];
	int zeros_index;
	int ones_index;
	int twos_index;
	int threes_index;
	int fours_index;
	int fives_index;
	int sixs_index;
	int sevens_index;
	int eights_index;
	int nines_index;
	EffectSuujiPart* getEffectSuuji(int suuji_part, MYVECTOR3* pos);
	void setEffect(int suuji_part, MYVECTOR3* pos, MYVECTOR3* moto_pos, float offset, int keta, int jyunban); // suuji_part 0~9


public:
	EffectSuuji(int task_index, EffectManager* manager);
	~EffectSuuji(void);
	void updatePart(EffectSuujiPart* part, MYMATRIX* world);

	void update(MYVECTOR3* lookfromtoat);
	void render(int suuji, MYVECTOR3* lookfromtoat, MYVECTOR3* pos);
};



}
#endif