
dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ab = AnimationBuilder:getIS(0,0)
gu = GUI:getIS(0,0)
impl_id = gu:getTabIndex(MYIMPLTAB)
bone_id = gu:getTabIndex(MYTAB[impl_id+1])
ab:setNowIMPLIndex(impl_id)

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