#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/scene.h>

#include <string>
#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture
{
    unsigned int id{0};
    std::string path;
};

class Object
{
  public:
    Object(std::vector<unsigned int> indices, std::vector<Vertex> vertices, Texture texture);

    void draw();

  private:
    void setupObject();
    void fixPostion();

    std::vector<unsigned int> indices_;
    std::vector<Vertex> vertices_;
    Texture texture_;

    unsigned int VAO{};
    unsigned int VBO{};
    unsigned int EBO{};
};
#endif
