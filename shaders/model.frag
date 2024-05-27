#version 330 core

#define MAX_LIGHTS 4

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPosition;

out vec4 FragColor;

struct Material {
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
};

struct DirectionalLight {
    vec3 Direction;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

uniform DirectionalLight DirectionalLights[MAX_LIGHTS];
uniform int DirectionalLightCount;

struct PointLight {
    vec3 Position;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;

    float Constant;
    float Linear;
    float Quadratic;
};

uniform PointLight PointLights[MAX_LIGHTS];
uniform int PointLightCount;

struct SpotLight {
    vec3 Position;
    vec3 Direction;
    float Radius;
    float OuterRadius;

    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;

    float Constant;
    float Linear;
    float Quadratic;
};

uniform SpotLight SpotLights[MAX_LIGHTS];
uniform int SpotLightCount;

uniform Material MaterialData;
uniform vec3 CameraPosition;
uniform bool HasTextures;
uniform sampler2D TextureDiffuse;

vec4 CalculateDirectionalLight(DirectionalLight light) {
    // ambient
    vec3 ambient = light.Ambient * MaterialData.Ambient;

    // diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(-light.Direction);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.Diffuse * (diff * MaterialData.Diffuse);

    // specular
    vec3 viewDirection = normalize(CameraPosition - FragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), MaterialData.Shininess);
    vec3 specular = light.Specular * (spec * MaterialData.Specular);

    vec3 result = ambient + diffuse + specular;
    return vec4(result, 1.0);
}

vec4 CalculatePointLight(PointLight light) {
    // ambient
    vec3 ambient = light.Ambient * MaterialData.Ambient;

    // diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(light.Position - FragPosition);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.Diffuse * (diff * MaterialData.Diffuse);

    // specular
    vec3 viewDirection = normalize(CameraPosition - FragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), MaterialData.Shininess);
    vec3 specular = light.Specular * (spec * MaterialData.Specular);

    // attenuation
    float distance = length(light.Position - FragPosition);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    return vec4(result, 1.0);
}

vec4 CalcualteSpotLight(SpotLight light) {
    vec3 lightDirection = normalize(light.Position - FragPosition);

    // ambient
    vec3 ambient = light.Ambient * MaterialData.Ambient;

    // diffuse
    vec3 normal = normalize(Normal);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.Diffuse * (diff * MaterialData.Diffuse);

    // specular
    vec3 viewDirection = normalize(CameraPosition - FragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), MaterialData.Shininess);
    vec3 specular = light.Specular * (spec * MaterialData.Specular);

    // soft edges
    float theta = dot(lightDirection, normalize(-light.Direction));
    float epsilon = (light.Radius - light.OuterRadius);
    float intensity = clamp((theta - light.OuterRadius) / epsilon, 0.0, 1.0);

    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    // attenuation
    float distance = length(light.Position - FragPosition);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    return vec4(result, 1.0);
}

void main() {
    vec4 outputColor = vec4(0.0);
    outputColor = HasTextures ? texture(TextureDiffuse, TexCoord) : vec4(MaterialData.Diffuse, 1.0);
    if(outputColor.a < 0.1)
        discard;

    bool hasLights = DirectionalLightCount > 0 || PointLightCount > 0 || SpotLightCount > 0;
    vec4 lighting = vec4(hasLights ? 0.0 : 1.0);
    for(int i = 0; i < DirectionalLightCount; ++i) {
        lighting += CalculateDirectionalLight(DirectionalLights[i]);
    }

    for(int i = 0; i < PointLightCount; ++i) {
        lighting += CalculatePointLight(PointLights[i]);
    }

    for(int i = 0; i < SpotLightCount; ++i) {
        lighting += CalcualteSpotLight(SpotLights[i]);
    }

    FragColor = outputColor * lighting;
}
