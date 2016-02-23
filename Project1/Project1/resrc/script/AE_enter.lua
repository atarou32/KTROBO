
dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")



gu = GUI:getIS(0,0)


inputtext_scene = gu:makeInputText("50","400","500","16")
scene_but = gu:makeButton("550","400","30","30", "resrc/script/AE_saveScene.lua",29,"save")
load_but = gu:makeButton("650","400","30","30", "resrc/script/AE_loadScene.lua", 29,"load")
win4 = gu:makeWindow(50,400,630,30)
gu:setPartToWindow(win4, inputtext_scene)
gu:setPartToWindow(win4, scene_but)
gu:setPartToWindow(win4, load_but)
gu:setEffect(inputtext_scene, true)
gu:setRender(inputtext_scene, true)
gu:setEffect(scene_but, true)
gu:setRender(scene_but, true)
gu:setRender(load_but, true)
gu:setEffect(load_but, true)
gu:setEffect(win4, true)
gu:setRender(win4, true)





inputtext_call_lua = gu:makeInputText("50","700","500","20")
call_lua_but = gu:makeButton("550","700","110","40","resrc/script/AE_callLua.lua",31, "Lua")
win2 = gu:makeWindow(50,700,610,60)
gu:setPartToWindow(win2, inputtext_call_lua)
gu:setPartToWindow(win2,call_lua_but)
gu:setEffect(call_lua_but,true)
gu:setRender(call_lua_but,true)
gu:setEffect(inputtext_call_lua,true)
gu:setRender(inputtext_call_lua,true)
gu:setEffect(win2, true)
gu:setRender(win2, true)


-- impl—p‚Ìtab‚ð“o˜^‚·‚é
MYIMPLTAB   = gu:makeTab(1, "resrc/script/AE_tabimplpushed.lua")
MYHONMESHS  = MYHONMESHS or {}
MYSKELETONS = MYSKELETONS or {}
MYAKATS     = MYAKATS or {}
MYACTIONS   = MYACTIONS or {}





win3 = gu:makeWindow(-10,-10,1,1)
gu:setPartToWindow(win3, MYIMPLTAB)
gu:setPartToWindow(win3, win2)
gu:setPartToWindow(win3, win4)
gu:setEffect(MYIMPLTAB,true)
gu:setRender(MYIMPLTAB,true)


gu:setEffect(win3, true)
gu:setRender(win3, true)
gu:setLeafWindowToInputMessageDispatcher(win3)