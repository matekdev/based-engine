#version 330 core

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

uniform Material MaterialData;
uniform Light LightData;
uniform vec3 CameraPosition;
uniform bool HasTextures;
uniform sampler2D TextureDiffuse;

void main() {
    vec4 outputColor = vec4(1.0);
    if(HasTextures)
        outputColor = texture(TextureDiffuse, TexCoord);

    // ambient
    vec3 ambient = LightData.Ambient * MaterialData.Ambient;

    // diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(LightData.Position - FragPosition);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = LightData.Diffuse * (diff * MaterialData.Diffuse);

    // specular
    vec3 viewDirection = normalize(CameraPosition - FragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), MaterialData.Shininess);
    vec3 specular = LightData.Specular * (spec * MaterialData.Specular);

    vec3 lightResult = ambient + diffuse + specular;
    outputColor *= vec4(lightResult, 1.0);
    FragColor = outputColor;
}
