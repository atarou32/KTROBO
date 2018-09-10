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

	int getIndexOfPoint8(int pointindex, MYVECTOR3* pos, float rmin, float rmax, float mintate, float maxtate, 
		float minyoko, float maxyoko,float dtate, float dyoko, float dr);

	MYVECTOR3 getPosOfStudyPoint(int index, float rmin, float rmax, float mintate, float maxtate, 
		float minyoko, float maxyoko,float dtate, float dyoko, float dr);
	void byougaStudyPoint(Graphics* g, MYMATRIX* world,MYMATRIX* view, 
		float rmin, float rmax, float mintate, float maxtate, 
		float minyoko, float maxyoko,float dtate, float dyoko, float dr);
	void byougaBigStudyPoint(int index, Graphics* g, MYMATRIX* world,MYMATRIX* view, 
		float rmin, float rmax, float mintate, float maxtate, 
		float minyoko, float maxyoko,float dtate, float dyoko, float dr);
	void byougaRAY( Graphics* g, MYMATRIX* world,MYMATRIX* view, 
		float rmin, float rmax, float mintate, float maxtate, 
		float minyoko, float maxyoko,float dtate, float dyoko, float dr);

	bool isIn(MYVECTOR3* pos, float rmin, float rmax, float mintate, float maxtate, 
		float minyoko, float maxyoko,float dtate, float dyoko, float dr);


};
class Robo;
class AtariBase;
class TargetControlSystemPart {
private:
	bool has_target;
	bool has_one_lock;
	bool has_two_lock;
	int lock_count;
	Robo* fire_person;
	AtariBase* target;
	float tyusinrad;
	float tyusind;

public:
	TargetControlSystemPart() {
		has_target = false;
		has_one_lock = false;
		has_two_lock = false;
		lock_count = 0;
		fire_person = 0;
		target = 0;
		tyusinrad = 1000000;
		tyusind = 1000000;
	}
	void setFirePerson(Robo* ro) {
		fire_person = ro;
	}
	void getPosOfFire(MYVECTOR3* ans, int tcs_weapon);
	void getVecOfFire(MYVECTOR3* ans, int tcs_weapon);
	void setTargetNo();
	void setTarget(AtariBase* tget);
	void updateLockCount(float dsecond, int stamp);
	void calcTyusinRadAndTyusinD();
	float getTyusinRad();
	float getTyusinD();
};

#define KTROBO_TCS_PART_NUM 5
#define KTROBO_TCS_WEAPON_UNKNOWN 0
#define KTROBO_TCS_WEAPON_RARM 1
#define KTROBO_TCS_WEAPON_LARM 2
#define KTROBO_TCS_WEAPON_RSHOULDER 3
#define KTROBO_TCS_WEAPON_LSHOULDER 4
#define KTROBO_TCS_WEAPON_INSIDE 5

class TargetControlSystem {
private:
	Robo * robo;
	TargetControlSystemPart* nearest_tcs;
	TargetControlSystemPart tcss[KTROBO_TCS_PART_NUM];
	void calcNearestTCS();
	int secondstamp;
	int maesecondstamp;
public:
	void setTarget(AtariBase** bases, int num_bases); // bases ‚ÍŒvŽZ‚³‚ê‚Ä“n‚³‚ê‚Ä‚­‚é
	void updateCount(float dsecond, int stamp);
	bool getPosOfFire(MYVECTOR3* pos, int tcs_weapon);
	bool getVecOfFire(MYVECTOR3* vec, int tcs_weapon);
public:
	TargetControlSystem(Robo* robo) {
		this->robo = robo;
		for (int i = 0; i < KTROBO_TCS_PART_NUM; i++) {
			tcss[i].setFirePerson(robo);
		}
		secondstamp = 0;
		maesecondstamp = 0;
	}
};

}

#endif