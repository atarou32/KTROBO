#ifndef TOLUAGLUEFILE_H
#define TOLUAGLUEFILE_H
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


 void YARITORI_getMYMATRIX(lua_State* L, MYMATRIX* class_inst, int index);
 void YARITORI_setMYMATRIX(lua_State* L, MYMATRIX* class_inst, int index);
 void YARITORI_setKAERITIMYMATRIX(lua_State* L, MYMATRIX* class_inst);
 void YARITORI_getMYVECTOR4(lua_State* L, MYVECTOR4* class_inst, int index);
 void YARITORI_setMYVECTOR4(lua_State* L, MYVECTOR4* class_inst, int index);
 void YARITORI_setKAERITIMYVECTOR4(lua_State* L, MYVECTOR4* class_inst);



/////////////////////////////////////////////
int MeshInstanceds_makeInstancedIDGlue(lua_State* L);
int MeshInstanceds_setBoneIndexInfoGlue(lua_State* L);
int MeshInstanceds_setWorldGlue(lua_State* L);
int MeshInstanceds_getWorldGlue(lua_State* L);
int MeshInstanceds_setColorGlue(lua_State* L);
int MeshInstanceds_getColorGlue(lua_State* L);
int MeshInstanceds_setIsRenderGlue(lua_State* L);
int MeshInstanceds_getIsRenderGlue(lua_State* L);
int MeshInstanceds_getInstanceIndexGlue(lua_State* L);
int TextFromLuas_makeInstanceGlue(lua_State* L);
int TextFromLuas_setDebugTextGlue(lua_State* L);
int TextFromLuas_addGlue(lua_State* L);
int TextFromLuas_removeSceneGlue(lua_State* L);
int TextFromLuas_enterTWOTAKUGlue(lua_State* L);
int TextFromLuas_enterONEMESSAGEGlue(lua_State* L);
int TextFromLuas_enterLOADTYUUGlue(lua_State* L);
int TextFromLuas_makeAILuaGlue(lua_State* L);
int TextFromLuas_makeButukariLuaGlue(lua_State* L);
int TextFromLuas_makeLoadLuaGlue(lua_State* L);
int TextFromLuas_makeAnimeLuaGlue(lua_State* L);
int TextFromLuas_makeRenderLuaGlue(lua_State* L);
int CMeshs_makeInstanceGlue(lua_State* L);
int CMeshs_getMeshGlue(lua_State* L);
int CMeshs_getMeshWithAnimeGlue(lua_State* L);
int CMeshs_getIsLoadGlue(lua_State* L);
int CMeshs_setIsLoadGlue(lua_State* L);
int CMeshs_deleteMeshGlue(lua_State* L);
int CMeshs_deleteALLGlue(lua_State* L);
int CMeshs_copyDAYOGlue(lua_State* L);
int Textures_makeInstGlue(lua_State* L);
int Textures_getTextureGlue(lua_State* L);
int Textures_getRenderTexGlue(lua_State* L);
int Textures_getRenderBillBoardGlue(lua_State* L);
int Textures_setRenderTexParamGlue(lua_State* L);
int Textures_setRenderBillBoardParamGlue(lua_State* L);
int Textures_setRenderTexColorGlue(lua_State* L);
int Textures_setRenderBillBoardColorGlue(lua_State* L);
int Textures_setRenderTexPosGlue(lua_State* L);
int Textures_setRenderBillBoardPosGlue(lua_State* L);
int Textures_setRenderTexWHGlue(lua_State* L);
int Textures_setRenderBillBoardWHGlue(lua_State* L);
int Textures_setRenderTexTexPosGlue(lua_State* L);
int Textures_setRenderBillBoardTexPosGlue(lua_State* L);
int Textures_setRenderTexIsRenderGlue(lua_State* L);
int Textures_setRenderBillBoardIsRenderGlue(lua_State* L);
int Textures_setRenderTexChangeTexGlue(lua_State* L);
int Textures_setRenderBillBoardChangeTexGlue(lua_State* L);
int Textures_deleteRenderTexGlue(lua_State* L);
int Textures_deleteRenderBillBoardGlue(lua_State* L);
int Textures_lightdeleteRenderTexGlue(lua_State* L);
int Textures_lightdeleteRenderBillBoardGlue(lua_State* L);
int Textures_deleteAllGlue(lua_State* L);
int Textures_getRenderTextGlue(lua_State* L);
int Textures_setRenderTextPosGlue(lua_State* L);
int Textures_setRenderTextChangeTextGlue(lua_State* L);
int Textures_setRenderTextColorGlue(lua_State* L);
int Textures_setRenderTextIsRenderGlue(lua_State* L);
int Textures_lightdeleteRenderTextGlue(lua_State* L);
int Textures_getRenderTextWidthGlue(lua_State* L);
int Messages_makeInstGlue(lua_State* L);
int Messages_makeMessageGlue(lua_State* L);
int Messages_getMessageIndexsFromMsgIdGlue(lua_State* L);
int Messages_getMessageIndexsFromReceiverIdGlue(lua_State* L);
int Messages_getMsgBodyGlue(lua_State* L);
int SinaiFuruAnimeMakers_makeInstGlue(lua_State* L);
int SinaiFuruAnimeMakers_togglePlayAnimeGlue(lua_State* L);
int SinaiFuruAnimeMakers_nigiruhitoSetAnimeGlue(lua_State* L);
int SinaiFuruAnimeMakers_nigiruhitoEraseAnimeGlue(lua_State* L);
int SinaiFuruAnimeMakers_sinaiEraseAnimeGlue(lua_State* L);
int SinaiFuruAnimeMakers_sinaiSetAnimeGlue(lua_State* L);
int SinaiFuruAnimeMakers_undoGlue(lua_State* L);
int SinaiFuruAnimeMakers_sinaiNigiruhitoUpdateGlue(lua_State* L);
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
  vector<TextFromLuas*> m_TextFromLuas;
  vector<CMeshs*> m_CMeshs;
  vector<Textures*> m_Textures;
  vector<Messages*> m_Messages;
  vector<SinaiFuruAnimeMakers*> m_SinaiFuruAnimeMakers;
 public:
