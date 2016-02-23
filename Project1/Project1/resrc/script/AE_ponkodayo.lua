ae = ActionEditor:getIS(0,0)
gu = GUI:getIS(0,0)

character_id = ae:createActionCharacter("ponko");
hon_mesh_id = ae:setHonMesh(character_id,"resrc/model/ponko2-4/pk2sailordayo","","",true,{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1})
ae:setOnajiMesh(character_id, hon_mesh_id, "resrc/model/ponko2-3/pk2skirt")
ae:setOnajiMesh(character_id, hon_mesh_id, "resrc/model/ponko2-3/pk2face")
ae:setOnajiMesh(character_id, hon_mesh_id, "resrc/model/ponko2-3/pk2hidarime")
ae:setOnajiMesh(character_id, hon_mesh_id, "resrc/model/ponko2-3/pk2hidarimedama")
ae:setOnajiMesh(character_id, hon_mesh_id, "resrc/model/ponko2-3/pk2kami")
ae:setOnajiMesh(character_id, hon_mesh_id, "resrc/model/ponko2-3/pk2kuti")
ae:setOnajiMesh(character_id, hon_mesh_id, "resrc/model/ponko2-3/pk2kubi")
ae:setOnajiMesh(character_id, hon_mesh_id, "resrc/model/ponko2-3/pk2mayuge")
ae:setOnajiMesh(character_id, hon_mesh_id, "resrc/model/ponko2-3/pk2midarime")
ae:setOnajiMesh(character_id, hon_mesh_id, "resrc/model/ponko2-3/pk2migimedama")
ae:setHonMesh(character_id, "resrc/model/ponko2-4/pk2migihanddayo", "resrc/model/ponko2-4/pk2sailordayo", "migiArmTekubiBone", true,{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1})
ae:setHonMesh(character_id,"resrc/model/bazooka/rweaponbazookaguna", "resrc/model/ponko2-4/pk2migihanddayo", "MigiHandMotiBone", false,{0.00026,0.333,0.00026,0,-0.00026,0.00026,-0.333,0,-0.333,0,0.00026,0,0,0,0,1})
ae:setSkeleton(character_id, hon_mesh_id, "resrc/model/ponko2-4/pk2sailordayo.anime","resrc/script/pon4.akat")
ae:setSkeleton(character_id, hon_mesh_id, "resrc/script/pon4.anime","resrc/script/pon4.akat")
ae:setNowCharacterId(character_id)
--void setOnajiMesh(int impl_id, char* onaji_filepath);
--void setKoMesh(int impl_id, char* ko_filepath, char* oya_filepath,char* parent_bone_name, bool is_connect_without_material_local, YARITORI MYMATRIX* kakeru);
