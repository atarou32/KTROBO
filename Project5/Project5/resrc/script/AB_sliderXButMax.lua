if MYSLIDERS ~= nil then
  gu = GUI:getIS(0,0)
  ab = AnimationBuilder:getIS(0,0)
  now_rotx = gu:getNowFromSlider(MYSLIDERS[1])
 
  impl_id = ab:getNowIMPLIndex()
  bone_id = ab:getNowBoneIndex()
  ab:setHonMeshBoneRotXMax(impl_id, bone_id, now_rotx)
  
end