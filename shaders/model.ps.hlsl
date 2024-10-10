#include "camera_matrix.hlsl"
#include "directional_light.hlsl"

struct PSInput
{
    float4 Position : SV_Position;
    float3 FragPosition : POSITION;
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

float4 CalculateDirectionalLight(PSInput input)
{
    // ambient
    float3 ambient = DirectionalLightAmbient;

    // diffuse
    float3 normal = normalize(input.Normal);
    float3 lightDirection = normalize(-DirectionalLightDirection);
    float diff = max(dot(normal, lightDirection), 0.0f);
    float3 diffuse = DirectionalLightDiffuse * diff;

    // specular
    float3 viewDirection = normalize(CameraPosition - input.FragPosition);
    float3 halfWayDirection = normalize(lightDirection + viewDirection);
    float spec = pow(max(dot(normal, halfWayDirection), 0.0f), 32.0f); // material shininess
    float3 specular = DirectionalLightSpecular * spec;

    // TODO: calculate shadows here.

    float3 result = ambient * (diffuse + specular);
    return float4(result, 1.0f);
}

float4 CalculateCubeMapReflection(PSInput input)
{
    float3 I = normalize(input.FragPosition - CameraPosition);
    float3 R = reflect(I, normalize(input.Normal));
    return float4(CubeMapTexture.Sample(TextureSampler, R).rgb, 1.0f);
}

PSOutput Main(PSInput input)
{
    PSOutput output;

    float4 modelTexture = float4(Texture.Sample(TextureSampler, input.TexCoords).rgb, 1.0f);
    float4 cubeMapReflection = CalculateCubeMapReflection(input);
    float4 lighting = CalculateDirectionalLight(input);

    output.Color = lerp(modelTexture, cubeMapReflection, 0.2f);
    output.Color *= lighting;

    return output;
}
