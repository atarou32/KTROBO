struct VSInput {
float3 Position : POSITION;
uint Index0 : INDone;
uint Index1 : INDtwo;
uint Index2 : INDthree;
uint Index3 : INDfour;
uint4 Index4 : INDfive;
float4 Weight : WEIGHT;
float Weightfive : WEIGHTfive;
float3 Normal: NORMAL;
float2 TexCoord : TEXCOORD;
uint instance_id : INSTANCEIDDAYO;
column_major float4x4 world : WORLDMAT;
};

struct GSPSInput {
float4 Position: SV_POSITION;
float2 TexCoord: TEXCOORD;
float3 Normal : NORMAL;
uint instance_id : INSTANCEIDDAYO;
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
column_major float4x4 view;
column_major float4x4 proj;
float4 lightdir;
};

cbuffer c1dayo :register(c1){
uint color_id;
uint offset1;
uint offset2;
uint offset3;
};


float getValueFromTexColor(float4 color) {
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









float getMatrixValueFromTex(uint offset, Texture2D tex, float2 texcoord, int tex_width, int tex_height) {

float xcode = 0.5f/(float)tex_width;
float ycode = 0.5f/(float)tex_height;


float4 col = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*offset, texcoord.y + ycode),0);
float val = getValueFromTexColor(col);
return val;
/*
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*offset, texcoord.y + ycode),0);
value[i] = getValueFromTexColor(color[i]);
i = 1;
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*i, texcoord.y + ycode),0);
value[i] = getValueFromTexColor(color[i]);
i = 2;
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*i, texcoord.y + ycode),0);
value[i] = getValueFromTexColor(color[i]);
i = 3;
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*i, texcoord.y + ycode),0);
value[i] = getValueFromTexColor(color[i]);
i = 4;
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*i, texcoord.y + ycode),0);
value[i] = getValueFromTexColor(color[i]);
i = 5;
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*i, texcoord.y + ycode),0);
value[i] = getValueFromTexColor(color[i]);
i = 6;
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*i, texcoord.y + ycode),0);
value[i] = getValueFromTexColor(color[i]);
i = 7;
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*i, texcoord.y + ycode),0);
value[i] = getValueFromTexColor(color[i]);
i = 8;
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*i, texcoord.y + ycode),0);
value[i] = getValueFromTexColor(color[i]);
i = 9;
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*i, texcoord.y + ycode),0);
value[i] = getValueFromTexColor(color[i]);
i = 10;
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*i, texcoord.y + ycode),0);
value[i] = getValueFromTexColor(color[i]);
i = 11;
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*i, texcoord.y + ycode),0);
value[i] = getValueFromTexColor(color[i]);
i = 12;
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*i, texcoord.y + ycode),0);
value[i] = getValueFromTexColor(color[i]);
i = 13;
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*i, texcoord.y + ycode),0);
value[i] = getValueFromTexColor(color[i]);
i = 14;
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*i, texcoord.y + ycode),0);
value[i] = getValueFromTexColor(color[i]);
i = 15;
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*i, texcoord.y + ycode),0);
value[i] = getValueFromTexColor(color[i]);
*/
/*
ans._11 = value[0];
ans._12 = value[1];
ans._13 = value[2];
ans._14 = value[3];
ans._21 = value[4];
ans._22 = value[5];
ans._23 = value[6];
ans._24 = value[7];
ans._31 = value[8];
ans._32 = value[9];
ans._33 = value[10];
ans._34 = value[11];
ans._41 = value[12];
ans._42 = value[13];
ans._43 = value[14];
ans._44 = value[15];
return ans;
*/
}

