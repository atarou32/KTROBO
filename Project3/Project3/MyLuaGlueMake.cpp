#include "MyLuaGlueMake.h"
#include "MyTokenAnalyzer.h"
using namespace KTROBO;

MyLuaGlueMake::MyLuaGlueMake(void)
{
	//memset(&output,0,sizeof(MakeGlueOutput));
	MyTokenAnalyzer::Init();
}


MyLuaGlueMake::~MyLuaGlueMake(void)
{
	MyTokenAnalyzer::Del();
	vector<MakeGlueInput*>::iterator it = inputs.begin();
	while(it != inputs.end()) {

		MakeGlueInput* i = *it;
		delete i;
		i = 0;

		it++;
	}
	inputs.clear();

}

int MyLuaGlueMake::readInputsFromFile(char* filename) {
	MyTokenAnalyzer a;
	if (!a.load(filename)) {
		KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "failed to open file %s", filename);	
		return 0;
	}
	while(!a.enddayo()) {
		// { を探す

		a.GetToken();
		if (strcmp(a.Toke(), "{")==0) {
			// 設定の始まり
			MakeGlueInput* input = new MakeGlueInput();
			//memset(input,0,sizeof(MakeGlueInput));

			while(strcmp(a.Toke(), "}")!=0 && !a.enddayo()) {
				a.GetToken();
				if (strcmp(a.Toke(), MAKEGLUEINPUT_COLLECTION)==0) {
					a.GetToken();
					mystrcpy(input->collection_name,128, 0, a.Toke());
					input->collection_name[127] = '\0';
				} else if(strcmp(a.Toke(), MAKEGLUEINPUT_CONSTRUCTOR)==0) {
					a.GetToken();
					mystrcpy(input->constructor_name,128,0,a.Toke());
					input->constructor_name[127] = '\0';
				} else if(strcmp(a.Toke(), MAKEGLUEINPUT_DESTRUCTOR) ==0) {
					a.GetToken();
					mystrcpy(input->destructor_name, 128,0,a.Toke());
					input->destructor_name[127] = '\0';
				} else if(strcmp(a.Toke(), MAKEGLUEINPUT_FILEPATH) ==0) {
					a.GetToken();
					mystrcpy(input->filepath, 256,0,a.Toke());
					input->filepath[255] = '\0';
				} else if(strcmp(a.Toke(), MAKEGLUEINPUT_INTERFACE)==0) {
					a.GetToken();
					mystrcpy(input->interface_name,128,0, a.Toke());
					input->interface_name[127] = '\0';
				} else if(strcmp(a.Toke(), MAKEGLUEINPUT_COLLECTED)==0) {
					a.GetToken();
					mystrcpy(input->collected_name,128,0,a.Toke());
					input->collected_name[127]='\0';
				} else if(strcmp(a.Toke(), MAKEGLUEINPUT_SUPERCOLLECTED) ==0) {
					a.GetToken();
					mystrcpy(input->super_collected_name,128,0,a.Toke());
					input->super_collected_name[127] = '\0';
				}
			}
			if (strlen(input->collection_name) ==0 || 
					strlen(input->constructor_name) ==0|| 
					strlen(input->destructor_name) ==0 ||
					strlen(input->filepath) ==0 ||
					strlen(input->interface_name)==0 ||
					strlen(input->collected_name)==0) {
						
						a.deletedayo();
						KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "failed because of noname %s %s %s %s %s %s", input->collection_name, 
							input->constructor_name, input->destructor_name, input->filepath, input->interface_name, input->collected_name);
						delete input;
						return 0;
			}
			this->inputs.push_back(input);


		
		}
	}
	a.deletedayo();
	return inputs.size();
}



enum MyDefType MyLuaGlueMake::getType(MyTokenAnalyzer* a, MyFuncNUMWITHTYPE* f) {
	bool unsign = false;
	if (strcmp(a->Toke(), "unsigned")==0) {
		unsign = true;
		a->GetToken();
	}
	char mae_token[1024];
	memset(mae_token,0,1024);
	strcpy_s(mae_token, 1024, a->Toke());
	a->GetToken();
	int mpos = strpos(mae_token, "*");
	int pos = strpos(a->Toke(), "*");
	f->is_pointer = false;
	if ((pos != -1) || (mpos != -1)) {
		// pointer
		f->is_pointer = true;
		if (pos == -1) {
			mystrcpy2(f->kaeri6classname, 128,mpos, mae_token);
		} else {
			strcpy_s(f->kaeri6classname, 128, mae_token);
		}
		if (strpos(mae_token, "char") != -1) {
			if (unsign) {
				f->type = MyDefType::PUCHAR;
			
			} else {
				f->type = MyDefType::PCHAR;
			}
		} else {
			f->type = MyDefType::CLASSDAYO;
		}
	}

	if (strpos(mae_token, "float")!= -1) {
		f->type = MyDefType::FLOAT;
		return f->type;
	}

	if (strpos(mae_token, "double")!=-1) {
		f->type = MyDefType::DOUBLE;
		return f->type;
	}

	if (strpos(mae_token, "int")!=-1) {
		if (unsign) {
			f->type = MyDefType::UINT;
		} else {
			f->type = MyDefType::INT;
		}
		return f->type;
	}

	if (strpos(mae_token, "short")!=-1) {
		if (unsign) {
			f->type = MyDefType::USHORT;
		} else {
			f->type = MyDefType::SHORT;
		}
		return f->type;
	}

	if (strpos(mae_token, "char")!=-1) {
		if (unsign) {
			f->type = MyDefType::UCHAR;
		} else {
			f->type = MyDefType::CHAR;
		}
		return f->type;
	}

	if (strpos(mae_token, "void")!=-1) {
		f->type = MyDefType::VOIDDAYO;
		return f->type;
	}

	if(strpos(mae_token, "bool")!=-1) {
		f->type=MyDefType::BOOL;
		return f->type;
	}

	return f->type;




}
int MyLuaGlueMake::setKAERITI(MyTokenAnalyzer* a, MyFuncDef* f) {
	
	if (strcmp(a->Toke(), "YARITORI")==0) {
		f->kaeriti.is_yaritori = true;
		a->GetToken();
	} else {
		f->kaeriti.is_yaritori = false;
	}

	f->kaeriti.type = getType(a,&f->kaeriti);
	return 1;
}



int MyLuaGlueMake::setNAME(MyTokenAnalyzer* a, MyFuncDef* f) {
	
	if (strcmp(a->Toke(), "*")==0) {
		a->GetToken();
	}
	int pos = strpos(a->Toke(), "*");
	if (pos != -1) {
		mystrcpy(f->func_name,128,pos,a->Toke());
	} else {
		strcpy_s(f->func_name, 128, a->Toke());
	}

	return 1;

}


int MyLuaGlueMake::setHIKISUU(MyTokenAnalyzer* a, MyFuncDef* f, int index) {
	if (strcmp(a->Toke(), ")")==0) {

		return 0;
	}



	if (index >= HIKISUU_MAX) return 0;


	if (strcmp(a->Toke(), "IN_")==0) {
		f->hikisuu[index].is_in = true;
		f->hikisuu[index].is_out = false;
		a->GetToken();
	} else if(strcmp(a->Toke(), "OUT_")==0) {
		f->hikisuu[index].is_in = false;
		f->hikisuu[index].is_out = true;
		a->GetToken();
	} else if(strcmp(a->Toke(), "INOUT_")==0) {
		f->hikisuu[index].is_in = true;
		f->hikisuu[index].is_out = false;
		a->GetToken();
	} else {
		f->hikisuu[index].is_in = true;
		f->hikisuu[index].is_out = false;
	}

	if (strcmp(a->Toke(), "COLLECTED")==0) {
		f->hikisuu[index].is_collected = true;
		a->GetToken();
	} else {
		f->hikisuu[index].is_collected = false;
	}

	if (strcmp(a->Toke(), "YARITORI")==0) {
		f->hikisuu[index].is_yaritori = true;
		a->GetToken();
	} else {
		f->hikisuu[index].is_yaritori = false;
	}


	f->hikisuu[index].type = getType(a,&f->hikisuu[index]);
    if (strcmp(a->Toke(), "*")==0) {
		a->GetToken();
	}
	int pos = strpos(a->Toke(), "*");
	if (pos != -1) {
		mystrcpy(f->hikisuu[index].num_name,128,pos,a->Toke());
	} else {
		strcpy_s(f->hikisuu[index].num_name, 128, a->Toke());
	}

	return 1;
}

int MyLuaGlueMake::setFUNC(MyTokenAnalyzer* a, MakeGlueInput* input) {

	if (strcmp(a->Toke(), "TO_LUA")==0) {
		a->GetToken();
	}

	if (strcmp(a->Toke(), "virtual")==0) {
		a->GetToken();
	}
				
			
	if (strcmp(a->Toke(), "}")==0) {
		return 0;
	}

	// 返り値
	MyFuncDef* def = new MyFuncDef();
	memset(def, 0, sizeof(MyFuncDef));
	setKAERITI(a, def);
	setNAME(a, def);
	a->GetToken("(");
	int hikisuu_count = 0;
	while(((strcmp(a->Toke(), ")") != 0) && !a->enddayo()) && (hikisuu_count < HIKISUU_MAX)) {
		a->GetToken();// 型名


		if (!setHIKISUU(a, def, hikisuu_count)) {
			KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "error in hikisuu");
		}
		hikisuu_count++;
	}


	a->GetToken();
	if (strcmp(a->Toke(), "0")==0) {
		a->GetToken(); // これでvirtual の部分あたりから再開できる
	}
	int size = input->func_defs.size();
	def->func_index = size;

	input->func_defs.push_back(def);
	return 1;
}

