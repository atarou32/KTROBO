#ifndef KTROBOEFFECT_H
#define KTROBOEFFECT_H

#pragma once
#include "KTRoboTexture.h"
#include "MyButukari.h"

namespace KTROBO {

// billboard
class Effect
{
private:
	MYRECT all_anim_rect;
	int rect_size_width;
	int rect_size_height;
	MYRECT now_anim_rect;
	int count_max;
	int now_count;
	int next_anim_per_count;

	int texdayo;
	MYVECTOR3 pos;
	float tex_width;
	float tex_height;

public:
	Effect(void);
	~Effect(void);
};


}
#endif