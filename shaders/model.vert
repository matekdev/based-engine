#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPosition;

uniform mat4 ModelMatrix;
uniform mat4 CameraMatrix;

void main()
{
    gl_Position = CameraMatrix * ModelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    FragPosition = vec3(ModelMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(ModelMatrix))) * aNormal;;
    TexCoord = aTexCoord;
}