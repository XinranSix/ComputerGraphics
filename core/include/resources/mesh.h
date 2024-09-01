#pragma once

#include <glm/glm.hpp>

struct Vertex {
    // position
    glm::vec3 position;
    // normal
    glm::vec3 normal;
    // texCoords
    glm::vec2 texCoords;
    // tangent
    glm::vec3 tangent;
    // bitangent
    glm::vec3 bitangent;
    // bone indexes which will influence this vertex
};

class Mesh {};
