struct VSInput {
float3 Position : POSITION;
float3 TexCoord : TEXCOORD;
};

struct GSPSInput {
float4 Position: SV_POSITION;
float3 TexCoord: TEXCOORD;
};

GSPSInput VSFunc(VSInput input){
GSPSInput output;// = (GSPSInput)0;

//return input;
output.Position = float4(input.Position,1);
output.TexCoord = input.TexCoord.xyz;
return output;
}

Texture2D texDiffuse : register(t0);
sampler texSmp : register(s0);

sampler decalSmp {
  	Filter = MIN_MAG_MAP_POINT;//MIN_MAG_MIP_LINEAR;
  	AddressU = NONE;//WRAP;
  	AddressV = NONE;//WRAP;
  	AddressW = NONE;//WRAP;
  	ComparisonFunc = NEVER;
  	MinLOD = 0;
  	MaxLOD = FLOAT32_MAX;
};


float4 PSFunc(float4 output: SV_POSITION,float3 outputc : TEXCOORD) : SV_Target {

float4 test = texDiffuse.Sample( texSmp, float2(outputc.x , outputc.y) );
float4 test2 = texDiffuse.Sample(texSmp, float2(outputc.x-0.0001f , outputc.y-0.0001f) );

float4 test3 = texDiffuse.Sample( texSmp, float2(outputc.x+0.0001f , outputc.y-0.0001f) );
float4 test4 = texDiffuse.Sample(texSmp, float2(outputc.x-0.0001f , outputc.y+0.0001f) );





if (test.w >0.2 || test2.w > 0.2 || test3.w > 0.2 || test4.w > 0.2) {
return float4(1,1,1,1);
} else {

if (test.w < 0.2) discard;
return float4(test);
}
}