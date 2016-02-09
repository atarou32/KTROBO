

dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")



--[[
mes = Message:getIS(0,0)
msgsi = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
msg_siz = mes:getMessageIndexsFromMsgId(KTROBO_MESSAGE_ID_ANIMATIONBUILDER_BONE_PUSHED,msgsi)
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

mes:getMsgBody(msg_siz,msgsi, msg_id, sender_ids, receiver_ids, msgs, fmsgs)
]]




--[[
-- msgs ‚Ì‚È‚©‚Ébone_index‚ª“ü‚Á‚Ä‚¢‚é
gu = GUI::getIS(0,0)
ab = AnimationBuilder::getIS(0,0)
ab_impl_index = ab:getNowIMPLIndex()
tab_inde = MYTABS_INDEX[ab_impl_index]
bone_inde = ab:getNowBoneIndex()
gu:setTabIndex(tan_inde, bone_inde)

]]