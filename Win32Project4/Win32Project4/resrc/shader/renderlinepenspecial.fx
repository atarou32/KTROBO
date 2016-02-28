struct VSInput {
float2 Pos : POSTEN;
float2 MyInfo : INFO1D;
uint2 MyInfo2 : INFO2D;
uint2 offset : OFFSET;
};

struct GSPSInput {
float4 Position: SV_POSITION;
float2 TexCoord: TEXCOORD;
float2 MyInfo : INFO1D;
uint2 MyInfo2 : INFO2D;
float2 Pos: POSTEN;
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



[maxvertexcount(6)]
void GSFunc( triangle GSPSInput input[3], inout TriangleStream<GSPSInput> stream) {

GSPSInput inp;
inp.MyInfo = int2(0,0);
inp.MyInfo2 = uint2(0,0);
inp.Pos = uint2(0,0);
inp.TexCoord = float2(0.0f,0.0f);
float W = screen_x;
float H = screen_y;
float PosX = zoom * (input[0].Pos.x + transx-W/2)+W/2;
float PosY = zoom * (input[0].Pos.y + transy-H/2)+H/2;
float MyX = zoom * (float)(input[0].MyInfo.x);
float MyY = zoom * (float)(input[0].MyInfo.y);
float x = -1 + 2 * (PosX)/(float)screen_x;
float y = 1 - 2 * (PosY)/(float)screen_y;
float dx = 2 * MyX / (float) screen_x;
float dy = -2 * MyY / (float) screen_y;
int nwidthidx = (input[0].MyInfo2.x >> 4);
int widthidx = input[0].MyInfo2.x - (nwidthidx << 4);
int pen_index = input[0].MyInfo2.y;
float width = zoom*pen_width[pen_index][widthidx%4][widthidx/4];
float nwidth = zoom*pen_width[pen_index][nwidthidx%4][nwidthidx/4];

if (abs(dx) < 0.000001 && abs(dy) > 0.999) {
	// êÇíº
	inp.Position = float4(x - width / (float)screen_x, y,0,1);
	stream.Append(inp);
	inp.Position = float4(x + width / (float)screen_x, y,0,1);
	stream.Append(inp);
	inp.Position = float4(x + dx - nwidth / (float)screen_x, y+dy, 0, 1);
	stream.Append(inp);

	inp.Position = float4(x +dx +  nwidth / (float)screen_x, y+dy,0,1);
	stream.Append(inp);
	inp.Position = float4(x + width / (float)screen_x, y,0,1);
	stream.Append(inp);
	inp.Position = float4(x + dx - nwidth / (float)screen_x, y+dy, 0, 1);
	stream.Append(inp);


stream.RestartStrip();/*
} else if (abs(dy) <0.00001 && abs(dx) >0.9999) {

	inp.Position = float4(x , y-width/(float)screen_y,0,1);
	stream.Append(inp);
	inp.Position = float4(x , y+width/(float)screen_y,0,1);
	stream.Append(inp);
	inp.Position = float4(x + dx, y+dy-nwidth/(float)screen_y, 0, 1);
	stream.Append(inp);

	inp.Position = float4(x +dx, y+dy+nwidth/(float)screen_x,0,1);
	stream.Append(inp);
	inp.Position = float4(x, y+width/(float)screen_x,0,1);
	stream.Append(inp);
	inp.Position = float4(x + dx, y+dy- nwidth/(float)screen_x, 0, 1);
	stream.Append(inp);


stream.RestartStrip();




*/
} else {
	// atanÇ©ÇÁthetaÇÇ‡Ç∆ÇﬂÇÈ
	float dd = sqrt(dy*dy + dx*dx);
	float atantheta = atan(abs(dy)/(float)abs(dx));
	if (dx <0 && dy > 0) {
		atantheta = 3.141592 + atantheta;
	}else if(dx < 0 && dy < 0) {
		atantheta = 3.14 + atantheta;
	}
	float ddx = sin(atantheta);
	float ddy = cos(atantheta);

	if (dx > 0 && dy >= 0) {
		//dx = dx + cos(atantheta)*1/(float)screen_x;
		//dy = dy + sin(atantheta)*1/(float)screen_y;
		ddy = ddy * -1;		
	} else if (dx >0 && dy < 0 ) {
		//dx = dx + cos(atantheta)*1/(float)screen_x;
		//dy = dy - sin(atantheta)*1/(float)screen_y;
	 } else if(dx <0 && dy >=0) {
		//dx = dx;// + cos(atantheta)*1/(float)screen_x;
		//dy = dy - sin(atantheta)*1/(float)screen_y;

		ddx = ddx * -1;
		ddy = ddy * -1;
	} else if(dx < 0 && dy <0) { 
		//dx = dx;// + cos(atantheta)*1/(float)screen_x;
		//dy = dy + sin(atantheta)*1/(float)screen_y;
		ddx = ddx * -1;
	}
	inp.Position = float4(x - ddx* width / (float)screen_x, y - ddy * width/(float)screen_y,0,1);
	stream.Append(inp);
	inp.Position = float4(x + ddx *width / (float)screen_x, y + ddy * width/(float)screen_y,0,1);
	stream.Append(inp);
	inp.Position = float4(x + dx - ddx * nwidth / (float)screen_x, y+dy -ddy * nwidth/(float)screen_y, 0, 1);
	stream.Append(inp);
	
	inp.Position = float4(x +dx +  ddx* nwidth / (float)screen_x, y+dy + ddy*nwidth/(float)screen_y,0,1);
	stream.Append(inp);
	inp.Position = float4(x + ddx* width / (float)screen_x, y+ddy*width/(float)screen_y,0,1);
	stream.Append(inp);
	inp.Position = float4(x + dx - ddx * nwidth / (float)screen_x, y+dy- ddy * nwidth/(float)screen_y, 0, 1);
	stream.Append(inp);


stream.RestartStrip();
	

}

}



sampler texSmp : register(s0);

float4 PSFunc(GSPSInput input ) : SV_Target {
return float4(0,0,0,1);
//return input.Color;
}