struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoords : TEXCOORD;
};

struct VSOutput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float2 TexCoords : TEXCOORD;
};

cbuffer CameraMatrix : register(b0)
{
    matrix CameraViewMatrix;
    matrix CameraProjectionMatrix;
};

cbuffer ModelMatrix : register(b1)
{
    matrix ModelMatrix;
}

VSOutput Main(VSInput input)
{
    matrix cameraMatrix = mul(CameraProjectionMatrix, CameraViewMatrix);
    VSOutput output;

    output.Position = mul(cameraMatrix, mul(ModelMatrix, float4(input.Position, 1.0f)));
    output.Normal = input.Normal;
    output.TexCoords = input.TexCoords;

    return output;
}
