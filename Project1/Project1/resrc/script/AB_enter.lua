
dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")



gu = GUI:getIS(0,0)


inputtext_scene = gu:makeInputText("50","400","500","16")
scene_but = gu:makeButton("550","400","30","30", "resrc/script/AB_saveScene.lua",29,"save")
load_but = gu:makeButton("650","400","30","30", "resrc/script/AB_loadScene.lua", 29,"load")
win4 = gu:makeWindow(50,400,630,30)
gu:setPartToWindow(win4, inputtext_scene)
gu:setPartToWindow(win4, scene_but)
gu:setPartToWindow(win4, load_but)
gu:setEffect(inputtext_scene, true)
gu:setRender(inputtext_scene, true)
gu:setEffect(scene_but, true)
gu:setRender(scene_but, true)
gu:setEffect(win4, true)
gu:setRender(win4, true)




inputtext_hon_mesh = gu:makeInputText("50","500","500","20")
hon_mesh_but = gu:makeButton("550","500","110","40","resrc/script/AB_makehonmesh.lua",31, "–{mesh")
win = gu:makeWindow(50,500,610,60)
gu:setPartToWindow(win, inputtext_hon_mesh)
gu:setPartToWindow(win,hon_mesh_but)
gu:setEffect(hon_mesh_but,true)
gu:setRender(hon_mesh_but,true)
gu:setEffect(inputtext_hon_mesh,true)
gu:setRender(inputtext_hon_mesh,true)
gu:setEffect(win, true)
gu:setRender(win, true)

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


-- impl—p‚Ìtab‚ð“o˜^‚·‚é
MYIMPLTAB = gu:makeTab(1, "resrc/script/AB_tabimplpushed.lua")

inputtext_animename = gu:makeInputText("50", "750", "500", "20")
inputtext_animeframe = gu:makeInputText("570", "750", "60", "18")
anime_but = gu:makeButton("640", "750", "40","40", "resrc/script/AB_makeAnime.lua", 29, "anime")
win5 = gu:makeWindow(50,750,610,40)
gu:setPartToWindow(win5, inputtext_animename)
gu:setPartToWindow(win5, inputtext_animeframe)
gu:setPartToWindow(win5, anime_but)
gu:setRender(inputtext_animename, true)
gu:setEffect(inputtext_animename, true)
gu:setRender(inputtext_animeframe, true)
gu:setEffect(inputtext_animeframe, true)
gu:setRender(anime_but, true)
gu:setEffect(anime_but, true)
gu:setRender(win5, true)
gu:setEffect(win5, true)

win3 = gu:makeWindow(-10,-10,1,1)
gu:setPartToWindow(win3, MYIMPLTAB)
gu:setPartToWindow(win3, win5)
gu:setPartToWindow(win3, win2)
gu:setPartToWindow(win3, win)
gu:setPartToWindow(win3, win4)
gu:setEffect(MYIMPLTAB,true)
gu:setRender(MYIMPLTAB,true)


gu:setEffect(win3, true)
gu:setRender(win3, true)
gu:setLeafWindowToInputMessageDispatcher(win3)