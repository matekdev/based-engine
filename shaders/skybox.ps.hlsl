struct PSInput
{
    float4 Position : SV_Position;
    float3 TexCoords : TEXCOORD;
};

struct PSOutput
{
    float4 Color : SV_Target0;
};

TextureCube CubeMapTexture : register(t1);
SamplerState TextureSampler : register(s0);

PSOutput Main(PSInput input)
{
    PSOutput output;
    output.Color = float4(CubeMapTexture.Sample(TextureSampler, input.TexCoords).rgb, 1.0f);
    return output;
}
