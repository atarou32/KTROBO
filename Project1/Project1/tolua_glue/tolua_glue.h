#ifndef TOLUAGLUEFILE_H
#define TOLUAGLUEFILE_H
#pragma once
#include <vector>
#include "lua.hpp"
#include "lualib.h"
#include "lauxlib.h"
#include "MyLuaGlueMakeCommon.h"
#include "../KTRoboMeshInstanced.h"
using namespace std;
namespace KTROBO {
/////  please write this functions!///////


void YARITORI_getMYMATRIX(lua_State* L, MYMATRIX* class_inst, int index);
void YARITORI_setMYMATRIX(lua_State* L, MYMATRIX* class_inst, int index);
void YARITORI_setKAERITIMYMATRIX(lua_State* L, MYMATRIX* class_inst);
void YARITORI_getMYVECTOR4(lua_State* L, MYVECTOR4* class_inst, int index);
void YARITORI_setMYVECTOR4(lua_State* L, MYVECTOR4* class_inst, int index);
void YARITORI_setKAERITIMYVECTOR4(lua_State* L, MYVECTOR4* class_inst);



/////////////////////////////////////////////
class MyLuaGlueSingleton{
 private:
  MyLuaGlueSingleton() {};
  MyLuaGlueSingleton(const MyLuaGlueSingleton& g) {};
  MyLuaGlueSingleton& operator = (const MyLuaGlueSingleton& o) {};
 public:
  static MyLuaGlueSingleton* getInstance() {
    static MyLuaGlueSingleton instance;
    return &instance;
  };
 private:
  vector<MeshInstanceds*> m_MeshInstanceds;
 public:
MeshInstanceds* getColMeshInstanceds ( int index) {
  return m_MeshInstanceds[index];
}
void setColMeshInstanceds ( MeshInstanceds* m) {
  m_MeshInstanceds.push_back(m);
}
void registerdayo(lua_State* L) {
  lua_register(L, "MeshInstanceds_setBoneIndexInfo", MeshInstanceds_setBoneIndexInfoGlue);
  lua_register(L, "MeshInstanceds_setWorld", MeshInstanceds_setWorldGlue);
  lua_register(L, "MeshInstanceds_getWorld", MeshInstanceds_getWorldGlue);
  lua_register(L, "MeshInstanceds_setColor", MeshInstanceds_setColorGlue);
  lua_register(L, "MeshInstanceds_getColor", MeshInstanceds_getColorGlue);
  lua_register(L, "MeshInstanceds_setIsRender", MeshInstanceds_setIsRenderGlue);
  lua_register(L, "MeshInstanceds_getIsRender", MeshInstanceds_getIsRenderGlue);
  lua_register(L, "MeshInstanceds_getInstanceIndex", MeshInstanceds_getInstanceIndexGlue);
  lua_register(L, "MeshInstanceds_makeInstancedID", MeshInstanceds_makeInstancedIDGlue);
}};
int MeshInstanceds_makeInstancedIDGlue(lua_State* L) {
luaL_checktype(L,-3, LUA_TTABLE);
luaL_checktype(L,-4, LUA_TTABLE);
luaL_checktype(L,-5, LUA_TTABLE);
luaL_checktype(L,-6, LUA_TNUMBER);
luaL_checktype(L,-7, LUA_TBOOLEAN);
luaL_checktype(L,-8, LUA_TTABLE);
Mesh* mesh;
  mesh = NULL;
Mesh* skeleton;
  skeleton = NULL;
MeshInstanced* parent_instance;
  parent_instance = NULL;
int parent_bone_index;
parent_bone_index = lua_tonumber(L,-6);
bool connect_without_matrix_local;
connect_without_matrix_local = lua_toboolean(L,-7);
MYMATRIX* matrix_local_kakeru;
  matrix_local_kakeru = NULL;
int ci = lua_tonumber(L, -1);
int cci = lua_tonumber(L, -2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->makeInstancedID(mesh , skeleton , parent_instance , parent_bone_index , connect_without_matrix_local , matrix_local_kakeru );
{
int ci = lua_tonumber(L, -1);
int cci = kaeriti;
lua_newtable(L);
int top = lua_gettop(L);
lua_pushstring(L, "MeshInstanced");
lua_setfield(L, top, "collected_class_name");
{
char tempp[1024];memset(tempp,0,1024);
sprintf_s(tempp, 1024, "%d", cci);
lua_pushstring(L, tempp);
lua_setfield(L, top, "collected_index");
}
lua_pushstring(L, "IMeshInstanced");
lua_setfield(L, top, "collected_interface_name");
lua_pushstring(L, "MeshInstanceds");
lua_setfield(L, top, "collection_class_name");
{
char tempp[1024];memset(tempp,0,1024);
sprintf_s(tempp, 1024, "%d", ci);
lua_pushstring(L, tempp);
lua_setfield(L, top, "collection_index");
}
}
return 9;
}
int MeshInstanceds_setBoneIndexInfoGlue(lua_State* L) {
luaL_checktype(L,-3, LUA_TNUMBER);
luaL_checktype(L,-4, LUA_TNUMBER);
luaL_checktype(L,-5, LUA_TSTRING);
unsigned short bones_anime_first_index[32];
memset(bones_anime_first_index,0, sizeof(unsigned short)* 32);
{
int temp = lua_rawlen(L,-3);
for (int i=0;i<temp && i < 32;i++) {
  lua_pushnumber(L, i+1);
  lua_gettable(L, -3);
  bones_anime_first_index[i] = (unsigned short)(lua_tonumber(L, 1));
  lua_remove(L,1);
}
}
unsigned short bones_anime_last_index[32];
memset(bones_anime_last_index,0, sizeof(unsigned short)* 32);
{
int temp = lua_rawlen(L,-4);
for (int i=0;i<temp && i < 32;i++) {
  lua_pushnumber(L, i+1);
  lua_gettable(L, -4);
  bones_anime_last_index[i] = (unsigned short)(lua_tonumber(L, 1));
  lua_remove(L,1);
}
}
float bones_anime_first_weight[32];
memset(bones_anime_first_weight,0, sizeof(float)* 32);
{
int temp = lua_rawlen(L,-5);
for (int i=0;i<temp && i < 32;i++) {
  lua_pushnumber(L, i+1);
  lua_gettable(L, -5);
  bones_anime_first_weight[i] = atof(lua_tostring(L, 1));
  lua_remove(L,1);
}
}
int ci = lua_tonumber(L, -1);
int cci = lua_tonumber(L, -2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->getInterface(cci)->setBoneIndexInfo(bones_anime_first_index , bones_anime_last_index , bones_anime_first_weight );
return 5;
}
int MeshInstanceds_setWorldGlue(lua_State* L) {
luaL_checktype(L,-3, LUA_TTABLE);
MYMATRIX world_m;
MYMATRIX* world=&world_m;
YARITORI_getMYMATRIX(L, world, 0);
int ci = lua_tonumber(L, -1);
int cci = lua_tonumber(L, -2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->getInterface(cci)->setWorld(world );
return 3;
}
int MeshInstanceds_getWorldGlue(lua_State* L) {
int ci = lua_tonumber(L, -1);
int cci = lua_tonumber(L, -2);
MYMATRIX* kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->getInterface(cci)->getWorld();
YARITORI_setKAERITIMYMATRIX(L, kaeriti);
return 3;
}
int MeshInstanceds_setColorGlue(lua_State* L) {
luaL_checktype(L,-3, LUA_TTABLE);
MYVECTOR4* colors;
  colors = NULL;
int ci = lua_tonumber(L, -1);
int cci = lua_tonumber(L, -2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->getInterface(cci)->setColor(colors );
return 3;
}
int MeshInstanceds_getColorGlue(lua_State* L) {
luaL_checktype(L,-3, LUA_TNUMBER);
int index;
index = lua_tonumber(L,-3);
int ci = lua_tonumber(L, -1);
int cci = lua_tonumber(L, -2);
MYVECTOR4* kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->getInterface(cci)->getColor(index );
YARITORI_setKAERITIMYVECTOR4(L, kaeriti);
return 4;
}
int MeshInstanceds_setIsRenderGlue(lua_State* L) {
luaL_checktype(L,-3, LUA_TBOOLEAN);
bool t;
t = lua_toboolean(L,-3);
int ci = lua_tonumber(L, -1);
int cci = lua_tonumber(L, -2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->getInterface(cci)->setIsRender(t );
return 3;
}
int MeshInstanceds_getIsRenderGlue(lua_State* L) {
int ci = lua_tonumber(L, -1);
int cci = lua_tonumber(L, -2);
bool kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->getInterface(cci)->getIsRender();
lua_pushboolean(L,kaeriti);
return 3;
}
int MeshInstanceds_getInstanceIndexGlue(lua_State* L) {
int ci = lua_tonumber(L, -1);
int cci = lua_tonumber(L, -2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->getInterface(cci)->getInstanceIndex();
{char tempp[1024];memset(tempp,0,1024);
sprintf_s(tempp, 1024, "%d",kaeriti);
lua_pushstring(L, tempp);
}return 3;
}
}
#endif
