#define TINYOBJLOADER_IMPLEMENTATION

#include "Object.hpp"

BEGIN_VISUALIZER_NAMESPACE

Modern3DRendering::Object::Object()
{
}

Modern3DRendering::Object::~Object()
{
}

bool Modern3DRendering::Object::Initialize(std::string path)
{
    path = "../../" + path;
    //Load object
    std::cout << "reading file " << path << std::endl;
    /*
    if (!tinyobj::LoadObj(&m_attribute, &m_shape, &m_material, &m_warn, &m_err, path.c_str(), NULL, true)) {
        if (!m_err.empty())
            std::cerr << "WARN: " << m_err << std::endl;
        return false;
    }
    if (!m_err.empty())
        std::cerr << "WARN: " << m_err << std::endl;
    */
    m_reader.ParseFromFile(path);
    if (!m_reader.Valid()) {
        std::cerr << "WARN: " << m_reader.Warning() << std::endl;
        std::cerr << "ERR: " << m_reader.Error() << std::endl;
        return false;
    }

    m_attribute = m_reader.GetAttrib();
    m_shape = m_reader.GetShapes();
	
    m_vertices = m_attribute.vertices;
    //m_indices = m_shape[0].mesh.indices;

    for (tinyobj::index_t index : m_shape[0].mesh.indices) {
        m_indices.push_back(index.vertex_index);
    }

    size_t vertexCount = m_vertices.size();
    m_indexes = m_indices.size();

    // Create the VBO, IBO and UBO and initialize them
    GL_CALL(glCreateBuffers, 1, &m_VBO);
    GL_CALL(glCreateBuffers, 1, &m_IBO);
    GL_CALL(glCreateVertexArrays, 1, &m_VAO);
    
    // Map the UBO
    GL_CALL(glNamedBufferStorage, m_VBO, sizeof(tinyobj::real_t) * vertexCount, m_vertices.data(), 0);
    GL_CALL(glNamedBufferStorage, m_IBO, sizeof(int) * m_indexes, m_indices.data(), 0);
    GL_CALL(glBindVertexArray, m_VAO);

    GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, m_VBO);
    GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    GL_CALL(glEnableVertexAttribArray, 0);
    GL_CALL(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, sizeof(tinyobj::real_t) * 3, nullptr);
    return true;
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
