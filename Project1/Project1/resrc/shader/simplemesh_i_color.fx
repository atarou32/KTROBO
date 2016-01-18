struct VSInput {
uint4 indexs: INDEXSDAYO;
float4 colors: MCOLOR;
};

struct GSPSInput {
float4 Position : SV_POSITION;
float2 TexCoord: TEXCOORD;
float4 color : COLOR;
};


#define KTROBO_SHADER_VALUE_COLORTEX_WIDTH_HEIGHT 512
#define KTROBO_SHADER_VALUE_COLOR_NUM 12

GSPSInput VSFunc(VSInput input) {
GSPSInput output;

output.TexCoord = float2(0,1);

uint instance_id = input.indexs.x;
uint color_index = input.indexs.y;
uint vertex_index = input.indexs.z;

// ˆÊ’uŒvŽZ‚ª–Ê“|‚©‚à‚µ‚ê‚È‚¢
float offsetdayo_x[6];
float offsetdayo_y[6];
float offsetx = 2* 1/(float)KTROBO_SHADER_VALUE_COLORTEX_WIDTH_HEIGHT;
float offsety = 2* 1/(float)KTROBO_SHADER_VALUE_COLORTEX_WIDTH_HEIGHT;

float base_x = -1 + color_index * offsetx;
float base_y = 1 - instance_id * offsety;
uint indedayo = vertex_index % 6;

offsetdayo_x[0] = offsetx * 0; offsetdayo_x[1] = offsetx *0;
offsetdayo_y[0] = offsety * 0; offsetdayo_y[1] = offsety *(-1);
offsetdayo_x[2] = offsetx * 1;
offsetdayo_y[2] = offsety * 0;

offsetdayo_x[3] = offsetx * 1; offsetdayo_x[4] = offsetx *0;
offsetdayo_y[3] = offsety * 0; offsetdayo_y[4] = offsety *(-1);
offsetdayo_x[5] = offsetx * 1;
offsetdayo_y[5] = offsety * (-1);





output.Position = float4(base_x+offsetdayo_x[indedayo], base_y+offsetdayo_y[indedayo], 0.0f,1.0f);
 
output.color = input.colors;
return output;
}


[maxvertexcount(3)]
void GSFunc( triangle GSPSInput input[3], inout TriangleStream<GSPSInput> stream) {
stream.Append(input[2]);
stream.Append(input[1]);
stream.Append(input[0]);
stream.RestartStrip();
}


float4 PSFunc(GSPSInput input) : SV_Target {
return input.color;
}