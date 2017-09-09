#ifndef KTROBO_BULLET_H
#define KTROBO_BULLET_H

#pragma once
#ifndef KTROBO_ATARI_H
#include "KTRoboAtari.h"
#endif

#ifndef KTROBO_ROBO_H
#include "KTRoboRobo.h"
#endif

namespace KTROBO {

	class Bullet : public AtariBase {
	private:
		bool is_use;
	public:
		Bullet() {
			atarihan = 0;
			is_use = false;
		}

		~Bullet();

		void setIsUse(bool t) {
			is_use = t;
		}
		bool getIsUse() {return is_use;}
		void Init(Graphics* g, AtariHantei* h);
		void setParam(Robo* robo, RoboParts* parts, MYVECTOR3* hassyapos, MYVECTOR3* vdayo);
		bool fire();
		void atariShori(AtariHantei* hantei, MYMATRIX* view, float dsecond, int stamp);
		void byouga(Graphics* g, MYMATRIX* view, MYMATRIX* proj, float dsecond, int stamp);
		void update(Graphics* g, AtariHantei* hantei, float dsecond, int stamp);



	};

#define KTROBO_BULLET_CONTROLLER_BULLET_NUM 400

class BulletController {
private:
	AtariHantei* hantei;
	Bullet* bullets;
public:
	BulletController();
	~BulletController();
	void Init(Graphics* g, AtariHantei* hantei);
	Bullet* getEmptyBullet();
	void Release(); // AtariHantei ‚ªƒNƒŠƒA‚³‚ê‚é

	void atariShori(AtariHantei* hantei, MYMATRIX* view, float dsecond, int stamp);
	void byouga(Graphics* g, MYMATRIX* view, MYMATRIX* proj, float dsecond, int stamp);
	void update(Graphics* g, AtariHantei* hantei, float dsecond, int stamp);




};


}

#endif