int MyLuaGlueMake::setConstructFunc(MyTokenAnalyzer* a, MakeGlueInput* input) {

	MyFuncDef* def = new MyFuncDef();
	strcpy_s(def->func_name,128, input->constructor_name);
	//setKAERITI(a, def);
	def->kaeriti.is_collected = false;
	def->kaeriti.is_in = true;
	def->kaeriti.is_out = false;
	def->kaeriti.is_pointer = false;
	def->kaeriti.is_yaritori = false;
	def->kaeriti.type = MyDefType::INT;
	// name は入っている
	//a->GetToken();
	a->GetToken("(");
	int hikisuu_count = 0;
	while((strcmp(a->Toke(), ")") != 0 && !a->enddayo())&& (hikisuu_count < HIKISUU_MAX)) {
		a->GetToken();// 型名
		if (!setHIKISUU(a, def, hikisuu_count)) {
			KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "error in hikisuu");
		}
		hikisuu_count++;
	}

	input->construct_defs.push_back(def);
	return 1;
}


int MyLuaGlueMake::setDestructFunc(MyTokenAnalyzer* a, MakeGlueInput* input) {
	MyFuncDef* def = new MyFuncDef();
	strcpy_s(def->func_name,128, input->constructor_name);
	def->kaeriti.type = MyDefType::INT;
	a->GetToken();
	// name は入っている
	a->GetToken();
	a->GetToken("(");
	int hikisuu_count = 0;
	while((strcmp(a->Toke(), ")") != 0 && !a->enddayo()) && (hikisuu_count < HIKISUU_MAX)) {
		a->GetToken();// 型名
		if (!setHIKISUU(a, def, hikisuu_count)) {
			KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "error in hikisuu");
		}
		hikisuu_count++;
	}

	input->destruct_defs.push_back(def);
	return 1;

}


int MyLuaGlueMake::readInputs(int input_index) {
	MakeGlueInput* input = inputs[input_index];
	MyTokenAnalyzer a;
	if (!a.load(input->filepath)) {
		return 0;
	}

	// インターフェース名を取得して関数の情報を入れ込む
	while(!a.enddayo()) {
		a.GetToken();
		if (strcmp(a.Toke(), "interface")==0) {
			a.GetToken();
			if (strcmp(a.Toke(), input->interface_name)==0) {
				// インターフェース名まできたので関数の情報を取得する
				a.GetToken();
				a.GetToken("{"); // {
				while((strcmp(a.Toke(),"public:")!=0) && !a.enddayo()) {
					a.GetToken();
				}
				a.GetToken();

				while ((strcmp(a.Toke(), "}") !=0) && !a.enddayo()) {
					if (strcmp(a.Toke(), "TO_LUA")==0) {
						setFUNC(&a,input);
					} else {
						a.GetToken();
					}
				}
			}
		}
	}
				
				
				
	// コレクション名を取得してコンストラクタ、デストラクタの情報を入れ込む
	a.resetPointer();

	while(!a.enddayo()) {
		a.GetToken();
		if (strcmp(a.Toke(), "class")==0) {
			a.GetToken();
			if (strcmp(a.Toke(), input->collection_name)==0) {

				while (!a.enddayo()) {
					a.GetToken();
					
					while((strpos(a.Toke(), input->constructor_name) == -1) && !a.enddayo()) {
						a.GetToken();
						
					}
					if (!a.enddayo()) {
						// コレクションの該当インターフェースのコンストラクタの帰り値はインスタンスID（int)と決め討ちで処理する
						setConstructFunc(&a,input);
					}
				}
			}
		}
	}

	// 今度はデストラクタ
	a.resetPointer();
	while(!a.enddayo()) {
		a.GetToken();
		if (strcmp(a.Toke(), "class")==0) {
			a.GetToken();
			if (strcmp(a.Toke(), input->collection_name)==0) {

				while (!a.enddayo()) {
					a.GetToken();
					
					while((strpos(a.Toke(), input->destructor_name) == -1) && !a.enddayo()) {
						a.GetToken();
						
					}
					if (!a.enddayo()) {
						// コレクションの該当インターフェースのデストラクタの帰り値はintと決め討ちする
						setDestructFunc(&a,input);
					}
				}
			}
		}
	}

	a.deletedayo();
	return 1;
}


void MyLuaGlueMake::writeInclude(char* filename) {
	KTROBO::mylog::writelog(filename, "#pragma once\n");
	KTROBO::mylog::writelog(filename, "#include <vector>\n");
	KTROBO::mylog::writelog(filename, "#include \"lua.hpp\"\n");
	KTROBO::mylog::writelog(filename, "#include \"lualib.h\"\n");
	KTROBO::mylog::writelog(filename, "#include \"lauxlib.h\"\n");
	KTROBO::mylog::writelog(filename, "#include \"../KTRoboGameError.h\"\n");
	KTROBO::mylog::writelog(filename, "#include \"MyLuaGlueMakeCommon.h\"\n");
	vector<MakeGlueInput*>::iterator it = inputs.begin();
	while(it != inputs.end()) {
		MakeGlueInput* input = *it;
		KTROBO::mylog::writelog(filename, "#include \"../%s\"\n", input->filepath);
		it++;
	}

	KTROBO::mylog::writelog(filename, "using namespace std;\n");
}


void MyLuaGlueMake::writeSingletonFunc(char* filename) {

	KTROBO::mylog::writelog(filename, " public:\n");
	vector<MakeGlueInput*>::iterator it = inputs.begin();
	while (it != inputs.end()) {
		MakeGlueInput* input = *it;
		// コレクションクラスの要素に対するアセクササメソッド
		KTROBO::mylog::writelog(filename, "%s* getCol%s ( int index) {\n", input->collection_name, input->collection_name);
		KTROBO::mylog::writelog(filename, "  int size = m_%s.size();\n", input->collection_name);
		KTROBO::mylog::writelog(filename, "  if (size > index && index >=0) {\n");
		KTROBO::mylog::writelog(filename, "    return m_%s[index];\n", input->collection_name);
		KTROBO::mylog::writelog(filename, "  } else {\n");
		KTROBO::mylog::writelog(filename, "    throw new GameError(KTROBO::WARNING, \" try to access outside vector %s\");\n", input->collection_name);
		KTROBO::mylog::writelog(filename, "  }\n");
		KTROBO::mylog::writelog(filename, "}\n");

		KTROBO::mylog::writelog(filename, "void setCol%s ( %s* m) {\n", input->collection_name, input->collection_name);
		KTROBO::mylog::writelog(filename, "  m_%s.push_back(m);\n", input->collection_name);
		KTROBO::mylog::writelog(filename, "}\n");

		it = it + 1;
	}

	// registerするregister関数
	//KTROBO::mylog::writelog(filename, "void registerdayo(lua_State* L);\n");



}

void MyLuaGlueMake::writeRegister(char* filename) {
	KTROBO::mylog::writelog(filename, "void registerdayo(lua_State* L) {\n");

	vector<MakeGlueInput*>::iterator it = inputs.begin();
	while (it != inputs.end()) {
		MakeGlueInput* input = *it;
		int func_size = input->func_defs.size();
		// func

		for (int i=0;i<func_size; i++) {
			KTROBO::mylog::writelog(filename, "  lua_register(L, \"%s_%s\", KTROBO::%s_%sGlue);\n", input->collection_name, input->func_defs[i]->func_name,
				input->collection_name, input->func_defs[i]->func_name);
		}

		// constructor
		int con_size  = input->construct_defs.size();
		for (int i=0;i<con_size; i++) {
			KTROBO::mylog::writelog(filename, "  lua_register(L, \"%s_%s\", KTROBO::%s_%sGlue);\n", input->collection_name, input->construct_defs[i]->func_name,
				input->collection_name, input->construct_defs[i]->func_name);
		}

		// destructor
		int des_size = input->destruct_defs.size();
		for (int i=0;i<des_size; i++) {
			KTROBO::mylog::writelog(filename, "  lua_register(L, \"%s_%s\", KTROBO::%s_%sGlue);\n", input->collection_name, input->destruct_defs[i]->func_name,
				input->collection_name, input->destruct_defs[i]->func_name);
		}

		it = it + 1;
	}
	KTROBO::mylog::writelog(filename, "}");




}

void MyLuaGlueMake::writeSingletonMember(char* filename) {
	KTROBO::mylog::writelog(filename, " private:\n");
	vector<MakeGlueInput*>::iterator it = inputs.begin();
	while (it != inputs.end()) {
		MakeGlueInput* input = *it;

		KTROBO::mylog::writelog(filename, "  vector<%s*> m_%s;\n", input->collection_name, input->collection_name);
		it = it + 1;
	}


}


void MyFuncDef::getKATAStr(MyFuncNUMWITHTYPE* num, char* ans) {


	char* str = getMyDefTypeStr(num->type);

	if ((num->type == MyDefType::PCHAR) || (num->type == MyDefType::PUCHAR)) {

		strcpy_s(ans,1024, str);// charの場合1024確保する

	} else if(num->type == MyDefType::CLASSDAYO) {
		if (num->is_pointer) {
			sprintf_s(ans, 1024,"%s*", num->kaeri6classname);// クラスのポインタの場合ひとつだけと考えて処理を行う
			return;
		} else {
			sprintf_s(ans,1024, "%s", num->kaeri6classname);
			return;
		}
	} else if(num->is_pointer) {
		sprintf_s(ans,1024, "%s", str);// クラス以外の場合のポインタは配列と考えて確保する　32
		return;
	}

	strcpy_s(ans,1024,str);
	return;
}

void MyFuncDef::getKAERITIStr(MakeGlueInput* input, char* ans) {
	char kata[1024];
	memset(kata,0,1024);
	this->getKATAStr(&kaeriti,kata);
	sprintf_s(ans,1024, "%s", kata);
}

