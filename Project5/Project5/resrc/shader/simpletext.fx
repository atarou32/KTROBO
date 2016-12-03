struct VSInput {
float2 Position : POS;
float2 TexCoord : TEX;
uint4 Color : COLOR;
};

struct GSPSInput {
float4 Position: SV_POSITION;
float2 TexCoord: TEXCOORD;
float4 Color: COLOR;
};

GSPSInput VSFunc(VSInput input){
GSPSInput output;// = (GSPSInput)0;

//return input;
output.Position = float4(input.Position.xy,0,1);
output.TexCoord = input.TexCoord;

output.Color = float4(input.Color.x / 255.0f, input.Color.y /255.0f, input.Color.z/255.0f, input.Color.w/255.0f);
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

[maxvertexcount(3)]
void GSFunc( triangle GSPSInput input[3], inout TriangleStream<GSPSInput> stream) {
GSPSInput output;
/*
for (int i=0;i<3;i++) {
output.Position = input[i].Position;
stream.Append(output);
}
*/
output = input[2];
stream.Append(output);

output = input[1];
stream.Append(output);
output = input[0];
stream.Append(output);
stream.RestartStrip();
}

float4 PSFunc(float4 output: SV_POSITION,float2 outputc : TEXCOORD, float4 color: COLOR) : SV_Target {

float4 test = texDiffuse.Sample( texSmp, float2(outputc.x , outputc.y) );
float4 test2 = texDiffuse.Sample(texSmp, float2(outputc.x-0.0001f , outputc.y-0.0001f) );

float4 test3 = texDiffuse.Sample( texSmp, float2(outputc.x+0.0001f , outputc.y-0.0001f) );
float4 test4 = texDiffuse.Sample(texSmp, float2(outputc.x-0.0001f , outputc.y+0.0001f) );


if (test.w >0.2 || test2.w > 0.2 || test3.w > 0.2 || test4.w > 0.2) {
return color;//float4(1,1,1,1);
} else {

if (test.w < 0.2) discard;
return float4(test) * color;
}
}