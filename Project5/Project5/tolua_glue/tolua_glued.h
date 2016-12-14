#ifndef USERWRITETOLUA_H
#define USERWRITETOLUA_H
#pragma once
#include <vector>
#include "lua.hpp"
#include "lualib.h"
#include "lauxlib.h"
#include "../KTRoboGameError.h"
#include "MyLuaGlueMakeCommon.h"
#include "../KTRoboMeshInstanced.h"
#include "../KTRoboTextFromLua.h"
#include "../KTRoboLuaCMesh.h"
#include "../KTRoboTexture.h"
#include "../KTRoboMessage.h"
#include "../KTRoboKendoSinaiGenerator.h"
using namespace std;
namespace KTROBO {
/////  please write this functions!///////


extern void YARITORI_getMYMATRIX(lua_State* L, MYMATRIX* class_inst, int index);
extern void YARITORI_setMYMATRIX(lua_State* L, MYMATRIX* class_inst, int index);
extern void YARITORI_setKAERITIMYMATRIX(lua_State* L, MYMATRIX* class_inst);
extern void YARITORI_getMYVECTOR4(lua_State* L, MYVECTOR4* class_inst, int index);
extern void YARITORI_setMYVECTOR4(lua_State* L, MYVECTOR4* class_inst, int index);
extern void YARITORI_setKAERITIMYVECTOR4(lua_State* L, MYVECTOR4* class_inst);



/////////////////////////////////////////////
}
#endif
