#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Object.hpp"
#include "shader.hpp"

BEGIN_VISUALIZER_NAMESPACE

class Camera;

class Renderer
{
public:
    Renderer(uint32_t width, uint32_t height, const std::shared_ptr<Camera>& camera)
        : m_ViewportWidth(width)
        , m_ViewportHeight(height)
        , m_Camera(camera)
		, m_tree_shader("../../res/palm.vertex.shader.c", "../../res/palm.fragment.shader.c")
		, m_desert_shader("../../res/palm.vertex.shader.c", "../../res/palm.fragment.shader.c")
    {}

    Renderer() = delete;
    ~Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    bool Initialize();
    void Render();
    void Cleanup();

    void UpdateViewport(uint32_t width, uint32_t height);
    void UpdateCamera();

	void Draw(Modern3DRendering::Object& object, Shader& shader, bool backAndFront = false);

private:
    GLuint m_UBO, m_VBO, m_IBO, m_VAO, m_ShaderProgram;

    uint32_t m_IndexCount;

    glm::mat4* m_UBOData;

    std::shared_ptr<Camera> m_Camera;
    uint32_t m_ViewportWidth, m_ViewportHeight;

    Modern3DRendering::Object m_tree;
    Modern3DRendering::Object m_desert;

    Shader m_tree_shader;
    Shader m_desert_shader;
    
    std::vector<glm::vec4> m_trees_info;
};

END_VISUALIZER_NAMESPACE

#endif // !RENDERER_HPP
