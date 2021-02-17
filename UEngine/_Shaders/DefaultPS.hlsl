struct PixelInput
{
    float4 coordinate : SV_POSITION; //화면좌표계 포지션
};

cbuffer MaterialBuffer : register(b0)
{
    float4 color;
};

float4 main(PixelInput input) : SV_TARGET //SV_TARGET 은 타겟이될 색깔 
{
    return color;
}