column_major float4x4 getMatrixFromTex(Texture2D tex, float2 texcoord, int tex_width, int tex_height) {

column_major float4x4 mat = (float4x4)0;

mat._11 = getMatrixValueFromTex(0,tex,texcoord,tex_width,tex_height);
mat._21 = getMatrixValueFromTex(1,tex,texcoord,tex_width,tex_height);
mat._31 = getMatrixValueFromTex(2,tex,texcoord,tex_width,tex_height);
mat._41 = getMatrixValueFromTex(3,tex,texcoord,tex_width,tex_height);
mat._12 = getMatrixValueFromTex(4,tex,texcoord,tex_width,tex_height);
mat._22 = getMatrixValueFromTex(5,tex,texcoord,tex_width,tex_height);
mat._32 = getMatrixValueFromTex(6,tex,texcoord,tex_width,tex_height);
mat._42 = getMatrixValueFromTex(7,tex,texcoord,tex_width,tex_height);
mat._13 = getMatrixValueFromTex(8,tex,texcoord,tex_width,tex_height);
mat._23 = getMatrixValueFromTex(9,tex,texcoord,tex_width,tex_height);
mat._33 = getMatrixValueFromTex(10,tex,texcoord,tex_width,tex_height);
mat._43 = getMatrixValueFromTex(11,tex,texcoord,tex_width,tex_height);
mat._14 = getMatrixValueFromTex(12,tex,texcoord,tex_width,tex_height);
mat._24 = getMatrixValueFromTex(13,tex,texcoord,tex_width,tex_height);
mat._34 = getMatrixValueFromTex(14,tex,texcoord,tex_width,tex_height);
mat._44 = getMatrixValueFromTex(15,tex,texcoord,tex_width,tex_height);


/*
mat._11 = getMatrixValueFromTex(0,tex,texcoord,tex_width,tex_height);
mat._12 = getMatrixValueFromTex(1,tex,texcoord,tex_width,tex_height);
mat._13 = getMatrixValueFromTex(2,tex,texcoord,tex_width,tex_height);
mat._14 = getMatrixValueFromTex(3,tex,texcoord,tex_width,tex_height);
mat._21 = getMatrixValueFromTex(4,tex,texcoord,tex_width,tex_height);
mat._22 = getMatrixValueFromTex(5,tex,texcoord,tex_width,tex_height);
mat._23 = getMatrixValueFromTex(6,tex,texcoord,tex_width,tex_height);
mat._24 = getMatrixValueFromTex(7,tex,texcoord,tex_width,tex_height);
mat._31 = getMatrixValueFromTex(8,tex,texcoord,tex_width,tex_height);
mat._32 = getMatrixValueFromTex(9,tex,texcoord,tex_width,tex_height);
mat._33 = getMatrixValueFromTex(10,tex,texcoord,tex_width,tex_height);
mat._34 = getMatrixValueFromTex(11,tex,texcoord,tex_width,tex_height);
mat._41 = getMatrixValueFromTex(12,tex,texcoord,tex_width,tex_height);
mat._42 = getMatrixValueFromTex(13,tex,texcoord,tex_width,tex_height);
mat._43 = getMatrixValueFromTex(14,tex,texcoord,tex_width,tex_height);
mat._44 = getMatrixValueFromTex(15,tex,texcoord,tex_width,tex_height);
*/









return mat;
}


#define KTROBO_MESH_INSTANCED_BONE_MAX 32
#define KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT 512
Texture2D comTex : register(t0);

column_major float4x4 getCombinedMatrix(uint instance_index, uint bone_index) {
column_major float4x4 ans;
uint texel_per_bone = 16;
uint texel_per_instance = KTROBO_MESH_INSTANCED_BONE_MAX * texel_per_bone;
uint place = texel_per_instance * instance_index + texel_per_bone* bone_index;
uint x = place % KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT;
uint y = place / KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT;
float xoffset = 1/(float)KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT;
float yoffset = 1/(float)KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT;

float2 tex_coord = float2(x*xoffset, y*yoffset);

ans._11 = 1;
ans._12 = 0;
ans._13 = 0;
ans._14 = 0;
ans._21 = 0;
ans._22 = 1;
ans._23 = 0;
ans._24 = 0;
ans._31 = 0;
ans._32 = 0;
ans._33 = 1;
ans._34 = 0;
ans._41 = 0;
ans._42 = 0;
ans._43 = 0;
ans._44 = 1;

ans = getMatrixFromTex(comTex, tex_coord, KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT,
 KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT);
//ans = MyMatrixTranspose(ans);
//ans = MyMatrixTranslation(0,0,4);
return ans;// MyMatrixTranspose(ans);
}