void MyFuncDef::getHIKISUUStr(MakeGlueInput* input, int index, char* ans) {
	
	if (this->hikisuu[index].type == MyDefType::NOUSE) return;


	if (this->hikisuu[index].is_collected) {
		// コレクションのindexとクラスのindexの二つになる 
		sprintf_s(ans,1024,"int %ss_index, int %s_index", this->hikisuu[index].kaeri6classname, hikisuu[index].kaeri6classname);
		return;
	} else if(this->hikisuu[index].is_yaritori) {
		// やり取りストラクトのインデックスになる
		sprintf_s(ans,1024,"int %s_index", this->hikisuu[index].num_name);
	}
	
	char kata[1024];
	memset(kata,0,1024);
	this->getKATAStr(&hikisuu[index],kata);
	
	sprintf_s(ans,1024, "%s %s", kata, hikisuu[index].num_name);
}


void MyLuaGlueMake::checkType(char *filename, MakeGlueInput* input, MyFuncDef* def, int i) {
	// -i-3なのは　-1にcollection_のインデックスが入るため -2 にcollectedのインデックスが入る
	if (def->hikisuu[i].is_pointer && 
		(!(def->hikisuu[i].type != MyDefType::CHAR)) &&
		(!(def->hikisuu[i].type != MyDefType::PCHAR)) &&
		(!(def->hikisuu[i].type != MyDefType::PUCHAR)) &&
		(!(def->hikisuu[i].type != MyDefType::UCHAR))) {
		KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TTABLE);\n", 3+i);
		return;
	}

	switch (def->hikisuu[i].type) {
			case MyDefType::BOOL:
			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TBOOLEAN);\n", i+3);

			break;
			case MyDefType::CHAR:
			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", i+3);
			break;
			case MyDefType::CLASSDAYO:
			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TTABLE);\n", i+3);
			break;
			case MyDefType::DOUBLE:
			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", i+3);
			break;
			case MyDefType::FLOAT:
			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", i+3);
			break;
			case MyDefType::INT:
			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TNUMBER);\n", i+3);
			break;
			case MyDefType::PCHAR:
			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", i+3);
			break;
			case MyDefType::PUCHAR:
			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", i+3);
			break;
			case MyDefType::SHORT:
			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TNUMBER);\n", i+3);
			break;
			case MyDefType::UCHAR:
			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", i+3);
			break;
			case MyDefType::UINT:
			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TNUMBER);\n", i+3);
			break;
			case MyDefType::USHORT:
			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TNUMBER);\n", i+3);
			break;
			case MyDefType::VOIDDAYO:
			break;
			default:
			break;
			
	}
}

bool MyLuaGlueMake::isCharType(MyFuncNUMWITHTYPE* num) {
	if (num->type == MyDefType::CHAR) {
		return true;
	}
	if (num->type == MyDefType::PCHAR) {
		return true;
	}
	if (num->type == MyDefType::PUCHAR) {
		return true;
	}
	if (num->type == MyDefType::UCHAR) {
		return true;
	}
	return false;
}

void MyLuaGlueMake::sengen(char* filename, MakeGlueInput* input, MyFuncDef* def, int i) {
	char kata[1024];
	memset(kata,0,1024);
	def->getKATAStr(&def->hikisuu[i],kata);

	if ((def->hikisuu[i].type == MyDefType::PCHAR) || (def->hikisuu[i].type == MyDefType::PUCHAR)) {

		KTROBO::mylog::writelog(filename, "%s %s[%d];\n", kata, def->hikisuu[i].num_name, TEMPSTRING_SIZE);
		KTROBO::mylog::writelog(filename, "memset(%s,0, sizeof(%s)* %d);\n", def->hikisuu[i].num_name, kata, TEMPSTRING_SIZE);
		return;
	} else if(def->hikisuu[i].type == MyDefType::CLASSDAYO) {
		if (def->hikisuu[i].is_yaritori) {
		KTROBO::mylog::writelog(filename, "%s %s_m;\n", def->hikisuu[i].kaeri6classname, def->hikisuu[i].num_name);
		KTROBO::mylog::writelog(filename, "%s %s=&%s_m;\n", kata, def->hikisuu[i].num_name, def->hikisuu[i].num_name);
		} else {
			KTROBO::mylog::writelog(filename, "%s %s;\n", kata, def->hikisuu[i].num_name);
		}
		return;
	} else if(def->hikisuu[i].is_pointer) {
		if (isCharType(&def->hikisuu[i])) {

			KTROBO::mylog::writelog(filename, "%s %s[%d];\n", kata, def->hikisuu[i].num_name, TEMPSTRING_SIZE);
			KTROBO::mylog::writelog(filename, "memset(%s,0, sizeof(%s)* %d);\n", def->hikisuu[i].num_name, kata, TEMPSTRING_SIZE);
	

		} else {

		KTROBO::mylog::writelog(filename, "%s %s[%d];\n", kata, def->hikisuu[i].num_name, TEMPHAIRETU_SIZE);
		KTROBO::mylog::writelog(filename, "memset(%s,0, sizeof(%s)* %d);\n", def->hikisuu[i].num_name, kata, TEMPHAIRETU_SIZE);
		}
		return;
	}
	KTROBO::mylog::writelog(filename, "%s %s;\n", kata, def->hikisuu[i].num_name);
}
char* MyLuaGlueMake::getCollectedHIKISUUName(MyFuncNUMWITHTYPE* num) {
	if (num->is_collected) {
		int isize = inputs.size();
		for (int i=0;i<isize; i++) {
			MakeGlueInput* input = inputs[i];
			if (strlen(input->super_collected_name)) {
				if (strcmp(input->super_collected_name, num->kaeri6classname)==0) {
					return input->collected_name;
				}
			}
		}
		return num->kaeri6classname;

	} else {
		KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "error in get collected hikisuu name");
		return "ERRROR!!";
	}

}



