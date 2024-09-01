#include <assimp/Importer.hpp>
#include <assimp/material.h>

#include "animation/animation.h"

namespace CG {

    Animation::Animation(const std::string& animationPath, Mesh* model) {}

    void Animation::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src) {}
} // namespace CG
