#include <glm/gtc/matrix_transform.hpp>

#include "graphics_api/opengl/renderer/orthographic_camera.h"

namespace CG::Utils {

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : projectionMatrix_ { glm::ortho(left, right, bottom, top, -1.0f, 1.0f) }, viewMatrix_ { 1.0f } {
        viewProjectionMatrix_ = projectionMatrix_ * viewMatrix_;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top) {
        projectionMatrix_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        viewProjectionMatrix_ = projectionMatrix_ * viewMatrix_;
    }

    void OrthographicCamera::RecalculateViewMatrix() {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position_) *
                              glm::rotate(glm::mat4(1.0f), glm::radians(rotation_), glm::vec3 { 0, 0, 1 });

        viewMatrix_ = glm::inverse(transform);
        viewProjectionMatrix_ = projectionMatrix_ * viewMatrix_;
    }

} // namespace CG::Utils
