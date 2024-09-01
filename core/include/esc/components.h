#pragma once

#include <core/math/math.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

struct TransformComponent {
    glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& translation) : translation { translation } {}

    [[nodiscard]] inline glm::mat4 GetTransform() const {
        glm::mat4 rotation = glm::toMat4(glm::quat(rotation));
        return glm::translate(glm::mat4(1.0f), translation) * rotation * glm::scale(glm::mat4(1.0f), scale);
    }
};
