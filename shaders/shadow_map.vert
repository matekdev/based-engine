#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 ModelMatrix;
uniform mat4 LightSpaceMatrix;

void main() {
    gl_Position = LightSpaceMatrix * ModelMatrix * vec4(aPos, 1.0);
}
