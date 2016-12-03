
dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ae = ActionEditor:getIS(0,0)
gu = GUI:getIS(0,0)
impl_id = gu:getTabIndex(MYIMPLTAB)
ae:setNowCharacterId(impl_id)

