
dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")



gu = GUI:makeInst(0)


inputtext_scene = gu:makeInputText("50","400","500","16")
scene_but = gu:makeButton("550","400","30","30", "resrc/script/AB_saveScene.lua",29,"save")
win4 = gu:makeWindow(50,400,530,30)
gu:setPartToWindow(win4, inputtext_scene)
gu:setPartToWindow(win4, scene_but)
gu:setEffect(inputtext_scene, true)
gu:setRender(inputtext_scene, true)
gu:setEffect(scene_but, true)
gu:setRender(scene_but, true)
gu:setEffect(win4, true)
gu:setRender(win4, true)
--gu:setRootWindowToInputMessageDispatcher(win4)


--TextFromLua:getIS(0,0):enterONEMESSAGE("testdesuyo")
TextFromLua:getIS(0,0):enterTWOTAKU("resrc/script/SCENE_remove.lua", "resrc/script/SCENE_remove.lua", "�����������܂��傤")
inputtext_hon_mesh = gu:makeInputText("50","500","500","20")
hon_mesh_but = gu:makeButton("550","500","110","40","resrc/script/AB_makehonmesh.lua",31, "�{mesh")
win = gu:makeWindow(50,500,610,60)
gu:setPartToWindow(win, inputtext_hon_mesh)
gu:setPartToWindow(win,hon_mesh_but)
gu:setEffect(hon_mesh_but,true)
gu:setRender(hon_mesh_but,true)
gu:setEffect(inputtext_hon_mesh,true)
gu:setRender(inputtext_hon_mesh,true)
gu:setEffect(win, true)
gu:setRender(win, true)
--gu:setRootWindowToInputMessageDispatcher(win)

inputtext_call_lua = gu:makeInputText("50","700","500","20")
call_lua_but = gu:makeButton("550","700","110","40","resrc/script/AB_callLua.lua",31, "Lua")
win2 = gu:makeWindow(50,700,610,60)
gu:setPartToWindow(win2, inputtext_call_lua)
gu:setPartToWindow(win2,call_lua_but)
gu:setEffect(call_lua_but,true)
gu:setRender(call_lua_but,true)
gu:setEffect(inputtext_call_lua,true)
gu:setRender(inputtext_call_lua,true)
gu:setEffect(win2, true)
gu:setRender(win2, true)
--gu:setRootWindowToInputMessageDispatcher(win2)

-- impl�p��tab��o�^����
MYIMPLTAB = gu:makeTab(1, "test")
win3 = gu:makeWindow(-10,-10,1,1)
gu:setPartToWindow(win3, MYIMPLTAB)
gu:setPartToWindow(win3,win2)
gu:setPartToWindow(win3, win)
gu:setPartToWindow(win3, win4)
gu:setEffect(MYIMPLTAB,true)
gu:setRender(MYIMPLTAB,true)
gu:setEffect(win3, true)
gu:setRender(win3, true)
gu:setLeafWindowToInputMessageDispatcher(win3)


--AnimationBuilders_createAnimationBuilderImplGlue(0,0,"resrc/model/ponko2-4/pk2sailordayo")

--tex = Texture:getIS(0,0)
--texid = tex:getTexture("resrc/sample/gui.png",128)
--button_no = tex:getRenderTex(texid, 0xFFFFFFFF, 300, 400, 100, 100, 0, 0, 128, 128)
--tex:setRenderTexIsRender(button_no,true)
