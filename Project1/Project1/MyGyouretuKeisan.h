#ifndef MYGYOURETUKEISAN_H
#define MYGYOURETUKEISAN_H

#pragma once
#include "KTRoboMesh.h"
#include "vector"
#include "MyDefine.h"

using namespace KTROBO;
using namespace std;

#define GYOURETU_MAX 12


class MyGyouretuKeisan
{
public:
	MyGyouretuKeisan(void);
	~MyGyouretuKeisan(void);
	float det(const float* mat, int retu);
	void getMatrixInverse(float* mat, int retu, float* out_mat);
	void getMatrixTranspose(float* mat, int gyou, int retu, float* out_mat);
	void getMatrixMultiply(int gyou, int retu, float* mat, int gyou2, int retu2, float* mat2, float* out_mat);
	void getInverseYFromY(int gyou, int retu, float* mat, int XM, int NTHETA, float* out_mat);

};


class MyIKMODOKI {
private:
	float ymat[GYOURETU_MAX*GYOURETU_MAX];
	MYVECTOR3 mokuhyou;
	string mokuhyou_bonename;
	string moto_bonename;
	Mesh* mesh;
	vector<string> x_freebone;
	vector<string> y_freebone;
	vector<string> z_freebone;

	void setFreeBone();
	void calcYMat();
	void updateStep();
public:
	MyIKMODOKI(Mesh* m, MYVECTOR3* mo, char* moto, char* moku) {
		setMesh(m);
		setMokuhyou(mo);
		setbonename(moto, moku);
	}

	void updateMesh();
	void setMesh(Mesh* m) {
		mesh = m;
	}
	void setMokuhyou(MYVECTOR3* mo) {
		mokuhyou = *mo;
	}
	void setbonename(char* moto, char* moku) {
		mokuhyou_bonename = string(moku);
		moto_bonename = string(moto);
	}
	

};
#endif