#ifndef SHADER_HPP_
#define SHADER_HPP_

BEGIN_VISUALIZER_NAMESPACE

#include <GL/glew.h>

class Shader
{
public:
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* shaderLibPath = nullptr) :
		m_VertexShaderPath(vertexShaderPath),
		m_FragmentShaderPath(fragmentShaderPath),
		m_ShaderLibPath(shaderLibPath),
		m_ShaderProgram(0)
	{}
	~Shader() = default;
	Shader(const Shader&) = delete;
	Shader(Shader&&) = delete;

	Shader& operator=(const Shader&) = delete;
	Shader& operator=(Shader&&) = delete;

	bool Initialize();
	void Cleanup();

	void Use() const;

private:
	GLuint m_ShaderProgram;
	const char* m_VertexShaderPath;
	const char* m_FragmentShaderPath;
	const char* m_ShaderLibPath;
};

END_VISUALIZER_NAMESPACE

#endif // !SHADER_HPP_
