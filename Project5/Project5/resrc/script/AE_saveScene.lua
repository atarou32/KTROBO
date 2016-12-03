

dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

gu = GUI:getIS(0,0)
ae = ActionEditor:getIS(0,0)

if inputtext_scene ~= nil then

  test = gu:getStrFromInput(inputtext_scene)
 
  if ae:saveNowToFile(test) then
  
  else
   
    TextFromLua:getIS(0,0):enterTWOTAKU("resrc/script/AE_saveForceScene.lua", "resrc/script/SCENE_remove.lua", "すでにファイルがありますがセーブしますか")
    
  
  end

end