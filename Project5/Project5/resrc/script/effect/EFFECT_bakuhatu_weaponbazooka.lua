dofile("tolua_glue/tolua.lua")

manager = EffectManager:getIS(0,0)

bakuhatu_id = manager:getEffect("bakuhatu_weaponbazooka", "resrc/img/effect/jyudanbakuhatu.png","resrc/script/effect/EFFECT_bakuhatu_weaponbazooka.lua")
manager:deleteEffect(bakuhatu_id)
manager:deleteEffectImpl()
manager:makeEffectPart(bakuhatu_id, 0, "1000000", true,"100000","100000")
manager:setEffectPartPos(bakuhatu_id,0,"0","1000000","0","0","0","0","0","0")
manager:setEffectPartTexPos(bakuhatu_id,0,"0","160",100,0,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"161","320",133,0,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"321","480",132+33,0,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"481","640",132+32+33,0,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"641","820",100,33,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"821","1000",133,33,32,32)

manager:setEffectPartTexPos(bakuhatu_id,0,"1001","1180",132+33,33,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"1181","1340",132+32+33,33,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"1341","1500",100,33+32,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"1501","1800",133,33+32,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"1801","2000",133+32,33+32,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"2001","2200",133+32+33,33+32,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"2201","2400",100,33+32+32,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"2401","2600",133,33+32+32,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"2601","2800",132+33,33+32+32,32,32)
manager:setEffectPartTexPos(bakuhatu_id,0,"2801","3000",132+33+32,33+32+32,32,32)


manager:setEffectPartWH(bakuhatu_id,0,"0","3000","4","4","15","15")
manager:setEffectPartRot(bakuhatu_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(bakuhatu_id,0,"0","3000",0xFFFF00FF,0xFF000033)

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