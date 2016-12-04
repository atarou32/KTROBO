ab = AnimationBuilder:getIS(0,0)
gu = GUI:getIS(0,0)
test = gu:getStrFromInput(inputtext_animename)
test2 = gu:getStrFromInput(inputtext_animeframe)
impl_id = ab:getNowIMPLIndex()
MYANIMETABINDEX = MYANMETABINDEX or {}
  -- ‚³‚­‚¹‚¢‚·‚é
  exe_id = ab:createFrameExe(impl_id, test, test2)
  MYANIMETABINDEX[impl_id+1] = MYANIMETABINDEX[impl_id+1] or {}
  wind = gu:makeWindow(0,0,1000,800)
  index = gu:setWindowToTab(MYANIMETAB[impl_id+1], wind, test)
  MYANIMETABINDEX[impl_id+1][exe_id] =  index
  
