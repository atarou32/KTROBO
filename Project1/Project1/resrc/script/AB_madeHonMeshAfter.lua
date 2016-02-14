dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

--[[


ab_impl_index = ab:getNowIMPLIndex()
tab_inde = MYTABS_INDEX[ab_impl_index]
bone_inde = ab:getNowBoneIndex()
gu:setTabIndex(tan_inde, bone_inde)
]]

gu = GUI:getIS(0,0)
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
ab = AnimationBuilder:getIS(0,0)
mes:getMsgBody(msg_siz,msgsi, msg_ids, sender_ids, receiver_ids, msgs, fmsgs)
MYBONENAME = MYBONENAME or {}
MYBONETAB = MYBONETAB or {}
MYTAB = MYTAB or {}
--MYSLIDERS = MYSLIDERS or {}

if MYSLIDERS==nil then
  MYSLIDERS = {}
   offset = 150
  MYSLIDERS[1] = gu:makeSliderH({500+offset,800+offset,100,120},"3.14","-3.14","0","resrc/script/AB_sliderX.lua")
  MYSLIDERS[2] = gu:makeSliderH({500+offset,800+offset,130,150},"10","-10","0","resrc/script/AB_sliderX.lua")
  MYSLIDERS[3] = gu:makeButton(tostring(450+offset),"100","40","40", "resrc/script/AB_sliderXBut.lua", 30, "tog")
     TextFromLua:getIS(0,0):setDebugText("test12")
    -- roty
    -- transy
    -- is_rotybutton
    -- tabÇ÷ÇÃìoò^
  MYSLIDERS[4] = gu:makeSliderH({500+offset,800+offset,160,180},"3.14","-3.14","0","resrc/script/AB_sliderY.lua")
  MYSLIDERS[5] = gu:makeSliderH({500+offset,800+offset,190,210},"10","-10","0","resrc/script/AB_sliderY.lua")
  MYSLIDERS[6] = gu:makeButton(tostring(offset+450),"160","40","40", "resrc/script/AB_sliderYBut.lua", 30, "tog")
        
    -- rotz
    -- transz
    -- is_rotzbutton
    -- tabÇ÷ÇÃìoò^
  MYSLIDERS[7] = gu:makeSliderH({500+offset,800+offset,220,240},"3.14","-3.14","0","resrc/script/AB_sliderZ.lua")
  MYSLIDERS[8] = gu:makeSliderH({500+offset,800+offset,250,270},"10","-10","0","resrc/script/AB_sliderZ.lua")
  MYSLIDERS[9] = gu:makeButton(tostring(offset+450),"220","40","40", "resrc/script/AB_sliderZBut.lua", 30, "tog")
  MYSLIDERS[10] = gu:makeWindow(0,0,1000,800)
  MYSLIDERS[11] = gu:makeSliderH({300+offset,812+offset,450,470},"511","1","1","resrc/script/AB_setSisei.lua") --gu:makeButton(tostring(offset+450), "350", "35","35", "resrc/script/AB_clearSisei.lua",30,"cl")
  MYSLIDERS[12] = gu:makeButton(tostring(offset+300), "410", "30","30", "resrc/script/AB_setFrame.lua", 28, "set")
  MYSLIDERS[13] = gu:makeSliderH({300+offset,812+offset,480,500},"511","1","1","resrc/script/AB_setSiseiFrameKakera.lua")
  MYSLIDERS[14] = gu:makeButton(tostring(offset+600), "410", "30","30", "resrc/script/AB_hetareIK.lua", 28, "ik")
  for i=1,9 do
    --   gu:setPartToWindow(win, MYBONETAB[impl_id+1][bon_in][i])
    --  gu:setRender(MYBONETAB[impl_id+1][bon_in][i],true)
    gu:setPartToWindow(MYSLIDERS[10], MYSLIDERS[i])
  end
  gu:setPartToWindow(MYSLIDERS[10], MYSLIDERS[11])
  gu:setPartToWindow(MYSLIDERS[10], MYSLIDERS[12])
  gu:setPartToWindow(MYSLIDERS[10], MYSLIDERS[13])
  gu:setPartToWindow(MYSLIDERS[10], MYSLIDERS[14])
  gu:setEffect(MYSLIDERS[10], true)
  gu:setRender(MYSLIDERS[10], true)
  --gu:setLeafWindowToInputMessageDispatcher(MYSLIDERS[10])
  gu:setPartToWindow(win3, MYSLIDERS[10])
  gu:setEffect(MYSLIDERS[12], true)
  gu:setRender(MYSLIDERS[12], true)
  
  gu:setEffect(MYSLIDERS[13], true)
  gu:setRender(MYSLIDERS[13], true)
  
  gu:setEffect(MYSLIDERS[14], true)
  gu:setRender(MYSLIDERS[14], true)
  
  --gu:setLeafWindowToInputMessageDispatcher(MYSLIDERS[11])


