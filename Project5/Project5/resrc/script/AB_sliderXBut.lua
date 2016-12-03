if MYSLIDERS ~= nil then
  gu = GUI:getIS(0,0)
  ab = AnimationBuilder:getIS(0,0)
  
  impl_id = ab:getNowIMPLIndex()
  bone_id = ab:getNowBoneIndex()
  tog = ab:toggleHonMeshBoneRotXIsChange(impl_id, bone_id)
  if tog then
    gu:setEnable(MYSLIDERS[1], true)
    gu:setEnable(MYSLIDERS[2], true)
  else
    gu:setEnable(MYSLIDERS[1], false)
    gu:setEnable(MYSLIDERS[2], false)
  end
  
end