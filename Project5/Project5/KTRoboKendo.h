#ifndef KTROBOKENDO_H
#define KTROBOKENDO_H

#pragma once
#include "MyDefine.h"
#include "KTRoboMesh.h"
namespace KTROBO {
class UMesh;
class UMeshUnit;
class AtariHantei;

class SinaiNigiru;

class Sinai {
private:
	Mesh* mesh;
public:
	UMeshUnit* umesh_unit;

public:
	Sinai(void);
	~Sinai(void);
	void init(Graphics* g, MyTextureLoader* loader, AtariHantei* hantei);
	void draw(Graphics* g, MYMATRIX* view, MYMATRIX* proj);
	MYVECTOR3 getMigitePos();
	MYVECTOR3 getHidaritePos();
};

class SinaiFuruPart {
public:
	float dt;// Ç«ÇÃéûä‘Ç≈ÇÃí|ìÅÇÃà íuépê®Çï\ÇµÇƒÇ¢ÇÈÇ©
	MYVECTOR3 tuka_dpos;
	float rotx;
	float rotz;
	SinaiFuruPart() {
		tuka_dpos = MYVECTOR3(0,0,0);
		dt = 0;
		rotx = 0;
		rotz = 0;
	}
	~SinaiFuruPart() {
	}
};
class SinaiFuru {
private:
	Sinai* sinai;
	UMeshUnit* nigiruhito;
	SinaiNigiru* sn;
public:
	SinaiFuru(Sinai* sinai, UMeshUnit* nigiruhito);
	~SinaiFuru();

	void men_furaseru(Graphics* g,MYMATRIX *view, MYMATRIX* proj, float dt, float anime);
	enum SINAIFURU_JYOUTAI {
		FURIAGEHAJIME=0,
		FURIAGETYUU = 1,
		FURIAGEOWARI = 2,
		FURIOROSIHAJIME = 3,
		FURIOROSITYUU = 4,
		FURIOROSIOWARI = 5,
		END = 6,
	};
	SINAIFURU_JYOUTAI getNowJyoutai();

private:
	SINAIFURU_JYOUTAI jyoutai;
#define SINAIFURU_DT_FURIAGE_HAJIMETOTYUU 0
#define SINAIFURU_DT_FURIAGE_TYUUTOOWARI 1
#define SINAIFURU_DT_FURIAGE_OWARI_TO_FURIOROSI_HAJIME 2
#define SINAIFURU_DT_FURIOROSI_HAJIMETOTYUU 3
#define SINAIFURU_DT_FURIOROSI_TYUUTOOWARI 4
#define SINAIFURU_DT_FURIOROSI_OWARI_TO_END 5
	float dts[6];
	MYVECTOR3 kamae_tuka_pos;
	float kamae_rotz;
	float kamae_rotx;
	vector<SinaiFuruPart> men_parts;

public:
	void setMenParts(SinaiFuruPart* p) {
		men_parts.push_back(*p);
	}

	void clearParts() {
		men_parts.clear();
	}


	void setJYOUTAIDT(SINAIFURU_JYOUTAI jyoutai_id, float dt) {
		dts[jyoutai_id] = dt;
	}
	void setKAMAE(float x,float z, MYVECTOR3* p) {
		kamae_rotx = x;
		kamae_rotz = z;
		kamae_tuka_pos = *p;
	}
};


class SinaiNigiru {
private:
	Sinai* sinai;
	UMeshUnit* nigiruhito;

	float RotX[KTROBO_MESH_BONE_MAX];
	float RotY[KTROBO_MESH_BONE_MAX];
	float RotZ[KTROBO_MESH_BONE_MAX];

	float meshboneDefaultAnimeFrame[KTROBO_MESH_BONE_MAX];
	MYMATRIX mesh_offset_matrix[KTROBO_MESH_BONE_MAX];

	void setRotX(int bone_index, float f) {
		RotX[bone_index] = f;
	}

	void setRotY(int bone_index, float f) {
		RotY[bone_index] = f;
	}

	void setRotZ(int bone_index, float f) {
		RotZ[bone_index] = f;
	}

	

	float getRotX(int bone_index) {
		return RotX[bone_index];
	}

	float getRotY(int bone_index) {
		return RotY[bone_index];
	}

	float getRotZ(int bone_index) {
		return RotZ[bone_index];
	}

	float getHonRotX(char* bone_name, float rotx);

	float getHonRotY(char* bone_name, float roty); 
	float getHonRotZ(char* bone_name, float rotz);

public:
	SinaiNigiru(Sinai* sinai, UMeshUnit* nigiru) {
		this->sinai = sinai;
		this->nigiruhito = nigiru;
		for (int i=0;i<KTROBO_MESH_BONE_MAX;i++) {

			RotX[i] = 0;
			RotY[i] =0;
			RotZ[i] = 0;
		
			meshboneDefaultAnimeFrame[i] = 0;
			MyMatrixIdentity(mesh_offset_matrix[i]);
		}

	}
	void setOffsetMatrixToNigiru();
	void setDefaultAnimeFrameAll(float f) {
		for (int i=0;i<KTROBO_MESH_BONE_MAX;i++) {
			meshboneDefaultAnimeFrame[i] = f;
		}
	}

	void nigiraseru(Graphics* g,MYMATRIX *view, MYMATRIX* proj);

};
class Kendo
{
public:
	Kendo(void);
	~Kendo(void);
};

}

#endif


