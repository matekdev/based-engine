#version 330 core

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPosition;

out vec4 FragColor;

uniform vec3 CameraPosition;
uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform bool HasTextures;
uniform sampler2D TextureDiffuse;

void main() {
    vec4 outputColor = vec4(1.0);
    if(HasTextures)
        outputColor = texture(TextureDiffuse, TexCoord);

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * LightColor;

    // diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(LightPosition - FragPosition);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diff * LightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDirection = normalize(CameraPosition - FragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);
    vec3 specular = specularStrength * spec * LightColor;

    vec3 lightResult = ambient + diffuse + specular;
    outputColor *= vec4(lightResult, 1.0);
    FragColor = outputColor;
}
