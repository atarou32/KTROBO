#ifndef KTROBOARMPOSITIONER_H
#define KTROBOARMPOSITIONER_H

#pragma once
#include "MyDefine.h"
#include "KTRoboGUI.h"
#include <string>

using namespace MYDEFINE;

namespace KTROBO {



#define KTROBO_ARMPOSITION_OK 1
#define KTROBO_ARMPOSITION_DAME 2
#define KTROBO_ARMPOSITION_FINISH 3

class Robo;
class Graphics;
class MeshBone;

class ArmPositioner;

struct ArmPoint {
	MYVECTOR3 pos;
	float dthetaxa;
	float dthetaxb;
	float dthetaya;
	float dthetayb;
	float dthetaza;
	float dthetazb;
	bool is_ok;

	ArmPoint operator = (ArmPoint& t) {
		
		this->dthetaxa = t.dthetaxa;
		this->dthetaxb = t.dthetaxb;
		this->dthetaya = t.dthetaya;
		this->dthetayb = t.dthetayb;
		this->dthetaza = t.dthetaza;
		this->dthetazb = t.dthetazb;
		this->pos = t.pos;
		this->is_ok = t.is_ok;
		return *this;

		//return MYXMFLOAT3(t.float3.x, t.float3.y, t.float3.z);
	}
};

#define KTROBO_ARMPOINT8_MHU 0
#define KTROBO_ARMPOINT8_MHS 1
#define KTROBO_ARMPOINT8_MMU 2
#define KTROBO_ARMPOINT8_MMS 3
#define KTROBO_ARMPOINT8_UHU 4
#define KTROBO_ARMPOINT8_UHS 5
#define KTROBO_ARMPOINT8_UMU 6
#define KTROBO_ARMPOINT8_UMS 7

class ArmPoint8Positioner {
public:
	ArmPoint points[8];
public:
	ArmPoint8Positioner(){
		for (int i=0;i<8;i++) {
			points[i].pos = MYVECTOR3(0,0,0);
			points[i].dthetaxa = 0;
			points[i].dthetaxb = 0;
			points[i].dthetaya = 0;
			points[i].dthetayb = 0;
			points[i].dthetaza = 0;
			points[i].dthetazb = 0;
		}

		points[0].pos = MYVECTOR3(-6,-10,-6);
		points[0].dthetaxa = 0.064;
		points[0].dthetaxb = -1.041;
		points[0].dthetaya = 0;
		points[0].dthetayb = 0;
		points[0].dthetaza = -0.06;
		points[0].dthetazb = -0.44;

		points[1].pos = MYVECTOR3(6,-10,-6);
		points[1].dthetaxa = -0.151;
		points[1].dthetaxb = -1.032;
		points[1].dthetaya = 0;
		points[1].dthetayb = 0;
		points[1].dthetaza = -0.0123;
		points[1].dthetazb = 0.394;

		points[2].pos = MYVECTOR3(-6,-10,6);
		points[2].dthetaxa = 2.9917;
		points[2].dthetaxb = 0.8070;
		points[2].dthetaya = 0;
		points[2].dthetayb = 0;
		points[2].dthetaza = 0.056;
		points[2].dthetazb = -0.12;

		points[3].pos = MYVECTOR3(6,-10,6);
		points[3].dthetaxa = 0.8106;
		points[3].dthetaxb = -1.147;
		points[3].dthetaya = 0.026;
		points[3].dthetayb = 0;
		points[3].dthetaza = 0.0444;
		points[3].dthetazb = 0.832;

		points[4].pos = MYVECTOR3(-10,-100,-10);
		points[4].dthetaxa = 0.4984;
		points[4].dthetaxb = -0.9650;
		points[4].dthetaya = 0;
		points[4].dthetayb = 0;
		points[4].dthetaza = -0.0916;
		points[4].dthetazb = -0.0301;

		points[5].pos = MYVECTOR3(10,-100,-10);
		points[5].dthetaxa = 0.040;
		points[5].dthetaxb = -1.366;
		points[5].dthetaya = 0;
		points[5].dthetayb = 0;
		points[5].dthetaza = 0.0395;
		points[5].dthetazb = 0.2182;

		points[6].pos = MYVECTOR3(-10,-100,10);
		points[6].dthetaxa = 1.3270;
		points[6].dthetaxb = -0.3329;
		points[6].dthetaya = 0.0004;
		points[6].dthetayb = 0;
		points[6].dthetaza = 0.02805;
		points[6].dthetazb = 0.1533;
	
		points[7].pos = MYVECTOR3(10,-100,10);
		points[7].dthetaxa = 1.194807;
		points[7].dthetaxb = -0.369107;
		points[7].dthetaya = 0.0264571;
		points[7].dthetayb = 0;
		points[7].dthetaza = 0.180440;
		points[7].dthetazb = 0.360992;

	
	};
	~ArmPoint8Positioner(){};

