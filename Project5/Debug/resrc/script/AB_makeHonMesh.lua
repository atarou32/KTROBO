ab = AnimationBuilder:getIS(0,0)
gu = GUI:getIS(0,0)
test = gu:getStrFromInput(inputtext_hon_mesh)
--TextFromLua:getIS(0,0):enterLOADTYUU()
ab:createAnimationBuilderImpl(test)
impl_id = ab:getNowIMPLIndex()
-- implのタブへの登録

  -- impl のwindow を作成する
  
-- impl内のtabの作成と登録
