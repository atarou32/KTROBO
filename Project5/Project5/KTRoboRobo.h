#ifndef KTROBO_ROBO_H
#define KTROBO_ROBO_H

#pragma once
#include "KTRoboMesh.h"
#include "MyTokenAnalyzer.h"

namespace KTROBO {

struct RoboDataPart {
public:
	int int_data;
	char data_name[16];
	char data_name2[32];
	char string_data[96];
	float float_data;
	RoboDataPart() {
		int_data = 0;
		memset(data_name,0,16);
		memset(data_name2,0,32);
		memset(string_data,0,96);
		float_data = 0;
	}
};

class RoboData {
	vector<RoboDataPart*> datas;
public:
	void setData(int int_data, char* data_name, char* data_name2, char* string_data, float float_data) {
		RoboDataPart* p = new RoboDataPart();
		p->int_data = int_data;
		strcpy_s(p->data_name,16,data_name);
		strcpy_s(p->data_name2,32,data_name2);
		strcpy_s(p->string_data,96,string_data);
		p->float_data = float_data;
	}
	RoboData* clone() {
		RoboData* new_data = new RoboData();
		vector<RoboDataPart*>::iterator it;
		it = datas.begin();
		while(datas.end() != it) {
			RoboDataPart* p = *it;
			new_data->setData(p->int_data,p->data_name,p->data_name2, p->string_data,p->float_data);
			it = it + 1;
		}
	}
	~RoboData() {
		vector<RoboDataPart*>::iterator it;
		it = datas.begin();
		while(it != datas.end()) {
			RoboDataPart* p = *it;
			delete p;
			p = 0;
			it = it + 1;
		}
		datas.clear();
	}
};
struct RoboMetaDataPart {
public:
	char data_name[16];
	char data_name2[32];
	char data_type[32];
	char data_sentence[32];
	char data_compare[32];
	RoboMetaDataPart() {
		memset(data_name,0,16);
		memset(data_name2,0,32);
		memset(data_type,0,32);
		memset(data_sentence,0,32);
		memset(data_compare,0,32);
	}
};

class RoboDataMetaData {
	vector<RoboMetaDataPart*> metadatas;
public:
	void setData(char* data_name, char* data_name2, char* data_type, char* data_sentence, char* data_compare) {
		RoboMetaDataPart* p = new RoboMetaDataPart();
		strcpy_s(p->data_name,16,data_name);
		strcpy_s(p->data_name2,32,data_name2);
		strcpy_s(p->data_type, 32, data_type);
		strcpy_s(p->data_sentence,32,data_sentence);
		strcpy_s(p->data_compare,32,data_sentence);
	}
	RoboMetaDataPart* getData(char* data_name) {
		vector<RoboMetaDataPart*>::iterator it;
		it = metadatas.begin();
		while(it != metadatas.end()) {
			RoboMetaDataPart* p = *it;
			if (strcmp(data_name,p->data_name)==0) {
				return p;
			}
			it = it + 1;
		}

		return 0;
	}

	~RoboDataMetaData() {
		vector<RoboMetaDataPart*>::iterator it;
		it = metadatas.begin();
		while(it != metadatas.end()) {
			RoboMetaDataPart* p = *it;
			delete p;
			p = 0;
			it = it + 1;
		}
		metadatas.clear();
	}
};


class RoboHead {
private:
	Mesh* head;
	Mesh* head2;
	Mesh* head3;

	RoboData* data;

public:
	RoboHead() {
		
		head = 0;
		head2 = 0;
		head3 = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data);
	void init(RoboHead* shead) {
		if (shead->head) {
			head = shead->head->clone();
		}
		if (shead->head2) {
			head2 = shead->head2->clone();
		}
		if( shead->head3) {
			head3 = shead->head3->clone();
		}
		if (shead->data) {
			data = shead->data->clone();
		}
	}

	void Release(){
	
		if (data) {
			delete data;
			data = 0;
		}
		if (head) {
			head->Release();
			delete head;
			head = 0;
		}
		if (head2) {
			head2->Release();
			delete head2;
			head2 = 0;
		}
		if (head3) {
			head3->Release();
			delete head3;
			head3 = 0;
		}
	}

};

class RoboArm {
private:
	Mesh* rarm;
	Mesh* larm;
	RoboData* data;
public:
	RoboArm() {
		rarm = 0;
		larm = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data);
	void init(RoboArm* sarm) {
		if (sarm->rarm) {
		rarm = sarm->rarm->clone();
		}
		if (sarm->larm) {
		larm = sarm->larm->clone();
		}
		if (sarm->data) {
		data = sarm->data->clone();
		}
	}

