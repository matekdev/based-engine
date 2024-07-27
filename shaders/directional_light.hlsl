cbuffer DirectionalLight : register(b2)
{
    float3 Direction;
    float3 Ambient;
    float3 Diffuse;
    float3 Specular;
};

float4 CalculateDirectionalLight(float3 cameraPosition, float4 fragPosition, float3 normal)
{
    // ambient
    float3 ambient = Ambient;

    // diffuse
    float3 lightDirection = normalize(-Direction);
    float diff = max(dot(normal, lightDirection), 0.0);
    float3 diffuse = Diffuse * diff;

    // ambient
    float3 viewDirection = normalize(fragPosition - cameraPosition);
    float3 halfWayDirection = normalize(lightDirection + viewDirection);
    float spec = pow(max(dot(normal, halfWayDirection), 0.0), 64);
    float3 specular = Specular * spec;

    float3 result = ambient + diffuse + specular;
    return float4(result, 1.0);
}
