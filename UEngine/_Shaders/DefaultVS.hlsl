struct VertexInput
{
    //멤버변수
    //              : 시멘틱 (사용용도)
    float4 coordinate : POSITION0;
};

struct VertexOutput
{
    float4 coordinate : SV_POSITION; //화면좌표계 포지션
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
    //공간변환이 있을 예정인 곳
    VertexOutput output;

    output.coordinate = input.coordinate;
    //output.coordinate = mul(input.coordinate, worldMat);
    //output.coordinate = mul(output.coordinate, view);
    //output.coordinate = mul(output.coordinate, projection);

    return output;
}