end
gu = GUI:getIS(0,0)
BIGWIN = BIGWIN or {}
TextFromLua:getIS(0,0):setDebugText(""..msg_siz)
for i=1,msg_siz do
  -- now_index(impl_id)ÇÕmsgsÇ…ì¸Ç¡ÇƒÇ¢ÇÈ
  impl_id = tonumber(msgs[i])
  --int getHonMeshBoneNum(int impl_id);
  --char* getHonMeshBoneName(int impl_id, int bone_index);
  bon_num = ab:getHonMeshBoneNum(impl_id)
  for j=1,bon_num do
    bone_name = ab:getHonMeshBoneName(impl_id, j)
    MYBONENAME[impl_id+1] = MYBONENAME[impl_id+1] or {}
    MYBONENAME[impl_id+1][j] = bone_name
  end
  TextFromLua:getIS(0,0):setDebugText("impl_id="..impl_id)
  TextFromLua:getIS(0,0):setDebugText("impl_id+1="..(impl_id+1))
 -- TextFromLua:getIS(0,0):setDebugText("bone_tab=".. MYBONETAB)
  MYBONETAB[impl_id+1] = {}
  BIGWIN[impl_id+1] = gu:makeWindow(0,100,1000,100)
  MYTAB[impl_id+1] = gu:makeTab(0, "resrc/script/AB_tabbonepushed.lua")
  TextFromLua:getIS(0,0):setDebugText("test1")
  for bon_in, bon_nam in ipairs(MYBONENAME[impl_id+1]) do
    TextFromLua:getIS(0,0):setDebugText("bon_in="..bon_in.."bon_nam"..bon_nam)
    -- É{Å[ÉìÇÃâÒì]ÇÇ¬Ç©Ç≥Ç«ÇÈÉXÉâÉCÉ_Å[ÇÉEÉBÉìÉhÉEÇ…ìoò^Ç∑ÇÈ
    -- ÉEÉBÉìÉhÉEÇÉ^ÉuÇ…ìoò^Ç∑ÇÈ(Ç†ÇÈimplÇÃÇ»Ç©Ç≈ÇÃï°êîÇÃÉ{Å[Éì)
    -- É^ÉuÇÉEÉBÉìÉhÉEÇ…ìoò^Ç∑ÇÈ
    -- ÉEÉBÉìÉhÉEÇÉ^ÉuÇ…ìoò^Ç∑ÇÈ(ï°êîÇÃimpl)
    TextFromLua:getIS(0,0):setDebugText("test11")
    -- rotx
    -- transx
    -- is_rotxbutton
    -- tabÇ÷ÇÃìoò^
    -- TO_LUA virtual int makeSliderH(YARITORI MYRECT* zentai, float max, float min, float now, char* l_str)=0;
    offset = 150
    MYBONETAB[impl_id+1][bon_in] = {}
   -- MYBONETAB[impl_id+1][bon_in][1] = gu:makeSliderH({500+offset,800+offset,100,120},"3.14","0","0","resrc/script/AB_sliderX.lua")
   -- MYBONETAB[impl_id+1][bon_in][2] = gu:makeSliderH({500+offset,800+offset,130,150},"10","0","0","resrc/script/AB_sliderX.lua")
   -- MYBONETAB[impl_id+1][bon_in][3] = gu:makeButton(tostring(450+offset),"100","40","40", "resrc/script/AB_sliderXBut.lua", 30, "tog")
     TextFromLua:getIS(0,0):setDebugText("test12")
    -- roty
    -- transy
    -- is_rotybutton
    -- tabÇ÷ÇÃìoò^
   -- MYBONETAB[impl_id+1][bon_in][4] = gu:makeSliderH({500+offset,800+offset,160,180},"3.14","0","0","resrc/script/AB_sliderY.lua")
   -- MYBONETAB[impl_id+1][bon_in][5] = gu:makeSliderH({500+offset,800+offset,190,210},"10","0","0","resrc/script/AB_sliderY.lua")
   -- MYBONETAB[impl_id+1][bon_in][6] = gu:makeButton(tostring(offset+450),"160","40","40", "resrc/script/AB_sliderYBut.lua", 30, "tog")
        
    -- rotz
    -- transz
    -- is_rotzbutton
    -- tabÇ÷ÇÃìoò^
   -- MYBONETAB[impl_id+1][bon_in][7] = gu:makeSliderH({500+offset,800+offset,220,240},"3.14","0","0","resrc/script/AB_sliderZ.lua")
   -- MYBONETAB[impl_id+1][bon_in][8] = gu:makeSliderH({500+offset,800+offset,250,270},"10","0","0","resrc/script/AB_sliderZ.lua")
   -- MYBONETAB[impl_id+1][bon_in][9] = gu:makeButton(tostring(offset+450),"220","40","40", "resrc/script/AB_sliderZBut.lua", 30, "tog")
    TextFromLua:getIS(0,0):setDebugText("test13")
    win  = gu:makeWindow(0,0,1000,800)--450, 100, 340, 40)
    for i=1,9 do
   --   gu:setPartToWindow(win, MYBONETAB[impl_id+1][bon_in][i])
    --  gu:setRender(MYBONETAB[impl_id+1][bon_in][i],true)
      
    end
    gu:setRender(win,true)
    gu:setEffect(win,true)
  --  TextFromLua:getIS(0,0):setDebugText("test1")
    MYBONETAB[impl_id+1][bon_in][10] = gu:setWindowToTab(MYTAB[impl_id+1], win, bon_nam)
	--TextFromLua:getIS(0,0):setDebugText("test1")
	
  end
  gu:setEffect(MYTAB[impl_id+1],true)
  gu:setRender(MYTAB[impl_id+1],true)
  gu:setEffect(MYBONETAB[impl_id+1][bon_num][10], true)
  gu:setRender(MYBONETAB[impl_id+1][bon_num][10], true)
  gu:setPartToWindow(BIGWIN[impl_id+1], MYTAB[impl_id+1])
  -- MYIMPLTABÇ…ìoò^
  gu:setWindowToTab(MYIMPLTAB, BIGWIN[impl_id+1],""..impl_id)
  TextFromLua:getIS(0,0):setDebugText("test2")
end

gu:setRender(MYIMPLTAB, true)
gu:setEffect(MYIMPLTAB, true)
TextFromLua:getIS(0,0):setDebugText("test3")

TextFromLua:getIS(0,0):setDebugText("unko")