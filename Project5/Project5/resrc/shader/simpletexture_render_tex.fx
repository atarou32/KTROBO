struct VSInput {
uint tex_id: TEX_ID;
};

struct GSPSInput {
float4 Position: SV_POSITION;
float4 WHS: NORMAL;
float2 TexCoord: TEXCOORD0;
float4 Color: COLOR;
};

sampler texSmp {
	Filter = MIN_MAG_MIP_LINEAR;
  	AddressU = WRAP;
  	AddressV = WRAP;
  	AddressW = WRAP;
  	ComparisonFunc = NEVER;
  	MinLOD = 0;
  	MaxLOD = FLOAT32_MAX;
};
sampler decalSmp {
  	Filter = MIN_MAG_MAP_POINT;//MIN_MAG_MIP_LINEAR;
  	AddressU = WRAP;
  	AddressV = WRAP;
  	AddressW = WRAP;
  	ComparisonFunc = NEVER;
  	MinLOD = 0;
  	MaxLOD = FLOAT32_MAX;
};


cbuffer c0dayo :register(c0){
uint screen_width;
uint screen_height;
uint tex_width;
uint tex_height;
uint vtex_width;
uint vtex_height;
uint offset;
uint offset2;



};


float getFValueFromTexColor(float4 color) {
    float ans = 0;
    float code = 0.1 * 1/255.0f;

    uint r = floor((color.x + code)*255);
    uint g = floor((color.y + code)*255);
    uint b = floor((color.z + code)*255);
    uint a = floor((color.w + code)*255);
    uint gu = a + (b << 8) + (g << 16) + (r <<24);
    ans = asfloat(gu);
    return ans;
}

uint getIValueFromTexColor(float4 color) {
  	uint ans = 0;
    float code = 0.1 * 1/255.0f;

    uint r = floor((color.x + code)*255);
    uint g = floor((color.y + code)*255);
    uint b = floor((color.z + code)*255);
    uint a = floor((color.w + code)*255);
    uint gu = a + (b << 8) + (g << 16) + (r <<24);
    ans = gu;
    return ans;
}

column_major float4x4 MyMatrixIdentity() {
	column_major float4x4 t;
	t = 0;
	t._11 = 1;t._22 = 1;t._33 =1; t._44 = 1;
	return t;
}

column_major float4x4 MyMatrixTranslation(float x, float y, float z) {
	column_major float4x4 ans = MyMatrixIdentity();
	ans._41 = x;
	ans._42 = y;
	ans._43 = z;
	return ans;
}





column_major float4x4 MyMatrixRotationX(float a) {
column_major float4x4 ans=(float4x4)0;
ans._11 = 1;
ans._44 = 1;
ans._22 = cos(a);
ans._32 = -sin(a);
ans._23 = sin(a);
ans._33 = cos(a);
return ans;
}

column_major float4x4 MyMatrixTranspose(float4x4 test) {
float4x4 a = test;
float4x4 ans;
ans._11 = a._11;
ans._12 = a._21;
ans._13 = a._31;
ans._14 = a._41;
ans._21 = a._21;
ans._22 = a._22;
ans._23 = a._32;
ans._24 = a._42;
ans._31 = a._13;
ans._32 = a._23;
ans._33 = a._33;
ans._34 = a._43;
ans._41 = a._14;
ans._42 = a._24;
ans._43 = a._34;
ans._44 = a._44;
return ans;
}












Texture2D texDiffuse : register(t0);
Texture2D tex : register(t1);


#define KTROBO_TEXTURE_TEX_COLOR_OFFSET 0
#define KTROBO_TEXTURE_TEX_XY_OFFSET 1
#define KTROBO_TEXTURE_TEX_WH_OFFSET 2
#define KTROBO_TEXTURE_TEX_TEXXY_OFFSET 3
#define KTROBO_TEXTURE_TEX_TEXWH_OFFSET 4
#define KTROBO_TEXTURE_TEX_OFFSET_MAX 5

uint getColorFromTex(uint tex_id) {
uint place = KTROBO_TEXTURE_TEX_OFFSET_MAX * tex_id + KTROBO_TEXTURE_TEX_COLOR_OFFSET;
uint x = place % vtex_width;
uint y = place / vtex_width;
float xcode = 0.5f/(float)vtex_width;
float ycode = 0.5f/(float)vtex_height;

float2 texcoord = float2(x/ (float)vtex_width + xcode , y / (float)vtex_height + ycode);

float4 col = tex.SampleLevel(decalSmp, texcoord,0);
uint val = getIValueFromTexColor(col);

return val;
}

uint getXYFromTex(uint tex_id) {
uint place = KTROBO_TEXTURE_TEX_OFFSET_MAX * tex_id + KTROBO_TEXTURE_TEX_XY_OFFSET;
uint x = place % vtex_width;
uint y = place / vtex_width;
float xcode = 0.5f/(float)vtex_width;
float ycode = 0.5f/(float)vtex_height;

float2 texcoord = float2(x/ (float)vtex_width + xcode, y / (float)vtex_height + ycode);

float4 col = tex.SampleLevel(decalSmp, texcoord,0);
uint val = getIValueFromTexColor(col);

return val;

}

uint getWHFromTex(uint tex_id) {

uint place = KTROBO_TEXTURE_TEX_OFFSET_MAX * tex_id + KTROBO_TEXTURE_TEX_WH_OFFSET;
uint x = place % vtex_width;
uint y = place / vtex_width;
float xcode = 0.5f/(float)vtex_width;
float ycode = 0.5f/(float)vtex_height;

float2 texcoord = float2(x/ (float)vtex_width+ xcode , y / (float)vtex_height+ycode);

float4 col = tex.SampleLevel(decalSmp, texcoord,0);
uint val = getIValueFromTexColor(col);

return val;


}