	void Release(){
	
		if (data) {
			delete data;
			data = 0;
		}
		if (rarm) {
			rarm->Release();
			delete rarm;
			rarm = 0;
		}
		if (larm) {
			larm->Release();
			larm = 0;
		}
	
	
	}

};

class RoboLeg {
private:
	Mesh* leg;
	RoboData* data;
public:
	RoboLeg() {
		leg = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data);
	void init(RoboLeg* sleg) {
		if (sleg->leg) {
			leg = sleg->leg->clone();
		}
		if (sleg->data) {
			data = sleg->data->clone();
		}
	}
	void Release(){
		if (data) {
			delete data;
			data = 0;
		}
		if (leg) {
			leg->Release();
			delete leg;
			leg = 0;
		}
	
	
	}
};

class RoboBody {
private:
	Mesh* body;
	RoboData* data;
public:
	RoboBody() {
		body = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data);
	void init(RoboBody* sbody) {
		if (sbody->body) {
		body = sbody->body->clone();
		}
		if (sbody->data) {
			data = sbody->data->clone();
		}
	}

	void Release() {
	
		if (data) {
			delete data;
			data = 0;
		}
		if (body) {
			body->Release();
			delete body;
			body = 0;
		}
	
	
	
	
	}
};


class RArmWeapon {
private:
	Mesh* weapon;
	RoboData* data;
public:
	RArmWeapon() {
		weapon = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data);
	void init(RArmWeapon* sweapon) {
		if (sweapon->weapon) {
		weapon = sweapon->weapon->clone();
		}
		if (sweapon->data) {
		data = sweapon->data->clone();
		}
	}
	void Release(){
	
		if (data) {
			delete data;
			data = 0;
		}
		if (weapon) {
			weapon->Release();
			delete weapon;
			weapon = 0;
		}
	
	
	}


};

class LArmWeapon {
private:
	Mesh* weapon;
	RoboData* data;
public:
	LArmWeapon() {
		weapon = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data);
	void init(LArmWeapon* sweapon) {
		if (sweapon->weapon) {
			weapon = sweapon->weapon->clone();
		}
		if (sweapon->data) {
			data = sweapon->data->clone();
		}
	}
	void Release(){
	
			if (data) {
			delete data;
			data = 0;
		}
		if (weapon) {
			weapon->Release();
			delete weapon;
			weapon = 0;
		}
	
	
	
	
	
	}

};

class RShoulderWeapon {
private:
		Mesh* weapon;
		RoboData* data;
public:
	RShoulderWeapon() {
		weapon = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data); 
	void init(RShoulderWeapon* sweapon) {
		if (sweapon->weapon) {
		weapon = sweapon->weapon->clone();
		}
		if (sweapon->data) {
		data = sweapon->data->clone();
		}
	}
	void Release(){
	
	
			if (data) {
			delete data;
			data = 0;
		}
		if (weapon) {
			weapon->Release();
			delete weapon;
			weapon = 0;
		}
	
	
	
	
	}

};

class LShoulderWeapon {
private:
	Mesh* weapon;
	RoboData* data;
public:
	LShoulderWeapon() {
		weapon = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data);
	void init(LShoulderWeapon* sweapon) {
		if (sweapon->weapon) {
		weapon = sweapon->weapon->clone();
		}
		if (sweapon->data) {
		data = sweapon->data->clone();
		}
	}

	void Release(){
	
		if (data) {
			delete data;
			data = 0;
		}
		if (weapon) {
			weapon->Release();
			delete weapon;
			weapon = 0;
		}
	
		
	
	
	
	}
};

class InsideWeapon {
private:
	Mesh* weapon;
	RoboData* data;
public:
	InsideWeapon() {
		weapon = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data);
	void init(InsideWeapon* sweapon) {
		if (sweapon->weapon) {
		weapon = sweapon->weapon->clone();
		}
		if (sweapon->data) {
		data = sweapon->data->clone();
		}
	}

	void Release(){
	
	
		if (data) {
			delete data;
			data = 0;
		}
		if (weapon) {
			weapon->Release();
			delete weapon;
			weapon = 0;
		}
	
		
	
	
	}
};


class Robo
{
private:
	RoboHead* head;
	RoboBody* body;
	RoboLeg* leg;
	RoboArm* arm;
	RArmWeapon* raweapon;
	LArmWeapon* laweapon;
	RShoulderWeapon* rsweapon;
	LShoulderWeapon* lsweapon;
	InsideWeapon* iweapon;

public:
	Robo(void);
	~Robo(void);

	void byouga(Graphics* g, MYMATRIX* world, MYMATRIX* view, MYMATRIX* proj);
	void init();
	void release();

};

}
#endif