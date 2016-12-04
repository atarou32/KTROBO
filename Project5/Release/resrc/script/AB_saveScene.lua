

dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ab = AnimationBuilder:getIS(0,0)
gu = GUI:getIS(0,0)

impl_id = ab:getNowIMPLIndex()
bone_id = ab:getNowBoneIndex()
if inputtext_scene ~= nil then

  test = gu:getStrFromInput(inputtext_scene)
 
  if ab:saveNowToFile(test) then
  
  else
   
    TextFromLua:getIS(0,0):enterTWOTAKU("resrc/script/AB_saveForceScene.lua", "resrc/script/SCENE_remove.lua", "すでにファイルがありますがセーブしますか")
    
  
  end

end