#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPosition;
out vec4 FragmentPositionLightSpace;

uniform mat4 ModelMatrix;
uniform mat4 CameraMatrix;
uniform mat4 LightSpaceMatrix;

void main() {
    Normal = mat3(transpose(inverse(ModelMatrix))) * aNormal;
    TexCoord = aTexCoord;
    FragPosition = vec3(ModelMatrix * vec4(aPos, 1.0));
    FragmentPositionLightSpace = LightSpaceMatrix * vec4(FragPosition, 1.0);

    gl_Position = CameraMatrix * ModelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
