#include "camera/ray.h"

Ray Ray::objectFromWorld(const glm::mat4& objec_from_world) const {
    glm::vec3 o = objec_from_world * glm::vec4 { origin, 1 };
    glm::vec3 d = objec_from_world * glm::vec4 { direction, 0 };
    return { o, d };
}
