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
		// { ��T��

		a.GetToken();
		if (strcmp(a.Toke(), "{")==0) {
			// �ݒ�̎n�܂�
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

	// �Ԃ�l
	MyFuncDef* def = new MyFuncDef();
	memset(def, 0, sizeof(MyFuncDef));
	setKAERITI(a, def);
	setNAME(a, def);
	a->GetToken("(");
	int hikisuu_count = 0;
	while(((strcmp(a->Toke(), ")") != 0) && !a->enddayo()) && (hikisuu_count < HIKISUU_MAX)) {
		a->GetToken();// �^��


		if (!setHIKISUU(a, def, hikisuu_count)) {
			KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "error in hikisuu");
		}
		hikisuu_count++;
	}


	a->GetToken();
	if (strcmp(a->Toke(), "0")==0) {
		a->GetToken(); // �����virtual �̕��������肩��ĊJ�ł���
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
	// name �͓����Ă���
	//a->GetToken();
	a->GetToken("(");
	int hikisuu_count = 0;
	while((strcmp(a->Toke(), ")") != 0 && !a->enddayo())&& (hikisuu_count < HIKISUU_MAX)) {
		a->GetToken();// �^��
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
	// name �͓����Ă���
	a->GetToken();
	a->GetToken("(");
	int hikisuu_count = 0;
	while((strcmp(a->Toke(), ")") != 0 && !a->enddayo()) && (hikisuu_count < HIKISUU_MAX)) {
		a->GetToken();// �^��
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

	// �C���^�[�t�F�[�X�����擾���Ċ֐��̏�����ꍞ��
	while(!a.enddayo()) {
		a.GetToken();
		if (strcmp(a.Toke(), "interface")==0) {
			a.GetToken();
			if (strcmp(a.Toke(), input->interface_name)==0) {
				// �C���^�[�t�F�[�X���܂ł����̂Ŋ֐��̏����擾����
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
				
				
				
	// �R���N�V���������擾���ăR���X�g���N�^�A�f�X�g���N�^�̏�����ꍞ��
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
						// �R���N�V�����̊Y���C���^�[�t�F�[�X�̃R���X�g���N�^�̋A��l�̓C���X�^���XID�iint)�ƌ��ߓ����ŏ�������
						setConstructFunc(&a,input);
					}
				}
			}
		}
	}

	// ���x�̓f�X�g���N�^
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
						// �R���N�V�����̊Y���C���^�[�t�F�[�X�̃f�X�g���N�^�̋A��l��int�ƌ��ߓ�������
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
		// �R���N�V�����N���X�̗v�f�ɑ΂���A�Z�N�T�T���\�b�h
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

	// register����register�֐�
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

		strcpy_s(ans,1024, str);// char�̏ꍇ1024�m�ۂ���

	} else if(num->type == MyDefType::CLASSDAYO) {
		if (num->is_pointer) {
			sprintf_s(ans, 1024,"%s*", num->kaeri6classname);// �N���X�̃|�C���^�̏ꍇ�ЂƂ����ƍl���ď������s��
			return;
		} else {
			sprintf_s(ans,1024, "%s", num->kaeri6classname);
			return;
		}
	} else if(num->is_pointer) {
		sprintf_s(ans,1024, "%s", str);// �N���X�ȊO�̏ꍇ�̃|�C���^�͔z��ƍl���Ċm�ۂ���@32
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
		// �R���N�V������index�ƃN���X��index�̓�ɂȂ� 
		sprintf_s(ans,1024,"int %ss_index, int %s_index", this->hikisuu[index].kaeri6classname, hikisuu[index].kaeri6classname);
		return;
	} else if(this->hikisuu[index].is_yaritori) {
		// �����X�g���N�g�̃C���f�b�N�X�ɂȂ�
		sprintf_s(ans,1024,"int %s_index", this->hikisuu[index].num_name);
	}
	
	char kata[1024];
	memset(kata,0,1024);
	this->getKATAStr(&hikisuu[index],kata);
	
	sprintf_s(ans,1024, "%s %s", kata, hikisuu[index].num_name);
}


