#version 330 core

in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube SkyBoxTexture;

void main() {
    FragColor = texture(SkyBoxTexture, TexCoords);
}
