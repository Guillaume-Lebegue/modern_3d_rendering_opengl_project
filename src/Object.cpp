#define TINYOBJLOADER_IMPLEMENTATION

#include "Object.hpp"

BEGIN_VISUALIZER_NAMESPACE

Modern3DRendering::Object::Object()
{
}

Modern3DRendering::Object::~Object()
{
}

void Modern3DRendering::Object::Initialize(std::string path)
{
    path = "../../" + path;
    //Load object
    std::cout << "reading file " << path << std::endl;
    tinyobj::LoadObj(&m_attribute, &m_shape, &m_material, &m_warn, &m_err, path.c_str(), NULL, true);

    if (!m_err.empty())
        std::cerr << "WARN: " << m_err << std::endl;
    
    /*
    m_vertices = std::vector<Vertice>({
        Vertice(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)),
        Vertice(glm::vec3(10, 0, 0), glm::vec3(0, 0, 0)),
        Vertice(glm::vec3(0, 10, 0), glm::vec3(0, 0, 0))
        });
    m_indices = std::vector<uint16_t>({1, 2, 3});
    */

    m_vertices = m_attribute.vertices;
    m_indices = m_shape[0].mesh.indices;

    size_t vertexCount = m_vertices.size();
    size_t indexCount = m_indices.size();

    // Create the VBO, IBO and UBO and initialize them
    GL_CALL(glCreateBuffers, 1, &m_VBO);
    GL_CALL(glCreateBuffers, 1, &m_IBO);
    GL_CALL(glCreateVertexArrays, 1, &m_VAO);
    
    // Map the UBO
    GL_CALL(glNamedBufferStorage, m_VBO, sizeof(tinyobj::real_t) * vertexCount, m_vertices.data(), 0);
    GL_CALL(glNamedBufferStorage, m_IBO, sizeof(tinyobj::index_t) * indexCount, m_indices.data(), 0);
    GL_CALL(glBindVertexArray, m_VAO);

    GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, m_VBO);
    GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    GL_CALL(glEnableVertexAttribArray, 0);
    GL_CALL(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, sizeof(tinyobj::real_t) * 3, nullptr);
}

void Modern3DRendering::Object::Bind()
{
    GL_CALL(glBindVertexArray, m_VAO);
}

void Modern3DRendering::Object::Cleanup()
{
    GL_CALL(glDeleteBuffers, 1, &m_VBO);
    GL_CALL(glDeleteBuffers, 1, &m_IBO);

    GL_CALL(glDeleteVertexArrays, 1, &m_VAO);
}

size_t Modern3DRendering::Object::GetIndexes()
{
    return m_indexes;
}

END_VISUALIZER_NAMESPACE
