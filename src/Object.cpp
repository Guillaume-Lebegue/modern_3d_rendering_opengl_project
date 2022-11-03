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
	m_reader.ParseFromFile(path);
    if (!m_reader.Valid()) {
        std::cerr << "WARN: " << m_reader.Warning() << std::endl;
        std::cerr << "ERR: " << m_reader.Error() << std::endl;
        return false;
    }

    m_attribute = m_reader.GetAttrib();
    m_shape = m_reader.GetShapes()[0];

    for (std::vector<tinyobj::index_t>::const_iterator i = m_shape.mesh.indices.begin(); i != m_shape.mesh.indices.end(); ++i)
        m_indices.push_back(i->vertex_index);

	std::cout << "normal size: " << m_attribute.normals.size() << std::endl;
    if (m_attribute.normals.size() > 0) {
        std::cout << "Get normal from file" << std::endl;
        for (int i = 0; i < m_attribute.vertices.size() / 3; i++) {
			m_vertices.push_back(
				Vertex{
					glm::vec3(m_attribute.vertices[i * 3], m_attribute.vertices[i * 3 + 1], m_attribute.vertices[i * 3 + 2]),
					glm::vec3(m_attribute.normals[i * 3], m_attribute.normals[i * 3 + 1], m_attribute.normals[i * 3 + 2])
				}
			);
			//std::cout << "normal.x: " << m_attribute.normals[i * 3] << std::endl;
		}
    }
    else {
        std::cout << "Default normal" << std::endl;
        for (std::vector<tinyobj::real_t>::const_iterator v = m_attribute.GetVertices().begin(); v != m_attribute.GetVertices().end(); v += 3)
        {
            m_vertices.push_back({
                glm::vec3{
                    *v,
                    *(v + 1),
                    *(v + 2)
                },
				glm::vec3{0, 0, 0}
                });
        }
    }

	m_indexes = m_indices.size();
	std::cout << "nb vertex: " << m_indexes << std::endl;

    // Create the VBO, IBO and UBO and initialize them
    GL_CALL(glCreateBuffers, 1, &m_VBO);
    GL_CALL(glCreateBuffers, 1, &m_IBO);
    GL_CALL(glCreateVertexArrays, 1, &m_VAO);

    GL_CALL(glNamedBufferStorage, m_VBO, m_vertices.size() * sizeof(Modern3DRendering::Vertex), m_vertices.data(), 0);
    GL_CALL(glNamedBufferStorage, m_IBO, sizeof(int) * m_indexes, m_indices.data(), 0);
	
    GL_CALL(glBindVertexArray, m_VAO);
    GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, m_VBO);
    GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    GL_CALL(glEnableVertexAttribArray, 0);
    GL_CALL(glEnableVertexAttribArray, 1);
    GL_CALL(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, sizeof(Modern3DRendering::Vertex), nullptr);
	GL_CALL(glVertexAttribPointer, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Modern3DRendering::Vertex), (void*)offsetof(Modern3DRendering::Vertex, normal));
	
    GL_CALL(glBindVertexArray, 0);
    GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, 0);
    GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
	
    GL_CALL(glDisableVertexAttribArray, 0);
    GL_CALL(glDisableVertexAttribArray, 1);
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
