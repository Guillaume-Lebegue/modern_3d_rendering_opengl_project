#include <GL/glew.h>

#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#pragma warning(pop, 0)

#include <glutils.hpp>
#include <shader.hpp>

BEGIN_VISUALIZER_NAMESPACE

bool Shader::Initialize()
{
	// Create the shaders
	GLuint vertexShaderID = GL_CALL(glCreateShader, GL_VERTEX_SHADER);
	GLuint fragmentShaderID = GL_CALL(glCreateShader, GL_FRAGMENT_SHADER);

	m_ShaderProgram = GL_CALL(glCreateProgram);
	GL_CALL(glAttachShader, m_ShaderProgram, vertexShaderID);
	GL_CALL(glAttachShader, m_ShaderProgram, fragmentShaderID);

	// Read the Vertex Shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(m_VertexShaderPath, std::ios::in);
	if (vertexShaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(vertexShaderStream, line))
			vertexShaderCode += "\n" + line;
		vertexShaderStream.close();
	}
	else
	{
		std::cout << "Impossible to open " << m_VertexShaderPath << "." << std::endl;
		return false;
	}

	// Read the Fragment Shader code from the file
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(m_FragmentShaderPath, std::ios::in);
	if (fragmentShaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(fragmentShaderStream, line))
			fragmentShaderCode += "\n" + line;
		fragmentShaderStream.close();
	}
	else
	{
		std::cout << "Impossible to open " << m_FragmentShaderPath << "." << std::endl;
		return false;
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	// Compile Vertex Shader
	std::cout << "Compiling shader : " << m_VertexShaderPath << std::endl;
	char const* vertexSourcePointer = vertexShaderCode.c_str();
	GL_CALL(glShaderSource, vertexShaderID, 1, &vertexSourcePointer, NULL);
	GL_CALL(glCompileShader, vertexShaderID);

	// Check Vertex Shader
	GL_CALL(glGetShaderiv, vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::string vertexShaderErrorMessage(infoLogLength + 1, '\0');
		GL_CALL(glGetShaderInfoLog, vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		std::cout << &vertexShaderErrorMessage[0] << std::endl;
	}

	// Compile Fragment Shader
	std::cout << "Compiling shader : " << m_FragmentShaderPath << std::endl;
	char const* fragmentSourcePointer = fragmentShaderCode.c_str();
	GL_CALL(glShaderSource, fragmentShaderID, 1, &fragmentSourcePointer, NULL);
	GL_CALL(glCompileShader, fragmentShaderID);

	// Check Fragment Shader
	GL_CALL(glGetShaderiv, fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::string fragmentShaderErrorMessage(infoLogLength + 1, '\0');
		GL_CALL(glGetShaderInfoLog, fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		std::cout << &fragmentShaderErrorMessage[0] << std::endl;
	}
	
	if (infoLogLength > 0)
	{
		std::cout << "Error compiling shader: " << m_VertexShaderPath << " and " << m_FragmentShaderPath << std::endl;
		return false;
	}

	// Link the program
	std::cout << "Linking program" << std::endl;
	GL_CALL(glLinkProgram, m_ShaderProgram);
	
	// Check the program
	GL_CALL(glGetProgramiv, m_ShaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::string programErrorMessage(infoLogLength + 1, '\0');
		GL_CALL(glGetProgramInfoLog, m_ShaderProgram, infoLogLength, NULL, &programErrorMessage[0]);
		std::cout << &programErrorMessage[0] << std::endl;
	}

	if (infoLogLength > 0)
	{
		std::cout << "Error linking shader: " << m_VertexShaderPath << " and " << m_FragmentShaderPath << std::endl;
		return false;
	}

	// Delete the shaders
	GL_CALL(glDetachShader, m_ShaderProgram, vertexShaderID);
	GL_CALL(glDetachShader, m_ShaderProgram, fragmentShaderID);
	GL_CALL(glDeleteShader, vertexShaderID);
	GL_CALL(glDeleteShader, fragmentShaderID);
	
	return true;
}

void Shader::Cleanup()
{
	GL_CALL(glDeleteProgram, m_ShaderProgram);
}

void Shader::Use() const
{
	GL_CALL(glUseProgram, m_ShaderProgram);
}

END_VISUALIZER_NAMESPACE