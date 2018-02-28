dofile("tolua_glue/tolua.lua")

manager = EffectManager:getIS(0,0)

bakuhatu_id = manager:getEffect("bakuhatu_weaponlaserrifle", "resrc/img/effect/jyudanbakuhatu.png","resrc/script/effect/EFFECT_bakuhatu_weaponlaserrifle.lua")
manager:deleteEffect(bakuhatu_id)
manager:deleteEffectImpl()
manager:makeEffectPart(bakuhatu_id, 0, "1000000", true,"100000","100000")
manager:setEffectPartPos(bakuhatu_id,0,"0","1000000","0","0","0","0","0","0")
manager:setEffectPartTexPos(bakuhatu_id,0,"0","160",240,0,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"161","320",240,33,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"321","480",240,33+32,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"481","640",240+33,0,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"641","820",240+33,33,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"821","1000",240+33,33+32,32,32)

manager:setEffectPartTexPos(bakuhatu_id,0,"1001","1180",240+33+32,30,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"1181","1340",240+33+32,33,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"1341","1500",240+33+32,33+32,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"1501","1800",240+33+32+32,0,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"1801","2000",240+33+32+32,33,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"2001","2200",240+33+32+32,33+32,32,32)

manager:setEffectPartWH(bakuhatu_id,0,"0","2200","14","14","55","55")
manager:setEffectPartRot(bakuhatu_id,0,"0","2200","0","0","0","0","0","0")
manager:setEffectPartColor(bakuhatu_id,0,"0","2200",0xFFFFFFFF,0xFFFFFF00)

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