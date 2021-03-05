struct VertexInput
{
    //�������
    //              : �ø�ƽ (���뵵)
    float4 coordinate   : POSITION0;
    float2 tex		    : TEXCOORD0;
};

struct VertexOutput
{
    float4 coordinate   : SV_POSITION; //ȭ����ǥ�� ������
    float2 tex		    : TEXCOORD0;
};

cbuffer WorldBuffer : register(b0)
{
    float4x4 worldMat;
};

cbuffer CameraBuffer : register(b1)
{
    float4x4 view;
    float4x4 projection;
};
cbuffer SpriteBuffer : register(b2)
{
    float4 UV;
};

VertexOutput main(VertexInput input)
{
    //������ȯ�� ���� ������ ��
    VertexOutput output;

    output.coordinate = mul(input.coordinate, worldMat);
    output.coordinate = mul(output.coordinate, view);
    output.coordinate = mul(output.coordinate, projection);
    output.tex.x = input.tex.x * (UV.z - UV.x) + UV.x;
    output.tex.y = input.tex.y * (UV.w - UV.y) + UV.y;

    return output;
}