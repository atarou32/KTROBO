if MYSLIDERS ~= nil then
  gu = GUI:getIS(0,0)
  ab = AnimationBuilder:getIS(0,0)
  now_rotx = gu:getNowFromSlider(MYSLIDERS[1])
  now_transx = gu:getNowFromSlider(MYSLIDERS[2])
  -- now_isrotx = gu:getNowFromSlider(MYSLIDERS[3])
  -- nowÇê›íËÇ∑ÇÈ
  --TO_LUA virtual int getNowIMPLIndex()=0;
  --	TO_LUA virtual int getNowBoneIndex()=0;
  impl_id = ab:getNowIMPLIndex()
  bone_id = ab:getNowBoneIndex()
  ab:setHonMeshBoneRotX(impl_id, bone_id, now_rotx)
  ab:setHonMeshBoneTransX(impl_id, bone_id, now_transx)
  

end