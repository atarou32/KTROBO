

sampler decalSmp {
  	Filter = MIN_MAG_MAP_POINT;//MIN_MAG_MIP_LINEAR;
  	AddressU = WRAP;
  	AddressV = WRAP;
  	AddressW = WRAP;
  	ComparisonFunc = NEVER;
  	MinLOD = 0;
  	MaxLOD = FLOAT32_MAX;
};






struct VSInput {
uint4 indexs : INDEXSDAYO;
float4 cbone : KAKERUMONE;
float4 cbtwo : KAKERUMTWO;
float4 cbthree : KAKERUMTHREE;
float4 cbfour : KAKERUMFOUR;



};

//struct VSInputFromInst {
//uint4 offset : OFFSETDAYO;
//};

struct GSPSInput {
float4 Position : SV_POSITION;
uint4 indexs : BLENDINDICES0;
float4 Value: NORMAL0; 
float2 TexCoord: TEXCOORD;
float2 TexCoord2: TEXCOORD2;
};


#define KTROBO_SHADER_VALUE_BASISTEX_WIDTH_HEIGHT 4096
#define KTROBO_SHADER_VALUE_LOCALTEX_WIDTH_HEIGHT 512
#define KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT 512
#define KTROBO_COMBINED_MATRIX_CONTROL_VALUE_ROOT_BONE 0x01
#define KTROBO_COMBINED_MATRIX_CONTROL_VALUE_CONNECT_WITHOUT_MATRIX_LOCAL 0x02
#define KTROBO_COMBINED_MATRIX_CONTROL_VALUE_VOFFSET 0x04
#define KTROBO_MESH_BONE_NULL 0xFFFF
#define KTROBO_MESH_INSTANCED_ANIME_MATRIX_BASIS_NUM_MAX 512
#define KTROBO_MESH_INSTANCED_BONE_MAX 32


float4 getPosFromIndex(uint instance_index, uint bone_index) {
    float4 ans;

uint texel_per_bone = 16;
uint texel_per_instance = KTROBO_MESH_INSTANCED_BONE_MAX * texel_per_bone;
uint place =  texel_per_instance * instance_index + texel_per_bone* bone_index;
uint x = place % KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT;
uint y = floor(place / (float)KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT);
float xoffset = 2*1/(float)KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT;
float yoffset = 2*1/(float)KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT;
ans.x =   -1 + (float)x * xoffset;
ans.y =   1 - (float)y * yoffset;
ans.z = 0.0f;
ans.w = 1.0f;
return ans;
}

GSPSInput VSFunc(VSInput input,uint4 offset : OFFSETDAYO /*,VSInputFromInst inst, */,uint InstanceId : SV_InstanceID){

GSPSInput output = (GSPSInput)0;
float4 p;
float mat[16];
uint mat_index;
mat[0] = input.cbone.x;
mat[1] = input.cbone.y;
mat[2] = input.cbone.z;
mat[3] = input.cbone.w;
mat[4] = input.cbtwo.x;
mat[5] = input.cbtwo.y;
mat[6] = input.cbtwo.z;
mat[7] = input.cbtwo.w;
mat[8] = input.cbthree.x;
mat[9] = input.cbthree.y;
mat[10] = input.cbthree.z;
mat[11] = input.cbthree.w;
mat[12] = input.cbfour.x;
mat[13] = input.cbfour.y;
mat[14] = input.cbfour.z;
mat[15] = input.cbfour.w;
mat_index = offset.w % 16;
output.Value = float4(mat[mat_index],0,0,0);

p = getPosFromIndex(input.indexs.x,input.indexs.y);
output.indexs.x = input.indexs.x;
output.indexs.y = input.indexs.y;
output.indexs.z = offset.z;
output.indexs.w = offset.w;
output.Position = p;
output.Value.z = p.x;
output.Value.w = p.y;
output.TexCoord = float2(0.5f,0.5f);

  return output;

}




[maxvertexcount(6)]
void GSFunc( triangle GSPSInput In[3], inout TriangleStream<GSPSInput> gsstream )
{
    GSPSInput   Out=(GSPSInput)0;
Out.indexs = float4(In[0].indexs.x,In[0].indexs.y,In[0].indexs.z,In[0].indexs.w) ;

Out.TexCoord = float2(In[0].TexCoord.x,0);
Out.Value = float4(In[0].Value.x,0,0,0);
float base_x = In[0].Position.x;
float base_y = In[0].Position.y;
float xoffset =  2/(float)KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT;
float yoffset =  2/(float)KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT;
uint mat_index = In[0].indexs.w %16;
uint i = mat_index;

       Out.Position = float4(base_x + xoffset*i, base_y, 0.0f,1.0f);
   
       gsstream.Append(Out);
       Out.Position = float4(base_x + xoffset*i, base_y - yoffset, 0.0f, 1.0f);

       gsstream.Append(Out);
       Out.Position = float4(base_x+xoffset*i + xoffset, base_y, 0.0f, 1.0f);

       gsstream.Append(Out);


   Out.Position = float4(base_x + xoffset+xoffset*i, base_y, 0.0f, 1.0f);

       gsstream.Append(Out);
       Out.Position = float4(base_x+xoffset*i, base_y - yoffset, 0.0f, 1.0f);

       gsstream.Append(Out);
       Out.Position = float4(base_x+xoffset+xoffset*i, base_y - yoffset , 0.0f, 1.0f);

       gsstream.Append(Out);    






gsstream.RestartStrip();
return;

if (float(In[0].indexs.z) > 0.5) {
// •\
   Out.Position = float4(base_x + xoffset*i, base_y, 0.0f,1.0f);

       gsstream.Append(Out);
       Out.Position = float4(base_x + xoffset*i, base_y - yoffset, 0.0f, 1.0f);

       gsstream.Append(Out);
       Out.Position = float4(base_x + xoffset+xoffset*i, base_y, 0.0f, 1.0f);

       gsstream.Append(Out);
 gsstream.RestartStrip();
} else {
// — 
    
       Out.Position = float4(base_x + xoffset+xoffset*i, base_y, 0.0f, 1.0f);

       gsstream.Append(Out);
       Out.Position = float4(base_x+xoffset*i, base_y - yoffset, 0.0f, 1.0f);

       gsstream.Append(Out);
       Out.Position = float4(base_x+xoffset+xoffset*i, base_y - yoffset , 0.0f, 1.0f);

       gsstream.Append(Out);                

 gsstream.RestartStrip();
}

   
}





float4 PSFunc(GSPSInput input ) : SV_Target {
uint gu = asuint(input.Value.x);
uint r = (gu >> 24);
uint g = (gu >> 16) - (r << 8);
uint b = (gu >> 8) - (g << 8) - (r << 16);
uint a = gu - (b <<8) - (g << 16) - (r << 24);

float4 color = float4(r/256.0f, g/256.0f, b/256.0f , a/256.0f);
return float4(color.xyz,color.w);
}