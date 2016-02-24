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
ae = ActionEditor:getIS(0,0)
mes:getMsgBody(msg_siz,msgsi, msg_ids, sender_ids, receiver_ids, msgs, fmsgs)

gu = GUI:getIS(0,0)
MYHONMESHS = MYHONMESHS or {}
MYSKELETONS = MYSKELETONS or {}
MYAKATS = MYAKATS or {} -- character, honmesh, skeleton, akat ごとにタブがある
MYACTIONS = MYACTIONS or {}

TABHONMESHS = TABHONMESHS or {} -- characterごとにタブがある
TABSKELETONS = TABSKELETONS or {} -- character,honmeshごとにタブがある
TABAKATS = TABAKATS or {} -- character,honmesh, skeleton ごとにタブがある
TABACTIONS = TABACTIONS or {} -- characterごとにタブがある


BIGHONMESHS = BIGHONMESHS or {}
--BIGSKELETONS = BIGSKELETONS or {}
--BIGAKATS = BIGAKATS or {}
--BIGACTIONS = BIGACTIONS or {}
if MYSLIDERS == nil then

  MYSLIDERS = {}
  MYSLIDERS[1] = gu:makeButton("300","400","60","40","resrc/script/AE_playakatBut.lua",31, "playakat")
  MYSLIDERS[2] = gu:makeButton("300","450","60","30","resrc/script/AE_playactionBut.lua",33, "playaction")
  MYSLIDERS[3] = gu:makeSliderH({"500","812","450","470"},"511","0","0","resrc/script/AE_setSisei.lua")
  MYSLIDERS[4] = gu:makeButton("300","500","60","30","resrc/script/AE_makeactionBut.lua",33,"mkact")
  MYSLIDERS[5] = gu:makeInputText("120","500","180","30")
  MYSLIDERS[6] = gu:makeWindow(120,500,240,30)
  MYSLIDERS[7] = gu:makeButton("380","500","40","20","resrc/script/AE_setAkatToActionBut.lua",35,"set")
  
  gu:setPartToWindow(MYSLIDERS[6], MYSLIDERS[4])
  gu:setPartToWindow(MYSLIDERS[6], MYSLIDERS[5])
  gu:setPartToWindow(win3, MYSLIDERS[1])
  gu:setPartToWindow(win3, MYSLIDERS[2])
  gu:setPartToWindow(win3, MYSLIDERS[5])
  gu:setPArtToWindow(MYSLIDERS[6],MYSLIDERS[7])
  gu:setEffect(MYSLIDERS[1], true)
  gu:setRender(MYSLIDERS[1], true)
  gu:setEffect(MYSLIDERS[2], true)
  gu:setRender(MYSLIDERS[2], true)
  gu:setEffect(MYSLIDERS[3], true)
  gu:setRender(MYSLIDERS[3], true)
  gu:setEffect(MYSLIDERS[4], true)
  gu:setRender(MYSLIDERS[4], true)
  gu:setEffect(MYSLIDERS[5], true)
  gu:setRender(MYSLIDERS[5], true)
  gu:setEffect(MYSLIDERS[6], true)
  gu:setRender(MYSLIDERS[6], true)
  gu:setRender(MYSLIDERS[7], true)
  gu:setEffect(MYSLIDERS[7], true)
  
