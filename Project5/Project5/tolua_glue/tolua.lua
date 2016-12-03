MeshInstanced = MeshInstanced or { count =0 }
function MeshInstanced:makeInstancedID(collection_index, ...)
  local t = MeshInstanceds_makeInstancedID(collection_index,0, ...)
  MeshInstanced.count = MeshInstanced.count+1
  setmetatable(t, {__index = MeshInstanced})
  return t
end
function MeshInstanced:getIS(c_index, cd_index)
  local t ={}
  t[1] = {}
  t[1].collection_index = c_index
  t[1].collected_index = cd_index
  t[1].collected_class_name = "MeshInstanced"
  t[1].collection_class_name = "MeshInstanceds"
  t[1].collected_interface_name = "IMeshInstanced"
  setmetatable(t, {__index= MeshInstanced})
  return t
end
function MeshInstanced:setBoneIndexInfo(...)
 return MeshInstanceds_setBoneIndexInfo(self[1].collection_index,self[1].collected_index , ...)
end
function MeshInstanced:setWorld(...)
 return MeshInstanceds_setWorld(self[1].collection_index,self[1].collected_index , ...)
end
function MeshInstanced:getWorld(...)
 return MeshInstanceds_getWorld(self[1].collection_index,self[1].collected_index , ...)
end
function MeshInstanced:setColor(...)
 return MeshInstanceds_setColor(self[1].collection_index,self[1].collected_index , ...)
end
function MeshInstanced:getColor(...)
 return MeshInstanceds_getColor(self[1].collection_index,self[1].collected_index , ...)
end
function MeshInstanced:setIsRender(...)
 return MeshInstanceds_setIsRender(self[1].collection_index,self[1].collected_index , ...)
end
function MeshInstanced:getIsRender(...)
 return MeshInstanceds_getIsRender(self[1].collection_index,self[1].collected_index , ...)
end
function MeshInstanced:getInstanceIndex(...)
 return MeshInstanceds_getInstanceIndex(self[1].collection_index,self[1].collected_index , ...)
end
TextFromLua = TextFromLua or { count =0 }
function TextFromLua:makeInstance(collection_index, ...)
  local t = TextFromLuas_makeInstance(collection_index,0, ...)
  TextFromLua.count = TextFromLua.count+1
  setmetatable(t, {__index = TextFromLua})
  return t
end
function TextFromLua:getIS(c_index, cd_index)
  local t ={}
  t[1] = {}
  t[1].collection_index = c_index
  t[1].collected_index = cd_index
  t[1].collected_class_name = "TextFromLua"
  t[1].collection_class_name = "TextFromLuas"
  t[1].collected_interface_name = "ITextFromLua"
  setmetatable(t, {__index= TextFromLua})
  return t
end
function TextFromLua:setDebugText(...)
 return TextFromLuas_setDebugText(self[1].collection_index,self[1].collected_index , ...)
end
function TextFromLua:add(...)
 return TextFromLuas_add(self[1].collection_index,self[1].collected_index , ...)
end
function TextFromLua:removeScene(...)
 return TextFromLuas_removeScene(self[1].collection_index,self[1].collected_index , ...)
end
function TextFromLua:enterTWOTAKU(...)
 return TextFromLuas_enterTWOTAKU(self[1].collection_index,self[1].collected_index , ...)
end
function TextFromLua:enterONEMESSAGE(...)
 return TextFromLuas_enterONEMESSAGE(self[1].collection_index,self[1].collected_index , ...)
end
function TextFromLua:enterLOADTYUU(...)
 return TextFromLuas_enterLOADTYUU(self[1].collection_index,self[1].collected_index , ...)
end
function TextFromLua:makeAILua(...)
 return TextFromLuas_makeAILua(self[1].collection_index,self[1].collected_index , ...)
end
function TextFromLua:makeButukariLua(...)
 return TextFromLuas_makeButukariLua(self[1].collection_index,self[1].collected_index , ...)
end
function TextFromLua:makeLoadLua(...)
 return TextFromLuas_makeLoadLua(self[1].collection_index,self[1].collected_index , ...)
end
function TextFromLua:makeAnimeLua(...)
 return TextFromLuas_makeAnimeLua(self[1].collection_index,self[1].collected_index , ...)
end
function TextFromLua:makeRenderLua(...)
 return TextFromLuas_makeRenderLua(self[1].collection_index,self[1].collected_index , ...)
end
CMesh = CMesh or { count =0 }
function CMesh:makeInstance(collection_index, ...)
  local t = CMeshs_makeInstance(collection_index,0, ...)
  CMesh.count = CMesh.count+1
  setmetatable(t, {__index = CMesh})
  return t
end
function CMesh:getIS(c_index, cd_index)
  local t ={}
  t[1] = {}
  t[1].collection_index = c_index
  t[1].collected_index = cd_index
  t[1].collected_class_name = "CMesh"
  t[1].collection_class_name = "CMeshs"
  t[1].collected_interface_name = "ICMesh"
  setmetatable(t, {__index= CMesh})
  return t
