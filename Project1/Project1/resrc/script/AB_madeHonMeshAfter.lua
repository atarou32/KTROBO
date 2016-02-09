dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

--[[


ab_impl_index = ab:getNowIMPLIndex()
tab_inde = MYTABS_INDEX[ab_impl_index]
bone_inde = ab:getNowBoneIndex()
gu:setTabIndex(tan_inde, bone_inde)
]]

--[[
mes = Message:getIS(0,0)
msgsi = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
msg_siz = mes:getMessageIndexsFromMsgId(KTROBO_MESSAGE_ID_ANIMATIONBUILDER_HON_MESH_AFTER,msgsi)
msg_ids = {}
sender_ids = {}
receiver_ids = {}
msgs = {}
fmsgs = {}
for i=1,msg_siz do
  msg_ids[i] = 0
  sender_ids[i] = 0
  receiver_ids[i] = 0
  msgs[i] = 0
  fmsgs[i] = 0
end
ab = AnimationBuilder::getIS(0,0)
mes:getMsgBody(msg_siz,msgsi, msg_id, sender_ids, receiver_ids, msgs, fmsgs)
MYBONENAME = MYBONENAME or {}
gu = GUI::getIS(0,0)

for i=1,msg_siz do
  -- now_index(impl_id)はmsgsに入っている
  impl_id = msgs[i]
  --int getHonMeshBoneNum(int impl_id);
  --char* getHonMeshBoneName(int impl_id, int bone_index);
  bon_num = ab:getHonMeshBoneNum(impl_id)
  for j=1,bon_num do
    bone_name = ab:getHonMeshBoneName(impl_id, j)
    MYBONENAME[impl_id] = MYBONENAME[impl_id] or {}
    MYBONENAME[impl_id][j] = bone_name
  end
  for bon_in, bon_nam in ipairs(MYBONENAME[impl_id]) do
    -- ボーンの回転をつかさどるスライダーをウィンドウに登録する
    -- ウィンドウをタブに登録する(あるimplのなかでの複数のボーン)
    -- これはすでにすんでいるタブをウィンドウに登録する
    -- これはすでにすんでいるウィンドウをタブに登録する(複数のimpl)
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
  end
end
]]
