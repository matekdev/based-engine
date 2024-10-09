#include "util.hlsl"
#include "camera_matrix.hlsl"
#include "model_matrix.hlsl"

struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoords : TEXCOORD;
};

struct VSOutput
{
    float4 Position : SV_Position;
    float3 WorldPosition : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoords : TEXCOORD;
};

VSOutput Main(VSInput input)
{
    matrix viewProjectionMatrix = mul(CameraProjectionMatrix, CameraViewMatrix);
    float4 worldPosition = mul(ModelMatrix, float4(input.Position, 1.0f));

    VSOutput output;
    output.Position = mul(viewProjectionMatrix, worldPosition);
    output.WorldPosition = worldPosition.xyz;
    output.Normal = mul(transpose(inverse(ModelMatrix)), input.Normal);
    output.TexCoords = input.TexCoords;
    return output;
}
