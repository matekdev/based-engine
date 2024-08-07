#include "camera_matrix.hlsl"
#include "directional_light.hlsl"

struct PSInput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float2 TexCoords : TEXCOORD;
};

struct PSOutput
{
    float4 Color : SV_Target0;
};

Texture2D AlbedoMap : register(t0);

SamplerState TextureSampler : register(s0);

PSOutput Main(PSInput input)
{
    PSOutput output;

    const float3 albedo = AlbedoMap.Sample(TextureSampler, input.TexCoords).rgb;
    output.Color = float4(albedo, 1.0f);
    output.Color *= CalculateDirectionalLight(CameraPosition, input.Position, normalize(input.Normal));

    return output;
}
