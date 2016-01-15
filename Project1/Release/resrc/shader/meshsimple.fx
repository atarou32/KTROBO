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
};

struct GSPSInput {
float4 Position: SV_POSITION;
float2 TexCoord: TEXCOORD;
float3 Normal : NORMAL;
};



cbuffer c0 :register(c0){
float4x4 view;
float4x4 proj;
};
cbuffer c1 :register(c1){
float4x4 world;
float4x4 combined_matrixs[32];
};

cbuffer c2 :register(c0) {
float4 color;
};

GSPSInput VSFunc(VSInput input){
GSPSInput output;// = (GSPSInput)0;

float4 temppos = float4( input.Position, 1.0f);

float4 localPos =  mul(combined_matrixs[input.Index0],temppos)*input.Weight[0] + 
					  mul(combined_matrixs[input.Index1],temppos)*input.Weight[1] +
					  mul(combined_matrixs[input.Index2],temppos)*input.Weight[2] + 
					  mul(combined_matrixs[input.Index3],temppos)*(input.Weight[3]) +
                                          mul(combined_matrixs[input.Index4.x], temppos)*(input.Weightfive);

float4 worldPos = mul( world, localPos );
float4 viewPos  = mul( view, worldPos );
float4 projPos  = mul( proj, viewPos );
output.Position = projPos;
//output.Position = float4(input.Position.xyz,1);
output.TexCoord = input.TexCoord.xy;
output.Normal = input.Normal;

return output;
}

Texture2D texDiffuse : register(t0);
sampler texSmp : register(s0);

float4 PSFunc(float4 output: SV_POSITION,float3 outputc : TEXCOORD, float3 normal: NORMAL ) : SV_Target {

float4 test = texDiffuse.Sample( texSmp, float2(outputc.x , outputc.y) );
return test * color;
}