#define TINYOBJLOADER_IMPLEMENTATION

#include "Object.hpp"

BEGIN_VISUALIZER_NAMESPACE

Modern3DRendering::Object::Object()
    : m_TBO{0}
{
}

Modern3DRendering::Object::~Object()
{
}

bool Modern3DRendering::Object::Initialize(std::string path, bool find_normal)
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
					glm::vec3(m_attribute.normals[i * 3], m_attribute.normals[i * 3 + 1], m_attribute.normals[i * 3 + 2]),
					glm::vec3(0.0f, 0.0f, 0.0f)
				}
			);
			//std::cout << "normal.x: " << m_attribute.normals[i * 3] << std::endl;
		}
    }
    else {
        std::cout << "Default normal" << std::endl;
        for (std::vector<tinyobj::real_t>::const_iterator v = m_attribute.GetVertices().begin(); v != m_attribute.GetVertices().end(); v += 3)
        {
            m_vertices.push_back(
                Vertex{
                    glm::vec3{*v, *(v + 1), *(v + 2) },
                    glm::vec3{0, 0, 0},
                    glm::vec3{0, 0, 0},
                }
            );
        }
        if (find_normal) {
            std::cout << "Find normal" << std::endl;
            FindNormal();
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
    GL_CALL(glEnableVertexAttribArray, 3);
    GL_CALL(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, sizeof(Modern3DRendering::Vertex), nullptr);
	GL_CALL(glVertexAttribPointer, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Modern3DRendering::Vertex), (void*)offsetof(Modern3DRendering::Vertex, normal));
    GL_CALL(glVertexAttribPointer, 2, 3, GL_FLOAT, GL_FALSE, sizeof(Modern3DRendering::Vertex), (void*)offsetof(Modern3DRendering::Vertex, transform));
	
    GL_CALL(glBindVertexArray, 0);
    GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, 0);
    GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
	
    GL_CALL(glDisableVertexAttribArray, 0);
    GL_CALL(glDisableVertexAttribArray, 1);
    GL_CALL(glDisableVertexAttribArray, 2);
    return true;
}

bool Modern3DRendering::Object::InitTransfo(std::string path)
{
    path = "../../" + path;

    GL_CALL(glCreateBuffers, 1, &m_TBO);

	// set m_transforms
    std::ifstream tree_info_file;
    tree_info_file.open(path);
    int nbr_trees;
    tree_info_file >> nbr_trees;
    std::cout << nbr_trees << " elements" << std::endl;
    while (nbr_trees > 0) {
        float x, y, z, w;
        tree_info_file >> x >> y >> z >> w;

        glm::vec4 new_pos(x, y, z, w);

        m_transforms.push_back(new_pos);
        nbr_trees--;
    }

	GL_CALL(glNamedBufferStorage, m_TBO, sizeof(glm::vec4) * m_transforms.size(), m_transforms.data(), 0);
    return true;
}

void Modern3DRendering::Object::Bind()
{
    GL_CALL(glBindVertexArray, m_VAO);
	GL_CALL(glBindBufferBase, GL_SHADER_STORAGE_BUFFER, 1, m_TBO);
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

size_t Modern3DRendering::Object::GetNbrObjects()
{
    return m_transforms.size();
}

void Modern3DRendering::Object::FindNormal()
{
    for (size_t i = 0; i < m_indices.size(); i += 3)
    {
        glm::vec3 v1 = m_vertices[m_indices[i]].position;
        glm::vec3 v2 = m_vertices[m_indices[i + 1]].position;
        glm::vec3 v3 = m_vertices[m_indices[i + 2]].position;

        glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

        if (i < 20) {
            std::cout << "v1" << v1.x << " " << v1.y << " " << v1.z << std::endl;
            std::cout << "v2" << v2.x << " " << v2.y << " " << v2.z << std::endl;
            std::cout << "v3" << v3.x << " " << v3.y << " " << v3.z << std::endl;

            std::cout << "normal" << normal.x << " " << normal.y << " " << normal.z << std::endl;

            std::cout << "__ normal" << m_vertices[m_indices[i]].normal.x << " " << m_vertices[m_indices[i]].normal.y << " " << m_vertices[m_indices[i]].normal.z << std::endl;
            std::cout << "__ normal" << m_vertices[m_indices[i + 1]].normal.x << " " << m_vertices[m_indices[i + 1]].normal.y << " " << m_vertices[m_indices[i + 1]].normal.z << std::endl;
            std::cout << "__ normal" << m_vertices[m_indices[i + 2]].normal.x << " " << m_vertices[m_indices[i + 2]].normal.y << " " << m_vertices[m_indices[i + 2]].normal.z << std::endl;
        }

        m_vertices[m_indices[i]].normal += normal;
        m_vertices[m_indices[i + 1]].normal += normal;
        m_vertices[m_indices[i + 2]].normal += normal;
        if (i < 20) {
            std::cout << "end __ normal" << m_vertices[m_indices[i]].normal.x << " " << m_vertices[m_indices[i]].normal.y << " " << m_vertices[m_indices[i]].normal.z << std::endl;
            std::cout << "end __ normal" << m_vertices[m_indices[i + 1]].normal.x << " " << m_vertices[m_indices[i + 1]].normal.y << " " << m_vertices[m_indices[i + 1]].normal.z << std::endl;
            std::cout << "end __ normal" << m_vertices[m_indices[i + 2]].normal.x << " " << m_vertices[m_indices[i + 2]].normal.y << " " << m_vertices[m_indices[i + 2]].normal.z << std::endl;
        }
    }

	for (size_t i = 0; i < m_vertices.size(); i++)
	{
		m_vertices[i].normal = glm::normalize(m_vertices[i].normal);
	}
}

END_VISUALIZER_NAMESPACE
