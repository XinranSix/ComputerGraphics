#include "camera.h"

#include <cmath>
#include <utility>

namespace
{
const float SPEED = 8.F;
const float SENSITIVITY = 0.1F;
} // namespace

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(getPosition(), center_, up_);
}

float Camera::getFovy() const
{
    return fovy_;
}

void Camera::updateCurrentTime(double currentTime)
{
    if (movementState_ != CameraMovement::NONE)
    {
        float offset = static_cast<float>((currentTime - currentTime_) * SPEED) * SENSITIVITY;
        if ((movementState_ & CameraMovement::FORWARD) != 0)
        {
            radius_ -= offset;
        }
        if ((movementState_ & CameraMovement::BACKWARD) != 0)
        {
            radius_ += offset;
        }
        if ((movementState_ & CameraMovement::LEFT) != 0)
        {
            yam_ += offset;
        }
        if ((movementState_ & CameraMovement::RIGHT) != 0)
        {
            yam_ -= offset;
        }
        if ((movementState_ & CameraMovement::UP) != 0)
        {
            pitch_ -= offset;
            pitch_ = std::max(pitch_, glm::radians(-89.0F));
        }
        if ((movementState_ & CameraMovement::DOWN) != 0)
        {
            pitch_ += offset;
            pitch_ = std::min(pitch_, glm::radians(89.0F));
        }
    }

    currentTime_ = currentTime;
}

void Camera::processMoveStatus(CameraMovement move, bool inProgress)
{
    if (inProgress)
    {
        movementState_ = static_cast<CameraMovement>(move | movementState_);
    }
    else
    {
        movementState_ = static_cast<CameraMovement>(movementState_ & (0xff ^ move));
    }
}

glm::vec3 Camera::getPosition() const
{
    float x = radius_ * std::cos(pitch_) * std::cos(yam_);
    float y = radius_ * std::sin(pitch_);
    float z = radius_ * std::cos(pitch_) * std::sin(yam_);

    return {x, y, z};
}
