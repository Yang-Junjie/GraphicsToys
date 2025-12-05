#pragma once
#include <glm/glm.hpp>

struct DirectionalLight
{
    glm::vec3 direction;
    glm::vec3 color;
    float intensity = 1.f;
};

struct Material
{

    glm::vec3 ambient = {0.1f, 0.1f, 0.1f};

    glm::vec3 diffuse = {1.f, 1.f, 1.f};

    glm::vec3 specular = {1.f, 1.f, 1.f};
    float shininess = 32.f;
};

inline glm::vec3 PhongLighting(const glm::vec3 &pos,
                               const glm::vec3 &normal,
                               const glm::vec3 &viewDir,
                               const DirectionalLight &light,
                               const Material &mat)
{
    glm::vec3 N = glm::normalize(normal);
    glm::vec3 L = glm::normalize(-light.direction);
    glm::vec3 V = glm::normalize(viewDir);

    glm::vec3 lightCol = light.color * light.intensity;

    glm::vec3 ambient = mat.ambient * lightCol;

    float diff = glm::max(glm::dot(N, L), 0.0f);
    glm::vec3 diffuse = diff * mat.diffuse * lightCol;

    glm::vec3 R = glm::reflect(-L, N);
    float spec = glm::pow(glm::max(glm::dot(R, V), 0.0f), mat.shininess);
    glm::vec3 specular = spec * mat.specular * lightCol;

    glm::vec3 result = ambient + diffuse + specular;

    return glm::clamp(result, glm::vec3(0.0f), glm::vec3(1.0f));
}
