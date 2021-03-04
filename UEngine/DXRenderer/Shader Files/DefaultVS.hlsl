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


VertexOutput main(VertexInput input)
{
    //������ȯ�� ���� ������ ��
    VertexOutput output;

    output.coordinate = input.coordinate;
    output.tex = input.tex;

    return output;
}