GSPSInput VSFunc(VSInput input){
GSPSInput output;// (GSPSInput)0;

float4 temppos = float4( input.Position, 1.0f);

float4x4 combined_matrixs[5];

combined_matrixs[0] = getCombinedMatrix(input.instance_id, input.Index0);
combined_matrixs[1] = getCombinedMatrix(input.instance_id, input.Index1);
combined_matrixs[2] = getCombinedMatrix(input.instance_id, input.Index2);
combined_matrixs[3] = getCombinedMatrix(input.instance_id, input.Index3);
combined_matrixs[4] = getCombinedMatrix(input.instance_id, input.Index4.x);







float4 localPos =  mul(combined_matrixs[0],temppos)*input.Weight[0] + 
					  mul(combined_matrixs[1],temppos)*input.Weight[1] +
					  mul(combined_matrixs[2],temppos)*input.Weight[2] + 
					  mul(combined_matrixs[3],temppos)*(input.Weight[3]) +
                                          mul(combined_matrixs[4], temppos)*(input.Weightfive);


float4 worldPos = mul( input.world, localPos );
float4 viewPos  = mul( view, worldPos );
float4 projPos  = mul( proj, viewPos );
output.Position = projPos;

output.TexCoord = input.TexCoord.xy;

float4 temp_normal = float4(input.Normal.xyz,0);
float4 local_normal =  mul(combined_matrixs[0],temp_normal)*input.Weight[0] + 
					  mul(combined_matrixs[1],temp_normal)*input.Weight[1] +
					  mul(combined_matrixs[2],temp_normal)*input.Weight[2] + 
					  mul(combined_matrixs[3],temp_normal)*(input.Weight[3]) +
                                          mul(combined_matrixs[4], temp_normal)*(input.Weightfive);
float4 world_normal = float4(input.Normal.xyz,0);//mul( input.world, temp_normal);//local_normal );

output.Normal = world_normal.xyz;
output.instance_id = input.instance_id;
return output;
}

Texture2D texColor : register(t1);
Texture2D texDiffuse : register(t2);

#define KTROBO_SHADER_VALUE_COLORTEX_WIDTH_HEIGHT 512


float4 getColorFromTex(uint instance_id, uint cid) {
float4 c = float4(1,1,0,1);//(float4)0;
float offsetx =  1/(float)KTROBO_SHADER_VALUE_COLORTEX_WIDTH_HEIGHT;
float offsety =  1/(float)KTROBO_SHADER_VALUE_COLORTEX_WIDTH_HEIGHT;

float base_x =  cid * offsetx;
float base_y =  instance_id * offsety;
c = texColor.Sample( decalSmp, float2(base_x + offsetx/2 , base_y + offsety/2) );
return c;


}



[maxvertexcount(3)]
void GSFunc( triangle GSPSInput input[3], inout TriangleStream<GSPSInput> stream) {
stream.Append(input[2]);
stream.Append(input[1]);
stream.Append(input[0]);
stream.RestartStrip();
}



float4 PSFunc(GSPSInput input ) : SV_Target {
float3 L = -normalize(float4(lightdir).xyz);
float3 N = normalize(input.Normal.xyz);
//float3 E = normalize(viewdir);
//float3 H = normalize(L+E);
//float4 cc = float4(1,1,1,1);
float3 diffuse;
float4 color = getColorFromTex(input.instance_id, color_id);
 diffuse = color.xyz * max(dot( L, N ), 0.20 );
//diffuse.a = color.a;
float4 amb = float4(0.4,0.4,0.4,1);
float4 test = texDiffuse.Sample( texSmp, float2(input.TexCoord.x , input.TexCoord.y) );
return test * float4(diffuse,1);// + amb;
}