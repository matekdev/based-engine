#version 330 core

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPosition;

out vec4 FragColor;

uniform vec3 CameraPosition;
uniform sampler2D texture_diffuse1;

void main()
{
    vec4 outputColor = vec4(1.0);
    outputColor = texture(texture_diffuse1, TexCoord);

    FragColor = outputColor;
}
