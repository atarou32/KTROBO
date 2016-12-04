if MYSLIDERS ~= nil then
  gu = GUI:getIS(0,0)
  ab = AnimationBuilder:getIS(0,0)
  
  impl_id = ab:getNowIMPLIndex()
  bone_id = ab:getNowBoneIndex()
  tog = ab:toggleHonMeshBoneRotYIsChange(impl_id, bone_id)
  if tog then
    gu:setEnable(MYSLIDERS[4], true)
    gu:setEnable(MYSLIDERS[5], true)
  else
    gu:setEnable(MYSLIDERS[4], false)
    gu:setEnable(MYSLIDERS[5], false)
  end
  
end