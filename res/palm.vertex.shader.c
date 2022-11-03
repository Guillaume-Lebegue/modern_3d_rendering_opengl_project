#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

layout(location = 0) out vec3 vNormal;

layout(std140, binding = 0) uniform uniformLayout{
    mat4 viewProjectionMatrix;
};

void main()
{
    vNormal = normal;
    gl_Position = viewProjectionMatrix * vec4(position.x, position.y, position.z, 1);
}