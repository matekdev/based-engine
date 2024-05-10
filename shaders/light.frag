#version 330 core

out vec4 FragColor;

struct Material {
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

uniform Material MaterialSettings;

void main()
{
    FragColor = vec4(MaterialSettings.Ambient * MaterialSettings.Diffuse, 1.0);
}
