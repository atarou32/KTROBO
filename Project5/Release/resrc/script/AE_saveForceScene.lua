

dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ae = ActionEditor:getIS(0,0)
gu = GUI:getIS(0,0)

if inputtext_scene ~= nil then
 -- TextFromLua:getIS(0,0):removeScene()
  test = gu:getStrFromInput(inputtext_scene)
  ae:forceSaveNowToFile(test)
  
 
end