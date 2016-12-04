struct VSInput {
float3 Position : POSITION;
float3 TexCoord : TEXCOORD;
};

struct GSPSInput {
float4 Position: SV_POSITION;
float3 TexCoord: TEXCOORD;
float4 Color: COLOR;
};



cbuffer c0 {
float plustime;
float text_speed;
float offset;
float offset2;
};
cbuffer c1 {
int screen_width;
int screen_height;
int transparent_text_line_num;
int text_height;
float4 text_color;
};

GSPSInput VSFunc(VSInput input){
GSPSInput output;// = (GSPSInput)0;

//return input;




output.Position = float4(input.Position.xyz + float3(0,text_speed* plustime/ screen_height,0),1);
output.TexCoord = input.TexCoord.xyz;

float num = (abs(output.Position.y) - 0 ) * screen_height / text_height ;
if (floor(num) >= transparent_text_line_num) {
	output.Color = text_color;
	output.Color.w = (1 - num / transparent_text_line_num * 0.7);
	if (output.Color.w < 0 ) {
		output.Color.w = 0;
		output.Color.x = 0;
		output.Color.y = 0;
		output.Color.z = 0;
	}
} else {
	output.Color = text_color;
}
return output;
}

Texture2D texDiffuse : register(t0);
sampler texSmp : register(s0);

float4 PSFunc(float4 output: SV_POSITION,float3 outputc : TEXCOORD, float4 colo : COLOR) : SV_Target {

float4 test = texDiffuse.Sample( texSmp, float2(outputc.x , outputc.y) );
float4 test2 = texDiffuse.Sample(texSmp, float2(outputc.x-0.0001f , outputc.y-0.0001f) );

float4 test3 = texDiffuse.Sample( texSmp, float2(outputc.x+0.0001f , outputc.y-0.0001f) );
float4 test4 = texDiffuse.Sample(texSmp, float2(outputc.x-0.0001f , outputc.y+0.0001f) );





if (test.w >0.2 || test2.w > 0.2 || test3.w > 0.2 || test4.w > 0.2) {
return colo;
} else {

if (test.w < 0.2) discard;

return colo;//float4(test);
}
}