#ifndef KTROBO_BULLET_H
#define KTROBO_BULLET_H

#pragma once
#ifndef KTROBO_ATARI_H
#include "KTRoboAtari.h"
#endif

#ifndef KTROBO_ROBO_H
#include "KTRoboRobo.h"
#endif

#ifndef KTROBO_MESH_INSTANCED_H
#include "KTRoboMeshInstanced.h"
#endif

namespace KTROBO {
#define BULLET_ATARI_JYUNBI_COUNT_MAX 2

class Bullet : public AtariBase {
	private:
		bool is_atari_jyunbi;
		int atari_jyunbi_count;
		bool is_use;
		bool is_fired;
		float fire_distance; // 発射してから移動した距離
		MYVECTOR3 h_pos; // 発射した地点
		MYVECTOR3 dpos; // 発射した地点からの相対位置
		MYVECTOR3 h_v; // 発射したときの速度
		MYMATRIX shoki_world;
		float dtime; // 発射してからの時間
	public:
		AtariBase* robo; // deleteしない
		RoboParts* robo_parts; //deleteしない
	public:
		MeshInstanced* mesh_i; // delete しない　
	private:
		AtariBase* atari_robo; // deleteしない
		AtariBase* atari_tikei; // deleteしない
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
			mesh_i = 0;
			MyMatrixIdentity(shoki_world);
			is_atari_jyunbi = false;
			atari_jyunbi_count = 0;
		}

		~Bullet();

		void setAtariJyunbi(bool t) {
			if (!t) {
				atari_jyunbi_count = 0;
				is_atari_jyunbi = false;
			} else {
				if (atari_jyunbi_count < BULLET_ATARI_JYUNBI_COUNT_MAX) {
					atari_jyunbi_count++;
				} else {
					is_atari_jyunbi = true;
				}
			}
		}

		bool getAtariJyunbi() {
			return is_atari_jyunbi;
		}



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
		void Init(Graphics* g, AtariHantei* h, MeshInstanced* mi);
		void setParam(AtariBase* robo, RoboParts* parts, MYVECTOR3* hassyapos, MYVECTOR3* vdayo, MYMATRIX* shoki_world);
		bool fire(Game* game, AtariHantei* hantei);
		void atariShori(AtariHantei* hantei, MYMATRIX* view, float dsecond, int stamp);
		void byouga(Graphics* g, MYMATRIX* view, MYMATRIX* proj, float dsecond, int stamp);
		void update(Graphics* g, AtariHantei* hantei, float dsecond, int stamp);



	};

#define KTROBO_BULLET_CONTROLLER_BULLET_NUM 400 // mesh_instancedsの関係上512以下にすること
#define KTROBO_BULLET_MESH_DUMMY_FILENAME "resrc/model/cube/pkcube.mesh"
#define KTROBO_BULLET_MESH_ANIME_DUMMY_FILENAME "resrc/model/cube/pkcube.anime"

#define KTROBO_BULLET_MESH_RIFLE_FILENAME "resrc/model/bullet/weaponbullet_rifle.mesh"
#define KTROBO_BULLET_MESH_ANIME_RIFLE_FILENAME "resrc/model/bullet/weaponbullet_rifle.anime"
#define KTROBO_BULLET_MESH_RIFLE_INDEXNAME "weapon_rifle"

#define KTROBO_BULLET_MESH_LASERRIFLE_FILENAME "resrc/model/bullet/weaponbullet_laserrifle.mesh"
#define KTROBO_BULLET_MESH_ANIME_LASERRIFLE_FILENAME "resrc/model/bullet/weaponbullet_laserrifle.anime"
#define KTROBO_BULLET_MESH_LASERRIFLE_INDEXNAME "weapon_laserrifle"






class BulletController {
private:
	AtariHantei* hantei;
	Bullet* bullets;
	map<int,int> umesh_id_to_bullet_indexs;
	MeshInstanceds* mis;
public:
	Mesh* dummy_mesh;
	vector<Mesh*> bullet_meshs;
	map<string, int> bullet_mesh_index;

public:
	BulletController();
	~BulletController();
		MeshInstanceds* getMeshInstanceds() {
		return mis;
	}

	void Init(Graphics* g, AtariHantei* hantei, MyTextureLoader* loader);
	Bullet* getEmptyBullet(); // 空のものがない場合はNULLが返る
	void Release(); // AtariHantei がクリアされる

	void atariShori(Game* game, AtariHantei* hantei, MYMATRIX* view, float dsecond, int stamp);
	void byouga(Graphics* g, MYMATRIX* view, MYMATRIX* proj, float dsecond, int stamp);
	void update(Graphics* g, AtariHantei* hantei, float dsecond, int stamp);

	void calcUpdate(Graphics* g); // mis用


};


}

#endif