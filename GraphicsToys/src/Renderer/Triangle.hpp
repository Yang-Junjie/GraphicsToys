#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gty
{
    struct Vertex3
    {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec4 color;

        Vertex3(const glm::vec3 &p = {0, 0, 0},
                const glm::vec3 &n = {0, 0, 1},
                const glm::vec4 &c = {1, 1, 1, 1})
            : pos(p), normal(n), color(c) {}
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

        void UpdateModelMatrix()
        {
            modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(offsetX, offsetY, 0.f)) *
                          glm::rotate(glm::mat4(1.f), rotation, glm::vec3(0, 0, 1)) *
                          glm::scale(glm::mat4(1.f), glm::vec3(scale, scale, 1.f));
        }

        glm::vec2 GetScreenPos(const Vertex3 &v, const glm::mat4 &MVP, uint32_t screenWidth, uint32_t screenHeight) const
        {
            glm::vec4 clipPos = MVP * glm::vec4(v.pos, 1.f);
            glm::vec3 ndc = glm::vec3(clipPos) / clipPos.w;
            float x = (ndc.x * 0.5f + 0.5f) * screenWidth;
            float y = (ndc.y * 0.5f + 0.5f) * screenHeight;
            return glm::vec2(x, y);
        }

        glm::vec3 GetScreenPos3D(const Vertex3 &v, const glm::mat4 &MVP, int screenWidth, int screenHeight) const
        {
            glm::vec4 p = MVP * glm::vec4(v.pos, 1.f);
            p /= p.w;
            float x = (p.x * 0.5f + 0.5f) * screenWidth;
            float y = (p.y * 0.5f + 0.5f) * screenHeight;
            float z = p.z * 0.5f + 0.5f;
            return glm::vec3(x, y, z);
        }

        glm::vec2 GetAABBMin(const glm::mat4 &MVP, uint32_t screenWidth, uint32_t screenHeight) const
        {
            glm::vec2 p0 = GetScreenPos(v0, MVP, screenWidth, screenHeight);
            glm::vec2 p1 = GetScreenPos(v1, MVP, screenWidth, screenHeight);
            glm::vec2 p2 = GetScreenPos(v2, MVP, screenWidth, screenHeight);
            return glm::min(glm::min(p0, p1), p2);
        }

        glm::vec2 GetAABBMax(const glm::mat4 &MVP, uint32_t screenWidth, uint32_t screenHeight) const
        {
            glm::vec2 p0 = GetScreenPos(v0, MVP, screenWidth, screenHeight);
            glm::vec2 p1 = GetScreenPos(v1, MVP, screenWidth, screenHeight);
            glm::vec2 p2 = GetScreenPos(v2, MVP, screenWidth, screenHeight);
            return glm::max(glm::max(p0, p1), p2);
        }

        glm::vec3 InterpolateNormal(const glm::vec3 &bary) const
        {
            return glm::normalize(v0.normal * bary.x + v1.normal * bary.y + v2.normal * bary.z);
        }

        glm::vec4 InterpolateColor(const glm::vec3 &bary) const
        {
            return v0.color * bary.x + v1.color * bary.y + v2.color * bary.z;
        }

        glm::vec3 GetBarycentricCoordinates(const glm::vec2 &p, const glm::mat4 &MVP, uint32_t screenWidth, uint32_t screenHeight) const
        {
            glm::vec2 a = GetScreenPos(v0, MVP, screenWidth, screenHeight);
            glm::vec2 b = GetScreenPos(v1, MVP, screenWidth, screenHeight);
            glm::vec2 c = GetScreenPos(v2, MVP, screenWidth, screenHeight);

            float denom = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
            float u = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / denom;
            float v = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / denom;
            float w = 1.f - u - v;
            return glm::vec3(u, v, w);
        }
    };
}
