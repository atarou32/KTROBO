

dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ae = ActionEditor:getIS(0,0)
gu = GUI:getIS(0,0)

if inputtext_scene ~= nil then
  test = gu:getStrFromInput(inputtext_scene)
  
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
  
  ae:forceLoadFromFile(test)
  
 
end