#pragma once

#include "core/base/base.h"
#include "core/math/math.hpp"

namespace CG {
    struct TransformComponent {
        glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const CGMath::vec3& translation) : translation { translation } {}

        [[nodiscard]] CG_FORCE_INLINE glm::mat4 GetTransform() const {
            glm::mat4 rotation = glm::toMat4(glm::quat(rotation));
            return glm::translate(glm::mat4(1.0f), translation) * rotation * glm::scale(glm::mat4(1.0f), scale);
        }
    };

} // namespace CG
