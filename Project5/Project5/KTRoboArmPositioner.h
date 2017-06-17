#ifndef KTROBOARMPOSITIONER_H
#define KTROBOARMPOSITIONER_H

#pragma once
#include "MyDefine.h"
using namespace MYDEFINE;

namespace KTROBO {
class Robo;
class Graphics;
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
public:
	ArmPositioner(float theta, float theta2, float nobabairitu) {
		this->theta = theta;
		this->theta2 = theta2;
		this->nobabairitu = nobabairitu;
		MyMatrixIdentity(migi_mae_arm1);
		MyMatrixIdentity(migi_mae_arm2);
		MyMatrixIdentity(hidari_mae_arm1);
		MyMatrixIdentity(hidari_mae_arm2);
	}

	~ArmPositioner(void);

	bool positionArm(Graphics* g , MYMATRIX* view, float framecnt, Robo* robo, MYVECTOR3* mokuhyou, bool is_migi);
	bool positionArm2(float epsiron, float e, Robo* robo, MYVECTOR3* mokuhyou, bool is_migi);




};
}
#endif

