#include "KTRoboBullet.h"


using namespace KTROBO;
Bullet::~Bullet() {
	if (atarihan) {
		delete atarihan;
		atarihan = 0;
	}
}

void Bullet::Init(Graphics* g, AtariHantei* h) {

	if (!atarihan) {
		atarihan = new UMeshUnit();
		atarihan->setIsEnabled(h, false);
		h->setUMeshUnit(atarihan,AtariUnit::AtariType::ATARI_WAZA);
	}
}

void Bullet::setParam(Robo* robo, RoboParts* parts, MYVECTOR3* hassyapos, MYVECTOR3* vdayo) {



}


bool Bullet::fire() {



}


void Bullet::atariShori(AtariHantei* hantei, MYMATRIX* view, float dsecond, int stamp) {



}


void Bullet::byouga(Graphics* g, MYMATRIX* view, MYMATRIX* proj, float dsecond, int stamp) {



}


void Bullet::update(Graphics* g, AtariHantei* hantei, float dsecond, int stamp) {




}




void BulletController::atariShori(AtariHantei* hantei, MYMATRIX* view, float dsecond, int stamp) {



}

void BulletController::byouga(Graphics* g, MYMATRIX* view, MYMATRIX* proj, float dsecond, int stamp) {



}


void BulletController::update(Graphics* g, AtariHantei* hantei, float dsecond, int stamp) {


}


BulletController::BulletController() {
	this->hantei = 0;
	this->bullets = 0;
}

BulletController::~BulletController() {
}
void BulletController::Init(Graphics* g, AtariHantei* hantei) {

	this->hantei = hantei;
	if (hantei) {
		bullets = new Bullet[KTROBO_BULLET_CONTROLLER_BULLET_NUM];
		for (int i=0;i<KTROBO_BULLET_CONTROLLER_BULLET_NUM; i++) {
			bullets[i].Init(g,hantei);
		}
	}
}

Bullet* BulletController::getEmptyBullet() {
	if (!bullets) return NULL;

	for (int i=0;i<KTROBO_BULLET_CONTROLLER_BULLET_NUM;i++) {
		if (!bullets[i].getIsUse()) {
			return &bullets[i];
		}
	}
	return NULL;

}


void BulletController::Release() {
	if (hantei) {
		hantei->clearUMeshUnits();
	}
	if (bullets) {
		delete[] bullets;
		bullets = 0;
	}
}