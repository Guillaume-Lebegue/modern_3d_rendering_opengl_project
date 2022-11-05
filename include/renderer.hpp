#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Object.hpp"
#include "shader.hpp"

BEGIN_VISUALIZER_NAMESPACE

class Camera;

class Renderer
{
public:
    struct UBOData
    {
        glm::mat4 viewProjectionMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 normalMatrix;
        glm::vec4 lightDirViewSpace;
        glm::vec4 ambiant_palm;
        glm::vec4 diffuse_palm;
        glm::vec4 specular_palm;
        glm::vec4 ambiant_desert;
        glm::vec4 diffuse_desert;
        glm::vec4 specular_desert;
    };

    Renderer(uint32_t width, uint32_t height, const std::shared_ptr<Camera>& camera)
        : m_ViewportWidth(width)
        , m_ViewportHeight(height)
        , m_Camera(camera)
		, m_tree_shader("../../res/palm.vertex.shader.c", "../../res/palm.fragment.shader.c")
		, m_desert_shader("../../res/desert.vertex.shader.c", "../../res/desert.fragment.shader.c")
        , m_screen_shader("../../res/screen.vertex.shader.c", "../../res/screen.fragment.shader.c")
    {}

    Renderer() = delete;
    ~Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    bool Initialize();
    void Render(float dt);
    void Cleanup();

    void UpdateViewport(uint32_t width, uint32_t height);
    void UpdateCamera();

	void Draw(Modern3DRendering::Object& object, Shader& shader, bool backAndFront = false);

private:
	GLuint m_UBO, m_FBO, m_FBOTexture, m_quadVBO, m_quadVAO, m_RBO;

    uint32_t m_IndexCount;

    UBOData* m_UBOData;

    std::shared_ptr<Camera> m_Camera;
    uint32_t m_ViewportWidth, m_ViewportHeight;

    Modern3DRendering::Object m_tree;
    Modern3DRendering::Object m_desert;

    Shader m_tree_shader;
    Shader m_desert_shader;
    Shader m_screen_shader;
    
    std::vector<glm::vec4> m_trees_info;

    glm::vec4 m_LightDir;
};

END_VISUALIZER_NAMESPACE

#endif // !RENDERER_HPP
