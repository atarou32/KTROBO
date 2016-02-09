ab = AnimationBuilder:getIS(0,0)
gu = GUI:getIS(0,0)
test = gu:getStrFromInput(inputtext_call_lua)
tfl = TextFromLua:getIS(0,0)
tfl:makeAILua(true, test)
--void makeAILua(bool is_lock_sita, char* lua_filename);