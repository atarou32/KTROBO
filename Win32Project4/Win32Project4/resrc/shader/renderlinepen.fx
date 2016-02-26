struct VSInput {
uint2 Pos : POSTEN;
int2 MyInfo : INFO1D;
uint2 MyInfo2 : INFO2D;
uint2 offset : OFFSET;
};

struct GSPSInput {
float4 Position: SV_POSITION;
float2 TexCoord: TEXCOORD;
int2 MyInfo : INFO1D;
uint2 MyInfo2 : INFO2D;
uint2 Pos: POSTEN;
};


GSPSInput VSFunc(VSInput input){
GSPSInput output;// = (GSPSInput)0;

output.Position = float4(0,0,0,1);
output.TexCoord = float2(0,0);// input.TexCoord.xy;
output.MyInfo = input.MyInfo;
output.Pos = input.Pos;
output.MyInfo2 = input.MyInfo2;
return output;
}



#define KTPAINT_SCREEN_WIDTH 994
#define KTPAINT_SCREEN_HEIGHT 775

[maxvertexcount(6)]
void GSFunc( triangle GSPSInput input[3], inout TriangleStream<GSPSInput> stream) {

GSPSInput inp;
inp.MyInfo = int2(0,0);
inp.MyInfo2 = uint2(0,0);
inp.Pos = uint2(0,0);
inp.TexCoord = float2(0.0f,0.0f);

float x = -1 + 2 * (input[0].Pos.x)/(float)KTPAINT_SCREEN_WIDTH;
float y = 1 - 2 * (input[0].Pos.y)/(float)KTPAINT_SCREEN_HEIGHT;
float dx = 2 * input[0].MyInfo.x / (float) KTPAINT_SCREEN_WIDTH;
float dy = -2 * input[0].MyInfo.y / (float) KTPAINT_SCREEN_HEIGHT;
float width = input[0].MyInfo2.x;
float nwidth = input[0].MyInfo2.y;

inp.Position = float4(x - width / (float)KTPAINT_SCREEN_WIDTH, y,0,1);
	stream.Append(inp);
	inp.Position = float4(x + width / (float)KTPAINT_SCREEN_WIDTH, y,0,1);
	stream.Append(inp);
	inp.Position = float4(x + dx - nwidth / (float)KTPAINT_SCREEN_WIDTH, y+dy, 0, 1);
	stream.Append(inp);

	inp.Position = float4(x +dx +  nwidth / (float)KTPAINT_SCREEN_WIDTH, y+dy,0,1);
	stream.Append(inp);
	inp.Position = float4(x + width / (float)KTPAINT_SCREEN_WIDTH, y,0,1);
	stream.Append(inp);
	inp.Position = float4(x + dx - nwidth / (float)KTPAINT_SCREEN_WIDTH, y+dy, 0, 1);
	stream.Append(inp);

stream.RestartStrip();
return;
if (abs(dx) < 0.000001) {
	// ‚’¼
	inp.Position = float4(x - width / (float)KTPAINT_SCREEN_WIDTH, y,0,1);
	stream.Append(inp);
	inp.Position = float4(x + width / (float)KTPAINT_SCREEN_WIDTH, y,0,1);
	stream.Append(inp);
	inp.Position = float4(x + dx - nwidth / (float)KTPAINT_SCREEN_WIDTH, y+dy, 0, 1);
	stream.Append(inp);

	inp.Position = float4(x +dx +  nwidth / (float)KTPAINT_SCREEN_WIDTH, y+dy,0,1);
	stream.Append(inp);
	inp.Position = float4(x + width / (float)KTPAINT_SCREEN_WIDTH, y,0,1);
	stream.Append(inp);
	inp.Position = float4(x + dx - nwidth / (float)KTPAINT_SCREEN_WIDTH, y+dy, 0, 1);
	stream.Append(inp);
stream.RestartStrip();
} else {
	// atan‚©‚çtheta‚ð‚à‚Æ‚ß‚é
	float atantheta = atan(abs(dy)/abs(dx));
	float ddx = sin(atantheta);
	float ddy = cos(atantheta);
	if (dx > 0 && dy >= 0) {
		ddy = ddy * -1;		
	} else if (dx >0 && dy < 0 ) {

	 } else if(dx <0 && dy >=0) {
		ddx = ddx * -1;
		ddy = ddy * -1;
	} else if(dx < 0 && dy <0) { 
		ddx = ddx * -1;
	}
	inp.Position = float4(x - ddx* width / (float)KTPAINT_SCREEN_WIDTH, y - ddy * width/(float)KTPAINT_SCREEN_HEIGHT,0,1);
	stream.Append(inp);
	inp.Position = float4(x + ddx *width / (float)KTPAINT_SCREEN_WIDTH, y + ddy * width/(float)KTPAINT_SCREEN_HEIGHT,0,1);
	stream.Append(inp);
	inp.Position = float4(x + dx - ddx * nwidth / (float)KTPAINT_SCREEN_WIDTH, y+dy -ddy * nwidth/(float)KTPAINT_SCREEN_HEIGHT, 0, 1);
	stream.Append(inp);

	inp.Position = float4(x +dx +  ddx* nwidth / (float)KTPAINT_SCREEN_WIDTH, y+dy + ddy*nwidth/(float)KTPAINT_SCREEN_HEIGHT,0,1);
	stream.Append(inp);
	inp.Position = float4(x + ddx* width / (float)KTPAINT_SCREEN_WIDTH, y+ddy*width/(float)KTPAINT_SCREEN_HEIGHT,0,1);
	stream.Append(inp);
	inp.Position = float4(x + dx - ddx * nwidth / (float)KTPAINT_SCREEN_WIDTH, y+dy- ddy * nwidth/(float)KTPAINT_SCREEN_HEIGHT, 0, 1);
	stream.Append(inp);
stream.RestartStrip();
	

}
}



sampler texSmp : register(s0);

float4 PSFunc(GSPSInput input ) : SV_Target {
return float4(1,1,1,1);
//return input.Color;
}