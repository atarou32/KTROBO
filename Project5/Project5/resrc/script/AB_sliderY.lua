if MYSLIDERS ~= nil then
  gu = GUI:getIS(0,0)
  ab = AnimationBuilder:getIS(0,0)
  now_roty = gu:getNowFromSlider(MYSLIDERS[4])
  now_transy = gu:getNowFromSlider(MYSLIDERS[5])
  -- now_isroty = gu:getNowFromSlider(MYSLIDERS[6])
  -- nowÇê›íËÇ∑ÇÈ
  --TO_LUA virtual int getNowIMPLIndex()=0;
  --	TO_LUA virtual int getNowBoneIndex()=0;
  impl_id = ab:getNowIMPLIndex()
  bone_id = ab:getNowBoneIndex()
  ab:setHonMeshBoneRotY(impl_id, bone_id, now_roty)
  ab:setHonMeshBoneTransY(impl_id, bone_id, now_transy)
  

end