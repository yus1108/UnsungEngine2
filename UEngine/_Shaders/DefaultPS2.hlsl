struct PixelInput
{
    float4 coordinate   : SV_POSITION; //화면좌표계 포지션
	float2 tex          : TEXCOORD0;
};

cbuffer MaterialBuffer : register(b0)
{
    float4 color;
};

Texture2D baseTexture : register(t0); // texture to draw
SamplerState filters : register(s0); // filter 0 using CLAMP, filter 1 using WRAP
float4 main(PixelInput input) : SV_TARGET //SV_TARGET 은 타겟이될 색깔 
{
    return color;
	//return color * baseTexture.Sample(filters, input.tex);
}