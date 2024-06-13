struct VSInput
{
    float3 position : POSITION;
};

struct VSOutput
{
    float4 position : SV_Position;
    float3 color : COLOR0;
};

cbuffer CameraMatrix : register(b0)
{
    matrix CameraMatrix;
};

cbuffer ModelMatrix : register(b1)
{
    matrix ModelMatrix;
}

VSOutput Main(VSInput input)
{
    VSOutput output;
    output.position = mul(CameraMatrix, mul(ModelMatrix, float4(input.position, 1.0f)));
    output.color = float4(0.7f, 0.5f, 1.0f, 1.0f);
    return output;
}