void MyLuaGlueMake::checkType(char *filename, MakeGlueInput* input, MyFuncDef* def, int i) {
	// -i-3�Ȃ̂́@-1��collection_�̃C���f�b�N�X�����邽�� -2 ��collected�̃C���f�b�N�X������
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
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// �z��̃T�C�Y���擾
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// �z��̗v�f�i�N���X�łȂ��ꍇ�͐��̔z��Ƃ݂Ȃ�)
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
				// ��ŏ���
				if (def->hikisuu[i].is_collected) {
					// �A�z�z��̒������擾��0�������ꍇNULL���w�肷��
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);
					KTROBO::mylog::writelog(filename, "if (temp ==0) {\n");
					KTROBO::mylog::writelog(filename, "  %s = NULL;\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "} else {\n");

					// �A�z�z�񂩂�L�[�C���f�b�N�X���擾����
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
					// YARITORI�̐�p�֐����Ă�
					KTROBO::mylog::writelog(filename, "YARITORI_get%s(L, %s, %d);\n", def->hikisuu[i].kaeri6classname, def->hikisuu[i].num_name, i);
				} else {
						KTROBO::mylog::writelog(filename, "  %s = NULL;\n", def->hikisuu[i].num_name);
				}

			break;
			case MyDefType::DOUBLE:
//			KTROBO::mylog::writelog(filename, "luaL_checktype(L,%d, LUA_TSTRING);\n", -i-2);
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// �z��̃T�C�Y���擾
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// �z��̗v�f�i�N���X�łȂ��ꍇ�͐��̔z��Ƃ݂Ȃ�)
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_gettable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "  %s[i] = (double)atof(lua_tostring(L, -1));\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_remove(L,-1);\n");
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");

			break;
			case MyDefType::FLOAT:
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// �z��̃T�C�Y���擾
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// �z��̗v�f�i�N���X�łȂ��ꍇ�͐��̔z��Ƃ݂Ȃ�)
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_gettable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "  %s[i] = atof(lua_tostring(L, -1));\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_remove(L,-1);\n");
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");

			break;
			case MyDefType::INT:
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// �z��̃T�C�Y���擾
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// �z��̗v�f�i�N���X�łȂ��ꍇ�͐��̔z��Ƃ݂Ȃ�)
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
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// �z��̃T�C�Y���擾
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// �z��̗v�f�i�N���X�łȂ��ꍇ�͐��̔z��Ƃ݂Ȃ�)
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
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// �z��̃T�C�Y���擾
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// �z��̗v�f�i�N���X�łȂ��ꍇ�͐��̔z��Ƃ݂Ȃ�)
					KTROBO::mylog::writelog(filename, "  lua_pushnumber(L, i+1);\n");
					KTROBO::mylog::writelog(filename, "  lua_gettable(L, %d);\n", i+3);
					KTROBO::mylog::writelog(filename, "  %s[i] = (unsigned int)(lua_tonumber(L, -1));\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "  lua_remove(L,-1);\n");
					KTROBO::mylog::writelog(filename, "}\n");
					KTROBO::mylog::writelog(filename, "}\n");

			break;
			case MyDefType::USHORT:
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// �z��̃T�C�Y���擾
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// �z��̗v�f�i�N���X�łȂ��ꍇ�͐��̔z��Ƃ݂Ȃ�)
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
				// ��ŏ���
				if (def->hikisuu[i].is_collected) {


					// �A�z�z��̒������擾��0�������ꍇNULL���w�肷��
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);
					KTROBO::mylog::writelog(filename, "if (temp ==0) {\n");
					KTROBO::mylog::writelog(filename, "  %s = NULL;\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "} else {\n");

					// �A�z�z�񂩂�L�[�C���f�b�N�X���擾����
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
					// YARITORI�̐�p�֐����Ă�
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
	// kaeriti�@�ɕۑ�����
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
	// ���������Ă���
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
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// �z��̃T�C�Y���擾
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// �z��̗v�f�i�N���X�łȂ��ꍇ�͐��̔z��Ƃ݂Ȃ�)
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
				// ��ŏ���
				if (def->hikisuu[i].is_collected) {

					/*
					// �A�z�z��̒������擾��0�������ꍇNULL���w�肷��
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_objlen(L,%d);\n", -i-3);
					KTROBO::mylog::writelog(filename, "if (temp ==0) {\n");
					KTROBO::mylog::writelog(filename, "  %s = NULL;\n", def->hikisuu[i].num_name);
					KTROBO::mylog::writelog(filename, "} else {\n");

					// �A�z�z�񂩂�L�[�C���f�b�N�X���擾����
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
					// YARITORI�̐�p�֐����Ă�
					KTROBO::mylog::writelog(filename, "YARITORI_set%s(L, %s, %d);\n", def->hikisuu[i].kaeri6classname, def->hikisuu[i].num_name, i);
				} else {
					//KTROBO::mylog::writelog(filename, "  %s = NULL;\n", def->hikisuu[i].num_name);
				}

			break;
			case MyDefType::DOUBLE:
					KTROBO::mylog::writelog(filename, "{\n");
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// �z��̃T�C�Y���擾
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// �z��̗v�f�i�N���X�łȂ��ꍇ�͐��̔z��Ƃ݂Ȃ�)
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
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// �z��̃T�C�Y���擾
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// �z��̗v�f�i�N���X�łȂ��ꍇ�͐��̔z��Ƃ݂Ȃ�)
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
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// �z��̃T�C�Y���擾
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// �z��̗v�f�i�N���X�łȂ��ꍇ�͐��̔z��Ƃ݂Ȃ�)
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
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// �z��̃T�C�Y���擾
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// �z��̗v�f�i�N���X�łȂ��ꍇ�͐��̔z��Ƃ݂Ȃ�)
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
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// �z��̃T�C�Y���擾
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// �z��̗v�f�i�N���X�łȂ��ꍇ�͐��̔z��Ƃ݂Ȃ�)
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
					KTROBO::mylog::writelog(filename, "int temp = lua_rawlen(L,%d);\n", i+3);// �z��̃T�C�Y���擾
					KTROBO::mylog::writelog(filename, "for (int i=0;i<temp && i < %d;i++) {\n", TEMPHAIRETU_SIZE);
					// �z��̗v�f�i�N���X�łȂ��ꍇ�͐��̔z��Ƃ݂Ȃ�)
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
	// is_construct �̏ꍇ�@c++���ł�int�ƂȂ邪lua���ɂ̓e�[�u���ŕԂ�
	// is_destruct �̏ꍇ�@c++����int ��Ԃ��@lua���ɂ�int�ŕԂ�
	if (is_construct) {
		// �X�^�b�N�ɐV�����z�����ꍞ��
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
	// func �̏ꍇ�@
	// type�ŏ������������


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
					// �������Ȃ�

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

	// �R���N�V�����N���X�̃C���f�b�N�X���w�肷��K�v������̂ł����������
	kaeriti_size++;kaeriti_size++; // �R���X�g���N�^�f�X�g���N�^�̏ꍇ������collected_index��Lua������n���Ă���

	if ((def->kaeriti.type != MyDefType::VOIDDAYO) && (def->kaeriti.type != MyDefType::NOUSE)) {
		kaeriti_size++;
	}

	// �e�����l�̂��{�{���̗p�ӂ��s��
	for (int i=0;i<hikisuu_size;i++) {
		this->checkType(filename, input, def, i);
	}
	for (int i =0; i< hikisuu_size;i++) {
		this->sengen(filename, input, def, i);
		this->getHIKISUU(filename, input, def, i);
	}

	// �֐��Ăяo��
	kannsuuYobidasi(filename, input, def, hikisuu_size,is_construct, is_destruct );


	// �e������out�̏ꍇ�ɔ��f������
	for (int i=0;i< hikisuu_size;i++) {
		this->hanneiHIKISUU(filename, input, def, i);
	
	}


	// �Ԃ�l���v�b�V������
	pushKAERITI(filename, input, def, hikisuu_size, is_construct, is_destruct);

	// ���^�[������
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

	// �R���N�V�����N���X�̃C���f�b�N�X���w�肷��K�v������̂ł����������
	kaeriti_size++;kaeriti_size++; // �R���X�g���N�^�f�X�g���N�^�̏ꍇ������collected_index��Lua������n���Ă���

	if ((def->kaeriti.type != MyDefType::VOIDDAYO) && (def->kaeriti.type != MyDefType::NOUSE)) {
		kaeriti_size++;
	}

	// �e�����l�̂��{�{���̗p�ӂ��s��
	for (int i=0;i<hikisuu_size;i++) {
		this->checkType(filename, input, def, i);
	}
	for (int i =0; i< hikisuu_size;i++) {
		this->sengen(filename, input, def, i);
		this->getHIKISUU(filename, input, def, i);
	}

	// �֐��Ăяo��
	kannsuuYobidasi(filename, input, def, hikisuu_size,is_construct, is_destruct );


	// �e������out�̏ꍇ�ɔ��f������
	for (int i=0;i< hikisuu_size;i++) {
		this->hanneiHIKISUU(filename, input, def, i);
	
	}


	// �Ԃ�l���v�b�V������
	pushKAERITI(filename, input, def, hikisuu_size, is_construct, is_destruct);

	// ���^�[������
	KTROBO::mylog::writelog(filename, "return %d;\n", kaeriti_size); 
	KTROBO::mylog::writelog(filename, "}\n");

	*/
}


void MyLuaGlueMake::_writeFunc2(char* filename, MakeGlueInput* input) {

// �g��Ȃ��I
/*
	KTROBO::mylog::writelog(filename, "int KTROBO::%s_getInstanceGlue(lua_State* L) {\n", input->collection_name);
	int kaeriti_size = 0;
	int hikisuu_size = 0;

	// �R���N�V�����N���X�̃C���f�b�N�X���w�肷��K�v������̂ł����������
	kaeriti_size++;kaeriti_size++; // �R���X�g���N�^�f�X�g���N�^�̏ꍇ������collected_index��Lua������n���Ă���
	kaeriti_size++;// �R���N�V�����N���X��Ԃ��̂ł��̂���
	
	// �ϐ��̐錾
	KTROBO::mylog::writelog(filename, "  int collection_index = lua_tonumber(L,1);\n");
	KTROBO::mylog::writelog(filename, "  int collected_index = lua_tonumber(L,2);\n");


	// �֐��Ăяo��
	KTROBO::mylog::writelog(filename, "  %s* kaeri = KTROBO::MyLuaGlueSingleton::getInstance()->getCol%s(collection_index)->getInstance(collected_index);\m", input->collected_name,input->collection_name);


	// �Ԃ�l���v�b�V������

	// ���^�[������
	KTROBO::mylog::writelog(filename, "return %d;\n", kaeriti_size); 
	KTROBO::mylog::writelog(filename, "}\n");
*/
}

void MyLuaGlueMake::writeFunc(char *filename) {
	//KTROBO::mylog::writelog(filename, " public:\n");
	vector<MakeGlueInput*>::iterator it = inputs.begin();
	while (it != inputs.end()) {
		MakeGlueInput* input = *it;
		//collection�N���X��collect�����N���X�̃R���X�g���N�^(�t�@�N�g���[�H)
		int csize = input->construct_defs.size();
		for (int i=0;i<csize;i++) {
			MyFuncDef* def = input->construct_defs[i];
			_writeFunc(filename, input, def, true, false);
		}
		//�f�X�g���N�^���\�b�h��������΁E�E
		int dsize = input->destruct_defs.size();
		for (int i=0;i<dsize;i++) {
			MyFuncDef* def = input->destruct_defs[i];
			_writeFunc(filename, input, def, false, true);
		}

		// collect�����N���X�̃C���^�[�t�F�[�X�̊e���\�b�h
		int func_size = input->func_defs.size();
		for (int i=0;i<func_size; i++) {
			MyFuncDef* def = input->func_defs[i];

			_writeFunc(filename, input, def, false, false);
		}

		// collection �N���X��collect����Ă���N���X�̃A�N�Z�X���\�b�h
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
		//collection�N���X��collect�����N���X�̃R���X�g���N�^(�t�@�N�g���[�H)
		int csize = input->construct_defs.size();
		for (int i=0;i<csize;i++) {
			MyFuncDef* def = input->construct_defs[i];
			_writeFuncSengen(filename, input, def, true, false);
		}
		//�f�X�g���N�^���\�b�h��������΁E�E
		int dsize = input->destruct_defs.size();
		for (int i=0;i<dsize;i++) {
			MyFuncDef* def = input->destruct_defs[i];
			_writeFuncSengen(filename, input, def, false, true);
		}

		// collect�����N���X�̃C���^�[�t�F�[�X�̊e���\�b�h
		int func_size = input->func_defs.size();
		for (int i=0;i<func_size; i++) {
			MyFuncDef* def = input->func_defs[i];

			_writeFuncSengen(filename, input, def, false, false);
		}

		// collection �N���X��collect�����N���X�̃A�N�Z�X���\�b�h
		_writeFuncSengen2(filename, input);
	
		it = it + 1;
	}
}

void MyLuaGlueMake::writeYARITORIFunc(char* filename, char* prefix) {

	// is_yaritori ��true��class�̖��O���W�߂�
	set<string> class_namesdayo;
	int isize = inputs.size();

	for (int i=0;i<isize;i++) {
		MakeGlueInput* input = inputs[i];
		int csize = input->construct_defs.size();
		for (int k=0;k<csize;k++) {
			MyFuncDef* def = input->construct_defs[k];
			// �Ԃ�l
			if ((def->kaeriti.type == MyDefType::CLASSDAYO) && def->kaeriti.is_yaritori) {
				class_namesdayo.insert(string(def->kaeriti.kaeri6classname));
			}
			// ����
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
			// �Ԃ�l
			if ((def->kaeriti.type == MyDefType::CLASSDAYO) && def->kaeriti.is_yaritori) {
				class_namesdayo.insert(string(def->kaeriti.kaeri6classname));
			}
			// ����
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
			// �Ԃ�l
			if ((def->kaeriti.type == MyDefType::CLASSDAYO) && def->kaeriti.is_yaritori) {
				class_namesdayo.insert(string(def->kaeriti.kaeri6classname));
			}
			// ����
			for (int l=0;l < HIKISUU_MAX;l++) {
				MyFuncNUMWITHTYPE* n = &def->hikisuu[l];
				if (n->is_yaritori && n->type == MyDefType::CLASSDAYO) {
					class_namesdayo.insert(string(n->kaeri6classname));
				}
			}
		}



	}


	// class_namesdayo �ɏW�܂����̂�

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
	fopen_s(&fp,filename,"w");// ��������N���A����
	if (fp == NULL) {
		KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "failed to clear output file");
		return 0;
	}
	fclose(fp);

	fopen_s(&fp,cpppath,"w");// ��������N���A����
	if (fp == NULL) {
		KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "failed to clear output file");
		return 0;
	}
	fclose(fp);

	fopen_s(&fp,user_write_h_path,"w");// ��������N���A����
	if (fp == NULL) {
		KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "failed to clear output file");
		return 0;
	}
	fclose(fp);




	KTROBO::mylog::writelog(filename, "#ifndef TOLUAGLUEFILE_H\n");
	KTROBO::mylog::writelog(filename, "#define TOLUAGLUEFILE_H\n");
	
	// include �t�@�C������������
	writeInclude(filename);
	
	KTROBO::mylog::writelog(filename, "namespace KTROBO {\n");
	
	writeYARITORIFunc(filename, "");

	
	// singleton �N���X�̓��e����������
	writeFuncSengen(filename);
	writeSingleton(filename);
	
	
	KTROBO::mylog::writelog(filename, "}\n");
	KTROBO::mylog::writelog(filename, "#endif\n");


	KTROBO::mylog::writelog(cpppath, "#include \"%s\"\n", header_name);
	KTROBO::mylog::writelog(cpppath, "using namespace KTROBO;\n");
	writeFunc(cpppath);


	KTROBO::mylog::writelog(user_write_h_path, "#ifndef USERWRITETOLUA_H\n");
	KTROBO::mylog::writelog(user_write_h_path, "#define USERWRITETOLUA_H\n");

	// include �t�@�C������������
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
	fopen_s(&fp,filename,"w");// ��������N���A����
	if (fp == NULL) {
		KTROBO::mylog::writelog(KTROBO::FATAL_ERROR, "failed to clear output file");
		return 0;
	}
	fclose(fp);
	
	// lua�t�@�C���������Ă���
	// �O���[�֐������b�v����N���X�̒�`������
	// global my_interfaces; �ɃN���X�̒�`��
	// global my_instances; �ɃC���X�^���X�̎Q�Ƃ�����

	// �X���b�h���ƂɈقȂ�lua_State���̗p����悤�ɂ���
	// �X���b�h�Ԃ̃f�[�^(�I�u�W�F�N�g)�A�g�͂ǂ�����̂�
	// ���Ƃ��΃��[�h�����X���b�h�ŃI�u�W�F�N�g�������܂��͔j�����ꂽ�Ƃ���
	// �X�V�����X���b�h�łǂ�����Ă��̃I�u�W�F�N�g��ǉ��܂��͍폜���邩
	// ���̂P�@lua�t�@�C�����Ăяo����邲�ƂɑS�I�u�W�F�N�g�����[�h����悤�ɂ���
	// ���̂Q�@�����j�������Ő����j�����ꂽ�I�u�W�F�N�g�����{�{���܂��̓t�@�C���ɋL�^���Ă����@
	//         ���̋L�^����т����ꂽlua�t�@�C�����ǂݍ���ŊY���̏������s��
	// ���̂R�@�X�V�X���b�h�Ő����j���X���b�h�ɐ����j���̎x���������A�����j���X���b�h�ł͊����̍��}��������
	//         �X�V�X���b�h�ł́@�j�����͎w��������g�킸�A�������͊����̍��}���m�F�����Ƃ��݂̂��炻�̃I�u�W�F�N�g�̍X�V���J�n����

	// �킯�Ă���悤�������ǂ̂Ƃ��뉽�����[�h����邩�̏����ǂ��Ŏ����Ƃ����b�ɂȂ�?
	// �����j�������X���b�h�ŌĂ΂��lua�t�@�C�����̂Ɏ�������̂�,����Ƃ��O�������݂���ł�������[�h����`�ɂ���̂�
	// lua�t�@�C���Ɍ��߂����Ń��[�h������̂��悢���ǂ��̏ꍇ�ǂ�����čX�V�����ɂ����Ă������̘b���o�Ă���
	// MeshInstanced �Ɋւ��čl���Ă݂�� 
	// ���܂������f�������炩���ߕ������Ă��ČŒ�̗ʂ̃��[�h�����đ��v�ł���Ύ������y����
	// ���V���ɐ����j�����s����K�v���o�Ă��邩�킩��Ȃ��Ƃ������ƂɂȂ�Ɨl�����ς���Ă���
	// �s�m��Ȑ����j�����s����ꍇ��MeshInstanced������ꍇ�ǂ��Ή����邩
	
	// ��������
	// 1. ���܂ł�MeshInstanceds�̃R�s�[���Ƃ�
	// 2. ���̃R�s�[�����[�h��Ԃɐݒ肷��
	// 3. �R�s�[�ɐV���ɉ������MeshInstanced��������(���[�h����j
	// 4. ���̃R�s�[�̃��[�h��Ԃ��������A���܂ł�MeshInstanceds�Ɠ���ւ���!(�ǂ�����Ď������邩�͍l���ǂ���)

	// �j������
	// 1. �t���O�𗧂ĂĂ��̃C���X�^���X���ǂ̃X���b�h�����p�ł��Ȃ��悤�ȏ�Ԃ܂��͎d�g�݂����
	// 2. �t���O�������Ă���C���X�^���X�̃f�[�^�̂݁i�C���X�^���X�̎Q�Ǝ��̂͏����Ȃ��j�j������
	//    (�����j���X���b�h��c++����GC���\�b�h�Ƃ�ׂ΂����̂�����Ԏd�g�݂����)
	// 3. �����OK�@�����MeshInstanceds�ł�MeshInstanced�̎Q�Ƃ�ۑ����Ă���x�N�g���\���́H����j������ƃC���f�b�N�X�������̂�
	// ���̎d�g�݂ő��v�Ȃ͂��I
     
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

		// �U���I�u�W�F�N�g��Ԃ����\�b�h
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