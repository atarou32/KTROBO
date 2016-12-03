struct VSInput {
uint bill_id: BILL_ID;
};


struct GSPSInput {
float4 Position: SV_POSITION;
float4 PosWH: NORMAL0;// SV_POSITIONにしたときの長さ分
float2 TexCoord: TEXCOORD0;
float2 TexWH : NORMAL1;
float4 Color: COLOR;
};

sampler texSmp {
	Filter = MIN_MAG_MIP_LINEAR;
  	AddressU = WRAP;
  	AddressV = WRAP;
  	AddressW = WRAP;
  	ComparisonFunc = NEVER;
  	MinLOD = FLOAT32_MIN;
  	MaxLOD = FLOAT32_MAX;
};
sampler decalSmp {
  	Filter = MIN_MAG_MAP_POINT;//MIN_MAG_MIP_LINEAR;
  	AddressU = WRAP;
  	AddressV = WRAP;
  	AddressW = WRAP;
  	ComparisonFunc = NEVER;
  	MinLOD = FLOAT32_MIN;
  	MaxLOD = FLOAT32_MAX;
};


cbuffer dayo2 :register(b0){
uint screen_width;
uint screen_height;
uint tex_width;
uint tex_height;
uint vtex_width;
uint vtex_height;
uint offset;
uint offset2;
};

cbuffer dayo1 :register(b1){
column_major float4x4 view : packoffset(c0);
column_major float4x4 proj : packoffset(c4);
column_major float4x4 mym  : packoffset(c8);

};



Texture2D texDiffuse : register(t0);
Texture2D tex : register(t1);

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


float4 getColorFromUINT(uint c) {
uint gu = asuint(c);
uint r = (gu >> 24);
uint g = (gu >> 16) - (r << 8);
uint b = (gu >> 8) - (g << 8) - (r << 16);
uint a = gu - (b <<8) - (g << 16) - (r << 24);
return float4(r/255.0f,g/255.0f,b/255.0f,a/255.0f);
}

#define KTROBO_TEXTURE_BILL_COLOR_OFFSET 0
#define KTROBO_TEXTURE_BILL_WORLD_OFFSET 1
#define KTROBO_TEXTURE_BILL_W_OFFSET 17
#define KTROBO_TEXTURE_BILL_H_OFFSET 18
#define KTROBO_TEXTURE_BILL_TEXXY_OFFSET 19
#define KTROBO_TEXTURE_BILL_TEXWH_OFFSET 20
#define KTROBO_TEXTURE_BILL_OFFSET_MAX 21

uint getColorFromBillId(uint bill_id) {
uint place = KTROBO_TEXTURE_BILL_OFFSET_MAX * bill_id + KTROBO_TEXTURE_BILL_COLOR_OFFSET;
uint x = place % vtex_width;
uint y = place / vtex_width;
float xcode = 0.5f/(float)vtex_width;
float ycode = 0.5f/(float)vtex_height;

float2 texcoord = float2(x/ (float)vtex_width + xcode , y / (float)vtex_height + ycode);

float4 col = tex.SampleLevel(decalSmp, texcoord,0);
uint val = getIValueFromTexColor(col);
return val;
}


float getMatrixValueFromBillId(uint bill_id, uint offset) {
uint place = KTROBO_TEXTURE_BILL_OFFSET_MAX * bill_id + KTROBO_TEXTURE_BILL_WORLD_OFFSET+offset;
uint x = place % vtex_width;
uint y = place / vtex_width;
float xcode = 0.5f/(float)vtex_width;
float ycode = 0.5f/(float)vtex_height;

float2 texcoord = float2(x/ (float)vtex_width + xcode , y / (float)vtex_height + ycode);

float4 col = tex.SampleLevel(decalSmp, texcoord,0);
float val = getFValueFromTexColor(col);
return val;
}


