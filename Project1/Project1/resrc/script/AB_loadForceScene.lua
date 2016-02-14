

dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ab = AnimationBuilder:getIS(0,0)
gu = GUI:getIS(0,0)

impl_id = ab:getNowIMPLIndex()
bone_id = ab:getNowBoneIndex()
if inputtext_scene ~= nil then
 -- TextFromLua:getIS(0,0):removeScene()
  test = gu:getStrFromInput(inputtext_scene)
  ab:force_loadFromFile(test)
  
 
end