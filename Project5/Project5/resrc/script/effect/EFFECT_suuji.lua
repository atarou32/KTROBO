dofile("tolua_glue/tolua.lua")

manager = EffectManager:getIS(0,0)

s_zero_id = manager:getEffect("s_zero", "resrc/img/effect/pilot.png","resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_zero_id)
manager:deleteEffectImpl()
manager:makeEffectPart(s_zero_id, 0, "3000", false,"3000","3000")
manager:setEffectPartPos(s_zero_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_zero_id,0,"0","160",0,65,64,64)
manager:setEffectPartWH(s_zero_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_zero_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_zero_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)

s_one_id = manager:getEffect("s_one", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_one_id)
manager:makeEffectPart(s_one_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_one_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_one_id,0,"0","160",65,65,64,64)
manager:setEffectPartWH(s_one_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_one_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_one_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)


s_two_id = manager:getEffect("s_two", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_two_id)
manager:makeEffectPart(s_two_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_two_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_two_id,0,"0","160",129,65,64,64)
manager:setEffectPartWH(s_two_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_two_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_two_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)



s_three_id = manager:getEffect("s_three", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_three_id)
manager:makeEffectPart(s_three_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_three_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_three_id,0,"0","160",193,65,64,64)
manager:setEffectPartWH(s_three_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_three_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_three_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)



s_four_id = manager:getEffect("s_four", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_four_id)
manager:makeEffectPart(s_four_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_four_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_four_id,0,"0","160",0,129,64,64)
manager:setEffectPartWH(s_four_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_four_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_four_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)



s_five_id = manager:getEffect("s_five", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_five_id)
manager:makeEffectPart(s_five_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_five_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_five_id,0,"0","160",65,129,64,64)
manager:setEffectPartWH(s_five_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_five_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_five_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)



s_six_id = manager:getEffect("s_six", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_six_id)
manager:makeEffectPart(s_six_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_six_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_six_id,0,"0","160",129,129,64,64)
manager:setEffectPartWH(s_six_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_six_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_six_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)


s_seven_id = manager:getEffect("s_seven", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_seven_id)
manager:makeEffectPart(s_seven_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_seven_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_seven_id,0,"0","160",193,129,64,64)
manager:setEffectPartWH(s_seven_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_seven_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_seven_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)



s_eight_id = manager:getEffect("s_eight", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_eight_id)
manager:makeEffectPart(s_eight_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_eight_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_eight_id,0,"0","160",0,193,64,64)
manager:setEffectPartWH(s_eight_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_eight_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_eight_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)

s_nine_id = manager:getEffect("s_nine", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_nine_id)
manager:makeEffectPart(s_nine_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_nine_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_nine_id,0,"0","160",65,193,64,64)
manager:setEffectPartWH(s_nine_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_nine_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_nine_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)








impl_id = manager:getEffectImpl(s_zero_id)
manager:setEffectImplIsRender(impl_id,true)
manager:setEffectImplIsStart(impl_id,true)