	void setPoint(int index, ArmPoint* ap);
	ArmPoint getPoint(MYVECTOR3* moku);
	bool isInPoint(MYVECTOR3* moku);
};


class ShudouArmPositioner
{
	Robo* robo;
	ArmPositioner* ap;
	GUI* gui;
	int screen_window_id;
	float dthetaxa;
	float dthetaxb;
	float dthetaya;
	float dthetayb;
	float dthetaza;
	float dthetazb;
	int slider_dxa;
	int slider_dxb;
	int slider_dya;
	int slider_dyb;
	int slider_dza;
	int slider_dzb;
	int toggle_button;
public:
	ShudouArmPositioner(Robo* robo,ArmPositioner* ap){
	this->ap = ap;
	this->robo = robo;
	gui = 0;
	dthetaxa=0;
	dthetaxb=0;
	dthetaya=0;
	dthetayb=0;
	dthetaza=0;
	dthetazb=0;
	slider_dxa=0;
	slider_dxb=0;
	slider_dya=0;
	slider_dyb=0;
	slider_dza=0;
	slider_dzb=0;
	toggle_button = 0;
	};
	~ShudouArmPositioner(){};

	void Init(HWND hw, Texture* tex, lua_State* l, int screen_width, int screen_height);
	void Del();
	bool update();

};
class ArmPositioner
{
	//ap = new armPositioner(3.14/60000,3.14/3,0.62);
	float theta; // 目標物からアームの付け根のベクトルと目標物からアームのベクトルのナス角度
	float theta2; // ｚ軸からどれだけ下にかたむかせて構えるか
	float nobabairitu; // アームの付け根から先までのベクトルでアームの付け根から目標物までのベクトルと平行な成分の長さのa+bに対する倍率
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

	bool reseted;
	bool is_set;
	MYVECTOR3 dmoku;
public:
	MYVECTOR3 mokudayo;
	void setDMOKU(MYVECTOR3* ve) {
		dmoku = *ve;
	}
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
		reseted = false;
		is_set = false;
		dmoku = MYVECTOR3(0,0,0);
	}

	~ArmPositioner(void);

	bool positionArm(Graphics* g , MYMATRIX* view, float framecnt, Robo* robo, MYVECTOR3* mokuhyou, bool is_migi);
	bool positionArm2(float epsiron, float e, Robo* robo, MYVECTOR3* mokuhyou, bool is_migi);
	bool getIsSet() {return is_set;}
	bool getReseted() {return reseted;}
	MYVECTOR3 getDMOKU() {return dmoku;}
	void setIsSet(bool t) {is_set = t;}
	int positionArm3(Graphics* g , MYMATRIX* view, Robo* robo, MYVECTOR3* moku, bool is_migi);
	int new_positionArm3(Graphics* g, MYMATRIX* view, Robo* robo, MYVECTOR3* moku, bool is_migi);
	bool positionArm33(Graphics* g, MYMATRIX* view, Robo* robo, MYVECTOR3* moku, bool is_migi);
	int positionArm34(Graphics* g , MYMATRIX* view, Robo* robo, MYVECTOR3* moku, bool is_migi);
	
	void hanneiSetTheta(Robo* robo, bool is_migi);

