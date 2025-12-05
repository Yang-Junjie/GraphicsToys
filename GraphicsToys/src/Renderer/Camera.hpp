#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gty
{
    struct Camera
    {
        glm::vec3 position = {0.f, 0.f, 5.f};
        glm::vec3 target = {0.f, 0.f, 0.f};
        glm::vec3 up = {0.f, 1.f, 0.f};

        float fov = 45.f;
        float nearZ = 0.1f;
        // 远平面稍微拉远一些，避免模型被裁剪掉
        float farZ = 1000.f;

        uint32_t screenWidth = 256;
        uint32_t screenHeight = 256;

        glm::mat4 GetViewMatrix() const
        {
            return glm::lookAt(position, target, up);
        }

        glm::mat4 GetProjectionMatrix() const
        {
            float aspect = float(screenWidth) / float(screenHeight);
            return glm::perspective(glm::radians(fov), aspect, nearZ, farZ);
            //  return glm::ortho(0.f, float(screenWidth), 0.f, float(screenHeight), -10.f, 10.f);
        }

        glm::mat4 GetVPMatrix() const
        {
            return GetProjectionMatrix() * GetViewMatrix();
        }
    };
}
