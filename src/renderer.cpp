#include <GL/glew.h>

#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#pragma warning(pop, 0)

#include <glutils.hpp>
#include <camera.hpp>
#include <renderer.hpp>

BEGIN_VISUALIZER_NAMESPACE

struct VertexDataPosition3fColor3f
{
    glm::vec3 position;
    glm::vec3 color;
};

void GenerateSphereMesh(std::vector<VertexDataPosition3fColor3f>& vertices, std::vector<uint16_t>& indices, uint16_t sphereStackCount, uint16_t sphereSectorCount, glm::vec3 sphereCenter, float sphereRadius)
{
    std::size_t vertexId = 0;

    for (uint16_t i = 0; i <= sphereStackCount; ++i)
    {
        const float t = static_cast<float>(i) / static_cast<float>(sphereStackCount);

        const float stackAngle = glm::pi<float>() / 2.0f - t * glm::pi<float>();

        const float nxz = glm::cos(stackAngle);
        const float ny = glm::sin(stackAngle);

        const float xz = sphereRadius * nxz;
        const float y = sphereRadius * ny;

        const float mul = stackAngle < 0.0f ? -1.0f : 1.0f;

        const float dxz = -ny * mul;

        // When stackAngle is null dxz is equal to 0 and it is impossible to compute the tangents and bitangents
        // So we use the up vector as a bitangent
        const bool useUpVector = (sphereStackCount % 2 == 0) && (i == sphereStackCount / 2);

        for (uint16_t j = 0; j <= sphereSectorCount; ++j)
        {
            const float s = static_cast<float>(j) / static_cast<float>(sphereSectorCount);

            const float sectorAngle = s * 2.0f * glm::pi<float>();

            const float csa = glm::sin(sectorAngle);
            const float ssa = glm::cos(sectorAngle);

            const float nx = nxz * csa;
            const float nz = nxz * ssa;

            const float x = xz * csa;
            const float z = xz * ssa;

            const glm::vec3 direction = glm::vec3(x, y, z);

            const glm::vec3 position = sphereCenter + direction;

            const glm::vec2 texcoords = glm::vec2(s, t);

            const glm::vec3 normal = glm::vec3(nx, ny, nz);

            const glm::vec3 color = glm::vec3(0.5f) + normal * 0.5f;

            vertices[vertexId++] = { position, color };

            if (useUpVector)
            {
                const glm::vec3 bitangent = glm::vec3(0.0f, 0.0f, 1.0f);
                const glm::vec3 tangent = glm::cross(normal, bitangent);
            }
            else
            {
                const glm::vec3 tangent = glm::normalize(glm::vec3(dxz * -glm::sin(sectorAngle), dxz * glm::cos(sectorAngle), 0.0f));
                const glm::vec3 bitangent = glm::cross(tangent, normal);
            }
        }
    }

    const uint16_t sectorCountplusOne = sphereSectorCount + 1;

    std::size_t indexID = 0;

    for (uint16_t j = 0; j < sphereSectorCount; ++j)
    {
        const uint16_t jp1 = j + 1;

        indices[indexID++] = j;
        indices[indexID++] = sectorCountplusOne + j;
        indices[indexID++] = sectorCountplusOne + jp1;
    }

    for (uint16_t i = 1; i < sphereStackCount - 1; ++i)
    {
        const uint16_t k1 = i * sectorCountplusOne;
        const uint16_t k2 = k1 + sectorCountplusOne;

        for (uint16_t j = 0; j < sphereSectorCount; ++j)
        {
            const uint16_t jp1 = j + 1;

            indices[indexID++] = k1 + j;
            indices[indexID++] = k2 + j;
            indices[indexID++] = k2 + jp1;
            
            indices[indexID++] = k1 + j;
            indices[indexID++] = k2 + jp1;
            indices[indexID++] = k1 + jp1;            
        }
    }

    const uint16_t k1 = (sphereStackCount - 1) * sectorCountplusOne;
    const uint16_t k2 = k1 + sectorCountplusOne;

    for (uint16_t j = 0; j < sphereSectorCount; ++j)
    {
        const uint16_t jp1 = j + 1;

        indices[indexID++] = k1 + j;
        indices[indexID++] = k2 + j;        
        indices[indexID++] = k1 + jp1;
    }
}

