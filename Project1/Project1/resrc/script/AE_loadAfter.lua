dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")


gu = GUI:getIS(0,0)
mes = Message:getIS(0,0)
msgsi = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
msg_siz = mes:getMessageIndexsFromMsgId(KTROBO_MESSAGE_ID_ACTIONEDITOR_LOAD_AFTER,msgsi)
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
ab = AnimationBuilder:getIS(0,0)
mes:getMsgBody(msg_siz,msgsi, msg_ids, sender_ids, receiver_ids, msgs, fmsgs)

gu = GUI:getIS(0,0)
MYHONMESHS = MYHONMESHS or {}
MYSKELETONS = MYSKELETONS or {}
MYAKATS = MYAKATS or {}
MYACTIONS = MYACTIONS or {}


for i=1,msg_siz do
  -- now_index(character_id)‚Ímsgs‚É“ü‚Á‚Ä‚¢‚é
  character_id = tonumber(msgs[i])
  MYHONMESHS[character_id+1] = MYHONMESHS[character_id+1] or {}
  MYSKELETONS[character_id+1] = MYSKELETONS[character_id+1] or {}
  MYAKATS[character_id+1] = MYAKATS[character_id+1] or {}
  MYACTIONS[character_id+1] = MYACTIONS[character_id+1] or {}
  
  
  
  
  
  
  
end

gu:setRender(MYIMPLTAB, true)
gu:setEffect(MYIMPLTAB, true)


