#include "tolua_glue.h"
using namespace KTROBO;
int KTROBO::MeshInstanceds_makeInstancedIDGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TTABLE);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TNUMBER);
luaL_checktype(L,6, LUA_TTABLE);
luaL_checktype(L,7, LUA_TNUMBER);
luaL_checktype(L,8, LUA_TBOOLEAN);
luaL_checktype(L,9, LUA_TTABLE);
CMesh* cmesh;
{
int temp = lua_rawlen(L,3);
if (temp ==0) {
  cmesh = NULL;
} else {
{
lua_pushnumber(L,1);
lua_gettable(L,3);
luaL_checktype(L, -1, LUA_TTABLE);
lua_getfield(L, -1, "collection_index");
int collection_index=lua_tonumber(L, -1);
lua_remove(L,-1);
lua_getfield(L, -1, "collected_index");
int collected_index=lua_tonumber(L, -1);
lua_remove(L,-1);
lua_remove(L,-1);
cmesh = KTROBO::MyLuaGlueSingleton::getInstance()->getColCMeshs(collection_index)->getInstance(collected_index);
}
}
}
int mesh_index;
mesh_index = lua_tonumber(L,4);
int skeleton_index;
skeleton_index = lua_tonumber(L,5);
MeshInstanced* parent_instance;
{
int temp = lua_rawlen(L,6);
if (temp ==0) {
  parent_instance = NULL;
} else {
{
lua_pushnumber(L,1);
lua_gettable(L,6);
luaL_checktype(L, -1, LUA_TTABLE);
lua_getfield(L, -1, "collection_index");
int collection_index=lua_tonumber(L, -1);
lua_remove(L,-1);
lua_getfield(L, -1, "collected_index");
int collected_index=lua_tonumber(L, -1);
lua_remove(L,-1);
lua_remove(L,-1);
parent_instance = KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(collection_index)->getInstance(collected_index);
}
}
}
int parent_bone_index;
parent_bone_index = lua_tonumber(L,7);
bool connect_without_matrix_local;
connect_without_matrix_local = (bool)lua_toboolean(L,8);
MYMATRIX matrix_local_kakeru_m;
MYMATRIX* matrix_local_kakeru=&matrix_local_kakeru_m;
YARITORI_getMYMATRIX(L, matrix_local_kakeru, 6);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->makeInstancedID(cmesh , mesh_index , skeleton_index , parent_instance , parent_bone_index , connect_without_matrix_local , matrix_local_kakeru );
{
int ci = lua_tonumber(L, 1);
int cci = kaeriti;
lua_newtable(L);
lua_pushnumber(L, 1);
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
lua_settable(L,-3);
lua_insert(L,1);
}
return 10;
}
int KTROBO::MeshInstanceds_setBoneIndexInfoGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TTABLE);
luaL_checktype(L,4, LUA_TTABLE);
luaL_checktype(L,5, LUA_TTABLE);
unsigned short bones_anime_first_index[32];
memset(bones_anime_first_index,0, sizeof(unsigned short)* 32);
{
int temp = lua_rawlen(L,3);
for (int i=0;i<temp && i < 32;i++) {
  lua_pushnumber(L, i+1);
  lua_gettable(L, 3);
  bones_anime_first_index[i] = (unsigned short)(lua_tonumber(L, -1));
  lua_remove(L,-1);
}
}
unsigned short bones_anime_last_index[32];
memset(bones_anime_last_index,0, sizeof(unsigned short)* 32);
{
int temp = lua_rawlen(L,4);
for (int i=0;i<temp && i < 32;i++) {
  lua_pushnumber(L, i+1);
  lua_gettable(L, 4);
  bones_anime_last_index[i] = (unsigned short)(lua_tonumber(L, -1));
  lua_remove(L,-1);
}
}
float bones_anime_first_weight[32];
memset(bones_anime_first_weight,0, sizeof(float)* 32);
{
int temp = lua_rawlen(L,5);
for (int i=0;i<temp && i < 32;i++) {
  lua_pushnumber(L, i+1);
  lua_gettable(L, 5);
  bones_anime_first_weight[i] = atof(lua_tostring(L, -1));
  lua_remove(L,-1);
}
}
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->getInterface(cci)->setBoneIndexInfo(bones_anime_first_index , bones_anime_last_index , bones_anime_first_weight );
return 5;
}
int KTROBO::MeshInstanceds_setWorldGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TTABLE);
MYMATRIX world_m;
MYMATRIX* world=&world_m;
YARITORI_getMYMATRIX(L, world, 0);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->getInterface(cci)->setWorld(world );
return 3;
}
int KTROBO::MeshInstanceds_getWorldGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TTABLE);
MYMATRIX gworld_m;
MYMATRIX* gworld=&gworld_m;
YARITORI_getMYMATRIX(L, gworld, 0);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->getInterface(cci)->getWorld(gworld );
YARITORI_setMYMATRIX(L, gworld, 0);
return 3;
}
int KTROBO::MeshInstanceds_setColorGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TTABLE);
MYVECTOR4* colors;
  colors = NULL;
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->getInterface(cci)->setColor(colors );
return 3;
}
int KTROBO::MeshInstanceds_getColorGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TTABLE);
int index;
index = lua_tonumber(L,3);
MYVECTOR4 gcolor_m;
MYVECTOR4* gcolor=&gcolor_m;
YARITORI_getMYVECTOR4(L, gcolor, 1);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->getInterface(cci)->getColor(index , gcolor );
YARITORI_setMYVECTOR4(L, gcolor, 1);
return 4;
}
int KTROBO::MeshInstanceds_setIsRenderGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TBOOLEAN);
bool t;
t = (bool)lua_toboolean(L,3);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->getInterface(cci)->setIsRender(t );
return 3;
}
int KTROBO::MeshInstanceds_getIsRenderGlue(lua_State* L) {
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
bool kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->getInterface(cci)->getIsRender();
lua_pushboolean(L,kaeriti);
lua_insert(L,1);
return 3;
}
int KTROBO::MeshInstanceds_getInstanceIndexGlue(lua_State* L) {
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(ci)->getInterface(cci)->getInstanceIndex();
lua_pushnumber(L, kaeriti);
lua_insert(L,1);
return 3;
}
int KTROBO::TextFromLuas_makeInstanceGlue(lua_State* L) {
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColTextFromLuas(ci)->makeInstance();
{
int ci = lua_tonumber(L, 1);
int cci = kaeriti;
lua_newtable(L);
lua_pushnumber(L, 1);
lua_newtable(L);
int top = lua_gettop(L);
lua_pushstring(L, "TextFromLua");
lua_setfield(L, top, "collected_class_name");
{
char tempp[1024];memset(tempp,0,1024);
sprintf_s(tempp, 1024, "%d", cci);
lua_pushstring(L, tempp);
lua_setfield(L, top, "collected_index");
}
lua_pushstring(L, "ITextFromLua");
lua_setfield(L, top, "collected_interface_name");
lua_pushstring(L, "TextFromLuas");
lua_setfield(L, top, "collection_class_name");
{
char tempp[1024];memset(tempp,0,1024);
sprintf_s(tempp, 1024, "%d", ci);
lua_pushstring(L, tempp);
lua_setfield(L, top, "collection_index");
}
lua_settable(L,-3);
lua_insert(L,1);
}
return 3;
}
int KTROBO::TextFromLuas_setDebugTextGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TSTRING);
char str[1024];
memset(str,0, sizeof(char)* 1024);
mystrcpy(str,1024,0,lua_tostring(L,3));
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextFromLuas(ci)->getInterface(cci)->setDebugText(str );
return 3;
}
int KTROBO::TextFromLuas_addGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
int a;
a = lua_tonumber(L,3);
int b;
b = lua_tonumber(L,4);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColTextFromLuas(ci)->getInterface(cci)->add(a , b );
lua_pushnumber(L, kaeriti);
lua_insert(L,1);
return 5;
}
int KTROBO::TextFromLuas_removeSceneGlue(lua_State* L) {
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextFromLuas(ci)->getInterface(cci)->removeScene();
return 2;
}
int KTROBO::TextFromLuas_enterTWOTAKUGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TSTRING);
luaL_checktype(L,4, LUA_TSTRING);
luaL_checktype(L,5, LUA_TSTRING);
char yes_str[1024];
memset(yes_str,0, sizeof(char)* 1024);
mystrcpy(yes_str,1024,0,lua_tostring(L,3));
char no_str[1024];
memset(no_str,0, sizeof(char)* 1024);
mystrcpy(no_str,1024,0,lua_tostring(L,4));
char render_text[1024];
memset(render_text,0, sizeof(char)* 1024);
mystrcpy(render_text,1024,0,lua_tostring(L,5));
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextFromLuas(ci)->getInterface(cci)->enterTWOTAKU(yes_str , no_str , render_text );
return 5;
}
int KTROBO::TextFromLuas_enterONEMESSAGEGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TSTRING);
char mes[1024];
memset(mes,0, sizeof(char)* 1024);
mystrcpy(mes,1024,0,lua_tostring(L,3));
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextFromLuas(ci)->getInterface(cci)->enterONEMESSAGE(mes );
return 3;
}
int KTROBO::TextFromLuas_enterLOADTYUUGlue(lua_State* L) {
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextFromLuas(ci)->getInterface(cci)->enterLOADTYUU();
return 2;
}
int KTROBO::TextFromLuas_makeAILuaGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TBOOLEAN);
luaL_checktype(L,4, LUA_TSTRING);
bool is_lock_sita;
is_lock_sita = (bool)lua_toboolean(L,3);
char lua_filename[1024];
memset(lua_filename,0, sizeof(char)* 1024);
mystrcpy(lua_filename,1024,0,lua_tostring(L,4));
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextFromLuas(ci)->getInterface(cci)->makeAILua(is_lock_sita , lua_filename );
return 4;
}
int KTROBO::TextFromLuas_makeButukariLuaGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TBOOLEAN);
luaL_checktype(L,4, LUA_TSTRING);
bool is_lock;
is_lock = (bool)lua_toboolean(L,3);
char lua_filename[1024];
memset(lua_filename,0, sizeof(char)* 1024);
mystrcpy(lua_filename,1024,0,lua_tostring(L,4));
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextFromLuas(ci)->getInterface(cci)->makeButukariLua(is_lock , lua_filename );
return 4;
}
int KTROBO::TextFromLuas_makeLoadLuaGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TBOOLEAN);
luaL_checktype(L,4, LUA_TSTRING);
bool is_lock;
is_lock = (bool)lua_toboolean(L,3);
char lua_filename[1024];
memset(lua_filename,0, sizeof(char)* 1024);
mystrcpy(lua_filename,1024,0,lua_tostring(L,4));
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextFromLuas(ci)->getInterface(cci)->makeLoadLua(is_lock , lua_filename );
return 4;
}
int KTROBO::TextFromLuas_makeAnimeLuaGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TBOOLEAN);
luaL_checktype(L,4, LUA_TSTRING);
bool is_lock;
is_lock = (bool)lua_toboolean(L,3);
char lua_filename[1024];
memset(lua_filename,0, sizeof(char)* 1024);
mystrcpy(lua_filename,1024,0,lua_tostring(L,4));
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextFromLuas(ci)->getInterface(cci)->makeAnimeLua(is_lock , lua_filename );
return 4;
}
int KTROBO::TextFromLuas_makeRenderLuaGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TBOOLEAN);
luaL_checktype(L,4, LUA_TSTRING);
bool is_lock;
is_lock = (bool)lua_toboolean(L,3);
char lua_filename[1024];
memset(lua_filename,0, sizeof(char)* 1024);
mystrcpy(lua_filename,1024,0,lua_tostring(L,4));
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextFromLuas(ci)->getInterface(cci)->makeRenderLua(is_lock , lua_filename );
return 4;
}
int KTROBO::CMeshs_makeInstanceGlue(lua_State* L) {
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColCMeshs(ci)->makeInstance();
{
int ci = lua_tonumber(L, 1);
int cci = kaeriti;
lua_newtable(L);
lua_pushnumber(L, 1);
lua_newtable(L);
int top = lua_gettop(L);
lua_pushstring(L, "CMesh");
lua_setfield(L, top, "collected_class_name");
{
char tempp[1024];memset(tempp,0,1024);
sprintf_s(tempp, 1024, "%d", cci);
lua_pushstring(L, tempp);
lua_setfield(L, top, "collected_index");
}
lua_pushstring(L, "ICMesh");
lua_setfield(L, top, "collected_interface_name");
lua_pushstring(L, "CMeshs");
lua_setfield(L, top, "collection_class_name");
{
char tempp[1024];memset(tempp,0,1024);
sprintf_s(tempp, 1024, "%d", ci);
lua_pushstring(L, tempp);
lua_setfield(L, top, "collection_index");
}
lua_settable(L,-3);
lua_insert(L,1);
}
return 3;
}
int KTROBO::CMeshs_getMeshGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TSTRING);
char path_without_dot[1024];
memset(path_without_dot,0, sizeof(char)* 1024);
mystrcpy(path_without_dot,1024,0,lua_tostring(L,3));
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColCMeshs(ci)->getInterface(cci)->getMesh(path_without_dot );
lua_pushnumber(L, kaeriti);
lua_insert(L,1);
return 4;
}
int KTROBO::CMeshs_getMeshWithAnimeGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TSTRING);
luaL_checktype(L,4, LUA_TSTRING);
char path_to_mesh[1024];
memset(path_to_mesh,0, sizeof(char)* 1024);
mystrcpy(path_to_mesh,1024,0,lua_tostring(L,3));
char path_to_anime[1024];
memset(path_to_anime,0, sizeof(char)* 1024);
mystrcpy(path_to_anime,1024,0,lua_tostring(L,4));
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColCMeshs(ci)->getInterface(cci)->getMeshWithAnime(path_to_mesh , path_to_anime );
lua_pushnumber(L, kaeriti);
lua_insert(L,1);
return 5;
}
int KTROBO::CMeshs_getIsLoadGlue(lua_State* L) {
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
bool kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColCMeshs(ci)->getInterface(cci)->getIsLoad();
lua_pushboolean(L,kaeriti);
lua_insert(L,1);
return 3;
}
int KTROBO::CMeshs_setIsLoadGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TBOOLEAN);
bool t;
t = (bool)lua_toboolean(L,3);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColCMeshs(ci)->getInterface(cci)->setIsLoad(t );
return 3;
}
int KTROBO::CMeshs_deleteMeshGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TSTRING);
char path_without_dot[1024];
memset(path_without_dot,0, sizeof(char)* 1024);
mystrcpy(path_without_dot,1024,0,lua_tostring(L,3));
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColCMeshs(ci)->getInterface(cci)->deleteMesh(path_without_dot );
return 3;
}
int KTROBO::CMeshs_deleteALLGlue(lua_State* L) {
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColCMeshs(ci)->getInterface(cci)->deleteALL();
return 2;
}
int KTROBO::CMeshs_copyDAYOGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TTABLE);
CMesh* src;
{
int temp = lua_rawlen(L,3);
if (temp ==0) {
  src = NULL;
} else {
{
lua_pushnumber(L,1);
lua_gettable(L,3);
luaL_checktype(L, -1, LUA_TTABLE);
lua_getfield(L, -1, "collection_index");
int collection_index=lua_tonumber(L, -1);
lua_remove(L,-1);
lua_getfield(L, -1, "collected_index");
int collected_index=lua_tonumber(L, -1);
lua_remove(L,-1);
lua_remove(L,-1);
src = KTROBO::MyLuaGlueSingleton::getInstance()->getColCMeshs(collection_index)->getInstance(collected_index);
}
}
}
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColCMeshs(ci)->getInterface(cci)->copyDAYO(src );
return 3;
}
int KTROBO::Textures_makeInstGlue(lua_State* L) {
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->makeInst();
{
int ci = lua_tonumber(L, 1);
int cci = kaeriti;
lua_newtable(L);
lua_pushnumber(L, 1);
lua_newtable(L);
int top = lua_gettop(L);
lua_pushstring(L, "Texture");
lua_setfield(L, top, "collected_class_name");
{
char tempp[1024];memset(tempp,0,1024);
sprintf_s(tempp, 1024, "%d", cci);
lua_pushstring(L, tempp);
lua_setfield(L, top, "collected_index");
}
lua_pushstring(L, "ITexture");
lua_setfield(L, top, "collected_interface_name");
lua_pushstring(L, "Textures");
lua_setfield(L, top, "collection_class_name");
{
char tempp[1024];memset(tempp,0,1024);
sprintf_s(tempp, 1024, "%d", ci);
lua_pushstring(L, tempp);
lua_setfield(L, top, "collection_index");
}
lua_settable(L,-3);
lua_insert(L,1);
}
return 3;
}
int KTROBO::Textures_getTextureGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TSTRING);
luaL_checktype(L,4, LUA_TNUMBER);
char tex_name[1024];
memset(tex_name,0, sizeof(char)* 1024);
mystrcpy(tex_name,1024,0,lua_tostring(L,3));
int index_count;
index_count = lua_tonumber(L,4);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->getTexture(tex_name , index_count );
lua_pushnumber(L, kaeriti);
lua_insert(L,1);
return 5;
}
int KTROBO::Textures_getRenderTexGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TNUMBER);
luaL_checktype(L,6, LUA_TNUMBER);
luaL_checktype(L,7, LUA_TNUMBER);
luaL_checktype(L,8, LUA_TNUMBER);
luaL_checktype(L,9, LUA_TNUMBER);
luaL_checktype(L,10, LUA_TNUMBER);
luaL_checktype(L,11, LUA_TNUMBER);
luaL_checktype(L,12, LUA_TNUMBER);
int tex_index;
tex_index = lua_tonumber(L,3);
unsigned int color;
color = (unsigned int)lua_tonumber(L,4);
int x;
x = lua_tonumber(L,5);
int y;
y = lua_tonumber(L,6);
int width;
width = lua_tonumber(L,7);
int height;
height = lua_tonumber(L,8);
int tex_x;
tex_x = lua_tonumber(L,9);
int tex_y;
tex_y = lua_tonumber(L,10);
int tex_width;
tex_width = lua_tonumber(L,11);
int tex_height;
tex_height = lua_tonumber(L,12);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->getRenderTex(tex_index , color , x , y , width , height , tex_x , tex_y , tex_width , tex_height );
lua_pushnumber(L, kaeriti);
lua_insert(L,1);
return 13;
}
int KTROBO::Textures_getRenderBillBoardGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TTABLE);
luaL_checktype(L,6, LUA_TSTRING);
luaL_checktype(L,7, LUA_TSTRING);
luaL_checktype(L,8, LUA_TNUMBER);
luaL_checktype(L,9, LUA_TNUMBER);
luaL_checktype(L,10, LUA_TNUMBER);
luaL_checktype(L,11, LUA_TNUMBER);
int tex_index;
tex_index = lua_tonumber(L,3);
unsigned int color;
color = (unsigned int)lua_tonumber(L,4);
MYMATRIX world_m;
MYMATRIX* world=&world_m;
YARITORI_getMYMATRIX(L, world, 2);
float width;
const char* width_temp = lua_tostring(L,6);
width = atof(width_temp);
float height;
const char* height_temp = lua_tostring(L,7);
height = atof(height_temp);
int tex_x;
tex_x = lua_tonumber(L,8);
int tex_y;
tex_y = lua_tonumber(L,9);
int tex_width;
tex_width = lua_tonumber(L,10);
int tex_height;
tex_height = lua_tonumber(L,11);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->getRenderBillBoard(tex_index , color , world , width , height , tex_x , tex_y , tex_width , tex_height );
lua_pushnumber(L, kaeriti);
lua_insert(L,1);
return 12;
}
int KTROBO::Textures_setRenderTexParamGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TNUMBER);
luaL_checktype(L,6, LUA_TNUMBER);
luaL_checktype(L,7, LUA_TNUMBER);
luaL_checktype(L,8, LUA_TNUMBER);
luaL_checktype(L,9, LUA_TNUMBER);
luaL_checktype(L,10, LUA_TNUMBER);
luaL_checktype(L,11, LUA_TNUMBER);
luaL_checktype(L,12, LUA_TNUMBER);
int render_tex_index;
render_tex_index = lua_tonumber(L,3);
unsigned int color;
color = (unsigned int)lua_tonumber(L,4);
int x;
x = lua_tonumber(L,5);
int y;
y = lua_tonumber(L,6);
int width;
width = lua_tonumber(L,7);
int height;
height = lua_tonumber(L,8);
int tex_x;
tex_x = lua_tonumber(L,9);
int tex_y;
tex_y = lua_tonumber(L,10);
int tex_width;
tex_width = lua_tonumber(L,11);
int tex_height;
tex_height = lua_tonumber(L,12);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderTexParam(render_tex_index , color , x , y , width , height , tex_x , tex_y , tex_width , tex_height );
return 12;
}
int KTROBO::Textures_setRenderBillBoardParamGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TTABLE);
luaL_checktype(L,6, LUA_TSTRING);
luaL_checktype(L,7, LUA_TSTRING);
luaL_checktype(L,8, LUA_TNUMBER);
luaL_checktype(L,9, LUA_TNUMBER);
luaL_checktype(L,10, LUA_TNUMBER);
luaL_checktype(L,11, LUA_TNUMBER);
int bill_index;
bill_index = lua_tonumber(L,3);
unsigned int color;
color = (unsigned int)lua_tonumber(L,4);
MYMATRIX world_m;
MYMATRIX* world=&world_m;
YARITORI_getMYMATRIX(L, world, 2);
float width;
const char* width_temp = lua_tostring(L,6);
width = atof(width_temp);
float height;
const char* height_temp = lua_tostring(L,7);
height = atof(height_temp);
int tex_x;
tex_x = lua_tonumber(L,8);
int tex_y;
tex_y = lua_tonumber(L,9);
int tex_width;
tex_width = lua_tonumber(L,10);
int tex_height;
tex_height = lua_tonumber(L,11);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderBillBoardParam(bill_index , color , world , width , height , tex_x , tex_y , tex_width , tex_height );
return 11;
}
int KTROBO::Textures_setRenderTexColorGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
int render_tex_index;
render_tex_index = lua_tonumber(L,3);
unsigned int color;
color = (unsigned int)lua_tonumber(L,4);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderTexColor(render_tex_index , color );
return 4;
}
int KTROBO::Textures_setRenderBillBoardColorGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
int bill_index;
bill_index = lua_tonumber(L,3);
unsigned int color;
color = (unsigned int)lua_tonumber(L,4);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderBillBoardColor(bill_index , color );
return 4;
}
int KTROBO::Textures_setRenderTexPosGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TNUMBER);
int render_tex_index;
render_tex_index = lua_tonumber(L,3);
int x;
x = lua_tonumber(L,4);
int y;
y = lua_tonumber(L,5);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderTexPos(render_tex_index , x , y );
return 5;
}
int KTROBO::Textures_setRenderBillBoardPosGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TTABLE);
int bill_index;
bill_index = lua_tonumber(L,3);
MYMATRIX world_m;
MYMATRIX* world=&world_m;
YARITORI_getMYMATRIX(L, world, 1);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderBillBoardPos(bill_index , world );
return 4;
}
int KTROBO::Textures_setRenderTexWHGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TNUMBER);
int render_tex_index;
render_tex_index = lua_tonumber(L,3);
int width;
width = lua_tonumber(L,4);
int height;
height = lua_tonumber(L,5);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderTexWH(render_tex_index , width , height );
return 5;
}
int KTROBO::Textures_setRenderBillBoardWHGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TSTRING);
luaL_checktype(L,5, LUA_TSTRING);
int bill_index;
bill_index = lua_tonumber(L,3);
float width;
const char* width_temp = lua_tostring(L,4);
width = atof(width_temp);
float height;
const char* height_temp = lua_tostring(L,5);
height = atof(height_temp);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderBillBoardWH(bill_index , width , height );
return 5;
}
int KTROBO::Textures_setRenderTexTexPosGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TNUMBER);
luaL_checktype(L,6, LUA_TNUMBER);
luaL_checktype(L,7, LUA_TNUMBER);
int render_tex_index;
render_tex_index = lua_tonumber(L,3);
int tex_x;
tex_x = lua_tonumber(L,4);
int tex_y;
tex_y = lua_tonumber(L,5);
int tex_width;
tex_width = lua_tonumber(L,6);
int tex_height;
tex_height = lua_tonumber(L,7);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderTexTexPos(render_tex_index , tex_x , tex_y , tex_width , tex_height );
return 7;
}
int KTROBO::Textures_setRenderBillBoardTexPosGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TNUMBER);
luaL_checktype(L,6, LUA_TNUMBER);
luaL_checktype(L,7, LUA_TNUMBER);
int bill_index;
bill_index = lua_tonumber(L,3);
int tex_x;
tex_x = lua_tonumber(L,4);
int tex_y;
tex_y = lua_tonumber(L,5);
int tex_width;
tex_width = lua_tonumber(L,6);
int tex_height;
tex_height = lua_tonumber(L,7);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderBillBoardTexPos(bill_index , tex_x , tex_y , tex_width , tex_height );
return 7;
}
int KTROBO::Textures_setRenderTexIsRenderGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TBOOLEAN);
int render_tex_index;
render_tex_index = lua_tonumber(L,3);
bool t;
t = (bool)lua_toboolean(L,4);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderTexIsRender(render_tex_index , t );
return 4;
}
int KTROBO::Textures_setRenderBillBoardIsRenderGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TBOOLEAN);
int bill_index;
bill_index = lua_tonumber(L,3);
bool t;
t = (bool)lua_toboolean(L,4);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderBillBoardIsRender(bill_index , t );
return 4;
}
int KTROBO::Textures_setRenderTexChangeTexGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TNUMBER);
int render_tex_index;
render_tex_index = lua_tonumber(L,3);
int tex_index;
tex_index = lua_tonumber(L,4);
int new_tex_index;
new_tex_index = lua_tonumber(L,5);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderTexChangeTex(render_tex_index , tex_index , new_tex_index );
return 5;
}
int KTROBO::Textures_setRenderBillBoardChangeTexGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TNUMBER);
int bill_index;
bill_index = lua_tonumber(L,3);
int tex_index;
tex_index = lua_tonumber(L,4);
int new_tex_index;
new_tex_index = lua_tonumber(L,5);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderBillBoardChangeTex(bill_index , tex_index , new_tex_index );
return 5;
}
int KTROBO::Textures_deleteRenderTexGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
int render_tex_index;
render_tex_index = lua_tonumber(L,3);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->deleteRenderTex(render_tex_index );
return 3;
}
int KTROBO::Textures_deleteRenderBillBoardGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
int bill_index;
bill_index = lua_tonumber(L,3);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->deleteRenderBillBoard(bill_index );
return 3;
}
int KTROBO::Textures_lightdeleteRenderTexGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
int render_tex_index;
render_tex_index = lua_tonumber(L,3);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->lightdeleteRenderTex(render_tex_index );
return 3;
}
int KTROBO::Textures_lightdeleteRenderBillBoardGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
int bill_index;
bill_index = lua_tonumber(L,3);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->lightdeleteRenderBillBoard(bill_index );
return 3;
}
int KTROBO::Textures_deleteAllGlue(lua_State* L) {
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->deleteAll();
return 2;
}
int KTROBO::Textures_getRenderTextGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TSTRING);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TNUMBER);
luaL_checktype(L,6, LUA_TNUMBER);
luaL_checktype(L,7, LUA_TNUMBER);
luaL_checktype(L,8, LUA_TNUMBER);
char t[1024];
memset(t,0, sizeof(char)* 1024);
mystrcpy(t,1024,0,lua_tostring(L,3));
int x;
x = lua_tonumber(L,4);
int y;
y = lua_tonumber(L,5);
int tex_h;
tex_h = lua_tonumber(L,6);
int width;
width = lua_tonumber(L,7);
int height;
height = lua_tonumber(L,8);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->getRenderText(t , x , y , tex_h , width , height );
lua_pushnumber(L, kaeriti);
lua_insert(L,1);
return 9;
}
int KTROBO::Textures_setRenderTextPosGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TNUMBER);
int text_id;
text_id = lua_tonumber(L,3);
int x;
x = lua_tonumber(L,4);
int y;
y = lua_tonumber(L,5);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderTextPos(text_id , x , y );
return 5;
}
int KTROBO::Textures_setRenderTextChangeTextGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TSTRING);
int text_id;
text_id = lua_tonumber(L,3);
char t[1024];
memset(t,0, sizeof(char)* 1024);
mystrcpy(t,1024,0,lua_tostring(L,4));
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderTextChangeText(text_id , t );
return 4;
}
int KTROBO::Textures_setRenderTextColorGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
int text_id;
text_id = lua_tonumber(L,3);
unsigned int color;
color = (unsigned int)lua_tonumber(L,4);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderTextColor(text_id , color );
return 4;
}
int KTROBO::Textures_setRenderTextIsRenderGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TBOOLEAN);
int text_id;
text_id = lua_tonumber(L,3);
bool t;
t = (bool)lua_toboolean(L,4);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->setRenderTextIsRender(text_id , t );
return 4;
}
int KTROBO::Textures_lightdeleteRenderTextGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
int text_id;
text_id = lua_tonumber(L,3);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->lightdeleteRenderText(text_id );
return 3;
}
int KTROBO::Textures_getRenderTextWidthGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
int text_id;
text_id = lua_tonumber(L,3);
int height;
height = lua_tonumber(L,4);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
float kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColTextures(ci)->getInterface(cci)->getRenderTextWidth(text_id , height );
{char tempp[1024];memset(tempp,0,1024);
sprintf_s(tempp, 1024, "%f",kaeriti);
lua_pushstring(L, tempp);
}lua_insert(L,1);
return 5;
}
int KTROBO::Messages_makeInstGlue(lua_State* L) {
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColMessages(ci)->makeInst();
{
int ci = lua_tonumber(L, 1);
int cci = kaeriti;
lua_newtable(L);
lua_pushnumber(L, 1);
lua_newtable(L);
int top = lua_gettop(L);
lua_pushstring(L, "Message");
lua_setfield(L, top, "collected_class_name");
{
char tempp[1024];memset(tempp,0,1024);
sprintf_s(tempp, 1024, "%d", cci);
lua_pushstring(L, tempp);
lua_setfield(L, top, "collected_index");
}
lua_pushstring(L, "IMessage");
lua_setfield(L, top, "collected_interface_name");
lua_pushstring(L, "Messages");
lua_setfield(L, top, "collection_class_name");
{
char tempp[1024];memset(tempp,0,1024);
sprintf_s(tempp, 1024, "%d", ci);
lua_pushstring(L, tempp);
lua_setfield(L, top, "collection_index");
}
lua_settable(L,-3);
lua_insert(L,1);
}
return 3;
}
int KTROBO::Messages_makeMessageGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TNUMBER);
luaL_checktype(L,6, LUA_TNUMBER);
luaL_checktype(L,7, LUA_TSTRING);
luaL_checktype(L,8, LUA_TBOOLEAN);
int msg_id;
msg_id = lua_tonumber(L,3);
int sender_id;
sender_id = lua_tonumber(L,4);
int receiver_id;
receiver_id = lua_tonumber(L,5);
int msg;
msg = lua_tonumber(L,6);
float fmsg;
const char* fmsg_temp = lua_tostring(L,7);
fmsg = atof(fmsg_temp);
bool is_need_to_send;
is_need_to_send = (bool)lua_toboolean(L,8);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColMessages(ci)->getInterface(cci)->makeMessage(msg_id , sender_id , receiver_id , msg , fmsg , is_need_to_send );
return 8;
}
int KTROBO::Messages_getMessageIndexsFromMsgIdGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TTABLE);
int msg_id;
msg_id = lua_tonumber(L,3);
int get_msgs[32];
memset(get_msgs,0, sizeof(int)* 32);
{
int temp = lua_rawlen(L,4);
for (int i=0;i<temp && i < 32;i++) {
  lua_pushnumber(L, i+1);
  lua_gettable(L, 4);
  get_msgs[i] = (lua_tonumber(L, -1));
  lua_remove(L,-1);
}
}
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColMessages(ci)->getInterface(cci)->getMessageIndexsFromMsgId(msg_id , get_msgs );
{
int temp = lua_rawlen(L,4);
for (int i=0;i<temp && i < 32;i++) {
  char tempp[1024];memset(tempp,0,1024);
  sprintf_s(tempp, 1024, "%d", get_msgs[i]);
  lua_pushnumber(L, i+1);
  lua_pushstring(L, tempp);
  lua_settable(L, 4);
}
}
lua_pushnumber(L, kaeriti);
lua_insert(L,1);
return 5;
}
int KTROBO::Messages_getMessageIndexsFromReceiverIdGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TTABLE);
int receiver_id;
receiver_id = lua_tonumber(L,3);
int get_msgs[32];
memset(get_msgs,0, sizeof(int)* 32);
{
int temp = lua_rawlen(L,4);
for (int i=0;i<temp && i < 32;i++) {
  lua_pushnumber(L, i+1);
  lua_gettable(L, 4);
  get_msgs[i] = (lua_tonumber(L, -1));
  lua_remove(L,-1);
}
}
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColMessages(ci)->getInterface(cci)->getMessageIndexsFromReceiverId(receiver_id , get_msgs );
{
int temp = lua_rawlen(L,4);
for (int i=0;i<temp && i < 32;i++) {
  char tempp[1024];memset(tempp,0,1024);
  sprintf_s(tempp, 1024, "%d", get_msgs[i]);
  lua_pushnumber(L, i+1);
  lua_pushstring(L, tempp);
  lua_settable(L, 4);
}
}
lua_pushnumber(L, kaeriti);
lua_insert(L,1);
return 5;
}
int KTROBO::Messages_getMsgBodyGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TTABLE);
luaL_checktype(L,5, LUA_TTABLE);
luaL_checktype(L,6, LUA_TTABLE);
luaL_checktype(L,7, LUA_TTABLE);
luaL_checktype(L,8, LUA_TTABLE);
luaL_checktype(L,9, LUA_TTABLE);
int mindex_len;
mindex_len = lua_tonumber(L,3);
int msg_indexs[32];
memset(msg_indexs,0, sizeof(int)* 32);
{
int temp = lua_rawlen(L,4);
for (int i=0;i<temp && i < 32;i++) {
  lua_pushnumber(L, i+1);
  lua_gettable(L, 4);
  msg_indexs[i] = (lua_tonumber(L, -1));
  lua_remove(L,-1);
}
}
int msg_ids[32];
memset(msg_ids,0, sizeof(int)* 32);
{
int temp = lua_rawlen(L,5);
for (int i=0;i<temp && i < 32;i++) {
  lua_pushnumber(L, i+1);
  lua_gettable(L, 5);
  msg_ids[i] = (lua_tonumber(L, -1));
  lua_remove(L,-1);
}
}
int sender_ids[32];
memset(sender_ids,0, sizeof(int)* 32);
{
int temp = lua_rawlen(L,6);
for (int i=0;i<temp && i < 32;i++) {
  lua_pushnumber(L, i+1);
  lua_gettable(L, 6);
  sender_ids[i] = (lua_tonumber(L, -1));
  lua_remove(L,-1);
}
}
int receiver_ids[32];
memset(receiver_ids,0, sizeof(int)* 32);
{
int temp = lua_rawlen(L,7);
for (int i=0;i<temp && i < 32;i++) {
  lua_pushnumber(L, i+1);
  lua_gettable(L, 7);
  receiver_ids[i] = (lua_tonumber(L, -1));
  lua_remove(L,-1);
}
}
int msgs[32];
memset(msgs,0, sizeof(int)* 32);
{
int temp = lua_rawlen(L,8);
for (int i=0;i<temp && i < 32;i++) {
  lua_pushnumber(L, i+1);
  lua_gettable(L, 8);
  msgs[i] = (lua_tonumber(L, -1));
  lua_remove(L,-1);
}
}
float fmsgs[32];
memset(fmsgs,0, sizeof(float)* 32);
{
int temp = lua_rawlen(L,9);
for (int i=0;i<temp && i < 32;i++) {
  lua_pushnumber(L, i+1);
  lua_gettable(L, 9);
  fmsgs[i] = atof(lua_tostring(L, -1));
  lua_remove(L,-1);
}
}
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColMessages(ci)->getInterface(cci)->getMsgBody(mindex_len , msg_indexs , msg_ids , sender_ids , receiver_ids , msgs , fmsgs );
{
int temp = lua_rawlen(L,5);
for (int i=0;i<temp && i < 32;i++) {
  char tempp[1024];memset(tempp,0,1024);
  sprintf_s(tempp, 1024, "%d", msg_ids[i]);
  lua_pushnumber(L, i+1);
  lua_pushstring(L, tempp);
  lua_settable(L, 5);
}
}
{
int temp = lua_rawlen(L,6);
for (int i=0;i<temp && i < 32;i++) {
  char tempp[1024];memset(tempp,0,1024);
  sprintf_s(tempp, 1024, "%d", sender_ids[i]);
  lua_pushnumber(L, i+1);
  lua_pushstring(L, tempp);
  lua_settable(L, 6);
}
}
{
int temp = lua_rawlen(L,7);
for (int i=0;i<temp && i < 32;i++) {
  char tempp[1024];memset(tempp,0,1024);
  sprintf_s(tempp, 1024, "%d", receiver_ids[i]);
  lua_pushnumber(L, i+1);
  lua_pushstring(L, tempp);
  lua_settable(L, 7);
}
}
{
int temp = lua_rawlen(L,8);
for (int i=0;i<temp && i < 32;i++) {
  char tempp[1024];memset(tempp,0,1024);
  sprintf_s(tempp, 1024, "%d", msgs[i]);
  lua_pushnumber(L, i+1);
  lua_pushstring(L, tempp);
  lua_settable(L, 8);
}
}
{
int temp = lua_rawlen(L,9);
for (int i=0;i<temp && i < 32;i++) {
  char tempp[1024];memset(tempp,0,1024);
  sprintf_s(tempp, 1024, "%f", fmsgs[i]);
  lua_pushnumber(L, i+1);
  lua_pushstring(L, tempp);
  lua_settable(L, 9);
}
}
return 9;
}
int KTROBO::EffectManagers_makeInstGlue(lua_State* L) {
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColEffectManagers(ci)->makeInst();
{
int ci = lua_tonumber(L, 1);
int cci = kaeriti;
lua_newtable(L);
lua_pushnumber(L, 1);
lua_newtable(L);
int top = lua_gettop(L);
lua_pushstring(L, "EffectManager");
lua_setfield(L, top, "collected_class_name");
{
char tempp[1024];memset(tempp,0,1024);
sprintf_s(tempp, 1024, "%d", cci);
lua_pushstring(L, tempp);
lua_setfield(L, top, "collected_index");
}
lua_pushstring(L, "IEffectManager");
lua_setfield(L, top, "collected_interface_name");
lua_pushstring(L, "EffectManagers");
lua_setfield(L, top, "collection_class_name");
{
char tempp[1024];memset(tempp,0,1024);
sprintf_s(tempp, 1024, "%d", ci);
lua_pushstring(L, tempp);
lua_setfield(L, top, "collection_index");
}
lua_settable(L,-3);
lua_insert(L,1);
}
return 3;
}
int KTROBO::EffectManagers_getEffectGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TSTRING);
luaL_checktype(L,4, LUA_TSTRING);
luaL_checktype(L,5, LUA_TSTRING);
char effect_name[1024];
memset(effect_name,0, sizeof(char)* 1024);
mystrcpy(effect_name,1024,0,lua_tostring(L,3));
char texture_name[1024];
memset(texture_name,0, sizeof(char)* 1024);
mystrcpy(texture_name,1024,0,lua_tostring(L,4));
char lua_file_name[1024];
memset(lua_file_name,0, sizeof(char)* 1024);
mystrcpy(lua_file_name,1024,0,lua_tostring(L,5));
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColEffectManagers(ci)->getInterface(cci)->getEffect(effect_name , texture_name , lua_file_name );
lua_pushnumber(L, kaeriti);
lua_insert(L,1);
return 6;
}
int KTROBO::EffectManagers_makeEffectPartGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TSTRING);
luaL_checktype(L,6, LUA_TBOOLEAN);
luaL_checktype(L,7, LUA_TSTRING);
luaL_checktype(L,8, LUA_TSTRING);
int effect_id;
effect_id = lua_tonumber(L,3);
int index;
index = lua_tonumber(L,4);
float endtime;
const char* endtime_temp = lua_tostring(L,5);
endtime = atof(endtime_temp);
bool is_loop;
is_loop = (bool)lua_toboolean(L,6);
float time_when_loop;
const char* time_when_loop_temp = lua_tostring(L,7);
time_when_loop = atof(time_when_loop_temp);
float plustime_loop;
const char* plustime_loop_temp = lua_tostring(L,8);
plustime_loop = atof(plustime_loop_temp);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColEffectManagers(ci)->getInterface(cci)->makeEffectPart(effect_id , index , endtime , is_loop , time_when_loop , plustime_loop );
return 8;
}
int KTROBO::EffectManagers_setEffectPartPosGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TSTRING);
luaL_checktype(L,6, LUA_TSTRING);
luaL_checktype(L,7, LUA_TSTRING);
luaL_checktype(L,8, LUA_TSTRING);
luaL_checktype(L,9, LUA_TSTRING);
luaL_checktype(L,10, LUA_TSTRING);
luaL_checktype(L,11, LUA_TSTRING);
luaL_checktype(L,12, LUA_TSTRING);
int effect_id;
effect_id = lua_tonumber(L,3);
int index;
index = lua_tonumber(L,4);
float dtime_start;
const char* dtime_start_temp = lua_tostring(L,5);
dtime_start = atof(dtime_start_temp);
float dtime_end;
const char* dtime_end_temp = lua_tostring(L,6);
dtime_end = atof(dtime_end_temp);
float start_x;
const char* start_x_temp = lua_tostring(L,7);
start_x = atof(start_x_temp);
float start_y;
const char* start_y_temp = lua_tostring(L,8);
start_y = atof(start_y_temp);
float start_z;
const char* start_z_temp = lua_tostring(L,9);
start_z = atof(start_z_temp);
float end_x;
const char* end_x_temp = lua_tostring(L,10);
end_x = atof(end_x_temp);
float end_y;
const char* end_y_temp = lua_tostring(L,11);
end_y = atof(end_y_temp);
float end_z;
const char* end_z_temp = lua_tostring(L,12);
end_z = atof(end_z_temp);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColEffectManagers(ci)->getInterface(cci)->setEffectPartPos(effect_id , index , dtime_start , dtime_end , start_x , start_y , start_z , end_x , end_y , end_z );
return 12;
}
int KTROBO::EffectManagers_setEffectPartTexPosGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TSTRING);
luaL_checktype(L,6, LUA_TSTRING);
luaL_checktype(L,7, LUA_TNUMBER);
luaL_checktype(L,8, LUA_TNUMBER);
luaL_checktype(L,9, LUA_TNUMBER);
luaL_checktype(L,10, LUA_TNUMBER);
int effect_id;
effect_id = lua_tonumber(L,3);
int index;
index = lua_tonumber(L,4);
float dtime_start;
const char* dtime_start_temp = lua_tostring(L,5);
dtime_start = atof(dtime_start_temp);
float dtime_end;
const char* dtime_end_temp = lua_tostring(L,6);
dtime_end = atof(dtime_end_temp);
int tex_x;
tex_x = lua_tonumber(L,7);
int tex_y;
tex_y = lua_tonumber(L,8);
int tex_width;
tex_width = lua_tonumber(L,9);
int tex_height;
tex_height = lua_tonumber(L,10);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColEffectManagers(ci)->getInterface(cci)->setEffectPartTexPos(effect_id , index , dtime_start , dtime_end , tex_x , tex_y , tex_width , tex_height );
return 10;
}
int KTROBO::EffectManagers_setEffectPartWHGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TSTRING);
luaL_checktype(L,6, LUA_TSTRING);
luaL_checktype(L,7, LUA_TSTRING);
luaL_checktype(L,8, LUA_TSTRING);
luaL_checktype(L,9, LUA_TSTRING);
luaL_checktype(L,10, LUA_TSTRING);
int effect_id;
effect_id = lua_tonumber(L,3);
int index;
index = lua_tonumber(L,4);
float dtime_start;
const char* dtime_start_temp = lua_tostring(L,5);
dtime_start = atof(dtime_start_temp);
float dtime_end;
const char* dtime_end_temp = lua_tostring(L,6);
dtime_end = atof(dtime_end_temp);
float start_width;
const char* start_width_temp = lua_tostring(L,7);
start_width = atof(start_width_temp);
float start_height;
const char* start_height_temp = lua_tostring(L,8);
start_height = atof(start_height_temp);
float end_width;
const char* end_width_temp = lua_tostring(L,9);
end_width = atof(end_width_temp);
float height;
const char* height_temp = lua_tostring(L,10);
height = atof(height_temp);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColEffectManagers(ci)->getInterface(cci)->setEffectPartWH(effect_id , index , dtime_start , dtime_end , start_width , start_height , end_width , height );
return 10;
}
int KTROBO::EffectManagers_setEffectPartRotGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TSTRING);
luaL_checktype(L,6, LUA_TSTRING);
luaL_checktype(L,7, LUA_TSTRING);
luaL_checktype(L,8, LUA_TSTRING);
luaL_checktype(L,9, LUA_TSTRING);
luaL_checktype(L,10, LUA_TSTRING);
luaL_checktype(L,11, LUA_TSTRING);
luaL_checktype(L,12, LUA_TSTRING);
int effect_id;
effect_id = lua_tonumber(L,3);
int index;
index = lua_tonumber(L,4);
float dtime_start;
const char* dtime_start_temp = lua_tostring(L,5);
dtime_start = atof(dtime_start_temp);
float dtime_end;
const char* dtime_end_temp = lua_tostring(L,6);
dtime_end = atof(dtime_end_temp);
float start_rotx;
const char* start_rotx_temp = lua_tostring(L,7);
start_rotx = atof(start_rotx_temp);
float start_roty;
const char* start_roty_temp = lua_tostring(L,8);
start_roty = atof(start_roty_temp);
float start_rotz;
const char* start_rotz_temp = lua_tostring(L,9);
start_rotz = atof(start_rotz_temp);
float end_rotx;
const char* end_rotx_temp = lua_tostring(L,10);
end_rotx = atof(end_rotx_temp);
float end_roty;
const char* end_roty_temp = lua_tostring(L,11);
end_roty = atof(end_roty_temp);
float end_rotz;
const char* end_rotz_temp = lua_tostring(L,12);
end_rotz = atof(end_rotz_temp);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColEffectManagers(ci)->getInterface(cci)->setEffectPartRot(effect_id , index , dtime_start , dtime_end , start_rotx , start_roty , start_rotz , end_rotx , end_roty , end_rotz );
return 12;
}
int KTROBO::EffectManagers_setEffectPartColorGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TNUMBER);
luaL_checktype(L,5, LUA_TSTRING);
luaL_checktype(L,6, LUA_TSTRING);
luaL_checktype(L,7, LUA_TNUMBER);
luaL_checktype(L,8, LUA_TNUMBER);
int effect_id;
effect_id = lua_tonumber(L,3);
int index;
index = lua_tonumber(L,4);
float dtime_start;
const char* dtime_start_temp = lua_tostring(L,5);
dtime_start = atof(dtime_start_temp);
float dtime_end;
const char* dtime_end_temp = lua_tostring(L,6);
dtime_end = atof(dtime_end_temp);
unsigned int start_color;
start_color = (unsigned int)lua_tonumber(L,7);
unsigned int end_color;
end_color = (unsigned int)lua_tonumber(L,8);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColEffectManagers(ci)->getInterface(cci)->setEffectPartColor(effect_id , index , dtime_start , dtime_end , start_color , end_color );
return 8;
}
int KTROBO::EffectManagers_deleteEffectGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
int effect_id;
effect_id = lua_tonumber(L,3);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColEffectManagers(ci)->getInterface(cci)->deleteEffect(effect_id );
return 3;
}
int KTROBO::EffectManagers_getEffectImplGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
int effect_id;
effect_id = lua_tonumber(L,3);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
int kaeriti = KTROBO::MyLuaGlueSingleton::getInstance()->getColEffectManagers(ci)->getInterface(cci)->getEffectImpl(effect_id );
lua_pushnumber(L, kaeriti);
lua_insert(L,1);
return 4;
}
int KTROBO::EffectManagers_setEffectImplWorldGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TTABLE);
int effect_impl_id;
effect_impl_id = lua_tonumber(L,3);
MYMATRIX world_m;
MYMATRIX* world=&world_m;
YARITORI_getMYMATRIX(L, world, 1);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColEffectManagers(ci)->getInterface(cci)->setEffectImplWorld(effect_impl_id , world );
return 4;
}
int KTROBO::EffectManagers_setEffectImplTimeGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TSTRING);
int effect_impl_id;
effect_impl_id = lua_tonumber(L,3);
float time;
const char* time_temp = lua_tostring(L,4);
time = atof(time_temp);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColEffectManagers(ci)->getInterface(cci)->setEffectImplTime(effect_impl_id , time );
return 4;
}
int KTROBO::EffectManagers_setEffectImplIsRenderGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TBOOLEAN);
int effect_impl_id;
effect_impl_id = lua_tonumber(L,3);
bool t;
t = (bool)lua_toboolean(L,4);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColEffectManagers(ci)->getInterface(cci)->setEffectImplIsRender(effect_impl_id , t );
return 4;
}
int KTROBO::EffectManagers_setEffectImplIsStartGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
luaL_checktype(L,4, LUA_TBOOLEAN);
int effect_impl_id;
effect_impl_id = lua_tonumber(L,3);
bool t;
t = (bool)lua_toboolean(L,4);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColEffectManagers(ci)->getInterface(cci)->setEffectImplIsStart(effect_impl_id , t );
return 4;
}
int KTROBO::EffectManagers_lightdeleteEffectImplGlue(lua_State* L) {
luaL_checktype(L,3, LUA_TNUMBER);
int effect_impl_id;
effect_impl_id = lua_tonumber(L,3);
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColEffectManagers(ci)->getInterface(cci)->lightdeleteEffectImpl(effect_impl_id );
return 3;
}
int KTROBO::EffectManagers_deleteEffectImplGlue(lua_State* L) {
int ci = lua_tonumber(L, 1);
int cci = lua_tonumber(L, 2);
KTROBO::MyLuaGlueSingleton::getInstance()->getColEffectManagers(ci)->getInterface(cci)->deleteEffectImpl();
return 2;
}
