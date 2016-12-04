struct VSInput {
float3 Position : POSITION;
float2 TexCoord : TEXCOORD;
};

struct GSPSInput {
float4 Position: SV_POSITION;
float2 TexCoord: TEXCOORD;
};

GSPSInput VSFunc(VSInput input){
GSPSInput output;// = (GSPSInput)0;

//return input;
output.Position = float4(input.Position,1);
output.TexCoord = input.TexCoord;
return output;
//return output;
}

//[maxvertexcount(3)]
//void GSFunc( triangle GSPSInput input[3], inout TriangleStream<GSPSInput> stream) {
//GSPSInput output;

//for (int i=0;i<3;i++) {
//output.Position = input[i].Position;
//stream.Append(output);
//}
//stream.RestartStrip();
//}

Texture2D texDiffuse : register(t0);
sampler texSmp : register(s0);
float4 PSFunc(float4 output: SV_POSITION,float2 outputc : TEXCOORD) : SV_Target {
uint width;
uint levels;
texDiffuse.GetDimensions(width,levels);
float3 texCoord = float3(outputc*width,0);

float4 test = texDiffuse.Sample( texSmp, outputc );
//float4 test =  texDiffuse.Load(texCoord);
if (test.w == 0.0f) {
discard;
}
return float4(test);
//return output;
//return float4(1.0f,1.0f,0.5f,1.0f);
}