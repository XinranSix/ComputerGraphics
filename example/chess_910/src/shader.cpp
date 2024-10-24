#include "shader.h"

Shader::Shader(const char *vertex, const char *fragment) : id_(glCreateProgram())
{
    GLuint shaderVertex = getShader(&vertex, GL_VERTEX_SHADER);
    GLuint shaderFragment = getShader(&fragment, GL_FRAGMENT_SHADER);
    glAttachShader(id_, shaderVertex);
    glAttachShader(id_, shaderFragment);
    glLinkProgram(id_);
    checkCompileErrors(id_);
    glDeleteShader(shaderVertex);
    glDeleteShader(shaderFragment);
}

GLuint Shader::id() const
{
    return id_;
}

void Shader::checkCompileErrors(unsigned int shader, GLenum type)
{
    char infoLog[256];
    int success = GL_FALSE;
    if (0 != type)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 256, nullptr, infoLog);
            std::cout << "Error compile type: " << type << " " << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            glGetProgramInfoLog(shader, 256, nullptr, infoLog);
            std::cout << "Error link type: : " << type << " " << infoLog << std::endl;
        }
    }
}

GLuint Shader::getShader(const char *const *source, GLenum type)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, source, nullptr);
    glCompileShader(shader);
    checkCompileErrors(shader, type);
    return shader;
}
