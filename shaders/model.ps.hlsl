#include "camera_matrix.hlsl"
#include "directional_light.hlsl"

struct PSInput
{
    float4 Position : SV_Position;
    float3 WorldPosition : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoords : TEXCOORD;
};

struct PSOutput
{
    float4 Color : SV_Target0;
};

Texture2D Texture : register(t0);
TextureCube CubeMapTexture : register(t1);
SamplerState TextureSampler : register(s0);

float4 CalculateCubeMapReflection(PSInput input)
{
    float3 I = normalize(input.WorldPosition - CameraPosition);
    float3 R = reflect(I, normalize(input.Normal));
    return float4(CubeMapTexture.Sample(TextureSampler, R).rgb, 1.0f);
}

PSOutput Main(PSInput input)
{
    PSOutput output;

    float4 modelTexture = float4(Texture.Sample(TextureSampler, input.TexCoords).rgb, 1.0f);
    float4 cubeMapReflection = CalculateCubeMapReflection(input);
    output.Color = lerp(modelTexture, cubeMapReflection, 0.2f);

    return output;
}
