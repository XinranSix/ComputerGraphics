#include "object.h"

#include <GL/glew.h>

Object::Object(std::vector<unsigned int> indices, std::vector<Vertex> vertices, Texture texture)
    : indices_(std::move(indices)), vertices_(std::move(vertices)), texture_(std::move(texture)) {
    fixPostion();
    setupObject();
}

void Object::fixPostion() {
    glm::vec3 min { 10000 };
    glm::vec3 max { -10000 };
    for (const auto& vec : vertices_) {
        min.x = std::min(min.x, vec.position.x);
        min.y = std::min(min.y, vec.position.y);
        min.z = std::min(min.z, vec.position.z);
        max.x = std::max(max.x, vec.position.x);
        max.y = std::max(max.y, vec.position.y);
        max.z = std::max(max.z, vec.position.z);
    }
    glm::vec3 tmp = (min + max) / 2.0F;
    tmp.y = min.y;

    for (auto& vec : vertices_) {
        vec.position -= tmp;
    }
}

void Object::draw() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_.id);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Object::setupObject() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices_.size() * sizeof(Vertex)), vertices_.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices_.size() * sizeof(unsigned int)),
                 indices_.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}