void MyLuaGlueMake::getHIKISUU(char* filename, MakeGlueInput* input, MyFuncDef* def, int i) {


	if (def->hikisuu[i].is_pointer) {
		switch (def->hikisuu[i].type) {
			case MyDefType::BOOL:
			//KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TBOOLEAN);\n", -i-2);
				//	KTROBO::mylog::writelog(filename, "%s = lua_toboolean(L,%d);\n", def->hikisuu[i].num_name, -i-2);
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// 配列のサイズを取得
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// 配列の要素（クラスでない場合は数の配列とみなす)
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_gettable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "  %s[i] = (bool)lua_toboolean(L, -1);\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_remove(L,-1);\n");
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");
				
			break;
			case MyDefType::CHAR:
//			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", -i-2);
			
					KTROBO::mylog::writelog(filename, "mystrcpy(%s,%d,0,lua_tostring(L,%d));\n", def->hikisuu[i].num_name,TEMPSTRING_SIZE, i+3);
					
			break;
			case MyDefType::CLASSDAYO:
//			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TTABLE);\n", -i-2);
				// 後で書く
				if (def->hikisuu[i].is_collected) {
					// 連想配列の長さを取得し0だった場合NULLを指定する
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);
					KTROBO::mylog::writelog(filename, "if (temp ==0) {\n");
					KTROBO::mylog::writelog(filename, "  %s = NULL;\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "} else {\n");

					// 連想配列からキーインデックスを取得する
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "lua_pushnumber(L,%d);\n", DEF_LUA_FIELD_KEY);
					KTROBO::mylog::writelog(filename, "lua_gettable(L,%d);\n", i+3);
					KTROBO::mylog::writelog(filename, "luaL_checktype(L, -1, LUA_TTABLE);\n");

//					KTROBO::mylog::writelog(filename, "lua_pushstring(L,\"%s\");\n", COLLECTION_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "lua_getfield(L, %d, \"%s\");\n", -1, COLLECTION_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "int %s=lua_tonumber(L, -1);\n", COLLECTION_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "lua_remove(L,-1);\n");
	//				KTROBO::mylog::writelog(filename, "lua_pushstring(L,\"%s\");\n", COLLECTED_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "lua_getfield(L, %d, \"%s\");\n", -1, COLLECTED_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "int %s=lua_tonumber(L, -1);\n", COLLECTED_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "lua_remove(L,-1);\n");
					KTROBO::mylog::writelog(filename, "lua_remove(L,-1);\n");
					KTROBO::mylog::writelog(filename, "%s = KTROBO::MyLuaGlueSingleton::getInstance()->getCol%ss(%s)->getInstance(%s);\n", 
						def->hikisuu[i].num_name, this->getCollectedHIKISUUName(&def->hikisuu[i]), COLLECTION_INDEX_LUA_FIELD, COLLECTED_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");
				} else if(def->hikisuu[i].is_yaritori) {
					// YARITORIの専用関数を呼ぶ
					KTROBO::mylog::writelog(filename, "YARITORI_get%s(L, %s, %d);\n", def->hikisuu[i].kaeri6classname, def->hikisuu[i].num_name, i);
				} else {
						KTROBO::mylog::writelog(filename, "  %s = NULL;\n", def->hikisuu[i].num_name);
				}

			break;
			case MyDefType::DOUBLE:
//			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", -i-2);
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// 配列のサイズを取得
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// 配列の要素（クラスでない場合は数の配列とみなす)
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_gettable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "  %s[i] = (double)atof(lua_tostring(L, -1));\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_remove(L,-1);\n");
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");

			break;
			case MyDefType::FLOAT:
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// 配列のサイズを取得
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// 配列の要素（クラスでない場合は数の配列とみなす)
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_gettable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "  %s[i] = atof(lua_tostring(L, -1));\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_remove(L,-1);\n");
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");

			break;
			case MyDefType::INT:
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// 配列のサイズを取得
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// 配列の要素（クラスでない場合は数の配列とみなす)
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_gettable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "  %s[i] = (lua_tonumber(L, -1));\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_remove(L,-1);\n");
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");

			break;
			case MyDefType::PCHAR:
				KTROBO::mylog::writelog(filename, "mystrcpy(%s,%d,0,lua_tostring(L,%d));\n", def->hikisuu[i].num_name,TEMPSTRING_SIZE, i+3);
		
			break;
			case MyDefType::PUCHAR:
				KTROBO::mylog::writelog(filename, "mystrcpy((char*)%s,%d,0,lua_tostring(L,%d));\n", def->hikisuu[i].num_name,TEMPSTRING_SIZE, i+3);
		
			break;
			case MyDefType::SHORT:
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// 配列のサイズを取得
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// 配列の要素（クラスでない場合は数の配列とみなす)
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_gettable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "  %s[i] = (short)(lua_tonumber(L, -1));\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_remove(L,-1);\n");
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");

			break;
			case MyDefType::UCHAR:
				KTROBO::mylog::writelog(filename, "mystrcpy((char*)%s,%d,0,lua_tostring(L,%d));\n", def->hikisuu[i].num_name,TEMPSTRING_SIZE, i+3);
		

			break;
			case MyDefType::UINT:
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// 配列のサイズを取得
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// 配列の要素（クラスでない場合は数の配列とみなす)
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_gettable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "  %s[i] = (unsigned int)(lua_tonumber(L, -1));\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_remove(L,-1);\n");
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");

			break;
			case MyDefType::USHORT:
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// 配列のサイズを取得
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// 配列の要素（クラスでない場合は数の配列とみなす)
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_gettable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "  %s[i] = (unsigned short)(lua_tonumber(L, -1));\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_remove(L,-1);\n");
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");

			break;
			case MyDefType::VOIDDAYO:
			break;
			default:
			break;
	}




	} else {

	switch (def->hikisuu[i].type) {
			case MyDefType::BOOL:
			//KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TBOOLEAN);\n", -i-2);
					KTROBO::mylog::writelog(filename, "%s = lua_toboolean(L,%d);\n", def->hikisuu[i].num_name, i+3);
			break;
			case MyDefType::CHAR:
//			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", -i-2);
			
			//		KTROBO::mylog::writelog(filename, "mystrcpy(%s,%s,0,lua_tostring(L,%d));\n", def->hikisuu[i].num_name,TEMPSTRING_SIZE, -i-2);
					KTROBO::mylog::writelog(filename, "%s =*lua_tostring(L,%d);\n", def->hikisuu[i].num_name, i+3);
			break;
			case MyDefType::CLASSDAYO:
//			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TTABLE);\n", -i-2);
				// 後で書く
				if (def->hikisuu[i].is_collected) {


					// 連想配列の長さを取得し0だった場合NULLを指定する
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);
					KTROBO::mylog::writelog(filename, "if (temp ==0) {\n");
					KTROBO::mylog::writelog(filename, "  %s = NULL;\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "} else {\n");

					// 連想配列からキーインデックスを取得する
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "lua_pushnumber(L,%d);\n", DEF_LUA_FIELD_KEY);
					KTROBO::mylog::writelog(filename, "lua_gettable(L,%d);\n", i+3);
				
					KTROBO::mylog::writelog(filename, "luaL_checktype(L, -1, LUA_TTABLE);\n");

//					KTROBO::mylog::writelog(filename, "lua_pushstring(L,\"%s\");\n", COLLECTION_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "lua_getfield(L, %d, \"%s\");\n", -1, COLLECTION_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "int %s=lua_tonumber(L, -1);\n", COLLECTION_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "lua_remove(L,-1);\n");
	//				KTROBO::mylog::writelog(filename, "lua_pushstring(L,\"%s\");\n", COLLECTED_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "lua_getfield(L, %d, \"%s\");\n", -1, COLLECTED_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "int %s=lua_tonumber(L, -1);\n", COLLECTED_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "lua_remove(L,-1);\n");
					KTROBO::mylog::writelog(filename, "lua_remove(L,-1);\n");
					KTROBO::mylog::writelog(filename, "%s = KTROBO::MyLuaGlueSingleton::getInstance()->getCol%ss(%s)->getInstance(%s);\n", 
						def->hikisuu[i].num_name, this->getCollectedHIKISUUName(&def->hikisuu[i]), COLLECTION_INDEX_LUA_FIELD, COLLECTED_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");
				} else if(def->hikisuu[i].is_yaritori) {
					// YARITORIの専用関数を呼ぶ
					KTROBO::mylog::writelog(filename, "YARITORI_get%s(L, %s, %d);\n", def->hikisuu[i].kaeri6classname, def->hikisuu[i].num_name, i);
				} else {
					KTROBO::mylog::writelog(filename, "  %s = NULL;\n", def->hikisuu[i].num_name);
				}

			break;
			case MyDefType::DOUBLE:
//			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", -i-2);
				KTROBO::mylog::writelog(filename, "const char* %s_temp = lua_tostring(L,%d));\n", def->hikisuu[i].num_name, i+3);
				KTROBO::mylog::writelog(filename, "%s = (double)atof(%s_temp);\n", def->hikisuu[i].num_name, def->hikisuu[i].num_name);


			break;
			case MyDefType::FLOAT:
//			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", -i-2);
				KTROBO::mylog::writelog(filename, "const char* %s_temp = lua_tostring(L,%d));\n", def->hikisuu[i].num_name, i+3);
				KTROBO::mylog::writelog(filename, "%s = atof(%s_temp);\n", def->hikisuu[i].num_name, def->hikisuu[i].num_name);

			break;
			case MyDefType::INT:
	//		KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TNUMBER);\n", -i-2);
				KTROBO::mylog::writelog(filename, "%s = lua_tonumber(L,%d);\n", def->hikisuu[i].num_name, i+3);
	
			break;
			case MyDefType::PCHAR:
//			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", -i-2);
			KTROBO::mylog::writelog(filename, "const char* %s_temp = lua_tostring(L,%d));\n", def->hikisuu[i].num_name, i+3);
			KTROBO::mylog::writelog(filename, "mystrcpy(%s, %d,0, %s_temp);\n", def->hikisuu[i].num_name,TEMPSTRING_SIZE, def->hikisuu[i].num_name, i+3);
		
			break;
			case MyDefType::PUCHAR:
//			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", -i-2);
			KTROBO::mylog::writelog(filename, "const char* %s_temp = lua_tostring(L,%d));\n", def->hikisuu[i].num_name, i+3);
			KTROBO::mylog::writelog(filename, "mystrcpy((char*)%s, %d,0, %s_temp);\n", def->hikisuu[i].num_name,TEMPSTRING_SIZE, def->hikisuu[i].num_name, i+3);
	
			break;
			case MyDefType::SHORT:
//			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TNUMBER);\n", -i-2);
			KTROBO::mylog::writelog(filename, "%s = (short)lua_tonumber(L,%d);\n", def->hikisuu[i].num_name, i+3);
	
			break;
			case MyDefType::UCHAR:
	//		KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", -i-2);
			KTROBO::mylog::writelog(filename, "%s =(unsigned char)*lua_tostring(L,%d);\n", def->hikisuu[i].num_name, i+3);
		

			break;
			case MyDefType::UINT:
	//		KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TNUMBER);\n", -i-2);
			KTROBO::mylog::writelog(filename, "%s = (unsigned int)lua_tonumber(L,%d);\n", def->hikisuu[i].num_name, i+3);
	
			break;
			case MyDefType::USHORT:
			//KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TNUMBER);\n", -i-2);
			KTROBO::mylog::writelog(filename, "%s = (unsigned short)lua_tonumber(L,%d);\n", def->hikisuu[i].num_name, i+3);
	
			break;
			case MyDefType::VOIDDAYO:
			break;
			default:
			break;
			
	}
	}
}

void MyLuaGlueMake::kannsuuYobidasi(char* filename, MakeGlueInput* input, MyFuncDef* def, int i, bool is_construct, bool is_destruct ) {
	// kaeriti　に保存する
	char kata[TEMPSTRING_SIZE];
	memset(kata,0,  TEMPSTRING_SIZE);
	def->getKATAStr(&def->kaeriti,kata);
	KTROBO::mylog::writelog(filename, "int ci = lua_tonumber(L, 1);\n");
	KTROBO::mylog::writelog(filename, "int cci = lua_tonumber(L, 2);\n");
	if (is_construct) {
		KTROBO::mylog::writelog(filename, "%s kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getCol%s(ci)->%s(",kata, input->collection_name,def->func_name);
	
	} else if(def->kaeriti.type != MyDefType::VOIDDAYO) {
	KTROBO::mylog::writelog(filename, "%s kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getCol%s(ci)->getInterface(cci)->%s(",kata, input->collection_name,def->func_name);
	} else {
		KTROBO::mylog::writelog(filename, "KTROBO::MyLuaGlueSingleton::getInstance()->getCol%s(ci)->getInterface(cci)->%s(",input->collection_name,def->func_name);
	}
	// 引数を入れていく
	for (int i=0;i<HIKISUU_MAX;i++) {
		if ((def->hikisuu[i].type == MyDefType::NOUSE) || def->hikisuu[i].type == MyDefType::VOIDDAYO) {
			break;
		}
		if (i ==0) {
			KTROBO::mylog::writelog(filename, "%s ", def->hikisuu[i].num_name);
		} else {
			KTROBO::mylog::writelog(filename, ", %s ", def->hikisuu[i].num_name);
		}
	}
	KTROBO::mylog::writelog(filename, ");\n");
}

