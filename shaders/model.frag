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

struct Light {
    vec3 Position;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

uniform Light Lights[MAX_LIGHTS];
uniform int LightCount;

uniform Material MaterialData;
uniform vec3 CameraPosition;
uniform bool HasTextures;
uniform sampler2D TextureDiffuse;

vec4 CalculateLight(Light light) {
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

    vec3 result = ambient + diffuse + specular;
    return vec4(result, 1.0);
}

void main() {
    vec4 outputColor = vec4(1.0);
    if(HasTextures) {
        outputColor = texture(TextureDiffuse, TexCoord);
    }

    for(int i = 0; i < LightCount; ++i) {
        outputColor *= CalculateLight(Lights[i]);
    }

    FragColor = outputColor;
}
