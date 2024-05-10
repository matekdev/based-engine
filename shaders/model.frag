#version 330 core

struct Material {
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess; // Impacts the scattering/radius of specular
};

struct Light {
    vec3 Position;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Attenuation;
};

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPosition;

out vec4 FragColor;

#define MAX_LIGHTS 5
uniform Light Lights[MAX_LIGHTS];
uniform int LightCount;

uniform vec3 CameraPosition;
uniform bool HasTexture;
uniform vec3 ModelColor;
uniform Light LightSettings;
uniform Material MaterialSettings;
uniform sampler2D texture_diffuse1;

vec3 CalculateLight(Light light, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
    // ambient light
    vec3 ambientLight = light.Ambient * MaterialSettings.Ambient;

    // diffuse light
    vec3 lightDirection = normalize(light.Position - fragPosition);
    float diffuseImpact = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseLight = light.Diffuse * (diffuseImpact * MaterialSettings.Diffuse);

    // specular light
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularStrength = pow(max(dot(viewDirection, reflectDirection), 0.0), MaterialSettings.Shininess);
    vec3 specularLight = light.Specular * (specularStrength * MaterialSettings.Specular);

    // attenuation
    float distance = length(light.Position - fragPosition);
    float attenuation = 1.0 / (distance * light.Attenuation);

    ambientLight  *= attenuation; 
    diffuseLight  *= attenuation;
    specularLight *= attenuation;   

    return (ambientLight + diffuseLight + specularLight);
}

void main()
{
    vec4 outputColor = vec4(1.0);
    if (HasTexture)
        outputColor = texture(texture_diffuse1, TexCoord);

    vec3 norm = normalize(Normal);
    vec3 viewDirection = normalize(CameraPosition - FragPosition);

    if (LightCount > 0)
    {
        vec3 lighting = vec3(0.0f);
        for (int i = 0; i < LightCount; ++i)
            lighting += CalculateLight(Lights[i], norm, FragPosition, viewDirection);

        outputColor *= vec4(lighting, 1.0);
    }
    else
    {
        outputColor *= vec4(MaterialSettings.Ambient, 1.0);
    }

    FragColor = outputColor;
}
