struct VSInput {
float3 Position : POS;
float4 Color : COLORDAYO;
};

struct GSPSInput {
float4 Position: SV_POSITION;
float2 TexCoord: TEXCOORD;
float4 Color : COLOR;
};



cbuffer c0dayo :register(b0){
column_major float4x4 world;
column_major float4x4 view;
column_major float4x4 proj;
};



GSPSInput VSFunc(VSInput input){
GSPSInput output;// = (GSPSInput)0;

float4 tempPos = float4( input.Position, 1.0f);

float4 worldPos = mul( world, tempPos );
float4 viewPos  = mul( view, worldPos );
float4 projPos  = mul( proj, viewPos );
output.Position = projPos;
//output.Position = float4(input.Position.xyz,1);
output.TexCoord = float2(0,0);// input.TexCoord.xy;
output.Color = input.Color;

return output;
}




[maxvertexcount(3)]
void GSFunc( triangle GSPSInput input[3], inout TriangleStream<GSPSInput> stream) {
stream.Append(input[2]);
stream.Append(input[1]);
stream.Append(input[0]);
stream.RestartStrip();
}



sampler texSmp : register(s0);

float4 PSFunc(GSPSInput input ) : SV_Target {
//return float4(1,1,1,1);
return input.Color;
}