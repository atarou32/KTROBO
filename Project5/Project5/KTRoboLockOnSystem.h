#ifndef KTROBOLOCKONSYSTEM_H
#define KTROBOLOCKONSYSTEM_H

#pragma once
#include "KTRoboGraphics.h"

namespace KTROBO {

class LockOnSystem
{
public:
	LockOnSystem(void);
	~LockOnSystem(void);
	int getStudyPointNum(float rmin, float rmax, float mintate, float maxtate, 
		float minyoko, float maxyoko,float dtate, float dyoko, float dr);
	MYVECTOR3 getPosOfStudyPoint(int index, float rmin, float rmax, float mintate, float maxtate, 
		float minyoko, float maxyoko,float dtate, float dyoko, float dr);
	void byougaStudyPoint(Graphics* g, MYMATRIX* world,MYMATRIX* view, 
		float rmin, float rmax, float mintate, float maxtate, 
		float minyoko, float maxyoko,float dtate, float dyoko, float dr);
	void byougaBigStudyPoint(int index, Graphics* g, MYMATRIX* world,MYMATRIX* view, 
		float rmin, float rmax, float mintate, float maxtate, 
		float minyoko, float maxyoko,float dtate, float dyoko, float dr);
};

}

#endif