cbuffer DirectionalLight : register(b2)
{
    float3 Direction;
    float3 Ambient;
    float3 Diffuse;
    float3 Specular;
};

vector CalculateDirectionalLight(float3 normal)
{
    // ambient
    float3 ambient = Ambient;

    // diffuse
    float3 lightDirection = normalize(-Direction);
    float diff = max(dot(normal, lightDirection), 0.0);
    float3 diffuse = Diffuse * diff;

    float result = ambient + diffuse;
    return result;
}
