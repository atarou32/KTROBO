#ifndef KTROBOARMPOSITIONER_H
#define KTROBOARMPOSITIONER_H

#pragma once
#include "MyDefine.h"
using namespace MYDEFINE;

namespace KTROBO {



#define KTROBO_ARMPOSITION_OK 1
#define KTROBO_ARMPOSITION_DAME 2
#define KTROBO_ARMPOSITION_FINISH 3

class Robo;
class Graphics;
class MeshBone;
class ArmPositioner
{
	//ap = new armPositioner(3.14/60000,3.14/3,0.62);
	float theta; // �ڕW������A�[���̕t�����̃x�N�g���ƖڕW������A�[���̃x�N�g���̃i�X�p�x
	float theta2; // ��������ǂꂾ�����ɂ����ނ����č\���邩
	float nobabairitu; // �A�[���̕t���������܂ł̃x�N�g���ŃA�[���̕t��������ڕW���܂ł̃x�N�g���ƕ��s�Ȑ����̒�����a+b�ɑ΂���{��
	MYMATRIX migi_mae_arm1;
	MYMATRIX migi_mae_arm2;
	MYMATRIX hidari_mae_arm1;
	MYMATRIX hidari_mae_arm2;

	float dthetaxa;
	float dthetaxb;
	float dthetaya;
	float dthetayb;
	float dthetaza;
	float dthetazb;


public:
	ArmPositioner(float theta, float theta2, float nobabairitu) {
		this->theta = theta;
		this->theta2 = theta2;
		this->nobabairitu = nobabairitu;
		MyMatrixIdentity(migi_mae_arm1);
		MyMatrixIdentity(migi_mae_arm2);
		MyMatrixIdentity(hidari_mae_arm1);
		MyMatrixIdentity(hidari_mae_arm2);
		dthetaxa=0;
		dthetaxb=0;
		dthetaya=0;
		dthetayb=0;
		dthetaza=0;
		dthetazb=0;
	}

	~ArmPositioner(void);

	bool positionArm(Graphics* g , MYMATRIX* view, float framecnt, Robo* robo, MYVECTOR3* mokuhyou, bool is_migi);
	bool positionArm2(float epsiron, float e, Robo* robo, MYVECTOR3* mokuhyou, bool is_migi);


	int positionArm3(Graphics* g , MYMATRIX* view, Robo* robo, MYVECTOR3* moku, bool is_migi);
	bool positionArm33(Graphics* g, MYMATRIX* view, Robo* robo, MYVECTOR3* moku, bool is_migi);

	void setArm3(Robo* robo, bool is_migi, MeshBone* uparm1, MeshBone* downarm2);
	void resetTheta();
	void setTheta(float dthetaxa, float dthetaxb, float dthetaya, float dthetayb, float dthetaza, float dthetazb) {
		this->dthetaxa = dthetaxa;
		this->dthetaxb = dthetaxb;
		this->dthetaya = dthetaya;
		this->dthetayb = dthetayb;
		this->dthetaza = dthetaza;
		this->dthetazb = dthetazb;
	}

};
}
#endif

