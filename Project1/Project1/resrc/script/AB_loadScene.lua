

dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ab = AnimationBuilder:getIS(0,0)
gu = GUI:getIS(0,0)

impl_id = ab:getNowIMPLIndex()
bone_id = ab:getNowBoneIndex()
if inputtext_scene ~= nil then

  test = gu:getStrFromInput(inputtext_scene)
 
  if ab:loadFromFile(test) then
   MYSLIDERS=nil
  else
   
    TextFromLua:getIS(0,0):enterTWOTAKU("resrc/script/AB_loadForceScene.lua", "resrc/script/SCENE_remove.lua", "åªç›ÇÃÉfÅ[É^ÇÕé∏ÇÌÇÍÇ‹Ç∑Ç™ÇÊÇ¢Ç≈Ç∑Ç©")
    
  
  end

end