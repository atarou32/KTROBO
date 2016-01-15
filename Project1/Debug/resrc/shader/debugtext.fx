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
float4 PSFunc(float4 output: SV_POSITION,float3 outputc : TEXCOORD) : SV_Target {

float4 test = texDiffuse.Sample( texSmp, outputc.xy );
return float4(test);
}