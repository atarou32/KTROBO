
dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ae = ActionEditor:getIS(0,0)
gu = GUI:getIS(0,0)
impl_id = gu:getTabIndex(MYIMPLTAB)
--ab:setNowCharacterId(impl_id)
action_id = gu:getTabIndex(TABACTIONS[impl_id+1])
ae:setNowAction(impl_id,action_id)