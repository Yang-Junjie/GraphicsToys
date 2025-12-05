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
    }

    void RasterizationRenderer::SetPixel(glm::vec2 pos, glm::vec4 color)
    {
        int x = static_cast<int>(pos.x);
        int y = static_cast<int>(pos.y);

        if (x < 0 || x >= int(m_Width) || y < 0 || y >= int(m_Height))
            return;

        // ·­×ªYÖá
        int flippedY = m_Height - 1 - y;
        int idx = (flippedY * m_Width + x) * 4;

        m_Data[idx + 0] = uint8_t(glm::clamp(color.r, 0.f, 1.f) * 255.f);
        m_Data[idx + 1] = uint8_t(glm::clamp(color.g, 0.f, 1.f) * 255.f);
        m_Data[idx + 2] = uint8_t(glm::clamp(color.b, 0.f, 1.f) * 255.f);
        m_Data[idx + 3] = uint8_t(glm::clamp(color.a, 0.f, 1.f) * 255.f);
    }

    void RasterizationRenderer::DrawTriangle(const Triangle &tri)
    {
        glm::vec2 min = tri.GetAABBMin();
        glm::vec2 max = tri.GetAABBMax();

        int x0 = std::max(0, int(std::floor(min.x)));
        int y0 = std::max(0, int(std::floor(min.y)));
        int x1 = std::min(int(m_Width - 1), int(std::ceil(max.x)));
        int y1 = std::min(int(m_Height - 1), int(std::ceil(max.y)));

        for (int y = y0; y <= y1; y++)
        {
            for (int x = x0; x <= x1; x++)
            {
                glm::vec2 p(x + 0.5f, y + 0.5f);
                glm::vec3 bary = tri.GetBarycentricCoordinates(p);

                if (bary.x >= 0 && bary.y >= 0 && bary.z >= 0)
                {
                    glm::vec4 color = tri.v0.color * bary.x + tri.v1.color * bary.y + tri.v2.color * bary.z;
                    SetPixel({float(x), float(y)}, color);
                }
            }
        }
    }
}
