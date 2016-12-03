
dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ae = ActionEditor:getIS(0,0)
gu = GUI:getIS(0,0)
character_id = ae:getNowCharacterId()
str = gu:getStrFromInput(MYSLIDERS[5])
ae:makeAction(character_id, str)