end
function CMesh:getMesh(...)
 return CMeshs_getMesh(self[1].collection_index,self[1].collected_index , ...)
end
function CMesh:getMeshWithAnime(...)
 return CMeshs_getMeshWithAnime(self[1].collection_index,self[1].collected_index , ...)
end
function CMesh:getIsLoad(...)
 return CMeshs_getIsLoad(self[1].collection_index,self[1].collected_index , ...)
end
function CMesh:setIsLoad(...)
 return CMeshs_setIsLoad(self[1].collection_index,self[1].collected_index , ...)
end
function CMesh:deleteMesh(...)
 return CMeshs_deleteMesh(self[1].collection_index,self[1].collected_index , ...)
end
function CMesh:deleteALL(...)
 return CMeshs_deleteALL(self[1].collection_index,self[1].collected_index , ...)
end
function CMesh:copyDAYO(...)
 return CMeshs_copyDAYO(self[1].collection_index,self[1].collected_index , ...)
end
Texture = Texture or { count =0 }
function Texture:makeInst(collection_index, ...)
  local t = Textures_makeInst(collection_index,0, ...)
  Texture.count = Texture.count+1
  setmetatable(t, {__index = Texture})
  return t
end
function Texture:getIS(c_index, cd_index)
  local t ={}
  t[1] = {}
  t[1].collection_index = c_index
  t[1].collected_index = cd_index
  t[1].collected_class_name = "Texture"
  t[1].collection_class_name = "Textures"
  t[1].collected_interface_name = "ITexture"
  setmetatable(t, {__index= Texture})
  return t
end
function Texture:getTexture(...)
 return Textures_getTexture(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:getRenderTex(...)
 return Textures_getRenderTex(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:getRenderBillBoard(...)
 return Textures_getRenderBillBoard(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderTexParam(...)
 return Textures_setRenderTexParam(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderBillBoardParam(...)
 return Textures_setRenderBillBoardParam(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderTexColor(...)
 return Textures_setRenderTexColor(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderBillBoardColor(...)
 return Textures_setRenderBillBoardColor(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderTexPos(...)
 return Textures_setRenderTexPos(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderBillBoardPos(...)
 return Textures_setRenderBillBoardPos(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderTexWH(...)
 return Textures_setRenderTexWH(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderBillBoardWH(...)
 return Textures_setRenderBillBoardWH(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderTexTexPos(...)
 return Textures_setRenderTexTexPos(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderBillBoardTexPos(...)
 return Textures_setRenderBillBoardTexPos(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderTexIsRender(...)
 return Textures_setRenderTexIsRender(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderBillBoardIsRender(...)
 return Textures_setRenderBillBoardIsRender(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderTexChangeTex(...)
 return Textures_setRenderTexChangeTex(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderBillBoardChangeTex(...)
 return Textures_setRenderBillBoardChangeTex(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:deleteRenderTex(...)
 return Textures_deleteRenderTex(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:deleteRenderBillBoard(...)
 return Textures_deleteRenderBillBoard(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:lightdeleteRenderTex(...)
 return Textures_lightdeleteRenderTex(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:lightdeleteRenderBillBoard(...)
 return Textures_lightdeleteRenderBillBoard(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:deleteAll(...)
 return Textures_deleteAll(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:getRenderText(...)
 return Textures_getRenderText(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderTextPos(...)
 return Textures_setRenderTextPos(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderTextChangeText(...)
 return Textures_setRenderTextChangeText(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderTextColor(...)
 return Textures_setRenderTextColor(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:setRenderTextIsRender(...)
 return Textures_setRenderTextIsRender(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:lightdeleteRenderText(...)
 return Textures_lightdeleteRenderText(self[1].collection_index,self[1].collected_index , ...)
end
function Texture:getRenderTextWidth(...)
 return Textures_getRenderTextWidth(self[1].collection_index,self[1].collected_index , ...)
end
Message = Message or { count =0 }
function Message:makeInst(collection_index, ...)
  local t = Messages_makeInst(collection_index,0, ...)
  Message.count = Message.count+1
  setmetatable(t, {__index = Message})
  return t
end
function Message:getIS(c_index, cd_index)
  local t ={}
  t[1] = {}
  t[1].collection_index = c_index
  t[1].collected_index = cd_index
  t[1].collected_class_name = "Message"
  t[1].collection_class_name = "Messages"
  t[1].collected_interface_name = "IMessage"
  setmetatable(t, {__index= Message})
  return t
end
function Message:makeMessage(...)
 return Messages_makeMessage(self[1].collection_index,self[1].collected_index , ...)
end
function Message:getMessageIndexsFromMsgId(...)
 return Messages_getMessageIndexsFromMsgId(self[1].collection_index,self[1].collected_index , ...)
end
function Message:getMessageIndexsFromReceiverId(...)
 return Messages_getMessageIndexsFromReceiverId(self[1].collection_index,self[1].collected_index , ...)
end
function Message:getMsgBody(...)
 return Messages_getMsgBody(self[1].collection_index,self[1].collected_index , ...)
end
