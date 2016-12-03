
dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ae = ActionEditor:getIS(0,0)
gu = GUI:getIS(0,0)
impl_id = gu:getTabIndex(MYIMPLTAB)
--ab:setNowCharacterId(impl_id)
if TABHONMESHS[character_id+1] ~= nil then
  hon_mesh_id = gu:getTabIndex(TABHONMESHS[character_id+1])
  ae:setNowHonMeshId(impl_id,hon_mesh_id)
end
