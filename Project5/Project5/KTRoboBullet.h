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

	class Bullet : public AtariBase {
	private:
		bool is_use;
		bool is_fired;
		float fire_distance; // ���˂��Ă���ړ���������
		MYVECTOR3 h_pos; // ���˂����n�_
		MYVECTOR3 dpos; // ���˂����n�_����̑��Έʒu
		MYVECTOR3 h_v; // ���˂����Ƃ��̑��x
		MYMATRIX shoki_world;
		float dtime; // ���˂��Ă���̎���
	public:
		AtariBase* robo; // delete���Ȃ�
		RoboParts* robo_parts; //delete���Ȃ�
	public:
		MeshInstanced* mesh_i; // delete ���Ȃ��@
	private:
		AtariBase* atari_robo; // delete���Ȃ�
		AtariBase* atari_tikei; // delete���Ȃ�
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
		void Init(Graphics* g, AtariHantei* h, MeshInstanced* mi);
		void setParam(AtariBase* robo, RoboParts* parts, MYVECTOR3* hassyapos, MYVECTOR3* vdayo, MYMATRIX* shoki_world);
		bool fire(Game* game, AtariHantei* hantei);
		void atariShori(AtariHantei* hantei, MYMATRIX* view, float dsecond, int stamp);
		void byouga(Graphics* g, MYMATRIX* view, MYMATRIX* proj, float dsecond, int stamp);
		void update(Graphics* g, AtariHantei* hantei, float dsecond, int stamp);



	};

#define KTROBO_BULLET_CONTROLLER_BULLET_NUM 400 // mesh_instanceds�̊֌W��512�ȉ��ɂ��邱��
#define KTROBO_BULLET_MESH_DUMMY_FILENAME "resrc/model/cube/pkcube.mesh"
#define KTROBO_BULLET_MESH_ANIME_DUMMY_FILENAME "resrc/model/cube/pkcube.anime"

class BulletController {
private:
	AtariHantei* hantei;
	Bullet* bullets;
	map<int,int> umesh_id_to_bullet_indexs;
	MeshInstanceds* mis;
public:
	Mesh* dummy_mesh;
public:
	BulletController();
	~BulletController();
	void Init(Graphics* g, AtariHantei* hantei, MyTextureLoader* loader);
	Bullet* getEmptyBullet(); // ��̂��̂��Ȃ��ꍇ��NULL���Ԃ�
	void Release(); // AtariHantei ���N���A�����

	void atariShori(Game* game, AtariHantei* hantei, MYMATRIX* view, float dsecond, int stamp);
	void byouga(Graphics* g, MYMATRIX* view, MYMATRIX* proj, float dsecond, int stamp);
	void update(Graphics* g, AtariHantei* hantei, float dsecond, int stamp);

	void calcUpdate(Graphics* g); // mis�p


};


}

#endif