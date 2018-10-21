dofile("tolua_glue/tolua.lua")

manager = EffectManager:getIS(0,0)

boosterhi_id = manager:getEffect("booster_taiki", "resrc/img/effect/boosterhi.png","resrc/script/effect/EFFECT_boostertaiki.lua")
manager:deleteEffect(boosterhi_id)
manager:deleteEffectImpl()
manager:makeEffectPart(boosterhi_id, 0, "1900", false,"2000","2000")
manager:setEffectPartPos(boosterhi_id,0,"0","1900","0","0","0","0","0","-3")
manager:setEffectPartTexPos(boosterhi_id,0,"0","160",0,0,128,128)
manager:setEffectPartTexPos(boosterhi_id,0,"161","320",129,0,128,128)
manager:setEffectPartTexPos(boosterhi_id,0,"321","480",128*2+1,0,128,128)
manager:setEffectPartTexPos(boosterhi_id,0,"481","640",128*3+1,0,128,128)
manager:setEffectPartTexPos(boosterhi_id,0,"641","820",128*4+1,0,128,128)
manager:setEffectPartTexPos(boosterhi_id,0,"821","1000",0,129,128,128)
manager:setEffectPartWH(boosterhi_id,0,"0","2000","2","2","0.5","0.5")
manager:setEffectPartRot(boosterhi_id,0,"0","2000","0","0","0","0","0","0")
manager:setEffectPartColor(boosterhi_id,0,"0","2000",0xFFFFFFFF,0xFFFFFF00)

--impl_id = manager:getEffectImpl(boosterhi_id)
--manager:setEffectImplIsRender(impl_id,true)
--manager:setEffectImplIsStart(impl_id,true)

--impl_id2 = manager:getEffectImpl(boosterhi_id)
--manager:setEffectImplWorld(impl_id2,{1,0,0,0,0,1,0,0,0,0,1,0,10,0,10,1})
--manager:setEffectImplTime(impl_id2,"200")
--manager:setEffectImplIsRender(impl_id2,true)
--manager:setEffectImplIsStart(impl_id2,true)

--impl_id3 = manager:getEffectImpl(boosterhi_id)
--manager:setEffectImplWorld(impl_id3,{1,0,0,0,0,1,0,0,0,0,1,0,10,10,10,1})
--manager:setEffectImplTime(impl_id3,"300")
--manager:setEffectImplIsRender(impl_id3,true)

--manager:setEffectImplIsStart(impl_id3,true)