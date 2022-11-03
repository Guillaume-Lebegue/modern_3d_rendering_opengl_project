#version 450 core
layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 vNormal;

void main()
{
    outColor = vec4(vNormal, 1.0) * vec4(1.0, 0, 1.0, 1.0);
}
