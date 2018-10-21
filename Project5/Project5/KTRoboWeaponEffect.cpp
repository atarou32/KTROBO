#include "KTRoboWeaponEffect.h"

using namespace KTROBO;

WeaponEffect::WeaponEffect(WeaponEffectManager* w)
{
	we_manager = w;
}


WeaponEffect::~WeaponEffect()
{
}

WeaponEffect* WeaponEffect::we_rifle = NULL;

WeaponEffect* WeaponEffect::we_laserrifle = NULL;

WeaponEffect* WeaponEffect::we_pulsegun = NULL;

WeaponEffect* WeaponEffect::we_bazooka = NULL;



void WeaponEffect::Init(WeaponEffectManager* w) {

}
void WeaponEffect::Del() {

}



void WE_Rifle::fireEffect(Game* game, Robo* robo, AtariBase* weaponfireparts, AtariBase* bullet, MYVECTOR3* vec) {
	

}

void WE_Rifle::butukariEffect(Game* game, Robo* robo, MYVECTOR3* pos, MYVECTOR3* vec) {

}

void WE_Rifle::butukariShori(Game* game, Robo* robo, Robo* aite, Bullet* bullet) {

}

void WE_Rifle::bulletMoveControl(Game* game, Robo* aite, Bullet* bullet) {


}

void WE_LaserRifle::fireEffect(Game* game, Robo* robo, AtariBase* weaponfireparts, AtariBase* bullet, MYVECTOR3* vec) {

}

void WE_LaserRifle::butukariEffect(Game* game, Robo* robo, MYVECTOR3* pos, MYVECTOR3* vec) {

}

void WE_LaserRifle::butukariShori(Game* game, Robo* robo, Robo* aite, Bullet* bullet) {

}

void WE_LaserRifle::bulletMoveControl(Game* game, Robo* aite, Bullet* bullet) {

}


void WE_Bazooka::fireEffect(Game* game, Robo* robo, AtariBase* weaponfireparts, AtariBase* bullet, MYVECTOR3* vec) {

}

void WE_Bazooka::butukariEffect(Game* game, Robo* robo, MYVECTOR3* pos, MYVECTOR3* vec) {


}

void WE_Bazooka::butukariShori(Game* game, Robo* robo, Robo* aite, Bullet* bullet) {

}

void WE_Bazooka::bulletMoveControl(Game* game, Robo* aite, Bullet* bullet) {

}


void WE_Pulsegun::fireEffect(Game* game, Robo* robo, AtariBase* weaponfireparts, AtariBase* bullet, MYVECTOR3* vec) {

}

void WE_Pulsegun::butukariEffect(Game* game, Robo* robo, MYVECTOR3* pos, MYVECTOR3* vec) {

}

void WE_Pulsegun::butukariShori(Game* game, Robo* robo, Robo* aite, Bullet* bullet) {

}

void WE_Pulsegun::bulletMoveControl(Game* game, Robo* aite, Bullet* bullet) {

}


int WE_Rifle::getBulletMeshIndexName() {

}

int WE_LaserRifle::getBulletMeshIndexName() {

}

int WE_Pulsegun::getBulletMeshIndexName() {

}

int WE_Bazooka::getBulletMeshIndexName() {

}
