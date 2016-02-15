if MYSLIDERS ~= nil then
  gu = GUI:getIS(0,0)
  ab = AnimationBuilder:getIS(0,0)
  now_roty = gu:getNowFromSlider(MYSLIDERS[4])
 
  impl_id = ab:getNowIMPLIndex()
  bone_id = ab:getNowBoneIndex()
  ab:setHonMeshBoneRotYMax(impl_id, bone_id, now_roty)
  
end