
dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ab = AnimationBuilder:getIS(0,0)
gu = GUI:getIS(0,0)

impl_id = ab:getNowIMPLIndex()
bone_id = gu:getTabIndex(MYTAB[impl_id+1])

frame = gu:getNowFromSlider(MYSLIDERS[21])
ab:setAnimePoseFrameExeAnime(impl_id, frame)