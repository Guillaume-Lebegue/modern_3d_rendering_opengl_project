#ifndef OBJECT_HPP
#define OBJECT_HPP

#pragma warning(push, 0)
#include "tiny_obj_loader.hpp"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glutils.hpp>
#include <filesystem>
#pragma warning(pop, 0)

BEGIN_VISUALIZER_NAMESPACE

namespace Modern3DRendering {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
        glm::vec3 transform;
	};
	
    class Object
    {
    public:
        Object();
        ~Object();
        
        bool Initialize(std::string path);

        void Bind();
        void Cleanup();
        size_t GetIndexes();

    private:
        tinyobj::ObjReader m_reader;

        tinyobj::attrib_t m_attribute;
        tinyobj::shape_t m_shape;

        std::string m_err;
        std::string m_warn;

        std::vector<Vertex> m_vertices;
        std::vector<int> m_indices;

        GLuint m_VBO, m_IBO, m_VAO;
        size_t m_indexes;

        glm::mat4* m_UBOData = nullptr;
    };
}

END_VISUALIZER_NAMESPACE

#endif // !OBJECT_HPP