column_major float4x4 getMatrixFromBillId(uint bill_id) {
column_major float4x4 ans;
ans._11 = getMatrixValueFromBillId(bill_id, 0);
ans._21 = getMatrixValueFromBillId(bill_id, 1);
ans._31 = getMatrixValueFromBillId(bill_id, 2);
ans._41 = getMatrixValueFromBillId(bill_id, 3);
ans._12 = getMatrixValueFromBillId(bill_id, 4);
ans._22 = getMatrixValueFromBillId(bill_id, 5);
ans._32 = getMatrixValueFromBillId(bill_id, 6);
ans._42 = getMatrixValueFromBillId(bill_id, 7);
ans._13 = getMatrixValueFromBillId(bill_id, 8);
ans._23 = getMatrixValueFromBillId(bill_id, 9);
ans._33 = getMatrixValueFromBillId(bill_id,10);
ans._43 = getMatrixValueFromBillId(bill_id,11);
ans._14 = getMatrixValueFromBillId(bill_id,12);
ans._24 = getMatrixValueFromBillId(bill_id,13);
ans._34 = getMatrixValueFromBillId(bill_id,14);
ans._44 = getMatrixValueFromBillId(bill_id,15);
return  ans;//MyMatrixTranspose(ans);
}



float getWidthFromBillId(uint bill_id) {
uint place = KTROBO_TEXTURE_BILL_OFFSET_MAX * bill_id + KTROBO_TEXTURE_BILL_W_OFFSET;
uint x = place % vtex_width;
uint y = place / vtex_width;
float xcode = 0.5f/(float)vtex_width;
float ycode = 0.5f/(float)vtex_height;

float2 texcoord = float2(x/ (float)vtex_width + xcode , y / (float)vtex_height + ycode);

float4 col = tex.SampleLevel(decalSmp, texcoord,0);
float val = getFValueFromTexColor(col);
return val;
}

float getHeightFromBillId(uint bill_id) {
uint place = KTROBO_TEXTURE_BILL_OFFSET_MAX * bill_id + KTROBO_TEXTURE_BILL_H_OFFSET;
uint x = place % vtex_width;
uint y = place / vtex_width;
float xcode = 0.5f/(float)vtex_width;
float ycode = 0.5f/(float)vtex_height;

float2 texcoord = float2(x/ (float)vtex_width + xcode , y / (float)vtex_height + ycode);

float4 col = tex.SampleLevel(decalSmp, texcoord,0);
float val = getFValueFromTexColor(col);
return val;
}

uint getTexXYFromBillId(uint bill_id) {
uint place = KTROBO_TEXTURE_BILL_OFFSET_MAX * bill_id + KTROBO_TEXTURE_BILL_TEXXY_OFFSET;
uint x = place % vtex_width;
uint y = place / vtex_width;
float xcode = 0.5f/(float)vtex_width;
float ycode = 0.5f/(float)vtex_height;

float2 texcoord = float2(x/ (float)vtex_width + xcode , y / (float)vtex_height + ycode);

float4 col = tex.SampleLevel(decalSmp, texcoord,0);
uint val = getIValueFromTexColor(col);
return val;
}

uint getTexWHFromBillId(uint bill_id) {
uint place = KTROBO_TEXTURE_BILL_OFFSET_MAX * bill_id + KTROBO_TEXTURE_BILL_TEXWH_OFFSET;
uint x = place % vtex_width;
uint y = place / vtex_width;
float xcode = 0.5f/(float)vtex_width;
float ycode = 0.5f/(float)vtex_height;

float2 texcoord = float2(x/ (float)vtex_width + xcode , y / (float)vtex_height + ycode);

float4 col = tex.SampleLevel(decalSmp, texcoord,0);
uint val = getIValueFromTexColor(col);
return val;
}




