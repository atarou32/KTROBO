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
		bool is_fired;
		float fire_distance; // 発射してから移動した距離
		MYVECTOR3 h_pos; // 発射した地点
		MYVECTOR3 h_v; // 発射したときの速度
		float dtime; // 発射してからの時間
		AtariBase* robo;
		RoboParts* robo_parts;

		AtariBase* atari_robo;
		AtariBase* atari_tikei;
	public:
		Bullet() {
			atarihan = 0;
			is_use = false;
			is_fired = false;
			fire_distance = 0;
			h_pos = MYVECTOR3(0,0,0);
			h_v = MYVECTOR3(0,0,0);
			dtime = 0;
			robo = 0;
			robo_parts = 0;
			atari_robo = 0;
			atari_tikei = 0;
		}

		~Bullet();

		void setIsUse(bool t) {
			is_use = t;
			if (!t) {
				is_fired = false;
			}
		}
		void setAtariRobo(AtariBase* a) {
			atari_robo = a;
		}
		void setAtariTikei(AtariBase* a) {
			atari_tikei = a;
		}

		bool getIsUse() {return is_use;}
		void Init(Graphics* g, AtariHantei* h);
		void setParam(AtariBase* robo, RoboParts* parts, MYVECTOR3* hassyapos, MYVECTOR3* vdayo);
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
	map<int,int> umesh_id_to_bullet_indexs;

public:
	BulletController();
	~BulletController();
	void Init(Graphics* g, AtariHantei* hantei);
	Bullet* getEmptyBullet();
	void Release(); // AtariHantei がクリアされる

	void atariShori(AtariHantei* hantei, MYMATRIX* view, float dsecond, int stamp);
	void byouga(Graphics* g, MYMATRIX* view, MYMATRIX* proj, float dsecond, int stamp);
	void update(Graphics* g, AtariHantei* hantei, float dsecond, int stamp);




};


}

#endif