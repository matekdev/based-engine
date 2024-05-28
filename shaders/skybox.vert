#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 CameraViewMatrix;
uniform mat4 CameraProjectionMatrix;

void main() {
    vec4 pos = CameraProjectionMatrix * CameraViewMatrix * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
    TexCoords = aPos;
}