GSPSInput VSFunc(VSInput input){
GSPSInput output;// (GSPSInput)0;

uint bill_id = input.bill_id;
uint color = getColorFromBillId(bill_id);
column_major float4x4 mat = getMatrixFromBillId(bill_id);
float w = getWidthFromBillId(bill_id);
float h = getHeightFromBillId(bill_id);
uint tex_xy = getTexXYFromBillId(bill_id);
uint tex_wh = getTexWHFromBillId(bill_id);
uint tex_x = tex_xy >> 16;
uint tex_y = tex_xy - (tex_x <<16);
uint tex_w = tex_wh >> 16;
uint tex_h = tex_wh - (tex_w << 16);

// 左上　と　右下の点のSV_POSITIONを計算して xy の差をposwhに入れる
// 中点が中心の点となるはずなので下は計算しなくても大丈夫
// 中心の点のSV_POSITIONを計算して　Positionに入れる


column_major float4x4 tview = view;


column_major float4x4 tproj = proj;

float4 localPos = float4(0,0,0,1);
float4 mymPos = float4(mul(mat, localPos).xyzw);
float4 worldPos = float4(mul(mym,mymPos ));
float4 viewPos  = float4(mul(  tview,worldPos ).xyzw);
float4 projPos  = float4(mul(  tproj,viewPos ).xyzw);


float4 localPos2 = float4(w/2,0,h/2,1);

float4 mymPos2 = float4(mul(mat, localPos2).xyz,1);
float4 worldPos2 = float4(mul(mym,mymPos2 ));
float4 viewPos2 = float4(mul( tview, worldPos2 ).xyzw);
float4 projPos2  = float4(mul( tproj, viewPos2 ).xyzw);


float4 localPos3 = float4(w/2,0,-h/2,1);
float4 mymPos3 = float4(mul(mat, localPos3).xyzw);
float4 worldPos3 = float4(mul(mym,mymPos3 ));
float4 viewPos3 = float4(mul( tview, worldPos3 ).xyzw);
float4 projPos3  = float4(mul( tproj, viewPos3 ).xyzw);

output.Position = float4(projPos.x,projPos.y,projPos.z,projPos.w);
output.Color = getColorFromUINT(color);
output.TexCoord = float2(tex_x / (float)tex_width, tex_y / (float)tex_height);
output.TexWH = float2(tex_w / (float)tex_width, tex_h / (float)tex_height);
output.PosWH = float4((projPos2.x - projPos.x)*2, (projPos2.y - projPos.y)*2, projPos3.x, projPos3.y);





return output;
}


[maxvertexcount(6)]
void GSFunc( triangle GSPSInput In[3], inout TriangleStream<GSPSInput> gsstream) {

GSPSInput Out;
Out.PosWH = float4(0,0,0,0);
Out.TexWH = float2(0,0);
Out.Color = In[0].Color;

float xoffset = In[0].PosWH.z - In[0].Position.x;  
float yoffset =  -In[0].PosWH.w + In[0].Position.y;

float xoffset2 =  In[0].PosWH.x/2;
float yoffset2 =  In[0].PosWH.y/2;
float base_x = In[0].Position.x;
float base_y = In[0].Position.y;

float base_texx = In[0].TexCoord.x;
float base_texy = In[0].TexCoord.y;
float tex_xoffset = In[0].TexWH.x;
float tex_yoffset = In[0].TexWH.y;

uint mat_index = 0;
uint i = mat_index;

Out.Position = float4(base_x-xoffset, base_y+yoffset, In[0].Position.z,In[0].Position.w);
Out.TexCoord = float2(base_texx,base_texy);   
gsstream.Append(Out);

Out.Position = float4(base_x - xoffset2, base_y - yoffset2, In[0].Position.z,In[0].Position.w);
Out.TexCoord = float2(base_texx, base_texy + tex_yoffset);

gsstream.Append(Out);

Out.Position = float4(base_x + xoffset2, base_y + yoffset2, In[0].Position.z,In[0].Position.w);
Out.TexCoord = float2(base_texx + tex_xoffset, base_texy);

gsstream.Append(Out);


Out.Position = float4(base_x + xoffset2, base_y + yoffset2, In[0].Position.z,In[0].Position.w);
Out.TexCoord = float2(base_texx + tex_xoffset, base_texy);


gsstream.Append(Out);

Out.Position = float4(base_x-xoffset2, base_y  - yoffset2, In[0].Position.z,In[0].Position.w);
Out.TexCoord = float2(base_texx, base_texy + tex_yoffset);

gsstream.Append(Out);
Out.Position = float4(base_x+xoffset, base_y - yoffset , In[0].Position.z,In[0].Position.w);
Out.TexCoord = float2(base_texx + tex_xoffset, base_texy + tex_yoffset);

gsstream.Append(Out);    

gsstream.RestartStrip();

}



float4 PSFunc(GSPSInput input ) : SV_Target {
//float3 L = -normalize(float4(1,1,-10,1));//lightdir.xyz);
//float3 N = normalize(input.Normal);
//float3 E = normalize(viewdir);
//float3 H = normalize(L+E);
//float4 cc = float4(1,1,1,1);
//float4 diffuse;
//float4 color = getColorFromTex(input.instance_id, color_id);
 //diffuse = color * max(dot( L, N ), 0.25 );
//diffuse.a = color.a;

float4 test = texDiffuse.Sample( texSmp, float2(input.TexCoord.x , input.TexCoord.y) );

if (test.x < 0.0001 && test.y < 0.0001 && test.z <0.0001 && test.a < 0.0001) discard;

float4 test2 = test;
// * input.Color;
test2 *= input.Color;
return test2;
//float4(diffuse);
}