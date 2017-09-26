#include "KTRoboLuaCollection.h"
#include "KTRoboLog.h"

using namespace KTROBO;

void TCB_luaExec(TCB* thisTCB) {
	Task* t = (Task*)thisTCB->data;
	

	int task_index = (int)(thisTCB->Work[2]);
	bool is_lock = false;
	if((thisTCB->Work[3])) {
		is_lock = true;
	}
	
	lua_State* L = (lua_State*)thisTCB->Work[1];

	if (is_lock) {
		for (int t_temp = task_index; t_temp >= 0; t_temp--) {
			CS::instance()->enter(CS_TASK_CS, "luaexec", t_temp);
		}		
	}

	CS::instance()->enter(CS_MESSAGE_CS, "luaexec");
	LuaTCBStruct* tt = (LuaTCBStruct*)thisTCB->Work[0];
	char* lua_filename = tt->lua_filename;

	int tee =lua_gettop(L);
	int error;
	try {
		error = luaL_loadfile(L, lua_filename) || lua_pcall(L, 0, 0, 0);

	}catch (GameError* err) {
		
		lua_settop(L,tee);
		mylog::writelog(err->getErrorCode(), err->getMessage());
		Sleep(500);
		//delete err;
		
		throw err;
	}
	
	if (error) {
		mylog::writelog("errtxt.txt", "%s", lua_tostring(L, -1));
		OutputDebugStringA(lua_tostring(L,-1));
		lua_settop(L,tee);
		
    } else {
		lua_settop(L,tee);
		tt->is_use = false;
		if (thisTCB->flag) {
			t->kill(thisTCB);
		}
	}
	CS::instance()->leave(CS_MESSAGE_CS, "luaexec");
	if (is_lock) {
		

		for( int t_temp = 0; t_temp <= task_index;t_temp++) {
			CS::instance()->leave(CS_TASK_CS, "luaexec",t_temp);
		}
	}

	if (error) {
		Sleep(500);
		throw new GameError(KTROBO::WARNING, "error lua ");
	}
}

void LuaTCBMaker::Init(Task** t, lua_State** l) {
	for (int i=0;i<TASKTHREAD_NUM;i++) {
		ts[i] = t[i];
		ls[i] = l[i];
	}

	for (int i=0;i<KTROBO_LUAEXEC_STRUCT_SIZE;i++) {
		structs[i].is_use = false;
		structs[i].is_lock_sita = false;
		structs[i].task_index = 0;
		memset(structs[i].lua_filename, 0, 256);
		structs[i].is_send = false;
	}

}
void LuaTCBMaker::makeTCBExec() {

	CS::instance()->enter(CS_MESSAGE_CS, "maketcbexec");

	for (int i=0;i<KTROBO_LUAEXEC_STRUCT_SIZE;i++) {
		if (structs[i].is_use && !structs[i].is_send) {
			unsigned long Work[TASK_WORK_SIZE];
			memset(Work, 0, sizeof(unsigned long)*TASK_WORK_SIZE);
			Work[0] = (unsigned long)&structs[i];
			Work[1] = (unsigned long)ls[structs[i].task_index];
			Work[2] = (unsigned long)structs[i].task_index;
			Work[3] = (unsigned long)structs[i].is_lock_sita;
			int task_index = structs[i].task_index;
			structs[i].is_send = true;
			CS::instance()->leave(CS_MESSAGE_CS, "maketcbexec");
			ts[task_index]->make(TCB_luaExec, ts[task_index], Work, 0x0000FFFF);
			CS::instance()->enter(CS_MESSAGE_CS, "maketcbexec");
			
		}
	}
	CS::instance()->leave(CS_MESSAGE_CS, "maketcbexec");

}

void LuaTCBMaker::doTCBnow(int task_index, bool is_lock_sita, char* lua_filename) {
	if (task_index >= 0 && task_index < TASKTHREAD_NUM) {
	
		CS::instance()->enter(CS_MESSAGE_CS, "maketcb");
		if (strcmp(lua_filename, "test") ==0 || strlen(lua_filename) ==0) {
			CS::instance()->leave(CS_MESSAGE_CS, "maketcb");
			return;
		}

		for (int i=0;i<KTROBO_LUAEXEC_STRUCT_SIZE;i++) {
			if (!structs[i].is_use) {
				structs[i].is_use = true;
				structs[i].is_send =false;
				structs[i].is_lock_sita = is_lock_sita;
				structs[i].task_index = task_index;
				strcpy_s(structs[i].lua_filename,256,lua_filename);
				structs[i].lua_filename[strlen(lua_filename)] = '\0';
			//	CS::instance()->leave(CS_MESSAGE_CS, "maketcb");

				unsigned long Work[TASK_WORK_SIZE];
				memset(Work, 0, sizeof(unsigned long)*TASK_WORK_SIZE);
				Work[0] = (unsigned long)&structs[i];
				Work[1] = (unsigned long)ls[structs[i].task_index];
				Work[2] = (unsigned long)structs[i].task_index;
				Work[3] = (unsigned long)structs[i].is_lock_sita;
				int task_index = structs[i].task_index;
				structs[i].is_send = true;
				CS::instance()->leave(CS_MESSAGE_CS, "maketcb");
				ts[task_index]->donow(TCB_luaExec, ts[task_index], Work, 0x0000FFFF);
				//CS::instance()->enter(CS_MESSAGE_CS, "maketcbexec");

				return;
			}
		}
		CS::instance()->leave(CS_MESSAGE_CS, "maketcb");
	
	}

}


void LuaTCBMaker::makeTCB(int task_index, bool is_lock_sita, char* lua_filename) {
	if (task_index >= 0 && task_index < TASKTHREAD_NUM) {
	
		CS::instance()->enter(CS_MESSAGE_CS, "maketcb");
		if (strcmp(lua_filename, "test") ==0 || strlen(lua_filename) ==0) {
			CS::instance()->leave(CS_MESSAGE_CS, "maketcb");
			return;
		}

		for (int i=0;i<KTROBO_LUAEXEC_STRUCT_SIZE;i++) {
			if (!structs[i].is_use) {
				structs[i].is_use = true;
				structs[i].is_send =false;
				structs[i].is_lock_sita = is_lock_sita;
				structs[i].task_index = task_index;
				strcpy_s(structs[i].lua_filename,256,lua_filename);
				structs[i].lua_filename[strlen(lua_filename)] = '\0';
				CS::instance()->leave(CS_MESSAGE_CS, "maketcb");
				return;
			}
		}
		CS::instance()->leave(CS_MESSAGE_CS, "maketcb");
	
	}
}

LuaTCBStruct LuaTCBMaker::structs[KTROBO_LUAEXEC_STRUCT_SIZE];
lua_State* LuaTCBMaker::ls[TASKTHREAD_NUM];
Task* LuaTCBMaker::ts[TASKTHREAD_NUM];