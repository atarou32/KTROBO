struct VSInput {
uint tex_id: TEX_ID;
uint offset: OFFSET;
uint value: VALUE;
};

struct GSPSInput {
float4 Position : SV_POSITION;
float2 TexCoord: TEXCOORD;
uint Value: VALUE;
};

#define KTROBO_TEXTURE_SHADER_VALUE_TEX_WIDTH_HEIGHT 512
#define KTROBO_TEXTURE_SHADER_VALUE_TEX_OFFSET_NUM 5

GSPSInput VSFunc(VSInput input) {
GSPSInput output;

output.TexCoord = float2(0,0);
output.Value = input.value;

uint tex_id = input.tex_id;
uint place = KTROBO_TEXTURE_SHADER_VALUE_TEX_OFFSET_NUM * tex_id + input.offset;



float x = place % KTROBO_TEXTURE_SHADER_VALUE_TEX_WIDTH_HEIGHT;
float y = place / KTROBO_TEXTURE_SHADER_VALUE_TEX_WIDTH_HEIGHT;
output.Position.x = x;
output.Position.y = y;
output.Position.z = 0;
output.Position.w = 1;

return output;
}


[maxvertexcount(6)]
void GSFunc( triangle GSPSInput input[3], inout TriangleStream<GSPSInput> stream) {
GSPSInput   Out=(GSPSInput)0;
Out.TexCoord = float2(0,0);
Out.Value = In[0].Value;
float base_x = In[0].Position.x;
float base_y = In[0].Position.y;
float xoffset =  2/(float)KTROBO_TEXTURE_SHADER_VALUE_TEX_WIDTH_HEIGHT;
float yoffset =  2/(float)KTROBO_TEXTURE_SHADER_VALUE_TEX_WIDTH_HEIGHT;
uint mat_index = 0;
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


}


float4 PSFunc(GSPSInput input) : SV_Target {

uint gu = asuint(input.Value);
uint r = (gu >> 24);
uint g = (gu >> 16) - (r << 8);
uint b = (gu >> 8) - (g << 8) - (r << 16);
uint a = gu - (b <<8) - (g << 16) - (r << 24);

float4 color = float4(r/255.0f, g/255.0f, b/255.0f , a/255.0f);
return float4(color.xyz,color.w);

}