struct VertexInput
{
    //�������
    //              : �ø�ƽ (���뵵)
    float4 coordinate : POSITION0;
};

struct VertexOutput
{
    float4 coordinate : SV_POSITION; //ȭ����ǥ�� ������
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
    //output.coordinate = mul(input.coordinate, worldMat);
    //output.coordinate = mul(output.coordinate, view);
    //output.coordinate = mul(output.coordinate, projection);

    return output;
}