bool Renderer::Initialize()
{
    GL_CALL(glCreateBuffers, 1, &m_UBO);

    m_LightDir = glm::normalize(glm::vec4(-0.5f, -1.0f, 0.25f, 0.0f));

    UBOData uboData;

    uboData.viewProjectionMatrix = m_Camera->GetViewProjectionMatrix();
    uboData.viewMatrix = m_Camera->GetViewMatrix();
    uboData.normalMatrix = glm::transpose(glm::inverse(m_Camera->GetViewMatrix()));
    uboData.lightDirViewSpace = uboData.viewMatrix * m_LightDir;

    uboData.ambiant_palm = glm::vec4(0.2313725f, 0.0901961f, 0.0431372f, 1.0f);
    uboData.diffuse_palm = glm::vec4(0.188235f, 0.380392f, 0.690196f, 1.0f);
    uboData.specular_palm = glm::vec4(0.0313725f, 0.5411765f, 0.0313725f, 1.0f);

    uboData.ambiant_desert = glm::vec4(0.2313725f, 0.0901961f, 0.0431372f, 1.0f);
    uboData.diffuse_desert = glm::vec4(0.9607843f, 0.8156863f, 0.6627451f, 1.0f);
    uboData.specular_desert = glm::vec4(1., 1., 1., 1.0f);

    GL_CALL(glNamedBufferStorage, m_UBO, sizeof(UBOData), &uboData/*glm::value_ptr(m_Camera->GetViewProjectionMatrix())*/ , GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);

    m_UBOData = GL_CALL_REINTERPRET_CAST_RETURN_VALUE(UBOData*, glMapNamedBufferRange, m_UBO, 0, sizeof(UBOData), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);

    m_tree.Initialize("res/palm.obj");
    m_tree.InitTransfo("res/palmTransfo.txt");
    m_desert.Initialize("res/desert.obj", true);

    if (!m_tree_shader.Initialize()) return false;
	if (!m_desert_shader.Initialize()) return false;
    return true;
}

void Renderer::Render()
{
    GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GL_CALL(glBindBufferBase, GL_UNIFORM_BUFFER, 0, m_UBO);
    Draw(m_tree, m_tree_shader, true);
    Draw(m_desert, m_desert_shader);

    GL_CALL(glBindVertexArray, 0);
    GL_CALL(glBindBufferBase, GL_UNIFORM_BUFFER, 0, 0);

    GL_CALL(glUseProgram, 0);
}

void Renderer::Cleanup()
{
    m_UBOData = nullptr;

    GL_CALL(glUnmapNamedBuffer, m_UBO);

    GL_CALL(glDeleteBuffers, 1, &m_UBO);
    m_tree.Cleanup();
	m_desert.Cleanup();
    m_tree_shader.Cleanup();
	m_desert_shader.Cleanup();
}

void Renderer::UpdateViewport(uint32_t width, uint32_t height)
{
    m_ViewportWidth = width;
    m_ViewportHeight = height;

    glViewport(0, 0, m_ViewportWidth, m_ViewportHeight);
    m_Camera->ComputeProjection(m_ViewportWidth, m_ViewportHeight);

    m_UBOData->viewProjectionMatrix = m_Camera->GetViewProjectionMatrix();
    m_UBOData->viewMatrix = m_Camera->GetViewMatrix();
    m_UBOData->normalMatrix = glm::transpose(glm::inverse(m_Camera->GetViewMatrix()));
    m_UBOData->lightDirViewSpace = m_UBOData->viewMatrix * m_LightDir;

    GL_CALL(glFlushMappedNamedBufferRange, m_UBO, 0, sizeof(UBOData));
}

void Renderer::UpdateCamera()
{
    /*
    std::memcpy(m_UBOData, glm::value_ptr(m_Camera->GetViewProjectionMatrix()), sizeof(glm::mat4));

    GL_CALL(glFlushMappedNamedBufferRange, m_UBO, 0, sizeof(glm::mat4));
    */

    m_UBOData->viewProjectionMatrix = m_Camera->GetViewProjectionMatrix();
    m_UBOData->viewMatrix = m_Camera->GetViewMatrix();
    m_UBOData->normalMatrix = glm::transpose(glm::inverse(m_Camera->GetViewMatrix()));
    m_UBOData->lightDirViewSpace = m_UBOData->viewMatrix * m_LightDir;

    GL_CALL(glFlushMappedNamedBufferRange, m_UBO, 0, sizeof(UBOData));
}

void Renderer::Draw(Modern3DRendering::Object& object, Shader& shader, bool backAndFront) {
    shader.Use();
    object.Bind();
	if (backAndFront) {
		GL_CALL(glDisable, GL_CULL_FACE);
	}
    size_t nbr_objects = object.GetNbrObjects();
    if (nbr_objects == 0) {
        GL_CALL(glDrawElements, GL_TRIANGLES, static_cast<uint32_t>(object.GetIndexes()), GL_UNSIGNED_INT, nullptr);
    }
    else {
        GL_CALL(glDrawElementsInstanced, GL_TRIANGLES, static_cast<uint32_t>(object.GetIndexes()), GL_UNSIGNED_INT, nullptr, static_cast<uint32_t>(nbr_objects));
    }
	if (backAndFront) {
		GL_CALL(glDisable, GL_CULL_FACE);
	}
}

END_VISUALIZER_NAMESPACE
