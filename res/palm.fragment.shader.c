#version 450 core
layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 vNormal;

layout(std140, binding = 0) uniform uniformLayout{
    mat4 viewProjectionMatrix;
    vec4 ambiant;
    vec4 foamAmbiant;
    vec4 diffuse;
    vec4 foamDiffuse;
    vec4 specular;
    vec4 foamSpecular;
};

vec3 computeAmbientLightning()
{
    return vec3(0, 0, 0);
}

vec3 computeDiffuseLightning()
{
    return vec3(0, 0, 0);
}

vec3 computeSpecularLightning()
{
    return vec3(0, 0, 0);
}

void main()
{
    vec3 final_color = vec3(0.5f, 0, 0);
    final_color += computeAmbientLightning();
    final_color += computeDiffuseLightning();
    final_color += computeSpecularLightning();

    outColor = vec4(final_color, 1);
}
