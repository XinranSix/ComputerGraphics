#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CameraMovement : unsigned char {
    NONE = 0,
    FORWARD = 1,
    BACKWARD = 1 << 1,
    LEFT = 1 << 2,
    RIGHT = 1 << 3,
    UP = 1 << 4,
    DOWN = 1 << 5
};

class Camera {
public:
    [[nodiscard]] glm::mat4 getViewMatrix() const;
    [[nodiscard]] float getFovy() const;

    void updateCurrentTime(double currentTime);
    void processMoveStatus(CameraMovement move, bool inProgress);

    [[nodiscard]] glm::vec3 getPosition() const;

private:
    glm::vec3 center_ { 0.0F, 0.0F, 0.0F };
    glm::vec3 up_ { 0.F, 1.F, 0.F };

    CameraMovement movementState_ { CameraMovement::NONE };

    double currentTime_ { 0 };

    float radius_ { 8.0F };
    float yam_ { glm::radians(90.0F) };
    float pitch_ { glm::radians(40.0F) };

    float fovy_ { 45.0F };
};
#endif
