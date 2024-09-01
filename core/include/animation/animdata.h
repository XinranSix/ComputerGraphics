#pragma once

#include <glm/glm.hpp>

namespace CG {
    struct BoneInfo {
        /*id is index in finalBoneMatrices*/
        int id;

        /*offset matrix transforms vertex from model space to bone space*/
        glm::mat4 offset;
    };
} // namespace CG
