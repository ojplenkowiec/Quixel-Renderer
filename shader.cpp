#include "shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // Error handling for GLSL code
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram()); // Creates a program
    GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader)); // Creates a vertex shader
    GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader)); // Creates a fragment shader

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));

    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    GLCall(glUseProgram(m_RendererID));
    GLCall(glUniform4f(GetUniformLocation(name), f0, f1, f2, f3));
}

void Shader::SetUniform4f(const std::string& name, glm::vec4 vector)
{
    GLCall(glUseProgram(m_RendererID));
    glUniform4fv(GetUniformLocation(name), 1, &vector[0]);
}

void Shader::SetUniform3f(const std::string& name, float f0, float f1, float f2)
{
    GLCall(glUseProgram(m_RendererID));
    GLCall(glUniform3f(GetUniformLocation(name), f0, f1, f2));
}

void Shader::SetUniform3f(const std::string& name, glm::vec3 vector)
{
    GLCall(glUseProgram(m_RendererID));
    glUniform3fv(GetUniformLocation(name), 1, &vector[0]);
}

void Shader::SetUniform1f(const std::string& name, float f0)
{
    GLCall(glUseProgram(m_RendererID));
    GLCall(glUniform1f(GetUniformLocation(name), f0));
}

void Shader::SetUniform1i(const std::string& name, int i0)
{
    GLCall(glUseProgram(m_RendererID));
    GLCall(glUniform1i(GetUniformLocation(name), i0));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 matrix)
{
    GLCall(glUseProgram(m_RendererID));
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) { // Checks hashmap to see if 'name' has already been declared, if so, returns the integer ID
        return m_UniformLocationCache[name];
    }
    else {
        GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
        if (location == -1)
        {
            std::cout << "Warning : Uniform '" << name << "' doesn't exist!" << std::endl;
        }
        m_UniformLocationCache[name] = location; // Otherwise the hashmap is appended with the ID at 'name'
        return location;
    }

}
