

dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ae = ActionEditor:getIS(0,0)
gu = GUI:getIS(0,0)

if inputtext_scene ~= nil then

  test = gu:getStrFromInput(inputtext_scene)
 
  if ae:loadFromFile(test) then
   
   MYSLIDERS=nil
   MYHONMESHS=nil
   MYSKELETONS=nil
   MYAKATS=nil
   MYACTIONS=nil
   TABHONMESHS=nil
   TABSKELETONS=nil
   TABAKATS=nil
   TABACTIONS=nil
   BIGHONMESHS=nil
   
  else   
    TextFromLua:getIS(0,0):enterTWOTAKU("resrc/script/AE_loadForceScene.lua", "resrc/script/SCENE_remove.lua", "åªç›ÇÃÉfÅ[É^ÇÕé∏ÇÌÇÍÇ‹Ç∑Ç™ÇÊÇ¢Ç≈Ç∑Ç©")
  
  end

end