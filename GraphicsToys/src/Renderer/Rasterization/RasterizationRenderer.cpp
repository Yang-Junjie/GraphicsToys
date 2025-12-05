#include "RasterizationRenderer.hpp"
#include <cmath>
#include <algorithm>
namespace gty
{
    void RasterizationRenderer::Render()
    {
        m_BufferImage.SetData(m_Data.data());
    }

    void RasterizationRenderer::SetPixel(glm::vec2 pos, glm::vec4 color)
    {
        if (pos.x < 0 || pos.x >= int(m_Width) || pos.y < 0 || pos.y >= int(m_Height))
            return;

        int flippedY = m_Height - 1 - static_cast<int>(pos.y);
        int index = (flippedY * m_Width + static_cast<int>(pos.x)) * 4;
        m_Data[index + 0] = uint8_t(glm::clamp(color.r, 0.f, 1.f) * 255.f);
        m_Data[index + 1] = uint8_t(glm::clamp(color.g, 0.f, 1.f) * 255.f);
        m_Data[index + 2] = uint8_t(glm::clamp(color.b, 0.f, 1.f) * 255.f);
        m_Data[index + 3] = uint8_t(glm::clamp(color.a, 0.f, 1.f) * 255.f);
    }

    void RasterizationRenderer::DrawLine(glm::vec2 start, glm::vec2 end, glm::vec4 color)
    {
        int y0 = static_cast<int>(start.y);
        int x0 = static_cast<int>(start.x);
        int x1 = static_cast<int>(end.x);
        int y1 = static_cast<int>(end.y);

        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);

        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;

        int err = dx - dy;

        while (true)
        {
            SetPixel({x0, y0}, color);

            if (x0 == x1 && y0 == y1)
                break;

            int e2 = 2 * err;

            if (e2 > -dy)
            {
                err -= dy;
                x0 += sx;
            }

            if (e2 < dx)
            {
                err += dx;
                y0 += sy;
            }
        }
    }
    
    void RasterizationRenderer::DrawTriangle(const gty::Triangle &tri)
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
                    glm::vec4 color =
                        tri.v0.color * bary.x +
                        tri.v1.color * bary.y +
                        tri.v2.color * bary.z;

                    SetPixel({x, y}, color);
                }
            }
        }
    }

    void RasterizationRenderer::setClearColor(glm::vec4 color)
    {
        float r = glm::clamp(color.r, 0.f, 1.f) * 255.f;
        float g = glm::clamp(color.g, 0.f, 1.f) * 255.f;
        float b = glm::clamp(color.b, 0.f, 1.f) * 255.f;
        float a = glm::clamp(color.a, 0.f, 1.f) * 255.f;

        m_ClearColor = {r, g, b, a};
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

    void RasterizationRenderer::SwapBuffers()
    {
        m_Image.CopyFrom(m_BufferImage);
    }
}