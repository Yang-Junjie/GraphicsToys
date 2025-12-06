#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Triangle.hpp"
#include "Texture2D.hpp"

namespace gty
{
    struct Mesh
    {
        std::vector<Triangle> triangles;
        glm::mat4 modelMatrix = glm::mat4(1.f);

        float scale = 1.f;
        glm::vec3 rotation = {0.f, 0.f, 0.f};
        glm::vec3 translation = {0.f, 0.f, 0.f};

        Texture2D diffuseTexture;
        bool hasDiffuseTexture = false;

        void UpdateModelMatrix()
        {
            glm::mat4 scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(scale));
            glm::mat4 rotMat =
                glm::rotate(glm::mat4(1.f), rotation.x, glm::vec3(1, 0, 0)) *
                glm::rotate(glm::mat4(1.f), rotation.y, glm::vec3(0, 1, 0)) *
                glm::rotate(glm::mat4(1.f), rotation.z, glm::vec3(0, 0, 1));
            glm::mat4 transMat = glm::translate(glm::mat4(1.f), translation);

            modelMatrix = transMat * rotMat * scaleMat;

            for (auto &tri : triangles)
                tri.modelMatrix = modelMatrix;
        }
    };

}
