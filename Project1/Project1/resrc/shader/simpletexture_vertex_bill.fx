struct VSInput {
uint bill_id: BILL_ID;
uint offset: OFFSET;
float fvalue: V_FLOAT;
uint ivalue: V_INT;
};

struct GSPSInput {
float4 Position : SV_POSITION;
float2 TexCoord: TEXCOORD;
uint Value: VALUE;
};

#define KTROBO_TEXTURE_SHADER_VALUE_BILL_WIDTH_HEIGHT 512
#define KTROBO_TEXTURE_SHADER_VALUE_BILL_OFFSET_NUM 21

GSPSInput VSFunc(VSInput input) {
GSPSInput output;

output.TexCoord = float2(0,0);

uint is_ivalue = (input.offset >> 8);
uint offset = input.offset - (is_ivalue << 8);
if (is_ivalue >0) {
  output.Value = input.ivalue;
} else {
  output.Value = asuint(input.fvalue);
}
  
uint bill_id = input.bill_id;
uint place = KTROBO_TEXTURE_SHADER_VALUE_BILL_OFFSET_NUM * bill_id + offset;



float x = place % KTROBO_TEXTURE_SHADER_VALUE_BILL_WIDTH_HEIGHT;
float y = place / KTROBO_TEXTURE_SHADER_VALUE_BILL_WIDTH_HEIGHT;
output.Position.x = x;
output.Position.y = y;
output.Position.z = 0;
output.Position.w = 1;

return output;
}


[maxvertexcount(6)]
void GSFunc( triangle GSPSInput In[3], inout TriangleStream<GSPSInput> gsstream) {
GSPSInput   Out=(GSPSInput)0;
Out.TexCoord = float2(0,0);
Out.Value = In[0].Value;
float base_x = -1 + In[0].Position.x* 2 / (float)KTROBO_TEXTURE_SHADER_VALUE_BILL_WIDTH_HEIGHT;
float base_y = 1 - 2 * In[0].Position.y / (float)KTROBO_TEXTURE_SHADER_VALUE_BILL_WIDTH_HEIGHT;
float xoffset =  2/(float)KTROBO_TEXTURE_SHADER_VALUE_BILL_WIDTH_HEIGHT;
float yoffset =  2/(float)KTROBO_TEXTURE_SHADER_VALUE_BILL_WIDTH_HEIGHT;
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