	void setArm3(Robo* robo, bool is_migi, MeshBone* uparm1, MeshBone* downarm2);
	void getTheta(ArmPoint* app) {
		app->dthetaxa = dthetaxa;
		app->dthetaxb = dthetaxb;
		app->dthetaya = dthetaya;
		app->dthetayb = dthetayb;
		app->dthetaza = dthetaza;
		app->dthetazb = dthetazb;
	}

	void resetTheta();
	void setTheta(float dthetaxa, float dthetaxb, float dthetaya, float dthetayb, float dthetaza, float dthetazb) {
		this->dthetaxa = dthetaxa;
		this->dthetaxb = dthetaxb;
		this->dthetaya = dthetaya;
		this->dthetayb = dthetayb;
		this->dthetaza = dthetaza;
		this->dthetazb = dthetazb;
		reseted = false;
	}

};
class Robo;

struct ArmPointWithIndex;

class ArmPointIndexInfo {
private:
	string filename;
public:
	float dmin;
	float dmax;
	float mintate;
	float maxtate;
	float minyoko;
	float maxyoko;
	float dtate;
	float dyoko;
	float dd;
private:
	vector<ArmPointWithIndex*> points;
	set<int> ponums;
	ArmPointWithIndex* apw;
	int now_index;
	ArmPoint8Positioner ap8;
public:
	ArmPointIndexInfo(string filename, float dmin, float dmax, float mintate, float maxtate, float minyoko, float maxyoko,
		float dtate, float dyoko, float dd);
	~ArmPointIndexInfo();

	void makeNewFile();
	void loadFile();
	bool hasFile();
	void saveFile();
	void saveFileWithA();

	int getNonCalcedIndex();
	void setNextIndex();
	MYVECTOR3 getIndexPos();
	bool isCalcFinished();
	bool isCalced();
	
	int getNowIndex();
	void saveDtheta(ArmPoint* save_data, int index);
	ArmPoint* getArmPoint(int index);
	ArmPoint* getArmPointFromPointindex(int pointindex, MYVECTOR3* pos);

};

struct ArmPointWithIndex {
	ArmPoint point;
	int index;
	ArmPointIndexInfo* indexinfo;
	bool is_calced;
};

class ArmPositionerHelper {
private:
	Robo* robo;
	ArmPositioner* ap;
	ArmPoint8Positioner ap8;
	MYVECTOR3 dmoku;
	MYVECTOR3 moku;
	MYVECTOR3 tempmoku;
	bool is_migi;
	bool nocalcyet;
	bool is_calced;


	MYVECTOR3 ddmoku;
	MYVECTOR3 ddunko;
	float dbb;
	float bairitu;
	bool uunko;
	int unko_count;
public:
	ArmPositionerHelper(Robo* robo, ArmPositioner* ap, bool is_migi) {
		this->robo = robo;
		this->ap = ap;
		this->is_migi = is_migi;
		ddmoku = MYVECTOR3(0,0,0);
		ddunko = MYVECTOR3(0,0,0);
		dbb = 100000;
		bairitu = 1;
		uunko = false;
		unko_count = 0;
	}
	void setNoCalcYet(bool t) {
		nocalcyet = t;
		unko_count = 10000;
	}
	void setTarget(MYVECTOR3* target) {
		this->moku = *target;
	}

	void setMoku(MYVECTOR3* moku) {
		this->moku = *moku;
		tempmoku = *moku;
		dmoku = MYVECTOR3(0,0,0);
		nocalcyet = true;
		is_calced = false;

		ddmoku = MYVECTOR3(0,0,0);
		ddunko = MYVECTOR3(0,0,0);
		dbb = 100000;
		bairitu = 1;
		uunko = false;
		unko_count = 0;
	}
	bool getIsCalced() {return is_calced;}

	void calc(Graphics* g, MYMATRIX* view);
	void byougaAP8(Graphics* g, MYMATRIX* view);
	void setArmPoint8(int index, ArmPoint* po);

	ArmPoint getArmPoint() {
		ArmPoint app;
		this->ap->getTheta(&app);
		return app;
	}

	void new_calc(Graphics* g, MYMATRIX* view);

};
}
#endif

