struct VSInput {
float3 Pos : POS;
float2 TexCoord : TEXCOORD;
};

struct GSPSInput {
float4 Position: SV_POSITION;
float2 TexCoord: TEXCOORD;
};


#define KTPAINT_SCREEN_WIDTH 994
#define KTPAINT_SCREEN_HEIGHT 775
#define KTPAINT_PEN_NUM_MAX 16

cbuffer c0dayo :register(b0){
uint screen_x;
uint screen_y;
int transx;
int transy;
column_major float4x4 pen_width[KTPAINT_PEN_NUM_MAX];
float zoom;
float offset;
float offset2;
float offset3;
};

GSPSInput VSFunc(VSInput input){
GSPSInput output;// = (GSPSInput)0;
float W = screen_x;
float H = screen_y;
float PosX = zoom * (input.Pos.x + transx-W/2)+W/2;
float PosY = zoom * (input.Pos.y + transy-H/2)+H/2;

float x = -1 + 2 * (PosX)/(float)screen_x;
float y = 1 - 2 * (PosY)/(float)screen_y;
output.Position = float4(x,y,0,1);
output.TexCoord = input.TexCoord.xy;

return output;
}




[maxvertexcount(6)]
void GSFunc( triangle GSPSInput input[3], inout TriangleStream<GSPSInput> stream) {

GSPSInput inp;
stream.Append(input[0]);
stream.Append(input[1]);
stream.Append(input[2]);
stream.RestartStrip();

}



sampler texSmp : register(s0);
Texture2D texDiffuse : register(t0);

float4 PSFunc(GSPSInput input ) : SV_Target {

float4 test = texDiffuse.Sample( texSmp, float2(input.TexCoord.x , input.TexCoord.y) );
return test;
}