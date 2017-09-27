dofile("tolua_glue/tolua.lua")

manager = EffectManager:getIS(0,0)
--if (suuji == 0) {
--		t->setRenderTexTexPos(tex_id, 1,69,65-1,132-69);
--	} else if(suuji == 1) {
--		t->setRenderTexTexPos(tex_id, 69,69,131-69,132-69);
--	} else if(suuji == 2) {
--		t->setRenderTexTexPos(tex_id, 137,69,199-137,132-69);
--	} else if(suuji == 3) {
--		t->setRenderTexTexPos(tex_id, 203,69,266-203,132-69);
--	} else if(suuji == 4) {
--		t->setRenderTexTexPos(tex_id, 1,136,63-1,199-136);
--	} else if(suuji == 5) {
--		t->setRenderTexTexPos(tex_id, 69,136,132-69,199-136);
--	} else if(suuji == 6) {
--		t->setRenderTexTexPos(tex_id, 137,136,200-137,199-136);
--	} else if(suuji == 7) {
--		t->setRenderTexTexPos(tex_id, 202,136,266-202,199-136);
--	} else if(suuji == 8) {
--		t->setRenderTexTexPos(tex_id, 1,203,64-1,265-203);
--	} else if(suuji == 9) {
--		t->setRenderTexTexPos(tex_id, 69,203,132-69,265-203);
--	}
--	
s_zero_id = manager:getEffect("s_zero", "resrc/img/effect/pilot.png","resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_zero_id)
--manager:deleteEffectImpl()
manager:makeEffectPart(s_zero_id, 0, "3000", false,"3000","3000")
manager:setEffectPartPos(s_zero_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_zero_id,0,"0","160",1,69,64,132-69)
manager:setEffectPartWH(s_zero_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_zero_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_zero_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)

s_one_id = manager:getEffect("s_one", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_one_id)
manager:makeEffectPart(s_one_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_one_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_one_id,0,"0","160",69,69,132-69,131-69)
manager:setEffectPartWH(s_one_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_one_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_one_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)


s_two_id = manager:getEffect("s_two", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_two_id)
manager:makeEffectPart(s_two_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_two_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_two_id,0,"0","160",137,69,199-137,132-69)
manager:setEffectPartWH(s_two_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_two_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_two_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)



s_three_id = manager:getEffect("s_three", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_three_id)
manager:makeEffectPart(s_three_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_three_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_three_id,0,"0","160",203,66,266-203,132-66)
manager:setEffectPartWH(s_three_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_three_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_three_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)



s_four_id = manager:getEffect("s_four", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_four_id)
manager:makeEffectPart(s_four_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_four_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_four_id,0,"0","160",1,136,64-1,199-136)
manager:setEffectPartWH(s_four_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_four_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_four_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)



s_five_id = manager:getEffect("s_five", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_five_id)
manager:makeEffectPart(s_five_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_five_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_five_id,0,"0","160",69,136,132-69,199-136)
manager:setEffectPartWH(s_five_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_five_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_five_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)



s_six_id = manager:getEffect("s_six", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_six_id)
manager:makeEffectPart(s_six_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_six_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_six_id,0,"0","160",137,136,200-137,199-136)
manager:setEffectPartWH(s_six_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_six_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_six_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)


s_seven_id = manager:getEffect("s_seven", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_seven_id)
manager:makeEffectPart(s_seven_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_seven_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_seven_id,0,"0","160",202,136,266-202,199-136)
manager:setEffectPartWH(s_seven_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_seven_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_seven_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)



s_eight_id = manager:getEffect("s_eight", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_eight_id)
manager:makeEffectPart(s_eight_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_eight_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_eight_id,0,"0","160",1,203,64-1,265-203)
manager:setEffectPartWH(s_eight_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_eight_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_eight_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)

s_nine_id = manager:getEffect("s_nine", "resrc/img/effect/pilot.png", "resrc/script/effect/EFFECT_suuji.lua")
manager:deleteEffect(s_nine_id)
manager:makeEffectPart(s_nine_id,0,"3000",false, "0","0")

manager:setEffectPartPos(s_nine_id,0,"0","3000","0","0","0","0","0","2.3")
manager:setEffectPartTexPos(s_nine_id,0,"0","160",69,203,132-69,265-203)
manager:setEffectPartWH(s_nine_id,0,"0","3000","2","2","3.2","3.2")
manager:setEffectPartRot(s_nine_id,0,"0","3000","0","0","0","0","0","0")
manager:setEffectPartColor(s_nine_id,0,"0","3000",0xFFFFFFFF,0xFFFFFF44)








impl_id = manager:getEffectImpl(s_zero_id)
manager:setEffectImplIsRender(impl_id,true)
manager:setEffectImplIsStart(impl_id,true)
