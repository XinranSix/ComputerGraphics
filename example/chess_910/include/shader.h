#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <iostream>

class Shader
{
  public:
    Shader(const char *vertex, const char *fragment);
    [[nodiscard]] GLuint id() const;

  private:
    GLuint getShader(const char *const *source, GLenum type);
    void checkCompileErrors(unsigned int shader, GLenum type = 0);

    GLuint id_{};
};
#endif