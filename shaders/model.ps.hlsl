#include "camera_matrix.hlsl"

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

PSOutput Main(PSInput input)
{
    PSOutput output;

    float3 I = normalize(input.WorldPosition - CameraPosition);
    float3 R = reflect(I, normalize(input.Normal));
    output.Color = float4(CubeMapTexture.Sample(TextureSampler, R).rgb, 1.0f);

    return output;
}
