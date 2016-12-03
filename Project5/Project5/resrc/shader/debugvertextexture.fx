
//RWByteAddressBuffer rwBuf :register(u0);
RWByteAddressBuffer ansBuf : register(u0);

//Texture2D basisTex : register(t0); // matrix_basis_texture
//Texture2D localTex : register(t0); // matrix_local_texture
Texture2D comTex: register(t0); // matrix_combined_texture
#define KTROBO_SHADER_VALUE_BASISTEX_WIDTH_HEIGHT 4096
#define KTROBO_SHADER_VALUE_LOCALTEX_WIDTH_HEIGHT 512
#define KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT 512
#define KTROBO_COMBINED_MATRIX_CONTROL_VALUE_ROOT_BONE 0x01
#define KTROBO_COMBINED_MATRIX_CONTROL_VALUE_CONNECT_WITHOUT_MATRIX_LOCAL 0x02
#define KTROBO_COMBINED_MATRIX_CONTROL_VALUE_VOFFSET 0x04
#define KTROBO_MESH_BONE_NULL 0xFFFF
#define KTROBO_MESH_INSTANCED_ANIME_MATRIX_BASIS_NUM_MAX 512
#define KTROBO_MESH_INSTANCED_BONE_MAX 32






sampler decalSmp {
  	Filter = MIN_MAG_MAP_POINT;//MIN_MAG_MIP_LINEAR;
  	AddressU = WRAP;
  	AddressV = WRAP;
  	AddressW = WRAP;
  	ComparisonFunc = NEVER;
  	MinLOD = 0;
  	MaxLOD = FLOAT32_MAX;
};


