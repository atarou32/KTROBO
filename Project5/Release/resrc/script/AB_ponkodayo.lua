ab = AnimationBuilder:getIS(0,0)
gu = GUI:getIS(0,0)

ab:createAnimationBuilderImpl("resrc/model/ponko2-4/pk2sailordayo")
impl_id = ab:getNowIMPLIndex()
ab:setOnajiMesh(impl_id, "resrc/model/ponko2-3/pk2skirt")
ab:setOnajiMesh(impl_id, "resrc/model/ponko2-3/pk2face")
ab:setOnajiMesh(impl_id, "resrc/model/ponko2-3/pk2hidarime")
ab:setOnajiMesh(impl_id, "resrc/model/ponko2-3/pk2hidarimedama")
ab:setOnajiMesh(impl_id, "resrc/model/ponko2-3/pk2kami")
ab:setOnajiMesh(impl_id, "resrc/model/ponko2-3/pk2kuti")
ab:setOnajiMesh(impl_id, "resrc/model/ponko2-3/pk2kubi")
ab:setOnajiMesh(impl_id, "resrc/model/ponko2-3/pk2mayuge")
ab:setOnajiMesh(impl_id, "resrc/model/ponko2-3/pk2midarime")
ab:setOnajiMesh(impl_id, "resrc/model/ponko2-3/pk2migimedama")
ab:setKoMesh(impl_id, "resrc/model/ponko2-4/pk2migihanddayo", "resrc/model/ponko2-4/pk2sailordayo", "migiArmTekubiBone", true,{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1})
ab:setKoMesh(impl_id,"resrc/model/bazooka/rweaponbazookaguna", "resrc/model/ponko2-4/pk2migihanddayo", "MigiHandMotiBone", false,{0.00026,0.333,0.00026,0,-0.00026,0.00026,-0.333,0,-0.333,0,0.00026,0,0,0,0,1})

--void setOnajiMesh(int impl_id, char* onaji_filepath);
--void setKoMesh(int impl_id, char* ko_filepath, char* oya_filepath,char* parent_bone_name, bool is_connect_without_material_local, YARITORI MYMATRIX* kakeru);
