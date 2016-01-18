struct VSInput {
float3 Position : POSITION;
float Value : MVALUE;
};

struct GSPSInput {
float4 Position: SV_POSITION;
float Value : MVALUE;
};

GSPSInput VSFunc(VSInput input){
GSPSInput output;// = (GSPSInput)0;

float4 temppos = float4( input.Position, 1.0f);

output.Position = temppos;
output.Value = input.Value;

return output;
}


[maxvertexcount(3)]
void GSFunc( triangle GSPSInput In[3], inout TriangleStream<GSPSInput> gsstream )
{
    GSPSInput   Out;

    Out.Position= In[0].Position;
    Out.Value = In[0].Value;

    gsstream.Append( Out );

    Out.Position= In[1].Position;
    Out.Value = In[1].Value;

    gsstream.Append( Out );

    Out.Position= In[2].Position;
    Out.Value = In[2].Value;
 
    gsstream.Append( Out );

    gsstream.RestartStrip();
}


float4 PSFunc(GSPSInput input ) : SV_Target {
uint gu = asuint(input.Value);
uint r = (gu >> 24);
uint g = (gu >> 16) - (r << 8);
uint b = (gu >> 8) - (g << 8) - (r << 16);
uint a = gu - (b <<8) - (g << 16) - (r << 24);

float4 color = float4(r/255.0f, g/255.0f, b/255.0f , a/255.0f);
return float4(color.xyz,color.w);//float4(0,1,1,1);//color;
}