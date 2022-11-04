#version 450 core
layout(location = 0) out vec4 outColor;

layout(location = 1) in vec3 eyeVec;
layout(location = 0) in vec3 vNormal;

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

vec3 computeAmbientLightning()
{
    return ambiant_desert.xyz;
}

vec3 computeDiffuseLightning(vec3 lightDir, vec3 normal)
{
    float lambertTern = max(dot(normal, -lightDir), 0.0f);

    return diffuse_desert.xyz * lambertTern * 0.5f;
}

vec3 computeSpecularLightning(vec3 lightDir, vec3 normal, vec3 eyeVector)
{
    vec4 mixedSpecular = specular_desert;

    vec3 r = reflect(lightDir, normal);

    float directional = pow(max(dot(r, eyeVector), 0.0f), 2);

    return mixedSpecular.rgb * directional / 50;
}

void main()
{
    vec3 newNormal = mat3(normalMatrix) * vNormal;
    vec3 lightDir = normalize(lightDirViewSpace.xyz);

    vec3 final_color = computeAmbientLightning();
    final_color += computeDiffuseLightning(lightDir, newNormal);
    final_color += computeSpecularLightning(lightDir, newNormal, normalize(eyeVec));

    outColor = vec4(final_color, 1);
}