end
for e=1,msg_siz do
  -- now_index(character_id)はmsgsに入っている
  character_id = tonumber(msgs[e])
  MYHONMESHS[character_id+1] = MYHONMESHS[character_id+1] or {}
  MYSKELETONS[character_id+1] = MYSKELETONS[character_id+1] or {}
  MYAKATS[character_id+1] = MYAKATS[character_id+1] or {}
  MYACTIONS[character_id+1] = MYACTIONS[character_id+1] or {}
  honmesh_num = ae:getHonMeshNum(character_id)
  for i=1,honmesh_num do
    if MYHONMESHS[character_id+1][i] ==nil then
      if TABHONMESHS[character_id+1] == nil then
        TABHONMESHS[character_id+1] = gu:makeTab(2,"resrc/script/AE_tabhonmeshpushed.lua")
        gu:setEffect(TABHONMESHS[character_id+1],true)
        gu:setRender(TABHONMESHS[character_id+1],true)
        if BIGHONMESHS[character_id+1] == nil then
          BIGHONMESHS[character_id+1] = gu:makeWindow(0,0,1000,800)
          gu:setRender(BIGHONMESHS[character_id+1],true)
          gu:setEffect(BIGHONMESHS[character_id+1],true)
          gu:setWindowToTab(MYIMPLTAB, BIGHONMESHS[character_id+1],""..character_id)
        end
        gu:setPartToWindow(BIGHONMESHS[character_id+1], TABHONMESHS[character_id+1])
      end
      MYHONMESHS[character_id+1][i] = gu:makeWindow(0,0,1000,800)
      gu:setWindowToTab(TABHONMESHS[character_id+1], MYHONMESHS[character_id+1][i],""..i)
      gu:setEffect(MYHONMESHS[character_id+1][i],true)
      gu:setRender(MYHONMESHS[character_id+1][i],true)
    end
    skeleton_num = ae:getSkeletonNum(character_id,i-1)
    for j=1,skeleton_num do
      MYSKELETONS[character_id+1][i] = MYSKELETONS[character_id+1][i] or {}
      if MYSKELETONS[character_id+1][i][j] == nil then
        TABSKELETONS[character_id+1] = TABSKELETONS[character_id+1] or {}
        if TABSKELETONS[character_id+1][i] == nil then
          TABSKELETONS[character_id+1][i] = gu:makeTab(3,"resrc/script/AE_tabskeletonpushed.lua")
          gu:setEffect(TABSKELETONS[character_id+1][i],true)
          gu:setRender(TABSKELETONS[character_id+1][i],true)
          gu:setPartToWindow(MYHONMESHS[character_id+1][i], TABSKELETONS[character_id+1][i])
          
          -- MYHONMESHSがあるからいらない
          --BIGSKELETONS[character_id+1] = BIGSKELETONS[character_id+1] or {}
          --if BIGSKELETONS[character_id+1][i] == nil then
           -- BIGSKELETONS[character_id+1][i] = gu:makeWindow(0,0,1000,800)
           -- gu:setRender(BIGSKELETONS[character_id+1][i],true)
           -- gu:setEffect(BIGSKELETONS[character_id+1][i],true)
          --  gu:setWindowToTab(
          -- end
        end
        MYSKELETONS[character_id+1][i][j] = gu:makeWindow(0,0,1000,800)
        gu:setWindowToTab(TABSKELETONS[character_id+1][i], MYSKELETONS[character_id+1][i][j], ""..j)
        gu:setEffect(MYSKELETONS[character_id+1][i][j],true)
        gu:setRender(MYSKELETONS[character_id+1][i][j],true)
        akat_num = ae:getAkatNum(character_id,i-1,j-1)
        for k=1,akat_num do
          MYAKATS[character_id+1][i] = MYAKATS[character_id+1][i] or {}
          MYAKATS[character_id+1][i][j] = MYAKATS[character_id+1][i][j] or {}
          if MYAKATS[character_id+1][i][j][k] == nil then
            TABAKATS[character_id+1] = TABAKATS[character_id+1] or {}
            TABAKATS[character_id+1][i] = TABAKATS[character_id+1][i] or {}
            if TABAKATS[character_id+1][i][j] == nil then
              TABAKATS[character_id+1][i][j] = gu:makeTab(4,"resrc/script/AE_tabakatpushed.lua")
              gu:setEffect(TABAKATS[character_id+1][i][j],true)
              gu:setRender(TABAKATS[character_id+1][i][j],true)
             
              gu:setPartToWindow(MYSKELETONS[character_id+1][i][j], TABAKATS[character_id+1][i][j])
           
            
            end
            MYAKATS[character_id+1][i][j][k] = gu:makeWindow(0,0,1000,800)
            akatname ={"test"}
            ae:getAkatName(character_id,i-1,j-1,k-1,akatname)
            gu:setWindowToTab(TABAKATS[character_id+1][i][j], MYAKATS[character_id+1][i][j][k],akatname[1])
            gu:setEffect(MYAKATS[character_id+1][i][j][k], true)
            gu:setRender(MYAKATS[character_id+1][i][j][k], true)
          end
        
        
        
        
        end
      
      
      end
    end
  
  end
  
  action_num = ae:getActionNum(character_id)
  for i=1,action_num do
    if MYACTIONS[character_id+1][i] ==nil then
      if TABACTIONS[character_id+1] == nil then
        TABACTIONS[character_id+1] = gu:makeTab(5,"resrc/script/AE_tabactionpushed.lua")
        gu:setEffect(TABACTIONS[character_id+1],true)
        gu:setRender(TABACTIONS[character_id+1],true)
        if BIGHONMESHS[character_id+1] == nil then
          BIGHONMESHS[character_id+1] = gu:makeWindow(0,0,1000,800)
          gu:setRender(BIGHONMESHS[character_id+1],true)
          gu:setEffect(BIGHONMESHS[character_id+1],true)
          gu:setWindowToTab(MYTABIMPL, BIGHONMESHS[character_id+1],""..character_id)
        end
        gu:setPartToWindow(BIGHONMESHS[character_id+1], TABACTIONS[character_id+1])
      end
      MYACTIONS[character_id+1][i] = gu:makeWindow(0,0,1000,800)
      gu:setWindowToTab(TABACTIONS[character_id+1], MYACTIONS[character_id+1][i],""..i)
      gu:setEffect(MYACTIONS[character_id+1][i],true)
      gu:setRender(MYACTIONS[character_id+1][i],true)
    end
  end
  
  
end

gu:setRender(MYIMPLTAB, true)
gu:setEffect(MYIMPLTAB, true)


