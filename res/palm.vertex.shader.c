#version 450 core

layout(location = 0) in vec3 position;

layout(std140, binding = 0) uniform uniformLayout{
    mat4 viewProjectionMatrix;
};

void main()
{
    gl_Position = viewProjectionMatrix * vec4(position.x, position.y, position.z, 1);
}