uint getTexXYFromTex(uint tex_id) {
uint place = KTROBO_TEXTURE_TEX_OFFSET_MAX * tex_id + KTROBO_TEXTURE_TEX_TEXXY_OFFSET;
uint x = place % vtex_width;
uint y = place / vtex_width;
float xcode = 0.5f/(float)vtex_width;
float ycode = 0.5f/(float)vtex_height;

float2 texcoord = float2(x/ (float)vtex_width+xcode , y / (float)vtex_height+ycode);

float4 col = tex.SampleLevel(decalSmp, texcoord,0);
uint val = getIValueFromTexColor(col);

return val;

}

uint getTexWHFromTex(uint tex_id) {
uint place = KTROBO_TEXTURE_TEX_OFFSET_MAX * tex_id + KTROBO_TEXTURE_TEX_TEXWH_OFFSET;
uint x = place % vtex_width;
uint y = place / vtex_width;
float xcode = 0.5f/(float)vtex_width;
float ycode = 0.5f/(float)vtex_height;

float2 texcoord = float2(x/ (float)vtex_width+xcode , y / (float)vtex_height+ycode);

float4 col = tex.SampleLevel(decalSmp, texcoord,0);
uint val = getIValueFromTexColor(col);

return val;


}


float4 getColorFromUINT(uint c) {
uint gu = asuint(c);
uint r = (gu >> 24);
uint g = (gu >> 16) - (r << 8);
uint b = (gu >> 8) - (g << 8) - (r << 16);
uint a = gu - (b <<8) - (g << 16) - (r << 24);
return float4(r/255.0f,g/255.0f,b/255.0f,a/255.0f);
}

GSPSInput VSFunc(VSInput input){
GSPSInput output;// (GSPSInput)0;

uint tex_id = input.tex_id;
uint color = getColorFromTex(tex_id);
uint xy = getXYFromTex(tex_id);
uint wh = getWHFromTex(tex_id);
uint tex_xy = getTexXYFromTex(tex_id);
uint tex_wh = getTexWHFromTex(tex_id);
uint x = (xy >>16);
uint y = xy - (x <<16);
uint w = (wh >> 16);
uint h = wh - (w << 16);
uint tex_x = tex_xy >> 16;
uint tex_y = tex_xy - (tex_x << 16);
uint tex_w = tex_wh  >> 16;
uint tex_h = tex_wh - (tex_w << 16);

output.Position.x = -1 + 2 *  x / (float)screen_width;
output.Position.y = 1 - 2 * y / (float)screen_height;
output.Position.z = 0.0f;
output.Position.w = 1.0f;
output.TexCoord.x = tex_x / (float)tex_width;
output.TexCoord.y = tex_y / (float)tex_height;
output.WHS.x = w/(float)screen_width;
output.WHS.y = h/(float)screen_height;
output.WHS.z = tex_w/(float)tex_width;
output.WHS.w = tex_h/(float)tex_height;
output.Color = getColorFromUINT(color);
return output;
}


[maxvertexcount(6)]
void GSFunc( triangle GSPSInput In[3], inout TriangleStream<GSPSInput> gsstream) {
GSPSInput Out;

Out.WHS = In[0].WHS;
Out.Color = In[0].Color;
float base_x = In[0].Position.x;
float base_y = In[0].Position.y;
float xoffset = In[0].WHS.x*2;
float yoffset = In[0].WHS.y*2;
float base_texx = In[0].TexCoord.x;
float base_texy = In[0].TexCoord.y;
float tex_xoffset = In[0].WHS.z;
float tex_yoffset = In[0].WHS.w;

uint mat_index = 0;
uint i = mat_index;

Out.Position = float4(base_x + xoffset*i, base_y, 0.0f,1.0f);
Out.TexCoord = float2(base_texx,base_texy);   
gsstream.Append(Out);

Out.Position = float4(base_x + xoffset*i, base_y - yoffset, 0.0f, 1.0f);
Out.TexCoord = float2(base_texx, base_texy + tex_yoffset);

gsstream.Append(Out);

Out.Position = float4(base_x+xoffset*i + xoffset, base_y, 0.0f, 1.0f);
Out.TexCoord = float2(base_texx + tex_xoffset, base_texy);

gsstream.Append(Out);


Out.Position = float4(base_x + xoffset+xoffset*i, base_y, 0.0f, 1.0f);
Out.TexCoord = float2(base_texx + tex_xoffset, base_texy);


gsstream.Append(Out);

Out.Position = float4(base_x+xoffset*i, base_y - yoffset, 0.0f, 1.0f);
Out.TexCoord = float2(base_texx, base_texy + tex_yoffset);

gsstream.Append(Out);
Out.Position = float4(base_x+xoffset+xoffset*i, base_y - yoffset , 0.0f, 1.0f);
Out.TexCoord = float2(base_texx + tex_xoffset, base_texy + tex_yoffset);

gsstream.Append(Out);    

gsstream.RestartStrip();


}



float4 PSFunc(GSPSInput input ) : SV_Target {

float4 test = texDiffuse.Sample( texSmp, float2(input.TexCoord.x , input.TexCoord.y) );


if (test.x < 0.0001 && test.y < 0.0001 && test.z <0.0001 && test.a < 0.0001) discard;
if (test.x > 0.999 && test.y > 0.999 && test.z > 0.999 && test.a < 0.0001) discard;
test =  test*input.Color; // * float4(diffuse);
test.a = input.Color.a;
return test;
}