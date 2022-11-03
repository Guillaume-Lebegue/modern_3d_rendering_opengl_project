#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

layout(location = 0) out vec3 vNormal;

layout(std140, binding = 0) uniform uniformLayout{
    mat4 viewProjectionMatrix;
    vec4 ambiant;
    vec4 foamAmbiant;
    vec4 diffuse;
    vec4 foamDiffuse;
    vec4 specular;
    vec4 foamSpecular;
};

layout(std430, binding = 1) buffer transformLayout{
    vec4 transforms[];
};

void main()
{
    vNormal = normal;
    gl_Position = viewProjectionMatrix * vec4(position.x + transforms[gl_InstanceID].x, position.y + transforms[gl_InstanceID].y, position.z + transforms[gl_InstanceID].z, 1);
}
