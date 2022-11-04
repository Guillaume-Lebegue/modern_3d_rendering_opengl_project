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

vec3 computeAmbientLightning(float foam)
{
    return mix(ambiant.xyz, foamAmbiant.xyz, foam);
}

vec3 computeDiffuseLightning(vec3 LightVector, vec3 normal, float foam)
{
    float lambertTerm = max(dot(normal, LightVector), 0.0f);

    return mix(diffuse.xyz, foamDiffuse.xyz, foam) * lambertTerm;
}

vec3 computeSpecularLightning(vec3 LightVector, vec3 normal)
{
//    vec3 r = reflect(LightVector, vNormal);

//    return pow(max(dot(r, eyeVector), 0.0f), specularPower) * specularColor;
    return vec3(0, 0, 0);
}

void main()
{
    vec3 LightVector = normalize(vec3(-1., 2., 6.));
    float foam = 0.5f;

    vec3 final_color = computeAmbientLightning(foam);
    final_color += computeDiffuseLightning(LightVector, vNormal, foam);
    //final_color += computeSpecularLightning(LightVector, vNormal);

    outColor = vec4(final_color, 1);
}
