#include "util.hlsl"
#include "camera_matrix.hlsl"
#include "model_matrix.hlsl"
#include "shadow_map.hlsl"

struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoords : TEXCOORD;
};

struct VSOutput
{
    float4 Position : SV_Position;
};

VSOutput Main(VSInput input)
{
    float4 worldPosition = mul(ModelMatrix, float4(input.Position, 1.0f));

    VSOutput output;
    output.Position = mul(LightSpaceMatrix, worldPosition);
    return output;
}
