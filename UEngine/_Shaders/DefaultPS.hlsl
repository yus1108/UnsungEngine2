struct PixelInput
{
    float4 coordinate : SV_POSITION; //ȭ����ǥ�� ������
};

cbuffer MaterialBuffer : register(b0)
{
    float4 color;
};

float4 main(PixelInput input) : SV_TARGET //SV_TARGET �� Ÿ���̵� ���� 
{
    return color;
}