column_major float4x4 MyMatrixTranspose(float4x4 test) {
float4x4 a = test;
float4x4 ans;
ans._11 = a._11;
ans._12 = a._21;
ans._13 = a._31;
ans._14 = a._41;
ans._21 = a._12;
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

column_major float4x4 MyMatrixIdentity() {
	column_major float4x4 t;
	t = 0;
	t._11 = 1;t._22 = 1;t._33 =1; t._44 = 1;
	return t;
}

column_major float4x4 MyMatrixUnko() {

	column_major float4x4 t = MyMatrixIdentity();
	
	t._22 = -1;
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

column_major float4x4 MyMatrixRotationZ(float a) {
column_major float4x4 ans=(float4x4)0;
ans._11 = cos(a);
ans._12 = sin(a);
ans._21 = -sin(a);
ans._44 = 1;
ans._22 = cos(a);
//ans._32 = -sin(a);
//ans._23 = sin(a);
ans._33 = 1;
return ans;
}




column_major float4x4 MyMatrixMultiply2(float4x4 a, float4x4 b) {

	column_major float4x4 ans;
	ans._11 = a._11 * b._11 + a._12 * b._21 + a._13 * b._31 + a._14 * b._41;
	ans._12 = a._11 * b._12 + a._12 * b._22 + a._13 * b._32 + a._14 * b._42;
	ans._13 = a._11 * b._13 + a._12 * b._23 + a._13 * b._33 + a._14 * b._43;
	ans._14 = a._11 * b._14 + a._12 * b._24 + a._13 * b._34 + a._14 * b._44;

	ans._21 = a._21 * b._11 + a._22 * b._21 + a._23 * b._31 + a._24 * b._41;
	ans._22 = a._21 * b._12 + a._22 * b._22 + a._23 * b._32 + a._24 * b._42;
	ans._23 = a._21 * b._13 + a._22 * b._23 + a._23 * b._33 + a._24 * b._43;
	ans._24 = a._21 * b._14 + a._22 * b._24 + a._23 * b._34 + a._24 * b._44;

	ans._31 = a._31 * b._11 + a._32 * b._21 + a._33 * b._31 + a._34 * b._41;
	ans._32 = a._31 * b._12 + a._32 * b._22 + a._33 * b._32 + a._34 * b._42;
	ans._33 = a._31 * b._13 + a._32 * b._23 + a._33 * b._33 + a._34 * b._43;
	ans._34 = a._31 * b._14 + a._32 * b._24 + a._33 * b._34 + a._34 * b._44;

	ans._41 = a._41 * b._11 + a._42 * b._21 + a._43 * b._31 + a._44 * b._41;
	ans._42 = a._41 * b._12 + a._42 * b._22 + a._43 * b._32 + a._44 * b._42;
	ans._43 = a._41 * b._13 + a._42 * b._23 + a._43 * b._33 + a._44 * b._43;
	ans._44 = a._41 * b._14 + a._42 * b._24 + a._43 * b._34 + a._44 * b._44;

	return ans;
}

column_major float4x4 MyMatrixMultiply(float4x4 a, float4x4 b) {
return MyMatrixMultiply2(b,a);
//return MyMatrixMultiply2(MyMatrixTranspose(b),MyMatrixTranspose(a));


}

column_major float4x4 MyMatrixInverse(float4x4 mat) {
//	float detA = MyMatrixDetA(mat);
//return MyMatrixIdentity();

	float detA = mat._11 * mat._22 * mat._33 * mat._44 + mat._11 * mat._23 * mat._34 * mat._42 + mat._11 * mat._24 * mat._32 * mat._43 +
		mat._12 * mat._21 * mat._34 * mat._43 + mat._12 * mat._23 * mat._31 * mat._44 + mat._12 * mat._24 * mat._33 * mat._41 +
		mat._13 * mat._21 * mat._32 * mat._44 + mat._13 * mat._22 * mat._34 * mat._41 + mat._13 * mat._24 * mat._31 * mat._42 +
		mat._14 * mat._21 * mat._33 * mat._42 + mat._14 * mat._22 * mat._31 * mat._43 + mat._14 * mat._23 * mat._32 * mat._41 -
		mat._11 * mat._22 * mat._34 * mat._43 - mat._11 * mat._23 * mat._32 * mat._44 - mat._11 * mat._24 * mat._33 * mat._42 -
		mat._12 * mat._21 * mat._33 * mat._44 - mat._12 * mat._23 * mat._34 * mat._41 - mat._12 * mat._24 * mat._31 * mat._43 -
		mat._13 * mat._21 * mat._34 * mat._42 - mat._13 * mat._22 * mat._31 * mat._44 - mat._13 * mat._24 * mat._32 * mat._41 -
		mat._14 * mat._21 * mat._32 * mat._43 - mat._14 * mat._22 * mat._33 * mat._41 - mat._14 * mat._23 * mat._31 * mat._42;

	if (detA == 0 ) {
		detA = 1;
	}
	/*
	float b11 = mat._22 * mat._33 * mat._44 + 
		mat._23 * mat._34 * mat._42 +
		mat._24 * mat._32 * mat._43 -
		mat._22 * mat._34 * mat._43 -
		mat._23 * mat._32 * mat._44 -
		mat._24 * mat._33 * mat._42;

	float b12 = mat._12 * mat._34 * mat._43 +
		mat._13 * mat._32 * mat._44 +
		mat._14 * mat._33 * mat._42 -
		mat._12 * mat._33 * mat._44 -
		mat._13 * mat._34 * mat._42 -
		mat._14 * mat._32 * mat._43;

	float b13 = mat._12 * mat._23 * mat._44 +
		mat._13 * mat._24* mat._42 +
		mat._14 * mat._22 * mat._43 -
		mat._12 * mat._24 * mat._43 - 
		mat._13 * mat._22 * mat._44 -
		mat._14 * mat._23 * mat._42;

	float b14 = mat._12 * mat._24 * mat._33 +
		mat._13 * mat._22 * mat._34 +
		+mat._14 * mat._23 * mat._32-
		mat._12 * mat._23 * mat._34 -
		mat._13 * mat._24 * mat._32 - 
		mat._14 * mat._22 * mat._33;

	float b21 = mat._21 * mat._34 * mat._43 +
		mat._23 * mat._31 * mat._44 +
		mat._24 * mat._33 * mat._41 -
		mat._21 * mat._33 * mat._44 -
		mat._23 * mat._34 * mat._41 -
		mat._24 * mat._31 * mat._43;

	float b22 = mat._11 * mat._33 * mat._44 +
		mat._13 * mat._34 * mat._41 +
		mat._14 * mat._31 * mat._43 -
		mat._11 * mat._34 * mat._43 -
		mat._13 * mat._31 * mat._44 -
		mat._14 * mat._33 * mat._41;

	float b23 = mat._11 * mat._24 * mat._43 +
		mat._13 * mat._21 * mat._44 +
		mat._14 * mat._23 * mat._41 -
		mat._11 * mat._23 * mat._44 -
		mat._13 * mat._24 * mat._41 -
		mat._14 * mat._21 * mat._43;

	float b24 = mat._11 * mat._23 * mat._34 +
		mat._13 * mat._24 * mat._31 +
		mat._14 * mat._21 * mat._33 -
		mat._11 * mat._24 * mat._33 -
		mat._13 * mat._21 * mat._34 -
		mat._14 * mat._23 * mat._31;

	float b31 = mat._21 * mat._32 * mat._44 +
		mat._22 * mat._34 * mat._41 +
		mat._24 * mat._31* mat._42 -
		mat._21 * mat._34 * mat._42 -
		mat._22 * mat._31 * mat._44 -
		mat._24 * mat._32 * mat._41;

	float b32 = mat._11 * mat._34 * mat._42 +
		mat._12 * mat._31 * mat._44 +
		mat._14 * mat._32 * mat._41 -
		mat._11 * mat._32 * mat._44 -
		mat._12 * mat._34 * mat._41 -
		mat._14 * mat._31 * mat._42;

	float b33 = mat._11 * mat._22 * mat._44 +
		mat._12 * mat._24 * mat._41 +
		mat._14 * mat._21 * mat._42 -
		mat._11 * mat._24 * mat._42 -
		mat._12 * mat._21 * mat._44 -
		mat._14 * mat._22 * mat._41;

	float b34 = mat._11 * mat._24 * mat._32 +
		mat._12 * mat._21 * mat._34 +
		mat._14 * mat._22 * mat._31 -
		mat._11 * mat._22 * mat._34 -
		mat._12 * mat._24 * mat._31 -
		mat._14 * mat._21 * mat._32;

	float b41 = mat._21 * mat._33 * mat._42 +
		mat._22 * mat._31 * mat._43 +
		mat._23 * mat._32 * mat._41 -
		mat._21 * mat._32 * mat._43 -
		mat._22 * mat._33 * mat._41 -
		mat._23 * mat._31 * mat._42;

	float b42 = mat._11 * mat._32 * mat._43 +
		mat._12 * mat._33 * mat._41 +
		mat._13 * mat._31 * mat._42 -
		mat._11 * mat._33 * mat._42 -
		mat._12 * mat._31 * mat._43 -
		mat._13 * mat._32 * mat._41;

	float b43 = mat._11 * mat._23 * mat._42 +
		mat._12 * mat._21 * mat._43 +
		mat._13 * mat._22 * mat._41 -
		mat._11 * mat._22 * mat._43 -
		mat._12 * mat._23 * mat._41 -
		mat._13 * mat._21 * mat._42;

	float b44 = mat._11 * mat._22 * mat._33 +
		mat._12 * mat._23 * mat._31 +
		mat._13 * mat._21 * mat._32 -
		mat._11 * mat._23 * mat._32 -
		mat._12 * mat._21 * mat._33 -
		mat._13 * mat._22 * mat._31;
		*/
			
	float4x4 ans;
	float b11 = mat._22 * mat._33 * mat._44 + 
		mat._23 * mat._34 * mat._42 +
		mat._24 * mat._32 * mat._43 -
		mat._22 * mat._34 * mat._43 -
		mat._23 * mat._32 * mat._44 -
		mat._24 * mat._33 * mat._42;
	ans._11 = b11 /detA;
	b11 = mat._12 * mat._34 * mat._43 +
		mat._13 * mat._32 * mat._44 +
		mat._14 * mat._33 * mat._42 -
		mat._12 * mat._33 * mat._44 -
		mat._13 * mat._34 * mat._42 -
		mat._14 * mat._32 * mat._43;
	ans._12 = b11 /detA;
	b11 = mat._12 * mat._23 * mat._44 +
		mat._13 * mat._24* mat._42 +
		mat._14 * mat._22 * mat._43 -
		mat._12 * mat._24 * mat._43 - 
		mat._13 * mat._22 * mat._44 -
		mat._14 * mat._23 * mat._42;
	ans._13 = b11 /detA;
	b11 = mat._12 * mat._24 * mat._33 +
		mat._13 * mat._22 * mat._34 +
		mat._14 * mat._23 * mat._32-
		mat._12 * mat._23 * mat._34 -
		mat._13 * mat._24 * mat._32 - 
		mat._14 * mat._22 * mat._33;
	ans._14 = b11 /detA;
	b11 = mat._21 * mat._34 * mat._43 +
		mat._23 * mat._31 * mat._44 +
		mat._24 * mat._33 * mat._41 -
		mat._21 * mat._33 * mat._44 -
		mat._23 * mat._34 * mat._41 -
		mat._24 * mat._31 * mat._43;
	ans._21 = b11 /detA;
	b11 = mat._11 * mat._33 * mat._44 +
		mat._13 * mat._34 * mat._41 +
		mat._14 * mat._31 * mat._43 -
		mat._11 * mat._34 * mat._43 -
		mat._13 * mat._31 * mat._44 -
		mat._14 * mat._33 * mat._41;
	ans._22 = b11 /detA;
	b11 = mat._11 * mat._24 * mat._43 +
		mat._13 * mat._21 * mat._44 +
		mat._14 * mat._23 * mat._41 -
		mat._11 * mat._23 * mat._44 -
		mat._13 * mat._24 * mat._41 -
		mat._14 * mat._21 * mat._43;
	ans._23 = b11 /detA;
	b11 = mat._11 * mat._23 * mat._34 +
		mat._13 * mat._24 * mat._31 +
		mat._14 * mat._21 * mat._33 -
		mat._11 * mat._24 * mat._33 -
		mat._13 * mat._21 * mat._34 -
		mat._14 * mat._23 * mat._31;
	ans._24 = b11 /detA;
	b11 = mat._21 * mat._32 * mat._44 +
		mat._22 * mat._34 * mat._41 +
		mat._24 * mat._31* mat._42 -
		mat._21 * mat._34 * mat._42 -
		mat._22 * mat._31 * mat._44 -
		mat._24 * mat._32 * mat._41;
	ans._31 = b11 /detA;
	b11 = mat._11 * mat._34 * mat._42 +
		mat._12 * mat._31 * mat._44 +
		mat._14 * mat._32 * mat._41 -
		mat._11 * mat._32 * mat._44 -
		mat._12 * mat._34 * mat._41 -
		mat._14 * mat._31 * mat._42;
	ans._32 = b11 /detA;
	b11 = mat._11 * mat._22 * mat._44 +
		mat._12 * mat._24 * mat._41 +
		mat._14 * mat._21 * mat._42 -
		mat._11 * mat._24 * mat._42 -
		mat._12 * mat._21 * mat._44 -
		mat._14 * mat._22 * mat._41;
	ans._33 = b11 /detA;
	b11 = mat._11 * mat._24 * mat._32 +
		mat._12 * mat._21 * mat._34 +
		mat._14 * mat._22 * mat._31 -
		mat._11 * mat._22 * mat._34 -
		mat._12 * mat._24 * mat._31 -
		mat._14 * mat._21 * mat._32;
	ans._34 = b11 /detA;
	b11 = mat._21 * mat._33 * mat._42 +
		mat._22 * mat._31 * mat._43 +
		mat._23 * mat._32 * mat._41 -
		mat._21 * mat._32 * mat._43 -
		mat._22 * mat._33 * mat._41 -
		mat._23 * mat._31 * mat._42;
	ans._41 = b11 /detA;
	b11 = mat._11 * mat._32 * mat._43 +
		mat._12 * mat._33 * mat._41 +
		mat._13 * mat._31 * mat._42 -
		mat._11 * mat._33 * mat._42 -
		mat._12 * mat._31 * mat._43 -
		mat._13 * mat._32 * mat._41;
	ans._42 = b11 /detA;
	b11 = mat._11 * mat._23 * mat._42 +
		mat._12 * mat._21 * mat._43 +
		mat._13 * mat._22 * mat._41 -
		mat._11 * mat._22 * mat._43 -
		mat._12 * mat._23 * mat._41 -
		mat._13 * mat._21 * mat._42;
	ans._43 = b11 /detA;
	b11 = mat._11 * mat._22 * mat._33 +
		mat._12 * mat._23 * mat._31 +
		mat._13 * mat._21 * mat._32 -
		mat._11 * mat._23 * mat._32 -
		mat._12 * mat._21 * mat._33 -
		mat._13 * mat._22 * mat._31;
	ans._44 = b11 /detA;	
	
	return ans;

}










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



column_major float4x4 getMatrixFromTex(Texture2D tex, float2 texcoord, int tex_width, int tex_height) {
column_major float4x4 ans;

float xcode = 0.5f/(float)tex_width;
float ycode = 0.5f/(float)tex_height;

float4 color[16];
float value[16];
uint i = 0;
color[i] = tex.SampleLevel(decalSmp, float2(texcoord.x + xcode + xcode*2*i, texcoord.y + ycode),0);
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
*/

ans._11 = value[0];
ans._21 = value[1];
ans._31 = value[2];
ans._41 = value[3];
ans._12 = value[4];
ans._22 = value[5];
ans._32 = value[6];
ans._42 = value[7];
ans._13 = value[8];
ans._23 = value[9];
ans._33 = value[10];
ans._43 = value[11];
ans._14 = value[12];
ans._24 = value[13];
ans._34 = value[14];
ans._44 = value[15];

return ans;
}


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
ans = getMatrixFromTex(comTex, tex_coord, KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT,
 KTROBO_SHADER_VALUE_COMBINEDTEX_WIDTH_HEIGHT);
 
return MyMatrixTranspose(ans);
}





/*
column_major float4x4 getLocalMatrix(uint skeleton_index, uint bone_index) {
column_major float4x4 ans;
uint texel_per_bone = 16;
uint texel_per_skeleton = KTROBO_MESH_INSTANCED_BONE_MAX * texel_per_bone;
uint place = texel_per_skeleton * skeleton_index + texel_per_bone* bone_index;
uint x = place % KTROBO_SHADER_VALUE_LOCALTEX_WIDTH_HEIGHT;
uint y = place / KTROBO_SHADER_VALUE_LOCALTEX_WIDTH_HEIGHT;
float xoffset = 1/(float)KTROBO_SHADER_VALUE_LOCALTEX_WIDTH_HEIGHT;
float yoffset = 1/(float)KTROBO_SHADER_VALUE_LOCALTEX_WIDTH_HEIGHT;

float2 tex_coord = float2(x*xoffset, y*yoffset);
ans = getMatrixFromTex(localTex, tex_coord, KTROBO_SHADER_VALUE_LOCALTEX_WIDTH_HEIGHT,
 KTROBO_SHADER_VALUE_LOCALTEX_WIDTH_HEIGHT);

return ans;
}
*/


/*
column_major float4x4 getOffsetMatrix(uint anime_first_index, 
uint anime_last_index, float anime_first_weight, 
uint skeleton_index, uint bone_index) {

column_major float4x4 ans;

uint texel_per_bone = KTROBO_MESH_INSTANCED_ANIME_MATRIX_BASIS_NUM_MAX * 16;
uint texel_per_skeleton = KTROBO_MESH_INSTANCED_BONE_MAX * texel_per_bone;
uint place = texel_per_skeleton * skeleton_index + texel_per_bone * bone_index + anime_first_index * 16;
uint x = place % KTROBO_SHADER_VALUE_BASISTEX_WIDTH_HEIGHT;
uint y = place / KTROBO_SHADER_VALUE_BASISTEX_WIDTH_HEIGHT;
float xoffset = 1/(float)KTROBO_SHADER_VALUE_BASISTEX_WIDTH_HEIGHT;
float yoffset = 1/(float)KTROBO_SHADER_VALUE_BASISTEX_WIDTH_HEIGHT;

float2 tex_coord = float2(x*xoffset, y*yoffset);
column_major float4x4 anime_first_matrix = getMatrixFromTex(basisTex, tex_coord, KTROBO_SHADER_VALUE_BASISTEX_WIDTH_HEIGHT, KTROBO_SHADER_VALUE_BASISTEX_WIDTH_HEIGHT);
place = texel_per_skeleton * skeleton_index + texel_per_bone * bone_index + anime_last_index * 16;
x = place % KTROBO_SHADER_VALUE_BASISTEX_WIDTH_HEIGHT;
y = place / KTROBO_SHADER_VALUE_BASISTEX_WIDTH_HEIGHT;
tex_coord = float2(x*xoffset, y*yoffset);
column_major float4x4 anime_last_matrix = getMatrixFromTex(basisTex, tex_coord,  KTROBO_SHADER_VALUE_BASISTEX_WIDTH_HEIGHT, KTROBO_SHADER_VALUE_BASISTEX_WIDTH_HEIGHT);
ans = anime_first_matrix * anime_first_weight + anime_last_matrix *  (1- anime_first_weight);
// ans ‚ÌŠg‘åk¬¬•ª‚ð–³‚­‚·
//ans = getZeroKAKUDAI(ans);
return ans;
}
*/













// group ‚É‚ÍmaxƒCƒ“ƒXƒ^ƒ“ƒX‚Ì”‚ðŽw’è‚·‚é
// numthreads ‚É‚Ímaxbonenum ‚Ì”‚ðŽw’è‚·‚é
[numthreads(32,1,1)]
void CalcCS(uint gtid: SV_GroupIndex, uint3 gid: SV_GroupID )
{
//uint iid = gtid;
//uint b_index = gid.x;
uint struct_idx = gtid + gid.x * 32;


column_major float4x4 cb = getCombinedMatrix(0,struct_idx);

//getOffsetMatrix(30, 30, 1, 0, struct_idx);

//getLocalMatrix(0,struct_idx);

column_major float4x4 ans_cm = cb;

uint index = struct_idx * 16 * 4;

// ŒvŽZŒ‹‰Ê‚ðƒoƒt‚ÉŠi”[‚·‚é
/*
ansBuf.Store4(index + 0, asuint(ans_cm._11));
ansBuf.Store4(index + 0 + 4* 1, asuint(ans_cm._12));
ansBuf.Store4(index + 0+4*2, asuint(ans_cm._13));
ansBuf.Store4(index + 0+4*3, asuint(ans_cm._14));
                       
ansBuf.Store4(index + 0+4*4, asuint(ans_cm._21));
ansBuf.Store4(index + 0+4*5, asuint(ans_cm._22));
ansBuf.Store4(index + 0+4*6, asuint(ans_cm._23));
ansBuf.Store4(index + 0+4*7, asuint(ans_cm._24));
                        
ansBuf.Store4(index + 0+4*8, asuint(ans_cm._31));
ansBuf.Store4(index + 0+4*9, asuint(ans_cm._32));
ansBuf.Store4(index + 0+4*10, asuint(ans_cm._33));
ansBuf.Store4(index + 0+4*11, asuint(ans_cm._34));
              
ansBuf.Store4(index + 0+4*12, asuint(ans_cm._41));
ansBuf.Store4(index + 0+4*13, asuint(ans_cm._42));
ansBuf.Store4(index + 0+4*14, asuint(ans_cm._43));
ansBuf.Store4(index + 0+4*15, asuint(ans_cm._44));
*/


ansBuf.Store(index + 0, asuint(ans_cm._11));
ansBuf.Store(index + 0 + 4* 1, asuint(ans_cm._12));
ansBuf.Store(index + 0+4*2, asuint(ans_cm._13));
ansBuf.Store(index + 0+4*3, asuint(ans_cm._14));
                       
ansBuf.Store(index + 0+4*4, asuint(ans_cm._21));
ansBuf.Store(index + 0+4*5, asuint(ans_cm._22));
ansBuf.Store(index + 0+4*6, asuint(ans_cm._23));
ansBuf.Store(index + 0+4*7, asuint(ans_cm._24));
                        
ansBuf.Store(index + 0+4*8, asuint(ans_cm._31));
ansBuf.Store(index + 0+4*9, asuint(ans_cm._32));
ansBuf.Store(index + 0+4*10, asuint(ans_cm._33));
ansBuf.Store(index + 0+4*11, asuint(ans_cm._34));
              
ansBuf.Store(index + 0+4*12, asuint(ans_cm._41));
ansBuf.Store(index + 0+4*13, asuint(ans_cm._42));
ansBuf.Store(index + 0+4*14, asuint(ans_cm._43));
ansBuf.Store(index + 0+4*15, asuint(ans_cm._44));













}
