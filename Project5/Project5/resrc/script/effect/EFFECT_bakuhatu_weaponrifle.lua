dofile("tolua_glue/tolua.lua")

manager = EffectManager:getIS(0,0)

bakuhatu_id = manager:getEffect("bakuhatu_weaponrifle", "resrc/img/effect/jyudanbakuhatu.png","resrc/script/effect/EFFECT_bakuhatu_weaponrifle.lua")
manager:deleteEffect(bakuhatu_id)
manager:deleteEffectImpl()
manager:makeEffectPart(bakuhatu_id, 0, "1000000", true,"100000","100000")
manager:setEffectPartPos(bakuhatu_id,0,"0","1000000","0","0","0","0","0","0")
manager:setEffectPartTexPos(bakuhatu_id,0,"0","160",0,0,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"161","320",33,0,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"321","480",33+32,0,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"481","640",0,33,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"641","820",33,33,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"821","1000",33+32,33,32,32)

manager:setEffectPartTexPos(bakuhatu_id,0,"1001","1180",0,33+32,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"1181","1340",33,33+32,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"1341","1500",33+32,33+32,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"1501","1800",0,33+32+32,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"1801","2000",33,33+32+32,32,32)

manager:setEffectPartWH(bakuhatu_id,0,"0","2000","4","4","15","15")
manager:setEffectPartRot(bakuhatu_id,0,"0","2000","0","0","0","0","0","0")
manager:setEffectPartColor(bakuhatu_id,0,"0","2000",0xFFFF00FF,0xFF000033)

--impl_id = manager:getEffectImpl(bakuhatu_id)
--manager:setEffectImplIsRender(impl_id,true)
--manager:setEffectImplIsStart(impl_id,true)

--impl_id2 = manager:getEffectImpl(bakuhatu_id)
--manager:setEffectImplWorld(impl_id2,{1,0,0,0,0,1,0,0,0,0,1,0,10,0,10,1})
--manager:setEffectImplTime(impl_id2,"200")
--manager:setEffectImplIsRender(impl_id2,true)
--manager:setEffectImplIsStart(impl_id2,true)

--impl_id3 = manager:getEffectImpl(bakuhatu_id)
--manager:setEffectImplWorld(impl_id3,{1,0,0,0,0,1,0,0,0,0,1,0,10,10,10,1})
--manager:setEffectImplTime(impl_id3,"300")
--manager:setEffectImplIsRender(impl_id3,true)

--manager:setEffectImplIsStart(impl_id3,true)