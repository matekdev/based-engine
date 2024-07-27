#include "camera_matrix.hlsl"

cbuffer DirectionalLight : register(b2)
{
    float3 Direction;
    float3 Ambient;
    float3 Diffuse;
    float3 Specular;
};

vector CalculateDirectionalLight(float4 position, float3 normal)
{
    // ambient
    float3 ambient = Ambient;

    // diffuse
    float3 lightDirection = normalize(-Direction);
    float diff = max(dot(normal, lightDirection), 0.0);
    float3 diffuse = Diffuse * diff;

    // ambient
    float3 viewDirection = normalize(CameraPosition - position);
    float3 halfWayDirection = normalize(lightDirection + viewDirection);
    float spec = pow(max(dot(normal, halfWayDirection), 0.0), 64);
    float3 specular = specular * spec;

    float result = ambient + diffuse + spec;
    return result;
}
