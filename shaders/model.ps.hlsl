struct PSInput
{
    float4 position : SV_Position;
    float3 color : COLOR0;
};

struct PSOutput
{
    float4 color : SV_Target0;
};

Texture2D AlbedoMap : register(t0);

SamplerState TextureSampler : register(s0);

PSOutput Main(PSInput input)
{
    PSOutput output;
    output.color = float4(input.color, 1.0);
    return output;
}
