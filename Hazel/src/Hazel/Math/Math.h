#pragma once
#include <glm/glm.hpp>

namespace Hazel::Math
{
    // 将 Transform 矩阵分解为位移、旋转、缩放
    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
}