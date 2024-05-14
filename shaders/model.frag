#version 330 core

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPosition;

out vec4 FragColor;

uniform vec3 CameraPosition;
uniform bool HasTextures;
uniform sampler2D TextureDiffuse;

void main() {
    vec4 outputColor = vec4(1.0);
    if(HasTextures)
        outputColor = texture(TextureDiffuse, TexCoord);

    FragColor = outputColor;
}
