
dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ae = ActionEditor:getIS(0,0)
gu = GUI:getIS(0,0)
impl_id = gu:getTabIndex(MYIMPLTAB)
if MYSLIDERS[3] ~= nil then
  now = gu:getNowFromSlider(MYSLIDERS[3])
  ae:setNOwActionSisei(impl_id,now)
end

