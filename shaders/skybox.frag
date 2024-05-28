#version 330 core

in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube Texture10;

void main() {
    FragColor = texture(Texture10, TexCoords);
}
