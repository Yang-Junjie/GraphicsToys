#include "RasterizationRenderer.hpp"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace gty
{

    void RasterizationRenderer::Render()
    {
        m_Image.SetData(m_Data.data());
    }

    void RasterizationRenderer::Clear()
    {
        for (size_t i = 0; i < m_Data.size(); i += 4)
        {
            m_Data[i + 0] = static_cast<uint8_t>(m_ClearColor.r);
            m_Data[i + 1] = static_cast<uint8_t>(m_ClearColor.g);
            m_Data[i + 2] = static_cast<uint8_t>(m_ClearColor.b);
            m_Data[i + 3] = static_cast<uint8_t>(m_ClearColor.a);
        }
        std::fill(m_DepthBuffer.begin(), m_DepthBuffer.end(), std::numeric_limits<float>::infinity());
    }

    void RasterizationRenderer::SetPixel(glm::vec2 pos, glm::vec4 color)
    {
        int x = static_cast<int>(pos.x);
        int y = static_cast<int>(pos.y);

        if (x < 0 || x >= int(m_Width) || y < 0 || y >= int(m_Height))
            return;

        int flippedY = m_Height - 1 - y;
        int idx = (flippedY * m_Width + x) * 4;

        m_Data[idx + 0] = uint8_t(glm::clamp(color.r, 0.f, 1.f) * 255.f);
        m_Data[idx + 1] = uint8_t(glm::clamp(color.g, 0.f, 1.f) * 255.f);
        m_Data[idx + 2] = uint8_t(glm::clamp(color.b, 0.f, 1.f) * 255.f);
        m_Data[idx + 3] = uint8_t(glm::clamp(color.a, 0.f, 1.f) * 255.f);
    }

    void RasterizationRenderer::DrawTriangle(const Triangle &tri, const glm::mat4 &MVP)
    {
        glm::vec3 p0 = tri.GetScreenPos3D(tri.v0, MVP, m_Width, m_Height);
        glm::vec3 p1 = tri.GetScreenPos3D(tri.v1, MVP, m_Width, m_Height);
        glm::vec3 p2 = tri.GetScreenPos3D(tri.v2, MVP, m_Width, m_Height);

        glm::vec2 min = glm::min(glm::min(glm::vec2(p0), glm::vec2(p1)), glm::vec2(p2));
        glm::vec2 max = glm::max(glm::max(glm::vec2(p0), glm::vec2(p1)), glm::vec2(p2));

        int x0 = std::max(0, int(std::floor(min.x)));
        int y0 = std::max(0, int(std::floor(min.y)));
        int x1 = std::min(int(m_Width - 1), int(std::ceil(max.x)));
        int y1 = std::min(int(m_Height - 1), int(std::ceil(max.y)));

        for (int y = y0; y <= y1; ++y)
        {
            for (int x = x0; x <= x1; ++x)
            {
                glm::vec2 p(x + 0.5f, y + 0.5f);

                float denom = (p1.y - p2.y) * (p0.x - p2.x) + (p2.x - p1.x) * (p0.y - p2.y);
                float u = ((p1.y - p2.y) * (p.x - p2.x) + (p2.x - p1.x) * (p.y - p2.y)) / denom;
                float v = ((p2.y - p0.y) * (p.x - p2.x) + (p0.x - p2.x) * (p.y - p2.y)) / denom;
                float w = 1.f - u - v;

                if (u >= 0 && v >= 0 && w >= 0)
                {
                    float z = u * p0.z + v * p1.z + w * p2.z;
                    int idx = y * m_Width + x;
                    if (z < m_DepthBuffer[idx])
                    {
                        m_DepthBuffer[idx] = z;
                        glm::vec4 color = tri.v0.color * u + tri.v1.color * v + tri.v2.color * w;
                        SetPixel({float(x), float(y)}, color);
                    }
                }
            }
        }
    }
    void RasterizationRenderer::DrawMesh(const Mesh &mesh, const glm::mat4 &VP)
    {
        for (const auto &tri : mesh.triangles)
        {
            glm::mat4 MVP = VP * tri.modelMatrix;
            DrawTriangle(tri, MVP);
        }
    }

}
