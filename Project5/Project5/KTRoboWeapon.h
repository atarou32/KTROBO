#ifndef KTROBOWEAPON_H
#define KTROBOWEAPON_H

#pragma once
#include "KTRoboGraphics.h"
#include "KTRoboAtari.h"
#include "KTRoboTexture.h"
#include "MySound.h"

#define KTROBO_WEAPON_BULLET_UNIT_NUM 2048
#define KTROBO_WEAPON_BULLET_
namespace KTROBO {
class WeaponController {

public:
	WeaponController();
	~WeaponController();

	UMeshUnit* bullets[KTROBO_WEAPON_BULLET_UNIT_NUM];
	map<int,int> umeshid2bulletindex;

};
class WeaponFire
{
protected:
	float reloadtime;
	bool is_canfire;
	float dtime;

public:
	WeaponFire(void);
	~WeaponFire(void);
	virtual void fire(Graphics* g, WeaponController* weapon_controller, AtariHantei* hantei, MySound* sound, Texture* tex, MYMATRIX* robo_world, MYMATRIX* view, MYVECTOR3* vec, MYVECTOR3* pos);
	void update(float dsecond, int stamp);
};

class WeaponFireRifle : public WeaponFire {
public:
	WeaponFireRifle();
	~WeaponFireRifle();
	void fire(Graphics* g, WeaponController* weapon_controller, AtariHantei* hantei, MySound* sound, Texture* tex, MYMATRIX* robo_world, MYMATRIX* view, MYVECTOR3* vec, MYVECTOR3* pos);

};




}
#endif