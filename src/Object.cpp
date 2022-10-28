#include "Object.hpp"

Modern3DRendering::Object::Object()
{
}

Modern3DRendering::Object::~Object()
{

}

void Modern3DRendering::Object::Initialize(std::string path)
{
    //Load object
    //tinyobj::LoadObj(&m_attribute, &m_shape, &m_material, &m_err, path.c_str(), nullptr, true);

    if (!m_err.empty())
        std::cerr << "WARN: " << m_err << std::endl;
    
    std::vector<tinyobj::real_t> *vertices = &(m_attribute.vertices);
    std::vector<tinyobj::index_t> *indices = &(m_shape[0].mesh.indices);

    uint16_t vertexCount = vertices->size();
    uint16_t indexCount = indices->size();

    // Create the VBO, IBO and UBO and initialize them
    GL_CALL(glCreateBuffers, 1, &m_VBO);
    GL_CALL(glCreateBuffers, 1, &m_IBO);
    GL_CALL(glCreateBuffers, 1, &m_UBO);
    
    // Map the UBO
    GL_CALL(glNamedBufferStorage, m_VBO, sizeof(tinyobj::real_t) * vertexCount, vertices->data(), 0);
    GL_CALL(glNamedBufferStorage, m_IBO, sizeof(tinyobj::index_t) * indexCount, indices->data(), 0);
    //GL_CALL(glNamedBufferStorage, m_UBO, sizeof(glm::mat4), &viewProjectionMatrix, GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
    
    //m_UBOData = (glm::mat4*)GL_CALL(glMapNamedBufferRange, m_UBO, 0, sizeof(glm::mat4), GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);

}

/*
void Modern3DRendering::Object::Render()
{

}
*/
