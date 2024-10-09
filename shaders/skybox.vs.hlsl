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
    float3 Normal : NORMAL;
    float2 TexCoords : TEXCOORD;
};

VSOutput Main(VSInput input)
{
    matrix cameraMatrix = mul(CameraProjectionMatrix, CameraViewMatrix);
    VSOutput output;

    output.Position = mul(cameraMatrix, float4(input.Position, 0.0)).xyww;
    output.Normal = input.Normal;
    output.TexCoords = input.TexCoords;

    return output;
}