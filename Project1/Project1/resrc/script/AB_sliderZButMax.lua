if MYSLIDERS ~= nil then
  gu = GUI:getIS(0,0)
  ab = AnimationBuilder:getIS(0,0)
  now_rotz = gu:getNowFromSlider(MYSLIDERS[7])
 
  impl_id = ab:getNowIMPLIndex()
  bone_id = ab:getNowBoneIndex()
  ab:setHonMeshBoneRotZMax(impl_id, bone_id, now_rotz)
  
end