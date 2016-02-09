
dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")



gu = GUI:makeInst(0)
-- hon_mesh‚Ì
made_guis = made_guis or {}
tab_guis = tab_guis or {}
inputtext_hon_mesh = gu:makeInputText("50","500","500","20")
hon_mesh_but = gu:makeButton("550","500","110","40","resrc/script/AB_makehonmesh.lua",31, "–{mesh")
win = gu:makeWindow(50,500,650,100)
gu:setPartToWindow(win, inputtext_hon_mesh)
gu:setPartToWindow(win,hon_mesh_but)
gu:setEffect(hon_mesh_but,true)
gu:setRender(hon_mesh_but,true)
gu:setEffect(inputtext_hon_mesh,true)
gu:setRender(inputtext_hon_mesh,true)
gu:setEffect(win, true)
gu:setRootWindowToInputMessageDispatcher(win)

inputtext_call_lua = gu:makeInputText("50","700","500","20")
call_lua_but = gu:makeButton("550","700","110","40","resrc/script/AB_callLua.lua",31, "Lua")
win = gu:makeWindow(50,700,650,100)
gu:setPartToWindow(win, inputtext_call_lua)
gu:setPartToWindow(win,call_lua_but)
gu:setEffect(call_lua_but,true)
gu:setRender(call_lua_but,true)
gu:setEffect(inputtext_call_lua,true)
gu:setRender(inputtext_call_lua,true)
gu:setEffect(win, true)
gu:setRootWindowToInputMessageDispatcher(win)



--AnimationBuilders_createAnimationBuilderImplGlue(0,0,"resrc/model/ponko2-4/pk2sailordayo")

--tex = Texture:getIS(0,0)
--texid = tex:getTexture("resrc/sample/gui.png",128)
--button_no = tex:getRenderTex(texid, 0xFFFFFFFF, 300, 400, 100, 100, 0, 0, 128, 128)
--tex:setRenderTexIsRender(button_no,true)
