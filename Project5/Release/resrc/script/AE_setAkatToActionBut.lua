
dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ae = ActionEditor:getIS(0,0)
gu = GUI:getIS(0,0)
character_id = ae:getNowCharacterId()
hon_mesh_id = gu:getTabIndex(TABHONMESHS[character_id+1])
skeleton_id = gu:getTabIndex(TABSKELETONS[character_id+1][hon_mesh_id+1])
akat_index = gu:getTabIndex(TABAKATS[character_id+1][hon_mesh_id+1][skeleton_id+1])
action_id = gu:getTabIndex(TABACTIONS[character_id+1])
ae:setAkatToAction(character_id, action_id, hon_mesh_id, skeleton_id, akat_index)

