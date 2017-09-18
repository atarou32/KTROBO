#include "KTRoboWeapon.h"
#include "KTRoboBullet.h"

using namespace KTROBO;

WeaponFire::WeaponFire(void)
{
	this->is_canfire = true;
	this->reloadtime = 3300;
	this->dtime = -1;
	
}


WeaponFire::~WeaponFire(void)
{

}


WeaponFireRifle::WeaponFireRifle(void)
{

}


WeaponFireRifle::~WeaponFireRifle(void)
{

}

void WeaponFire::update(float dsecond, int stamp) {
	if (is_canfire) {
	} else {
		dtime += dsecond;
		if (dtime > reloadtime) {
			dtime = -1;
			is_canfire = true;
		}
	}
}

void WeaponFire::fire(Robo* robo, RoboParts* parts, Graphics* g, Game* game, Scene* scene, BulletController* controller, AtariHantei* hantei, MySound* sound, MYMATRIX* robo_world, MYVECTOR3* vec, MYVECTOR3* pos) {

	if (is_canfire) {
		is_canfire = false;
		dtime = 0;
	}

}

void WeaponFireRifle::fire(Robo* robo, RoboParts* parts, Graphics* g, Game* game, Scene* scene, BulletController* controller, AtariHantei* hantei, MySound* sound, MYMATRIX* robo_world, MYVECTOR3* vec, MYVECTOR3* pos) {

	
	if (is_canfire) {
		Bullet* c = controller->getEmptyBullet();

		if (c) {
		} else {
			return;
		}

		c->setParam(robo,parts,pos,vec,robo_world);

		sound->playCue("se_maoudamashii_battle_gun01");
		
		WeaponFire::fire(robo, parts, g, game,scene, controller, hantei, sound, robo_world, vec,pos);
		c->fire(hantei);
	}
}
