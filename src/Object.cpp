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
    tinyobj::LoadObj(&m_attribute, &m_shape, &m_material, &m_warn, &m_err, path.c_str(), NULL, true);

    if (!m_err.empty())
        std::cerr << "WARN: " << m_err << std::endl;
    
    std::vector<Vertice> vertices{
        Vertice(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)),
        Vertice(glm::vec3(10, 0, 0), glm::vec3(0, 0, 0)),
        Vertice(glm::vec3(0, 10, 0), glm::vec3(0, 0, 0)),
    };
    std::vector<uint16_t> indices{ 1, 2, 3 };


    // Init internals
    m_vertices = &vertices;
    m_indices = &indices;

    size_t vertexCount = m_vertices->size();
    size_t indexCount = m_indices->size();

    // Create the VBO, IBO and UBO and initialize them
    GL_CALL(glCreateBuffers, 1, &m_VBO);
    GL_CALL(glCreateBuffers, 1, &m_IBO);
    GL_CALL(glCreateBuffers, 1, &m_UBO);
    
    // Map the UBO
    GL_CALL(glNamedBufferStorage, m_VBO, sizeof(tinyobj::real_t) * vertexCount, m_vertices->data(), 0);
    GL_CALL(glNamedBufferStorage, m_IBO, sizeof(tinyobj::index_t) * indexCount, m_indices->data(), 0);
    //GL_CALL(glNamedBufferStorage, m_UBO, sizeof(glm::mat4), &viewProjectionMatrix, GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
    
    //m_UBOData = (glm::mat4*)GL_CALL(glMapNamedBufferRange, m_UBO, 0, sizeof(glm::mat4), GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
}

void Modern3DRendering::Object::Render()
{

}

END_VISUALIZER_NAMESPACE
