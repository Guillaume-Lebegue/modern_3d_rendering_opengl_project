#ifndef OBJECT_HPP
#define OBJECT_HPP

#pragma warning(push, 0)
#include "tiny_obj_loader.hpp"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glutils.hpp>
#pragma warning(pop, 0)

BEGIN_VISUALIZER_NAMESPACE

namespace Modern3DRendering {
    class Object
    {
    public:
        Object();
        ~Object();
        
        void Initialize(std::string path);

        void Render();

        struct Vertice
        {
            glm::vec3 position;
            glm::vec3 color;
        };
    private:
        tinyobj::attrib_t m_attribute;
        std::vector<tinyobj::shape_t> m_shape;
        std::vector<tinyobj::material_t> m_material;

        std::string m_err;

        std::vector<Vertice> *m_vertices;
        std::vector<uint16_t> *m_indices;

        GLuint m_VBO, m_IBO, m_UBO;

        glm::mat4* m_UBOData = nullptr;
    };
}

END_VISUALIZER_NAMESPACE

#endif // !OBJECT_HPP
