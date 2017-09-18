#ifndef KTROBOWEAPON_H
#define KTROBOWEAPON_H

#pragma once
#include "KTRoboGraphics.h"
#include "KTRoboAtari.h"
#include "KTRoboTexture.h"
#include "MySound.h"

namespace KTROBO {

class Scene;
class Game;
class BulletController;
class Robo;
class RoboParts;
class WeaponFire
{
protected:
	float reloadtime;
	bool is_canfire;
	float dtime;

public:
	WeaponFire(void);
	~WeaponFire(void);
	virtual void fire(Robo* robo, RoboParts* parts,Graphics* g, Game* game, Scene* scene, BulletController* controller, AtariHantei* hantei, MySound* sound, MYMATRIX* robo_world, MYVECTOR3* vec, MYVECTOR3* pos);
	void update(float dsecond, int stamp);
};

class WeaponFireRifle : public WeaponFire {
public:
	WeaponFireRifle();
	~WeaponFireRifle();
	void fire(Robo* robo, RoboParts* parts, Graphics* g, Game* game, Scene* scene, BulletController* controller, AtariHantei* hantei, MySound* sound, MYMATRIX* robo_world, MYVECTOR3* vec, MYVECTOR3* pos);

};




}
#endif