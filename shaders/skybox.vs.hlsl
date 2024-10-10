#include "camera_matrix.hlsl"
#include "model_matrix.hlsl"

struct VSInput
{
    float3 Position : POSITION;
};

struct VSOutput
{
    float4 Position : SV_Position;
    float3 TexCoords : TEXCOORD;
};

VSOutput Main(VSInput input)
{
    matrix cameraMatrix = mul(CameraProjectionMatrix, CameraViewMatrix);
    VSOutput output;

    output.Position = mul(cameraMatrix, float4(input.Position, 0.0f)).xyzw;
    output.TexCoords = normalize(input.Position);

    return output;
}
