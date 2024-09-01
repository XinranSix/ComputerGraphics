#pragma once

/* Container for bone data */

#include <list>
#include <vector>

#include <assimp/scene.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace CG {
    namespace Utils {
        inline static glm::vec3 GetGLMVec(const aiVector3D& vec) { return { vec.x, vec.y, vec.z }; }

        static glm::quat GetGLMQuat(const aiQuaternion& pOrientation) {
            return { pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z };
        }
    } // namespace Utils

    struct KeyPosition {
        glm::vec3 position;
        float timeStamp;
    };

    struct KeyRotation {
        glm::quat orientation;
        float timeStamp;
    };

    struct KeyScale {
        glm::vec3 scale;
        float timeStamp;
    };

    class Bone {
    public:
        Bone(const std::string& name, int id, const aiNodeAnim* channel)
            : name_ { name }, id_ { id }, localTransform_ { 1.0f } {
            numPositions_ = channel->mNumPositionKeys;

            for (int positionIndex = 0; positionIndex < numPositions_; ++positionIndex) {
                aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
                float timeStamp = (float)channel->mPositionKeys[positionIndex].mTime;
                KeyPosition data;
                data.position = Utils::GetGLMVec(aiPosition);
                data.timeStamp = timeStamp;
                positions_.push_back(data);
            }

            numRotations_ = channel->mNumRotationKeys;
            for (int rotationIndex = 0; rotationIndex < numRotations_; ++rotationIndex) {
                aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
                float timeStamp = (float)channel->mRotationKeys[rotationIndex].mTime;
                KeyRotation data;
                data.orientation = Utils::GetGLMQuat(aiOrientation);
                data.timeStamp = timeStamp;
                rotations_.push_back(data);
            }

            numScalings_ = channel->mNumScalingKeys;
            for (int keyIndex = 0; keyIndex < numScalings_; ++keyIndex) {
                aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
                float timeStamp = (float)channel->mScalingKeys[keyIndex].mTime;
                KeyScale data;
                data.scale = Utils::GetGLMVec(scale);
                data.timeStamp = timeStamp;
                scales_.push_back(data);
            }
        }

        void Update(float animationTime) {
            glm::mat4 translation = InterpolatePosition(animationTime);
            glm::mat4 rotation = InterpolateRotation(animationTime);
            glm::mat4 scale = InterpolateScaling(animationTime);
            localTransform_ = translation * rotation * scale;
        }

        inline glm::mat4 GetLocalTransform() const { return localTransform_; }

        inline std::string GetBoneName() const { return name_; }

        inline int GetBoneID() const { return id_; }

        int GetPositionIndex(float animationTime) const {
            for (int index = 0; index < numPositions_ - 1; ++index) {
                if (animationTime < positions_[index + 1].timeStamp) {
                    return index;
                }
            }
            assert(0);
        }

        int GetRotationIndex(float animationTime) const {
            for (int index = 0; index < numRotations_ - 1; ++index) {
                if (animationTime < rotations_[index + 1].timeStamp) {
                    return index;
                }
            }
            assert(0);
        }

        int GetScaleIndex(float animationTime) const {
            for (int index = 0; index < numScalings_ - 1; ++index) {
                if (animationTime < scales_[index + 1].timeStamp) {
                    return index;
                }
            }
            assert(0);
        }

    private:
        inline float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) const {
            float scaleFactor = 0.0f;
            float midWayLength = animationTime - lastTimeStamp;
            float framesDiff = nextTimeStamp - lastTimeStamp;
            scaleFactor = midWayLength / framesDiff;
            return scaleFactor;
        }

        glm::mat4 InterpolatePosition(float animationTime) const {
            if (1 == numPositions_) {
                return glm::translate(glm::mat4(1.0f), positions_[0].position);
            }
            int p0Index = GetPositionIndex(animationTime);
            int p1Index = p0Index + 1;
            float scaleFactor =
                GetScaleFactor(positions_[p0Index].timeStamp, positions_[p1Index].timeStamp, animationTime);
            glm::vec3 finalPosition = glm::mix(positions_[p0Index].position, positions_[p1Index].position, scaleFactor);
            return glm::translate(glm::mat4 { 1.0f }, finalPosition);
        }

        glm::mat4 InterpolateRotation(float animationTime) {
            if (1 == numRotations_) {
                auto rotation = glm::normalize(rotations_[0].orientation);
                return glm::toMat4(rotation);
            }

            int p0Index = GetRotationIndex(animationTime);
            int p1Index = p0Index + 1;
            float scaleFactor =
                GetScaleFactor(rotations_[p0Index].timeStamp, rotations_[p1Index].timeStamp, animationTime);
            glm::quat finalRotation =
                glm::slerp(rotations_[p0Index].orientation, rotations_[p1Index].orientation, scaleFactor);
            finalRotation = glm::normalize(finalRotation);
            return glm::toMat4(finalRotation);
        }

        glm::mat4 InterpolateScaling(float animationTime) {
            if (1 == numScalings_) {
                return glm::scale(glm::mat4(1.0f), scales_[0].scale);
            }

            int p0Index = GetScaleIndex(animationTime);
            int p1Index = p0Index + 1;
            float scaleFactor = GetScaleFactor(scales_[p0Index].timeStamp, scales_[p1Index].timeStamp, animationTime);
            glm::vec3 finalScale = glm::mix(scales_[p0Index].scale, scales_[p1Index].scale, scaleFactor);
            return glm::scale(glm::mat4(1.0f), finalScale);
        }

        std::vector<KeyPosition> positions_;
        std::vector<KeyRotation> rotations_;
        std::vector<KeyScale> scales_;
        int numPositions_;
        int numRotations_;
        int numScalings_;

        glm::mat4 localTransform_;
        std::string name_;
        int id_;
    };

} // namespace CG
