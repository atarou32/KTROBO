#include "KTRoboWeapon.h"

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

void WeaponFire::fire(Graphics* g, WeaponController* weapon_controller, AtariHantei* hantei, MySound* sound, Texture* tex, MYMATRIX* robo_world, MYMATRIX* view, MYVECTOR3* vec, MYVECTOR3* pos) {

	if (is_canfire) {
		is_canfire = false;
		dtime = 0;
	}

}

void WeaponFireRifle::fire(Graphics* g, WeaponController* weapon_controller, AtariHantei* hantei, MySound* sound, Texture* tex, MYMATRIX* robo_world, MYMATRIX* view, MYVECTOR3* vec, MYVECTOR3* pos) {

	
	if (is_canfire) {
		sound->playCue("se_maoudamashii_battle_gun01");

		WeaponFire::fire(g, weapon_controller, hantei, sound, tex,robo_world, view, vec,pos);
	}
}
