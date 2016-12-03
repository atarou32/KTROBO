if MYSLIDERS ~= nil then
  gu = GUI:getIS(0,0)
  ab = AnimationBuilder:getIS(0,0)
  
  impl_id = ab:getNowIMPLIndex()
  bone_id = ab:getNowBoneIndex()
  tog = ab:toggleHonMeshBoneRotZIsChange(impl_id, bone_id)
  if tog then
    gu:setEnable(MYSLIDERS[7], true)
    gu:setEnable(MYSLIDERS[8], true)
  else
    gu:setEnable(MYSLIDERS[7], false)
    gu:setEnable(MYSLIDERS[8], false)
  end
  
end