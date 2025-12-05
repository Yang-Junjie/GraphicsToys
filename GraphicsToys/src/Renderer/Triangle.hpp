#pragma once
#include <glm/glm.hpp>
#include <algorithm>

namespace gty
{
    struct Vertex3
    {
        glm::vec3 pos;
        glm::vec4 color;
        glm::vec2 uv;
        glm::vec3 normal;
        Vertex3() : pos(0.0f), color(1.0f), uv(0.0f), normal(0.0f, 0.0f, 1.0f)
        {
        }
        Vertex3(const glm::vec3 &p, const glm::vec4 &c = glm::vec4(1.0f), const glm::vec2 &u = glm::vec2(0.0f), const glm::vec3 &n = glm::vec3(0.0f, 0.0f, 1.0f))
            : pos(p), color(c), uv(u), normal(n) {}
    };

    struct Triangle
    {
        Vertex3 v0;
        Vertex3 v1;
        Vertex3 v2;

        Triangle(const Vertex3 &a, const Vertex3 &b, const Vertex3 &c)
            : v0(a), v1(b), v2(c) {}

        glm::vec2 GetAABBMin() const
        {
            float minX = std::min({v0.pos.x, v1.pos.x, v2.pos.x});
            float minY = std::min({v0.pos.y, v1.pos.y, v2.pos.y});
            return glm::vec2(minX, minY);
        }

        glm::vec2 GetAABBMax() const
        {
            float maxX = std::max({v0.pos.x, v1.pos.x, v2.pos.x});
            float maxY = std::max({v0.pos.y, v1.pos.y, v2.pos.y});
            return glm::vec2(maxX, maxY);
        }

        glm::vec3 GetBarycentricCoordinates(const glm::vec2 &p) const
        {
            const glm::vec2 &a = glm::vec2(v0.pos);
            const glm::vec2 &b = glm::vec2(v1.pos);
            const glm::vec2 &c = glm::vec2(v2.pos);

            float denom = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
            float u = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / denom;
            float v = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / denom;
            float w = 1.0f - u - v;
            return glm::vec3(u, v, w);
        }
    };
}
