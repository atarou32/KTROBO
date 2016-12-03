
dofile("resrc/script/nanchatte.lua")
dofile("tolua_glue/tolua.lua")

ab = AnimationBuilder:getIS(0,0)
gu = GUI:getIS(0,0)

impl_id = ab:getNowIMPLIndex()
bone_id = gu:getTabIndex(MYTAB[impl_id+1])
anime_id = gu:getTabIndex(MYANIMETAB[impl_id+1])
ab:setNowAnimeIndex(anime_id)

-- slider‚Ì’l‚ğİ’è‚·‚é
