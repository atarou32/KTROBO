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
class EffectSuuji
{
private:
	EffectManager* manager;
	int zeros[KTROBO_EFFECT_SUUJI_NUM_ZERO];
	int ones[KTROBO_EFFECT_SUUJI_NUM];
	int twos[KTROBO_EFFECT_SUUJI_NUM];
	int threes[KTROBO_EFFECT_SUUJI_NUM];
	int fours[KTROBO_EFFECT_SUUJI_NUM];
	int fives[KTROBO_EFFECT_SUUJI_NUM];
	int sixs[KTROBO_EFFECT_SUUJI_NUM];
	int sevens[KTROBO_EFFECT_SUUJI_NUM];
	int eights[KTROBO_EFFECT_SUUJI_NUM];
	int nines[KTROBO_EFFECT_SUUJI_NUM];
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

	void setEffect(int suuji_part, MYVECTOR3* pos); // suuji_part 0~9


public:
	EffectSuuji(int task_index, EffectManager* manager);
	~EffectSuuji(void);

	void render(int suuji, MYVECTOR3* lookfromtoat, MYVECTOR3* pos);
};



}
#endif