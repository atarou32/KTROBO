#ifndef KTROBOKENDO_H
#define KTROBOKENDO_H

#pragma once
#include "MyDefine.h"
#include "KTRoboMesh.h"
namespace KTROBO {
class UMesh;
class UMeshUnit;
class AtariHantei;
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