void MyLuaGlueMake::hanneiHIKISUU(char* filename, MakeGlueInput* input, MyFuncDef* def, int i) {

	if (def->hikisuu[i].is_out && def->hikisuu[i].is_pointer) {
		switch(def->hikisuu[i].type) {
			case MyDefType::BOOL:
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// 配列のサイズを取得
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// 配列の要素（クラスでない場合は数の配列とみなす)
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_pushboolean(L, %s[i]);\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_settable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");
		
			break;
			case MyDefType::CHAR:
					KTROBO::mylog::writelog(filename, "lua_pushstring(L, %s);\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "lua_replace(L, %d);\n", i+3);
			break;
			case MyDefType::CLASSDAYO:
//			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TTABLE);\n", -i-2);
				// 後で書く
				if (def->hikisuu[i].is_collected) {

					/*
					// 連想配列の長さを取得し0だった場合NULLを指定する
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_objlen(L,%d);\n", -i-3);
					KTROBO::mylog::writelog(filename, "if (temp ==0) {\n");
					KTROBO::mylog::writelog(filename, "  %s = NULL;\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "} else {\n");

					// 連想配列からキーインデックスを取得する
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "lua_pushstring(L,\"%s\");\n", COLLECTION_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "lua_getfield(L, %d);\n", -i-3);
					KTROBO::mylog::writelog(filename, "int %s=lua_tonumber(L, 1);\n", COLLECTION_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "lua_remove(L,1);\n");
					KTROBO::mylog::writelog(filename, "lua_pushstring(L,\"%s\");\n", COLLECTED_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "lua_getfield(L, %d);\n", -i-3);
					KTROBO::mylog::writelog(filename, "int %s=lua_tonumber(L, 1);\n", COLLECTED_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "lua_remove(L,1);\n");
					KTROBO::mylog::writelog(filename, "%s = KTROBO::MyLuaGlueSingleton::getInstance()->getCol%ss(%s)->getInstance(%s);\n", 
						def->hikisuu[i].num_name, def->hikisuu[i].kaeri6classname, COLLECTION_INDEX_LUA_FIELD, COLLECTED_INDEX_LUA_FIELD);
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");
					*/
				} else if(def->hikisuu[i].is_yaritori) {
					// YARITORIの専用関数を呼ぶ
					KTROBO::mylog::writelog(filename, "YARITORI_set%s(L, %s, %d);\n", def->hikisuu[i].kaeri6classname, def->hikisuu[i].num_name, i);
				} else {
					//KTROBO::mylog::writelog(filename, "  %s = NULL;\n", def->hikisuu[i].num_name);
				}

			break;
			case MyDefType::DOUBLE:
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// 配列のサイズを取得
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// 配列の要素（クラスでない場合は数の配列とみなす)
					KTROBO::mylog::writelog(filename, "  char tempp[%d];memset(tempp,0,%d);\n",TEMPSTRING_SIZE, TEMPSTRING_SIZE);
					KTROBO::mylog::writelog(filename, "  sprintf_s(tempp, %d, \"%s\", %s[i]);\n", TEMPSTRING_SIZE, "%f", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_pushstring(L, tempp);\n");
					KTROBO::mylog::writelog(filename, "  lua_settable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");
		

			break;
			case MyDefType::FLOAT:
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// 配列のサイズを取得
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// 配列の要素（クラスでない場合は数の配列とみなす)
					KTROBO::mylog::writelog(filename, "  char tempp[%d];memset(tempp,0,%d);\n",TEMPSTRING_SIZE, TEMPSTRING_SIZE);
					KTROBO::mylog::writelog(filename, "  sprintf_s(tempp, %d, \"%s\", %s[i]);\n", TEMPSTRING_SIZE, "%f", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_pushstring(L, tempp);\n");
					KTROBO::mylog::writelog(filename, "  lua_settable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");
		
			break;
			case MyDefType::INT:
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// 配列のサイズを取得
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// 配列の要素（クラスでない場合は数の配列とみなす)
					KTROBO::mylog::writelog(filename, "  char tempp[%d];memset(tempp,0,%d);\n",TEMPSTRING_SIZE, TEMPSTRING_SIZE);
					KTROBO::mylog::writelog(filename, "  sprintf_s(tempp, %d, \"%s\", %s[i]);\n", TEMPSTRING_SIZE, "%d", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_pushstring(L, tempp);\n");
					KTROBO::mylog::writelog(filename, "  lua_settable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");
		
	
			break;
			case MyDefType::PCHAR:
				if (def->hikisuu[i].is_pointer) {
					KTROBO::mylog::writelog(filename, "lua_pushstring(L, %s);\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "lua_replace(L, %d);\n", i+3);

				}		
			break;
			case MyDefType::PUCHAR:
				if (def->hikisuu[i].is_pointer) {
					KTROBO::mylog::writelog(filename, "lua_pushstring(L, %s);\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "lua_replace(L, %d);\n", i+3);

				}	
			break;
			case MyDefType::SHORT:

					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// 配列のサイズを取得
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// 配列の要素（クラスでない場合は数の配列とみなす)
					KTROBO::mylog::writelog(filename, "  char tempp[%d];memset(tempp,0,%d);\n",TEMPSTRING_SIZE, TEMPSTRING_SIZE);
					KTROBO::mylog::writelog(filename, "  sprintf_s(tempp, %d, \"%s\", %s[i]);\n", TEMPSTRING_SIZE, "%d", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_pushstring(L, tempp);\n");
					KTROBO::mylog::writelog(filename, "  lua_settable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");
		
			break;
			case MyDefType::UCHAR:
				if (def->hikisuu[i].is_pointer) {
					KTROBO::mylog::writelog(filename, "lua_pushstring(L, %s);\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "lua_replace(L, %d);\n", i+3);

				}	

			break;
			case MyDefType::UINT:
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// 配列のサイズを取得
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// 配列の要素（クラスでない場合は数の配列とみなす)
					KTROBO::mylog::writelog(filename, "  char tempp[%d];memset(tempp,0,%d);\n",TEMPSTRING_SIZE, TEMPSTRING_SIZE);
					KTROBO::mylog::writelog(filename, "  sprintf_s(tempp, %d, \"%s\", %s[i]);\n", TEMPSTRING_SIZE, "%d", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_pushstring(L, tempp);\n");
					KTROBO::mylog::writelog(filename, "  lua_settable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");
				
			break;
			case MyDefType::USHORT:
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// 配列のサイズを取得
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// 配列の要素（クラスでない場合は数の配列とみなす)
					KTROBO::mylog::writelog(filename, "  char tempp[%d];memset(tempp,0,%d);\n",TEMPSTRING_SIZE, TEMPSTRING_SIZE);
					KTROBO::mylog::writelog(filename, "  sprintf_s(tempp, %d, \"%s\", %s[i]);\n", TEMPSTRING_SIZE, "%f", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_pushstring(L, tempp);\n");
					KTROBO::mylog::writelog(filename, "  lua_settable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");
		
	
			break;
			case MyDefType::VOIDDAYO:
			break;
			default:
			break;
		}	
	}
}

void MyLuaGlueMake::pushKAERITI(char* filename, MakeGlueInput* input, MyFuncDef* def, int i, bool is_construct, bool is_destruct) {
	// is_construct の場合　c++側ではintとなるがlua側にはテーブルで返す
	// is_destruct の場合　c++側でint を返し　lua側にもintで返す
	if (is_construct) {
		// スタックに新しい配列を入れ込む
		KTROBO::mylog::writelog(filename, "{\n");
		KTROBO::mylog::writelog(filename, "int ci = lua_tonumber(L, 1);\n");
		KTROBO::mylog::writelog(filename, "int cci = kaeriti;\n");

		KTROBO::mylog::writelog(filename, "lua_newtable(L);\n");
		KTROBO::mylog::writelog(filename, "lua_pushnumber(L, %d);\n", DEF_LUA_FIELD_KEY); 
		KTROBO::mylog::writelog(filename, "lua_newtable(L);\n");
		KTROBO::mylog::writelog(filename, "int top = lua_gettop(L);\n");
	//	KTROBO::mylog::writelog(filename, "lua_pushstring(L, \"%s\");\n", COLLECTED_CLASS_LUA_FIELD);
		KTROBO::mylog::writelog(filename, "lua_pushstring(L, \"%s\");\n", input->collected_name);
		KTROBO::mylog::writelog(filename, "lua_setfield(L, top, \"%s\");\n", COLLECTED_CLASS_LUA_FIELD);
		KTROBO::mylog::writelog(filename, "{\n");
	//	KTROBO::mylog::writelog(filename, "lua_pushstring(L, \"%s\");\n", COLLECTED_INDEX_LUA_FIELD);
		KTROBO::mylog::writelog(filename, "char tempp[%d];memset(tempp,0,%d);\n", TEMPSTRING_SIZE, TEMPSTRING_SIZE);
		KTROBO::mylog::writelog(filename, "sprintf_s(tempp, %d, \"%s\", cci);\n", TEMPSTRING_SIZE, "%d");
		KTROBO::mylog::writelog(filename, "lua_pushstring(L, tempp);\n");
		KTROBO::mylog::writelog(filename, "lua_setfield(L, top, \"%s\");\n", COLLECTED_INDEX_LUA_FIELD);
		KTROBO::mylog::writelog(filename, "}\n");
	//	KTROBO::mylog::writelog(filename, "lua_pushstring(L, \"%s\");\n", COLLECTED_INTERFACE_LUA_FIELD);
		KTROBO::mylog::writelog(filename, "lua_pushstring(L, \"%s\");\n", input->interface_name);
		KTROBO::mylog::writelog(filename, "lua_setfield(L, top, \"%s\");\n", COLLECTED_INTERFACE_LUA_FIELD);

	//	KTROBO::mylog::writelog(filename, "lua_pushstring(L, \"%s\");\n", COLLECTION_CLASS_LUA_FIELD);
		KTROBO::mylog::writelog(filename, "lua_pushstring(L, \"%s\");\n", input->collection_name);
		KTROBO::mylog::writelog(filename, "lua_setfield(L, top, \"%s\");\n", COLLECTION_CLASS_LUA_FIELD);

		
		KTROBO::mylog::writelog(filename, "{\n");
	//	KTROBO::mylog::writelog(filename, "lua_pushstring(L, \"%s\");\n", COLLECTION_INDEX_LUA_FIELD);
		KTROBO::mylog::writelog(filename, "char tempp[%d];memset(tempp,0,%d);\n", TEMPSTRING_SIZE, TEMPSTRING_SIZE);
		KTROBO::mylog::writelog(filename, "sprintf_s(tempp, %d, \"%s\", ci);\n", TEMPSTRING_SIZE, "%d");
		KTROBO::mylog::writelog(filename, "lua_pushstring(L, tempp);\n");
		KTROBO::mylog::writelog(filename, "lua_setfield(L, top, \"%s\");\n", COLLECTION_INDEX_LUA_FIELD);
		KTROBO::mylog::writelog(filename, "}\n");

		KTROBO::mylog::writelog(filename, "lua_settable(L,-3);\n");
		KTROBO::mylog::writelog(filename, "lua_insert(L,1);\n");

		KTROBO::mylog::writelog(filename, "}\n");
		return;
	}
	
	if (is_destruct) {
//		KTROBO::mylog::writelog(filename, "{char tempp[%d];memset(tempp,0,%d);\n", TEMPSTRING_SIZE, TEMPSTRING_SIZE);
//		KTROBO::mylog::writelog(filename, "sprintf_s(tempp, %d, \"%s\",%s);\n",TEMPSTRING_SIZE, "%d", "kaeriti");
		KTROBO::mylog::writelog(filename, "lua_pushnumber(L, kaeriti);\n");
		return;
	}
	// func の場合　
	// typeで処理が分かれる


	switch(def->kaeriti.type) {

			case MyDefType::BOOL:
			//KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TBOOLEAN);\n", -i-3);
				if (def->kaeriti.is_pointer) {
					KTROBO::mylog::writelog(filename, "lua_pushboolean(L,%s);\n", "*kaeriti");
				} else {
					KTROBO::mylog::writelog(filename, "lua_pushboolean(L,%s);\n", "kaeriti");
				}
			break;
			case MyDefType::CHAR:
			//KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", -i-3);
				if (def->kaeriti.is_pointer) {
					KTROBO::mylog::writelog(filename, "lua_pushstring(L,%s);\n", "kaeriti");
				} else {
					KTROBO::mylog::writelog(filename, "lua_pushstring(L,%s);\n", "&kaeriti");
				}
			break;
			case MyDefType::CLASSDAYO:
				if (def->kaeriti.is_yaritori) {
					KTROBO::mylog::writelog(filename, "YARITORI_setKAERITI%s(L, kaeriti);\n", def->kaeriti.kaeri6classname);
				} else if(def->kaeriti.is_collected) {
					// 何もしない

				}

			break;
			case MyDefType::DOUBLE:
//			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", -i-3);
				
				if (def->kaeriti.is_pointer) {
				KTROBO::mylog::writelog(filename, "{char tempp[%d];memset(tempp,0,%d);\n", TEMPSTRING_SIZE, TEMPSTRING_SIZE);
				KTROBO::mylog::writelog(filename, "sprintf_s(tempp, %d, \"%s\",%s);\n", TEMPSTRING_SIZE,  "%f","*kaeriti");
				KTROBO::mylog::writelog(filename, "lua_pushstring(L, tempp);\n}");
				} else {
				KTROBO::mylog::writelog(filename, "{char tempp[%d];memset(tempp,0,%d);\n", TEMPSTRING_SIZE, TEMPSTRING_SIZE);
				KTROBO::mylog::writelog(filename, "sprintf_s(tempp, %d, \"%s\",%s);\n", TEMPSTRING_SIZE, "%f", "kaeriti");
				KTROBO::mylog::writelog(filename, "lua_pushstring(L, tempp);\n}");
				}
			break;
			case MyDefType::FLOAT:
				if (def->kaeriti.is_pointer) {
				KTROBO::mylog::writelog(filename, "{char tempp[%d];memset(tempp,0,%d);\n", TEMPSTRING_SIZE, TEMPSTRING_SIZE);
				KTROBO::mylog::writelog(filename, "sprintf_s(tempp, %d, \"%s\",%s);\n", TEMPSTRING_SIZE, "%f",  "*kaeriti");
				KTROBO::mylog::writelog(filename, "lua_pushstring(L, tempp);\n}");
				} else {
				KTROBO::mylog::writelog(filename, "{char tempp[%d];memset(tempp,0,%d);\n", TEMPSTRING_SIZE, TEMPSTRING_SIZE);
				KTROBO::mylog::writelog(filename, "sprintf_s(tempp, %d, \"%s\",%s);\n", TEMPSTRING_SIZE, "%f", "kaeriti");
				KTROBO::mylog::writelog(filename, "lua_pushstring(L, tempp);\n}");
				}
			break;
			case MyDefType::INT:
				if (def->kaeriti.is_pointer) {
//				KTROBO::mylog::writelog(filename, "{char tempp[%d];memset(tempp,0,%d);\n", TEMPSTRING_SIZE, TEMPSTRING_SIZE);
//				KTROBO::mylog::writelog(filename, "sprintf_s(tempp, %d, \"%s\",%s);\n",TEMPSTRING_SIZE, "%d", "*kaeriti");
//				KTROBO::mylog::writelog(filename, "lua_pushstring(L, tempp);\n}");
					KTROBO::mylog::writelog(filename, "lua_pushnumber(L, *kaeriti);\n");
				} else {
//				KTROBO::mylog::writelog(filename, "{char tempp[%d];memset(tempp,0,%d);\n", TEMPSTRING_SIZE, TEMPSTRING_SIZE);
//				KTROBO::mylog::writelog(filename, "sprintf_s(tempp, %d, \"%s\",%s);\n", TEMPSTRING_SIZE, "%d",  "kaeriti");
//				KTROBO::mylog::writelog(filename, "lua_pushstring(L, tempp);\n}");
					KTROBO::mylog::writelog(filename, "lua_pushnumber(L, kaeriti);\n");
				}
			break;
			case MyDefType::PCHAR:
				if (def->kaeriti.is_pointer) {
					KTROBO::mylog::writelog(filename, "lua_pushstring(L,%s);\n", "kaeriti");
				} else {
					KTROBO::mylog::writelog(filename, "lua_pushstring(L,%s);\n", "kaeriti");
				}
			
			break;
			case MyDefType::PUCHAR:
				if (def->kaeriti.is_pointer) {
					KTROBO::mylog::writelog(filename, "lua_pushstring(L,%s);\n", "kaeriti");
				} else {
					KTROBO::mylog::writelog(filename, "lua_pushstring(L,%s);\n", "kaeriti");
				}
			
			break;
			case MyDefType::SHORT:
				if (def->kaeriti.is_pointer) {
					KTROBO::mylog::writelog(filename, "lua_pushnumber(L, *kaeriti);\n");
				} else {
					KTROBO::mylog::writelog(filename, "lua_pushnumber(L, kaeriti);\n");
				}
		
			break;
			case MyDefType::UCHAR:
				if (def->kaeriti.is_pointer) {
					KTROBO::mylog::writelog(filename, "lua_pushstring(L,%s);\n", "(char*)kaeriti");
				} else {
					KTROBO::mylog::writelog(filename, "lua_pushstring(L,%s);\n", "(char*)&kaeriti");
				}
			break;
			case MyDefType::UINT:
				if (def->kaeriti.is_pointer) {
					KTROBO::mylog::writelog(filename, "lua_pushnumber(L, *kaeriti);\n");
				} else {
					KTROBO::mylog::writelog(filename, "lua_pushnumber(L, kaeriti);\n");
				}
		
			break;
			case MyDefType::USHORT:
				if (def->kaeriti.is_pointer) {
					KTROBO::mylog::writelog(filename, "lua_pushnumber(L, *kaeriti);\n");
				} else {
					KTROBO::mylog::writelog(filename, "lua_pushnumber(L, kaeriti);\n");
				}
		
			break;
			case MyDefType::VOIDDAYO:
			return;
			default:
			return;
			
	}
	KTROBO::mylog::writelog(filename, "lua_insert(L,1);\n");

}

void MyLuaGlueMake::_writeFunc(char* filename, MakeGlueInput* input, MyFuncDef* def, bool is_construct, bool is_destruct) {

	KTROBO::mylog::writelog(filename, "int KTROBO::%s_%sGlue(lua_State* L) {\n", input->collection_name, def->func_name);
	int kaeriti_size = 0;
	for (int i=0;i<HIKISUU_MAX;i++) {
		if (def->hikisuu[i].type != MyDefType::NOUSE) {
			kaeriti_size++;
		} else {
			break;
		}
	}

	int hikisuu_size = kaeriti_size;

	// コレクションクラスのインデックスを指定する必要があるのでこれも加える
	kaeriti_size++;kaeriti_size++; // コンストラクタデストラクタの場合も仮にcollected_indexをLua側から渡しておく

	if ((def->kaeriti.type != MyDefType::VOIDDAYO) && (def->kaeriti.type != MyDefType::NOUSE)) {
		kaeriti_size++;
	}

	// 各引数値のｃ＋＋側の用意を行う
	for (int i=0;i<hikisuu_size;i++) {
		this->checkType(filename, input, def, i);
	}
	for (int i =0; i< hikisuu_size;i++) {
		this->sengen(filename, input, def, i);
		this->getHIKISUU(filename, input, def, i);
	}

	// 関数呼び出し
	kannsuuYobidasi(filename, input, def, hikisuu_size,is_construct, is_destruct );


	// 各引数がoutの場合に反映させる
	for (int i=0;i< hikisuu_size;i++) {
		this->hanneiHIKISUU(filename, input, def, i);
	
	}


	// 返り値をプッシュする
	pushKAERITI(filename, input, def, hikisuu_size, is_construct, is_destruct);

	// リターンする
	KTROBO::mylog::writelog(filename, "return %d;\n", kaeriti_size); 
	KTROBO::mylog::writelog(filename, "}\n");
}



void MyLuaGlueMake::_writeFuncSengen(char* filename, MakeGlueInput* input, MyFuncDef* def, bool is_construct, bool is_destruct) {

	KTROBO::mylog::writelog(filename, "int %s_%sGlue(lua_State* L);\n", input->collection_name, def->func_name);
	
	
	
/*	
	
	int kaeriti_size = 0;
	for (int i=0;i<HIKISUU_MAX;i++) {
		if (def->hikisuu[i].type != MyDefType::NOUSE) {
			kaeriti_size++;
		} else {
			break;
		}
	}

	int hikisuu_size = kaeriti_size;

	// コレクションクラスのインデックスを指定する必要があるのでこれも加える
	kaeriti_size++;kaeriti_size++; // コンストラクタデストラクタの場合も仮にcollected_indexをLua側から渡しておく

	if ((def->kaeriti.type != MyDefType::VOIDDAYO) && (def->kaeriti.type != MyDefType::NOUSE)) {
		kaeriti_size++;
	}

	// 各引数値のｃ＋＋側の用意を行う
	for (int i=0;i<hikisuu_size;i++) {
		this->checkType(filename, input, def, i);
	}
	for (int i =0; i< hikisuu_size;i++) {
		this->sengen(filename, input, def, i);
		this->getHIKISUU(filename, input, def, i);
	}

	// 関数呼び出し
	kannsuuYobidasi(filename, input, def, hikisuu_size,is_construct, is_destruct );


	// 各引数がoutの場合に反映させる
	for (int i=0;i< hikisuu_size;i++) {
		this->hanneiHIKISUU(filename, input, def, i);
	
	}


	// 返り値をプッシュする
	pushKAERITI(filename, input, def, hikisuu_size, is_construct, is_destruct);

	// リターンする
	KTROBO::mylog::writelog(filename, "return %d;\n", kaeriti_size); 
	KTROBO::mylog::writelog(filename, "}\n");

	*/
}


void MyLuaGlueMake::_writeFunc2(char* filename, MakeGlueInput* input) {

// 使わない！
/*
	KTROBO::mylog::writelog(filename, "int KTROBO::%s_getInstanceGlue(lua_State* L) {\n", input->collection_name);
	int kaeriti_size = 0;
	int hikisuu_size = 0;

	// コレクションクラスのインデックスを指定する必要があるのでこれも加える
	kaeriti_size++;kaeriti_size++; // コンストラクタデストラクタの場合も仮にcollected_indexをLua側から渡しておく
	kaeriti_size++;// コレクションクラスを返すのでそのため
	
	// 変数の宣言
	KTROBO::mylog::writelog(filename, "  int collection_index = lua_tonumber(L,1);\n");
	KTROBO::mylog::writelog(filename, "  int collected_index = lua_tonumber(L,2);\n");


	// 関数呼び出し
	KTROBO::mylog::writelog(filename, "  %s* kaeri = KTROBO::MyLuaGlueSingleton::getInstance()->getCol%s(collection_index)->getInstance(collected_index);\m", input->collected_name,input->collection_name);


	// 返り値をプッシュする

	// リターンする
	KTROBO::mylog::writelog(filename, "return %d;\n", kaeriti_size); 
	KTROBO::mylog::writelog(filename, "}\n");
*/
}

void MyLuaGlueMake::writeFunc(char *filename) {
	//KTROBO::mylog::writelog(filename, " public:\n");
	vector<MakeGlueInput*>::iterator it = inputs.begin();
	while (it != inputs.end()) {
		MakeGlueInput* input = *it;
		//collectionクラスのcollectされるクラスのコンストラクタ(ファクトリー？)
		int csize = input->construct_defs.size();
		for (int i=0;i<csize;i++) {
			MyFuncDef* def = input->construct_defs[i];
			_writeFunc(filename, input, def, true, false);
		}
		//デストラクタメソッドもしあれば・・
		int dsize = input->destruct_defs.size();
		for (int i=0;i<dsize;i++) {
			MyFuncDef* def = input->destruct_defs[i];
			_writeFunc(filename, input, def, false, true);
		}

		// collectされるクラスのインターフェースの各メソッド
		int func_size = input->func_defs.size();
		for (int i=0;i<func_size; i++) {
			MyFuncDef* def = input->func_defs[i];

			_writeFunc(filename, input, def, false, false);
		}

		// collection クラスのcollectされているクラスのアクセスメソッド
		_writeFunc2(filename, input);
	
		it = it + 1;
	}
}
void MyLuaGlueMake::_writeFuncSengen2(char* filename, MakeGlueInput* input) {


	//KTROBO::mylog::writelog(filename, "int %s_getInstanceGlue(lua_State* L);\n", input->collection_name);
}


void MyLuaGlueMake::writeFuncSengen(char *filename) {
	//KTROBO::mylog::writelog(filename, " public:\n");
	vector<MakeGlueInput*>::iterator it = inputs.begin();
	while (it != inputs.end()) {
		MakeGlueInput* input = *it;
		//collectionクラスのcollectされるクラスのコンストラクタ(ファクトリー？)
		int csize = input->construct_defs.size();
		for (int i=0;i<csize;i++) {
			MyFuncDef* def = input->construct_defs[i];
			_writeFuncSengen(filename, input, def, true, false);
		}
		//デストラクタメソッドもしあれば・・
		int dsize = input->destruct_defs.size();
		for (int i=0;i<dsize;i++) {
			MyFuncDef* def = input->destruct_defs[i];
			_writeFuncSengen(filename, input, def, false, true);
		}

		// collectされるクラスのインターフェースの各メソッド
		int func_size = input->func_defs.size();
		for (int i=0;i<func_size; i++) {
			MyFuncDef* def = input->func_defs[i];

			_writeFuncSengen(filename, input, def, false, false);
		}

		// collection クラスのcollectされるクラスのアクセスメソッド
		_writeFuncSengen2(filename, input);
	
		it = it + 1;
	}
}

void MyLuaGlueMake::writeYARITORIFunc(char* filename, char* prefix) {

	// is_yaritori がtrueでclassの名前を集める
	set<string> class_namesdayo;
	int isize = inputs.size();

	for (int i=0;i<isize;i++) {
		MakeGlueInput* input = inputs[i];
		int csize = input->construct_defs.size();
		for (int k=0;k<csize;k++) {
			MyFuncDef* def = input->construct_defs[k];
			// 返り値
			if ((def->kaeriti.type == MyDefType::CLASSDAYO) && def->kaeriti.is_yaritori) {
				class_namesdayo.insert(string(def->kaeriti.kaeri6classname));
			}
			// 引数
			for (int l=0;l < HIKISUU_MAX;l++) {
				MyFuncNUMWITHTYPE* n = &def->hikisuu[l];
				if (n->is_yaritori && n->type == MyDefType::CLASSDAYO) {
					class_namesdayo.insert(string(n->kaeri6classname));
				}
			}
		}

		int dsize = input->destruct_defs.size();
		for (int k=0;k<dsize;k++) {
			MyFuncDef* def = input->destruct_defs[k];
			// 返り値
			if ((def->kaeriti.type == MyDefType::CLASSDAYO) && def->kaeriti.is_yaritori) {
				class_namesdayo.insert(string(def->kaeriti.kaeri6classname));
			}
			// 引数
			for (int l=0;l < HIKISUU_MAX;l++) {
				MyFuncNUMWITHTYPE* n = &def->hikisuu[l];
				if (n->is_yaritori && n->type == MyDefType::CLASSDAYO) {
					class_namesdayo.insert(string(n->kaeri6classname));
				}
			}
		}

		int fsize = input->func_defs.size();
		for (int k=0;k<fsize;k++) {
			MyFuncDef* def = input->func_defs[k];
			// 返り値
			if ((def->kaeriti.type == MyDefType::CLASSDAYO) && def->kaeriti.is_yaritori) {
				class_namesdayo.insert(string(def->kaeriti.kaeri6classname));
			}
			// 引数
			for (int l=0;l < HIKISUU_MAX;l++) {
				MyFuncNUMWITHTYPE* n = &def->hikisuu[l];
				if (n->is_yaritori && n->type == MyDefType::CLASSDAYO) {
					class_namesdayo.insert(string(n->kaeri6classname));
				}
			}
		}



	}


	// class_namesdayo に集まったので

	set<string>::iterator it = class_namesdayo.begin();

	KTROBO::mylog::writelog(filename, "/////  please write this functions!///////\n\n\n");

	while (it != class_namesdayo.end()) {
		string s = *it;
		KTROBO::mylog::writelog(filename, "%s void YARITORI_get%s(lua_State* L, %s* class_inst, int index);\n",prefix, s.c_str(), s.c_str());
		KTROBO::mylog::writelog(filename, "%s void YARITORI_set%s(lua_State* L, %s* class_inst, int index);\n",prefix, s.c_str(), s.c_str());
		KTROBO::mylog::writelog(filename, "%s void YARITORI_setKAERITI%s(lua_State* L, %s* class_inst);\n", prefix,s.c_str(), s.c_str());
		it++;
	}

	KTROBO::mylog::writelog(filename, "\n\n\n/////////////////////////////////////////////\n");

}



void MyLuaGlueMake::writeSingleton(char* filename) {

	KTROBO::mylog::writelog(filename, "class MyLuaGlueSingleton{\n");
	KTROBO::mylog::writelog(filename, " private:\n");
	KTROBO::mylog::writelog(filename, "  MyLuaGlueSingleton() {};\n");
	KTROBO::mylog::writelog(filename, "  MyLuaGlueSingleton(const MyLuaGlueSingleton& g) {};\n");
	KTROBO::mylog::writelog(filename, "  MyLuaGlueSingleton& operator = (const MyLuaGlueSingleton& o) {};\n");
	KTROBO::mylog::writelog(filename, " public:\n");
	KTROBO::mylog::writelog(filename, "  static MyLuaGlueSingleton* getInstance() {\n");
	KTROBO::mylog::writelog(filename, "    static MyLuaGlueSingleton instance;\n");
	KTROBO::mylog::writelog(filename, "    return &instance;\n");
	KTROBO::mylog::writelog(filename, "  };\n");
	writeSingletonMember(filename);
	writeSingletonFunc(filename);
	writeRegister(filename);
	KTROBO::mylog::writelog(filename, "};\n");
}

int MyLuaGlueMake::outputToFile(char* filename, char* header_name, char* cpppath, char* cppname, char* user_write_h_path) {
	FILE * fp;
	fopen_s(&fp,filename,"w");// いったんクリアする
	if (fp == NULL) {
		KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "failed to clear output file");
		return 0;
	}
	fclose(fp);

	fopen_s(&fp,cpppath,"w");// いったんクリアする
	if (fp == NULL) {
		KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "failed to clear output file");
		return 0;
	}
	fclose(fp);

	fopen_s(&fp,user_write_h_path,"w");// いったんクリアする
	if (fp == NULL) {
		KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "failed to clear output file");
		return 0;
	}
	fclose(fp);




	KTROBO::mylog::writelog(filename, "#ifndef TOLUAGLUEFILE_H\n");
	KTROBO::mylog::writelog(filename, "#define TOLUAGLUEFILE_H\n");
	
	// include ファイルを書き込む
	writeInclude(filename);
	
	KTROBO::mylog::writelog(filename, "namespace KTROBO {\n");
	
	writeYARITORIFunc(filename, "");

	
	// singleton クラスの内容を書き込む
	writeFuncSengen(filename);
	writeSingleton(filename);
	
	
	KTROBO::mylog::writelog(filename, "}\n");
	KTROBO::mylog::writelog(filename, "#endif\n");


	KTROBO::mylog::writelog(cpppath, "#include \"%s\"\n", header_name);
	KTROBO::mylog::writelog(cpppath, "using namespace KTROBO;\n");
	writeFunc(cpppath);


	KTROBO::mylog::writelog(user_write_h_path, "#ifndef USERWRITETOLUA_H\n");
	KTROBO::mylog::writelog(user_write_h_path, "#define USERWRITETOLUA_H\n");

	// include ファイルを書き込む
	writeInclude(user_write_h_path);
	
	KTROBO::mylog::writelog(user_write_h_path, "namespace KTROBO {\n");
	writeYARITORIFunc(user_write_h_path, "extern");
	KTROBO::mylog::writelog(user_write_h_path, "}\n");
	KTROBO::mylog::writelog(user_write_h_path, "#endif\n");

	return 1;
}




int main(char* argc, char** argv) {
	MyLuaGlueMake* m = new MyLuaGlueMake();
	m->readInputAndOutput();
	delete m;
	return 0;
}




int MyLuaGlueMake::outputLua(char* filename) {
	FILE * fp;
	fopen_s(&fp,filename,"w");// いったんクリアする
	if (fp == NULL) {
		KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "failed to clear output file");
		return 0;
	}
	fclose(fp);
	
	// luaファイルを書いていく
	// グルー関数をラップするクラスの定義を書く
	// global my_interfaces; にクラスの定義を
	// global my_instances; にインスタンスの参照を入れる

	// スレッドごとに異なるlua_Stateを採用するようにする
	// スレッド間のデータ(オブジェクト)連携はどうするのか
	// たとえばロード処理スレッドでオブジェクトが生成または破棄されたときに
	// 更新処理スレッドでどうやってそのオブジェクトを追加または削除するか
	// その１　luaファイルが呼び出されるごとに全オブジェクトをロードするようにする
	// その２　生成破棄処理で生成破棄されたオブジェクトをｃ＋＋側またはファイルに記録しておき　
	//         その記録をよびだされたluaファイルが読み込んで該当の処理を行う
	// その３　更新スレッドで生成破棄スレッドに生成破棄の支持をだし、生成破棄スレッドでは完了の合図だけ送る
	//         更新スレッドでは　破棄時は指示時から使わず、生成時は完了の合図を確認したときのみからそのオブジェクトの更新を開始する

	// わけているようだが結局のところ何がロードされるかの情報をどこで持つかという話になる?
	// 生成破棄処理スレッドで呼ばれるluaファイル自体に持たせるのか,それとも外部からよみこんでそれをロードする形にするのか
	// luaファイルに決めうちでロードさせるのもよいけどその場合どうやって更新処理にもっていくかの話が出てくる
	// MeshInstanced に関して考えてみると 
	// 決まったモデルがあらかじめ分かっていて固定の量のロードをして大丈夫であれば実装が楽だが
	// いつ新たに生成破棄が行われる必要が出てくるかわからないということになると様相が変わってくる
	// 不確定な生成破棄が行われる場合のMeshInstancedがある場合どう対応するか
	
	// 生成処理
	// 1. 今までのMeshInstancedsのコピーをとる
	// 2. そのコピーをロード状態に設定する
	// 3. コピーに新たに加わったMeshInstancedを加える(ロードする）
	// 4. そのコピーのロード状態を解除し、今までのMeshInstancedsと入れ替える!(どうやって実現するかは考えどころ)

	// 破棄処理
	// 1. フラグを立ててそのインスタンスをどのスレッドも利用できないような状態または仕組みを作る
	// 2. フラグがたっているインスタンスのデータのみ（インスタンスの参照自体は消さない）破棄する
	//    (生成破棄スレッドでc++側のGCメソッドとよべばいいのかをよぶ仕組みを作る)
	// 3. これでOK　現状のMeshInstancedsではMeshInstancedの参照を保存しているベクトル構造体？から破棄するとインデックスが狂うので
	// この仕組みで大丈夫なはず！
     
	int isize = inputs.size();
	for (int i=0;i<isize;i++) {
		MakeGlueInput* input = inputs[i];
		KTROBO::mylog::writelog(filename, "%s = %s or { count =0 }\n", input->collected_name, input->collected_name);
		int csize = input->construct_defs.size();
		for (int k=0;k<csize;k++) {
			MyFuncDef* def = input->construct_defs[k];
			KTROBO::mylog::writelog(filename, "function %s:%s(collection_index, ...)\n", input->collected_name, def->func_name);
			KTROBO::mylog::writelog(filename, "  local t = %s_%s(collection_index,0, ...)\n", input->collection_name, def->func_name);
			KTROBO::mylog::writelog(filename, "  %s.count = %s.count+1\n", input->collected_name, input->collected_name);
			KTROBO::mylog::writelog(filename, "  setmetatable(t, {__index = %s})\n", input->collected_name);
			KTROBO::mylog::writelog(filename, "  return t\n");
			KTROBO::mylog::writelog(filename, "end\n");
		}

		// 偽装オブジェクトを返すメソッド
		KTROBO::mylog::writelog(filename, "function %s:getIS(c_index, cd_index)\n", input->collected_name);
		KTROBO::mylog::writelog(filename, "  local t ={}\n");
		KTROBO::mylog::writelog(filename, "  t[1] = {}\n");
		KTROBO::mylog::writelog(filename, "  t[1].%s = c_index\n", COLLECTION_INDEX_LUA_FIELD);
		KTROBO::mylog::writelog(filename, "  t[1].%s = cd_index\n", COLLECTED_INDEX_LUA_FIELD);
		KTROBO::mylog::writelog(filename, "  t[1].%s = \"%s\"\n", COLLECTED_CLASS_LUA_FIELD, input->collected_name);
		KTROBO::mylog::writelog(filename, "  t[1].%s = \"%s\"\n", COLLECTION_CLASS_LUA_FIELD, input->collection_name);
		KTROBO::mylog::writelog(filename, "  t[1].%s = \"%s\"\n", COLLECTED_INTERFACE_LUA_FIELD, input->interface_name);
		KTROBO::mylog::writelog(filename, "  setmetatable(t, {__index= %s})\n", input->collected_name);
		KTROBO::mylog::writelog(filename, "  return t\n");
		KTROBO::mylog::writelog(filename, "end\n");


		int dsize = input->destruct_defs.size();
		for (int k=0;k<dsize;k++) {
			MyFuncDef* def = input->destruct_defs[k];
			KTROBO::mylog::writelog(filename, "function %s:%s(...)\n", input->collected_name, def->func_name);
			KTROBO::mylog::writelog(filename, "  c = %s_%s(self[1].%s,self[1].%s , ...)\n", input->collection_name, def->func_name, COLLECTION_INDEX_LUA_FIELD, COLLECTED_INDEX_LUA_FIELD);
			KTROBO::mylog::writelog(filename, "  %s.count = %s.count-c\n", input->collected_name, input->collected_name);
			KTROBO::mylog::writelog(filename, "end\n");
		}


		int fsize = input->func_defs.size();
		for (int k=0;k<fsize;k++) {
			MyFuncDef* def = input->func_defs[k];
			KTROBO::mylog::writelog(filename, "function %s:%s(...)\n", input->collected_name, def->func_name);
			KTROBO::mylog::writelog(filename, " return %s_%s(self[1].%s,self[1].%s , ...)\n", input->collection_name, def->func_name, COLLECTION_INDEX_LUA_FIELD, COLLECTED_INDEX_LUA_FIELD);
			KTROBO::mylog::writelog(filename, "end\n");
		}
	
	}

	return 1;
	
};