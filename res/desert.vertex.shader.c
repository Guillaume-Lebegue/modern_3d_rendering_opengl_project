#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

layout(location = 0) out vec3 vNormal;
layout(location = 1) out vec3 eyeVec;

layout(std140, binding = 0) uniform uniformLayout{
    mat4 viewProjectionMatrix;
    mat4 viewMatrix;
    mat4 normalMatrix;
    vec4 lightDirViewSpace;
    vec4 ambiant_palm;
    vec4 diffuse_palm;
    vec4 specular_palm;
    vec4 ambiant_desert;
    vec4 diffuse_desert;
    vec4 specular_desert;
};

void main()
{
    vNormal = normal;
    gl_Position = viewProjectionMatrix * vec4(position.x, position.y, position.z, 1);
    eyeVec = normalize(-(viewMatrix * vec4(position.x, position.y, position.z, 1)).xyz);
}