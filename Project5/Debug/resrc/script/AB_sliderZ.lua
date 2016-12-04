if MYSLIDERS ~= nil then
  gu = GUI:getIS(0,0)
  ab = AnimationBuilder:getIS(0,0)
  now_rotz = gu:getNowFromSlider(MYSLIDERS[7])
  now_transz = gu:getNowFromSlider(MYSLIDERS[8])
  -- now_isroty = gu:getNowFromSlider(MYSLIDERS[9])
  -- nowÇê›íËÇ∑ÇÈ
  --TO_LUA virtual int getNowIMPLIndex()=0;
  --	TO_LUA virtual int getNowBoneIndex()=0;
  impl_id = ab:getNowIMPLIndex()
  bone_id = ab:getNowBoneIndex()
  ab:setHonMeshBoneRotZ(impl_id, bone_id, now_rotz)
  ab:setHonMeshBoneTransZ(impl_id, bone_id, now_transz)
  

end