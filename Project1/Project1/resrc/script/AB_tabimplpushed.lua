
dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ab = AnimationBuilder:getIS(0,0)
gu = GUI:getIS(0,0)
impl_id = gu:getTabIndex(MYIMPLTAB)
bone_id = gu:getTabIndex(MYTAB[impl_id+1])
ab:setNowIMPLIndex(impl_id)

gu:setTabIndex(MYANIMETAB[impl_id+1],0)


-- sliderÇÃílÇê›íËÇ∑ÇÈ
rotx = ab:getHonMeshBoneRotX(impl_id, bone_id)
roty = ab:getHonMeshBoneRotY(impl_id, bone_id)
rotz = ab:getHonMeshBoneRotZ(impl_id, bone_id)
transx = ab:getHonMeshBoneTransX(impl_id, bone_id)
transy = ab:getHonMeshBoneTransY(impl_id, bone_id)
transz = ab:getHonMeshBoneTransZ(impl_id, bone_id)
-- TO_LUA virtual void setNOWMAXMINToSlider(int gui_id, float max, float min, float now)=0;
gu:setNOWMAXMINToSlider(MYSLIDERS[1], "3.14","-3.14", rotx)
gu:setNOWMAXMINToSlider(MYSLIDERS[2], "10", "-10", transx)

gu:setNOWMAXMINToSlider(MYSLIDERS[4], "3.14","-3.14", roty)
gu:setNOWMAXMINToSlider(MYSLIDERS[5], "10", "-10", transy)

gu:setNOWMAXMINToSlider(MYSLIDERS[7], "3.14","-3.14", rotz)
gu:setNOWMAXMINToSlider(MYSLIDERS[8], "10", "-10", transz)


if MYSLIDERS ~= nil then
  gu = GUI:getIS(0,0)
  ab = AnimationBuilder:getIS(0,0)
  
  impl_id = ab:getNowIMPLIndex()
  bone_id = ab:getNowBoneIndex()
  tog = ab:getHonMeshBoneRotXIsChange(impl_id, bone_id)
  if tog then
    gu:setEnable(MYSLIDERS[1], true)
    gu:setEnable(MYSLIDERS[2], true)
  else
    gu:setEnable(MYSLIDERS[1], false)
    gu:setEnable(MYSLIDERS[2], false)
  end
  
  tog = ab:getHonMeshBoneRotYIsChange(impl_id, bone_id)
  if tog then
    gu:setEnable(MYSLIDERS[4], true)
    gu:setEnable(MYSLIDERS[5], true)
  else
    gu:setEnable(MYSLIDERS[4], false)
    gu:setEnable(MYSLIDERS[5], false)
  end
  
  tog = ab:getHonMeshBoneRotZIsChange(impl_id, bone_id)
  if tog then
    gu:setEnable(MYSLIDERS[7], true)
    gu:setEnable(MYSLIDERS[8], true)
  else
    gu:setEnable(MYSLIDERS[7], false)
    gu:setEnable(MYSLIDERS[8], false)
  end

end