
dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ae = ActionEditor:getIS(0,0)
gu = GUI:getIS(0,0)
impl_id = gu:getTabIndex(MYIMPLTAB)
--ab:setNowCharacterId(impl_id)
hon_mesh_id = gu:getTabIndex(TABHONMESHS[impl_id+1])
skeleton_id = gu:getTabIndex(TABSKELETONS[impl_id+1][hon_mesh_id+1])
akat_id = gu:getTabIndex(TABAKATS[impl_id+1][hon_mesh_id+1][skeleton_id+1])
ae:setNowAkat(impl_id, hon_mesh_id, skeleton_id, akat_id)