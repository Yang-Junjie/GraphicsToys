#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gty
{
    struct Vertex3
    {
        glm::vec3 pos;
        glm::vec4 color;
        Vertex3(const glm::vec3 &p = {0, 0, 0}, const glm::vec4 &c = {1, 1, 1, 1})
            : pos(p), color(c) {}
    };

    struct Triangle
    {
        Vertex3 v0, v1, v2;
        glm::mat4 modelMatrix = glm::mat4(1.f);

        float scale = 1.f;
        float rotation = 0.f;
        float offsetX = 0.f;
        float offsetY = 0.f;

        Triangle() = default;
        Triangle(const Vertex3 &a, const Vertex3 &b, const Vertex3 &c)
            : v0(a), v1(b), v2(c) {}

        void UpdateModelMatrix(uint32_t screenWidth, uint32_t screenHeight)
        {
            float halfW = screenWidth / 2.f;
            float halfH = screenHeight / 2.f;

            modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(halfW + offsetX, halfH + offsetY, 0.f)) *
                          glm::rotate(glm::mat4(1.f), rotation, glm::vec3(0, 0, 1)) *
                          glm::scale(glm::mat4(1.f), glm::vec3(scale * halfW, scale * halfH, 1.f));
        }

        glm::vec2 GetScreenPos(const Vertex3 &v) const
        {
            glm::vec4 p = modelMatrix * glm::vec4(v.pos, 1.f);
            return glm::vec2(p.x, p.y);
        }

        glm::vec2 GetAABBMin() const
        {
            glm::vec2 p0 = GetScreenPos(v0);
            glm::vec2 p1 = GetScreenPos(v1);
            glm::vec2 p2 = GetScreenPos(v2);
            return glm::min(glm::min(p0, p1), p2);
        }

        glm::vec2 GetAABBMax() const
        {
            glm::vec2 p0 = GetScreenPos(v0);
            glm::vec2 p1 = GetScreenPos(v1);
            glm::vec2 p2 = GetScreenPos(v2);
            return glm::max(glm::max(p0, p1), p2);
        }

        glm::vec3 GetBarycentricCoordinates(const glm::vec2 &p) const
        {
            glm::vec2 a = GetScreenPos(v0);
            glm::vec2 b = GetScreenPos(v1);
            glm::vec2 c = GetScreenPos(v2);

            float denom = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
            float u = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / denom;
            float v = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / denom;
            float w = 1.f - u - v;
            return glm::vec3(u, v, w);
        }
    };
}
