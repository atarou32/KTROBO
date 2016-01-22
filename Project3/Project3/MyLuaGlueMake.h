#ifndef MYLUAGLUEMAKE_H
#define MYLUAGLUEMAKE_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <vector>
#include <set>
#include "MyTokenAnalyzer.h"
#include "tolua_glue/MyLuaGlueMakeCommon.h"

using namespace std;
namespace KTROBO {
enum MyDefType {
	NOUSE = 0,
	INT = 12,
	UINT = 1,
	FLOAT = 2,
	DOUBLE=  3,
	CHAR = 4,
	PCHAR = 5,
	CLASSDAYO = 6,
	VOIDDAYO = 7,
	SHORT =8,
	USHORT = 9,
	UCHAR = 10,
	PUCHAR = 11,
	BOOL = 13,
};

struct MyFuncNUMWITHTYPE {
	MyDefType type;
	bool is_in;
	bool is_out;
	bool is_pointer;
	bool is_collected;
	bool is_yaritori;
	char kaeri6classname[128];
	char num_name[128];
};

class MakeGlueInput;


class MyFuncDef {
public:
	int func_index;// func_index;

	MyFuncNUMWITHTYPE kaeriti;// 返り値
	MyFuncNUMWITHTYPE hikisuu[HIKISUU_MAX];// 引数は16まで
	char func_name[128];// 関数名
//	char class_name[128];// クラス名
//	char collection_class_name[128];// クラスコレクション名

	MyFuncDef() {
		func_index =0;
		memset(&kaeriti,0,sizeof(MyFuncNUMWITHTYPE));
		for(int i=0;i<HIKISUU_MAX;i++) {
			memset(&hikisuu[i],0,sizeof(MyFuncNUMWITHTYPE));
		}
		memset(func_name,0,sizeof(func_name));
	//	memset(class_name,0,sizeof(class_name));
	//	memset(collection_class_name,0,sizeof(collection_class_name));
	}
	
	void getKATAStr(MyFuncNUMWITHTYPE* num, char* ans);
	void getKAERITIStr(MakeGlueInput* input, char* ans);
	void getHIKISUUStr(MakeGlueInput* input, int index, char* ans);


	char* getMyDefTypeStr(enum MyDefType def) {
		switch( def) {
		case MyDefType::NOUSE:
			return "";
		case MyDefType::INT:
			return "int";
		case MyDefType::UINT:
			return "unsigned int";
		case MyDefType::FLOAT:
			return "float";
		case MyDefType::DOUBLE:
			return "double";
		case MyDefType::CHAR:
			return "char";
		case MyDefType::PCHAR:
			return "char";
		case MyDefType::CLASSDAYO:
			return "";
		case MyDefType::VOIDDAYO:
			return "void";
		case MyDefType::SHORT:
			return "short";
		case MyDefType::USHORT:
			return "unsigned short";
		case MyDefType::UCHAR:
			return "unsigned char";
		case MyDefType::PUCHAR:
			return "unsigned char";
		case MyDefType::BOOL:
			return "bool";
		default:
			return "";
		}
	}


};


class MakeGlueInput{
public:
    char filepath[256];
	char interface_name[128];
	char collection_name[128];
	char constructor_name[128];
	char destructor_name[128];
	char collected_name[128];
	vector<MyFuncDef*> func_defs;
	vector<MyFuncDef*> construct_defs;
	vector<MyFuncDef*> destruct_defs;
	MakeGlueInput() {
		memset(filepath,0,256);
		memset(interface_name,0,128);
		memset(collection_name,0,128);
		memset(constructor_name,0,128);
		memset(destructor_name,0,128);
		memset(collected_name,0,128);
	};




	


	~MakeGlueInput() {
		Del();
	}
	void Del() {
		vector<MyFuncDef*>::iterator it = func_defs.begin();
		while( it != func_defs.end()) {
			MyFuncDef* f = *it;
			delete f;
			f = 0;

			it++;
		}
		func_defs.clear();


		it = construct_defs.begin();
		while( it != construct_defs.end()) {
			MyFuncDef* f = *it;
			delete f;
			f = 0;

			it++;
		}
		construct_defs.clear();


		it = destruct_defs.begin();
		while( it != destruct_defs.end()) {
			MyFuncDef* f = *it;
			delete f;
			f = 0;

			it++;
		}
		destruct_defs.clear();
	}
	

};

class MakeGlueOutput {
	char filepath[256]; // グルーコードのファイルパス(ヘッダファイルに直に書かれる)
};

class MyLuaGlueMake
{
private:
	vector<MakeGlueInput*> inputs;
	MakeGlueOutput output;
	int readInputsFromFile(char* filename);
	int readInputs(int input_index);
	int outputToFile(char* filename);

	int setNAME(MyTokenAnalyzer* a, MyFuncDef* f);
	int setKAERITI(MyTokenAnalyzer* a, MyFuncDef* f);
	int setFUNC(MyTokenAnalyzer* a, MakeGlueInput* input);
	int setHIKISUU(MyTokenAnalyzer* a, MyFuncDef* f, int index);
	int setConstructFunc(MyTokenAnalyzer* a, MakeGlueInput* input);
	int setDestructFunc(MyTokenAnalyzer* a, MakeGlueInput* input);
	enum MyDefType getType(MyTokenAnalyzer* a, MyFuncNUMWITHTYPE* def); // ポインタは　unsigned short ならshortの次になる
	void writeInclude(char* filename);
	void writeSingleton(char* filename);
	void writeSingletonMember(char* filename);
	void writeSingletonFunc(char* filename);
	void writeYARITORIFunc(char* filename);
	void writeFunc(char* filename);
	void _writeFunc(char* filename, MakeGlueInput* input, MyFuncDef* def, bool is_construct, bool is_destruct);
	void checkType(char *filename, MakeGlueInput* input, MyFuncDef* def, int i);

	void sengen(char* filename, MakeGlueInput* input, MyFuncDef* def, int i);
	void getHIKISUU(char* filename, MakeGlueInput* input, MyFuncDef* def, int i);
	void kannsuuYobidasi(char* filename, MakeGlueInput* input, MyFuncDef* def, int i, bool is_construct, bool is_destruct );
	void hanneiHIKISUU(char* filename, MakeGlueInput* input, MyFuncDef* def, int i);
	void pushKAERITI(char* filename, MakeGlueInput* input, MyFuncDef* def, int i, bool is_construct, bool is_destruct);

public:
	void readInputAndOutput() {
		int input_num = readInputsFromFile("tolua_glue/tolua_config.txt");
		if (input_num) {
			for (int i=0;i<input_num;i++) {
				if (readInputs(i) ==0) {
					//うまくいかなかったので書き込まないでリターン
					return;
				}
			}
			outputToFile("tolua_glue/tolua_glue.h");

			vector<MakeGlueInput*>::iterator it = inputs.begin();
			while(it != inputs.end()) {
				MakeGlueInput* in = *it;
				delete in;
				it++;
			}
			inputs.clear();
		}
	}

	MyLuaGlueMake(void);
	~MyLuaGlueMake(void);
};







}


#endif