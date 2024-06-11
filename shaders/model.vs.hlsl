struct VSInput
{
    float3 position : POSITION;
};

struct VSOutput
{
    float4 position : SV_Position;
    float3 color : COLOR0;
};

VSOutput Main(VSInput input)
{
    VSOutput output;
    output.position = float4(input.position, 1.0);
    output.color = float(1.0);
    return output;
}
