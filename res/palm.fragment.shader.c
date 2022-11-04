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

vec3 computeDiffuseLightning(vec3 LightVector)
{
    float lambertTerm = max(dot(vNormal, LightVector), 0.0f);

    return vec3(0.5, 0.5, 0.05) * lambertTerm;
}

vec3 computeSpecularLightning(vec3 LightVector)
{
//    vec3 r = reflect(LightVector, vNormal);

//    return pow(max(dot(r, eyeVector), 0.0f), specularPower) * specularColor;
    return vec3(0, 0, 0);
}

void main()
{
    vec3 LightVector = normalize(vec3(-1., 2., 6.));

    vec3 final_color = computeAmbientLightning();
    final_color += computeDiffuseLightning(LightVector);
    final_color += computeSpecularLightning(LightVector);

    outColor = vec4(final_color, 1);
}