MeshInstanceds* getColMeshInstanceds ( int index) {
  int size = m_MeshInstanceds.size();
  if (size > index && index >=0) {
    return m_MeshInstanceds[index];
  } else {
    throw new GameError(KTROBO::WARNING, " try to access outside vector MeshInstanceds");
  }
}
void setColMeshInstanceds ( MeshInstanceds* m) {
  m_MeshInstanceds.push_back(m);
}
TextFromLuas* getColTextFromLuas ( int index) {
  int size = m_TextFromLuas.size();
  if (size > index && index >=0) {
    return m_TextFromLuas[index];
  } else {
    throw new GameError(KTROBO::WARNING, " try to access outside vector TextFromLuas");
  }
}
void setColTextFromLuas ( TextFromLuas* m) {
  m_TextFromLuas.push_back(m);
}
CMeshs* getColCMeshs ( int index) {
  int size = m_CMeshs.size();
  if (size > index && index >=0) {
    return m_CMeshs[index];
  } else {
    throw new GameError(KTROBO::WARNING, " try to access outside vector CMeshs");
  }
}
void setColCMeshs ( CMeshs* m) {
  m_CMeshs.push_back(m);
}
Textures* getColTextures ( int index) {
  int size = m_Textures.size();
  if (size > index && index >=0) {
    return m_Textures[index];
  } else {
    throw new GameError(KTROBO::WARNING, " try to access outside vector Textures");
  }
}
void setColTextures ( Textures* m) {
  m_Textures.push_back(m);
}
Messages* getColMessages ( int index) {
  int size = m_Messages.size();
  if (size > index && index >=0) {
    return m_Messages[index];
  } else {
    throw new GameError(KTROBO::WARNING, " try to access outside vector Messages");
  }
}
void setColMessages ( Messages* m) {
  m_Messages.push_back(m);
}
SinaiFuruAnimeMakers* getColSinaiFuruAnimeMakers ( int index) {
  int size = m_SinaiFuruAnimeMakers.size();
  if (size > index && index >=0) {
    return m_SinaiFuruAnimeMakers[index];
  } else {
    throw new GameError(KTROBO::WARNING, " try to access outside vector SinaiFuruAnimeMakers");
  }
}
void setColSinaiFuruAnimeMakers ( SinaiFuruAnimeMakers* m) {
  m_SinaiFuruAnimeMakers.push_back(m);
}
void registerdayo(lua_State* L) {
  lua_register(L, "MeshInstanceds_setBoneIndexInfo", KTROBO::MeshInstanceds_setBoneIndexInfoGlue);
  lua_register(L, "MeshInstanceds_setWorld", KTROBO::MeshInstanceds_setWorldGlue);
  lua_register(L, "MeshInstanceds_getWorld", KTROBO::MeshInstanceds_getWorldGlue);
  lua_register(L, "MeshInstanceds_setColor", KTROBO::MeshInstanceds_setColorGlue);
  lua_register(L, "MeshInstanceds_getColor", KTROBO::MeshInstanceds_getColorGlue);
  lua_register(L, "MeshInstanceds_setIsRender", KTROBO::MeshInstanceds_setIsRenderGlue);
  lua_register(L, "MeshInstanceds_getIsRender", KTROBO::MeshInstanceds_getIsRenderGlue);
  lua_register(L, "MeshInstanceds_getInstanceIndex", KTROBO::MeshInstanceds_getInstanceIndexGlue);
  lua_register(L, "MeshInstanceds_makeInstancedID", KTROBO::MeshInstanceds_makeInstancedIDGlue);
  lua_register(L, "TextFromLuas_setDebugText", KTROBO::TextFromLuas_setDebugTextGlue);
  lua_register(L, "TextFromLuas_add", KTROBO::TextFromLuas_addGlue);
  lua_register(L, "TextFromLuas_removeScene", KTROBO::TextFromLuas_removeSceneGlue);
  lua_register(L, "TextFromLuas_enterTWOTAKU", KTROBO::TextFromLuas_enterTWOTAKUGlue);
  lua_register(L, "TextFromLuas_enterONEMESSAGE", KTROBO::TextFromLuas_enterONEMESSAGEGlue);
  lua_register(L, "TextFromLuas_enterLOADTYUU", KTROBO::TextFromLuas_enterLOADTYUUGlue);
  lua_register(L, "TextFromLuas_makeAILua", KTROBO::TextFromLuas_makeAILuaGlue);
  lua_register(L, "TextFromLuas_makeButukariLua", KTROBO::TextFromLuas_makeButukariLuaGlue);
  lua_register(L, "TextFromLuas_makeLoadLua", KTROBO::TextFromLuas_makeLoadLuaGlue);
  lua_register(L, "TextFromLuas_makeAnimeLua", KTROBO::TextFromLuas_makeAnimeLuaGlue);
  lua_register(L, "TextFromLuas_makeRenderLua", KTROBO::TextFromLuas_makeRenderLuaGlue);
  lua_register(L, "TextFromLuas_makeInstance", KTROBO::TextFromLuas_makeInstanceGlue);
  lua_register(L, "CMeshs_getMesh", KTROBO::CMeshs_getMeshGlue);
  lua_register(L, "CMeshs_getMeshWithAnime", KTROBO::CMeshs_getMeshWithAnimeGlue);
  lua_register(L, "CMeshs_getIsLoad", KTROBO::CMeshs_getIsLoadGlue);
  lua_register(L, "CMeshs_setIsLoad", KTROBO::CMeshs_setIsLoadGlue);
  lua_register(L, "CMeshs_deleteMesh", KTROBO::CMeshs_deleteMeshGlue);
  lua_register(L, "CMeshs_deleteALL", KTROBO::CMeshs_deleteALLGlue);
  lua_register(L, "CMeshs_copyDAYO", KTROBO::CMeshs_copyDAYOGlue);
  lua_register(L, "CMeshs_makeInstance", KTROBO::CMeshs_makeInstanceGlue);
  lua_register(L, "Textures_getTexture", KTROBO::Textures_getTextureGlue);
  lua_register(L, "Textures_getRenderTex", KTROBO::Textures_getRenderTexGlue);
  lua_register(L, "Textures_getRenderBillBoard", KTROBO::Textures_getRenderBillBoardGlue);
  lua_register(L, "Textures_setRenderTexParam", KTROBO::Textures_setRenderTexParamGlue);
  lua_register(L, "Textures_setRenderBillBoardParam", KTROBO::Textures_setRenderBillBoardParamGlue);
  lua_register(L, "Textures_setRenderTexColor", KTROBO::Textures_setRenderTexColorGlue);
  lua_register(L, "Textures_setRenderBillBoardColor", KTROBO::Textures_setRenderBillBoardColorGlue);
  lua_register(L, "Textures_setRenderTexPos", KTROBO::Textures_setRenderTexPosGlue);
  lua_register(L, "Textures_setRenderBillBoardPos", KTROBO::Textures_setRenderBillBoardPosGlue);
  lua_register(L, "Textures_setRenderTexWH", KTROBO::Textures_setRenderTexWHGlue);
  lua_register(L, "Textures_setRenderBillBoardWH", KTROBO::Textures_setRenderBillBoardWHGlue);
  lua_register(L, "Textures_setRenderTexTexPos", KTROBO::Textures_setRenderTexTexPosGlue);
  lua_register(L, "Textures_setRenderBillBoardTexPos", KTROBO::Textures_setRenderBillBoardTexPosGlue);
  lua_register(L, "Textures_setRenderTexIsRender", KTROBO::Textures_setRenderTexIsRenderGlue);
  lua_register(L, "Textures_setRenderBillBoardIsRender", KTROBO::Textures_setRenderBillBoardIsRenderGlue);
  lua_register(L, "Textures_setRenderTexChangeTex", KTROBO::Textures_setRenderTexChangeTexGlue);
  lua_register(L, "Textures_setRenderBillBoardChangeTex", KTROBO::Textures_setRenderBillBoardChangeTexGlue);
  lua_register(L, "Textures_deleteRenderTex", KTROBO::Textures_deleteRenderTexGlue);
  lua_register(L, "Textures_deleteRenderBillBoard", KTROBO::Textures_deleteRenderBillBoardGlue);
  lua_register(L, "Textures_lightdeleteRenderTex", KTROBO::Textures_lightdeleteRenderTexGlue);
  lua_register(L, "Textures_lightdeleteRenderBillBoard", KTROBO::Textures_lightdeleteRenderBillBoardGlue);
  lua_register(L, "Textures_deleteAll", KTROBO::Textures_deleteAllGlue);
  lua_register(L, "Textures_getRenderText", KTROBO::Textures_getRenderTextGlue);
  lua_register(L, "Textures_setRenderTextPos", KTROBO::Textures_setRenderTextPosGlue);
  lua_register(L, "Textures_setRenderTextChangeText", KTROBO::Textures_setRenderTextChangeTextGlue);
  lua_register(L, "Textures_setRenderTextColor", KTROBO::Textures_setRenderTextColorGlue);
  lua_register(L, "Textures_setRenderTextIsRender", KTROBO::Textures_setRenderTextIsRenderGlue);
  lua_register(L, "Textures_lightdeleteRenderText", KTROBO::Textures_lightdeleteRenderTextGlue);
  lua_register(L, "Textures_getRenderTextWidth", KTROBO::Textures_getRenderTextWidthGlue);
  lua_register(L, "Textures_makeInst", KTROBO::Textures_makeInstGlue);
  lua_register(L, "Messages_makeMessage", KTROBO::Messages_makeMessageGlue);
  lua_register(L, "Messages_getMessageIndexsFromMsgId", KTROBO::Messages_getMessageIndexsFromMsgIdGlue);
  lua_register(L, "Messages_getMessageIndexsFromReceiverId", KTROBO::Messages_getMessageIndexsFromReceiverIdGlue);
  lua_register(L, "Messages_getMsgBody", KTROBO::Messages_getMsgBodyGlue);
  lua_register(L, "Messages_makeInst", KTROBO::Messages_makeInstGlue);
  lua_register(L, "SinaiFuruAnimeMakers_togglePlayAnime", KTROBO::SinaiFuruAnimeMakers_togglePlayAnimeGlue);
  lua_register(L, "SinaiFuruAnimeMakers_nigiruhitoSetAnime", KTROBO::SinaiFuruAnimeMakers_nigiruhitoSetAnimeGlue);
  lua_register(L, "SinaiFuruAnimeMakers_nigiruhitoEraseAnime", KTROBO::SinaiFuruAnimeMakers_nigiruhitoEraseAnimeGlue);
  lua_register(L, "SinaiFuruAnimeMakers_sinaiEraseAnime", KTROBO::SinaiFuruAnimeMakers_sinaiEraseAnimeGlue);
  lua_register(L, "SinaiFuruAnimeMakers_sinaiSetAnime", KTROBO::SinaiFuruAnimeMakers_sinaiSetAnimeGlue);
  lua_register(L, "SinaiFuruAnimeMakers_undo", KTROBO::SinaiFuruAnimeMakers_undoGlue);
  lua_register(L, "SinaiFuruAnimeMakers_sinaiNigiruhitoUpdate", KTROBO::SinaiFuruAnimeMakers_sinaiNigiruhitoUpdateGlue);
  lua_register(L, "SinaiFuruAnimeMakers_makeInst", KTROBO::SinaiFuruAnimeMakers_makeInstGlue);
}};
}
#endif
