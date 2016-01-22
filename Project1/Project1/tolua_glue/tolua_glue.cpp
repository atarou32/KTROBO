#include "tolua_glue.h"
using namespace KTROBO;
void KTROBO::YARITORI_getMYMATRIX(lua_State* L, MYMATRIX* class_inst, int index) {

for (int i=0;i<16;i++) {
lua_pushnumber(L, i+1);
lua_gettable(L, -index-3);
float f = atof(lua_tostring(L,1));
lua_remove(L,1);
class_inst->m[i/4][i%4] = f;
}
}

void KTROBO::YARITORI_setMYMATRIX(lua_State* L, MYMATRIX* class_inst, int index) {

for (int i=0;i<16;i++) {
char temp[1024];
memset(temp,0,1024);
sprintf_s(temp, 1024, "%f", class_inst->m[i/4][i%4]);
lua_pushnumber(L, i+1);
lua_pushstring(L, temp);
lua_settable(L, -index-3);
}


}
void KTROBO::YARITORI_setKAERITIMYMATRIX(lua_State* L, MYMATRIX* class_inst) {

lua_newtable(L);
int top = lua_gettop(L);
YARITORI_setMYMATRIX(L, class_inst, top);



}
void KTROBO::YARITORI_getMYVECTOR4(lua_State* L, MYVECTOR4* class_inst, int index) {

lua_pushnumber(L, 1);
lua_gettable(L, -index-3);
float f = atof(lua_tostring(L,1));
lua_remove(L,1);
class_inst->x = f;

lua_pushnumber(L, 2);
lua_gettable(L, -index-3);
float f = atof(lua_tostring(L,1));
lua_remove(L,1);
class_inst->y = f;

lua_pushnumber(L, 3);
lua_gettable(L, -index-3);
float f = atof(lua_tostring(L,1));
lua_remove(L,1);
class_inst->z = f;

lua_pushnumber(L, 4);
lua_gettable(L, -index-3);
float f = atof(lua_tostring(L,1));
lua_remove(L,1);
class_inst->w = f;



}

void KTROBO::YARITORI_setMYVECTOR4(lua_State* L, MYVECTOR4* class_inst, int index) {
char temp[1024];
memset(temp,0,1024);
sprintf_s(temp, 1024, "%f", class_inst->x);
lua_pushnumber(L, 1);
lua_pushstring(L, temp);
lua_settable(L, -index-3);

memset(temp,0,1024);
sprintf_s(temp, 1024, "%f", class_inst->y);
lua_pushnumber(L, 2);
lua_pushstring(L, temp);
lua_settable(L, -index-3);

memset(temp,0,1024);
sprintf_s(temp, 1024, "%f", class_inst->z);
lua_pushnumber(L, 3);
lua_pushstring(L, temp);
lua_settable(L, -index-3);

memset(temp,0,1024);
sprintf_s(temp, 1024, "%f", class_inst->w);
lua_pushnumber(L, 4);
lua_pushstring(L, temp);
lua_settable(L, -index-3);


}

void KTROBO::YARITORI_setKAERITIMYVECTOR4(lua_State* L, MYVECTOR4* class_inst) {
lua_newtable(L);
int top = lua_gettop(L);
YARITORI_setMYVECTOR4(L, class_inst, top);
}
