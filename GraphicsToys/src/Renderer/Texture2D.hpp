#pragma once
#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

class Texture2D
{
public:
    int width = 0;
    int height = 0;
    std::vector<uint8_t> data; // RGBA

    bool IsValid() const { return width > 0 && height > 0 && !data.empty(); }

    glm::vec4 SampleNearest(const glm::vec2& uv) const
    {
        if (!IsValid()) return glm::vec4(1, 1, 1, 1);

        float u = glm::fract(uv.x);
        float v = glm::fract(uv.y);

        int x = int(u * width);
        int y = int(v * height);

        x = glm::clamp(x, 0, width - 1);
        y = glm::clamp(y, 0, height - 1);

        int idx = (y * width + x) * 4;

        return glm::vec4(
            data[idx + 0] / 255.f,
            data[idx + 1] / 255.f,
            data[idx + 2] / 255.f,
            data[idx + 3] / 255.f
        );
    }

    glm::vec4 SampleBilinear(const glm::vec2& uv) const
    {
        if (!IsValid()) return glm::vec4(1, 1, 1, 1);

        float u = glm::fract(uv.x) * width - 0.5f;
        float v = glm::fract(uv.y) * height - 0.5f;

        int x0 = floor(u);
        int y0 = floor(v);
        int x1 = x0 + 1;
        int y1 = y0 + 1;

        float tx = u - x0;
        float ty = v - y0;

        auto read = [&](int x, int y) {
            x = (x + width) % width;
            y = (y + height) % height;
            int idx = (y * width + x) * 4;
            return glm::vec4(
                data[idx] / 255.f,
                data[idx + 1] / 255.f,
                data[idx + 2] / 255.f,
                data[idx + 3] / 255.f
            );
        };

        glm::vec4 c00 = read(x0, y0);
        glm::vec4 c10 = read(x1, y0);
        glm::vec4 c01 = read(x0, y1);
        glm::vec4 c11 = read(x1, y1);

        glm::vec4 cx0 = glm::mix(c00, c10, tx);
        glm::vec4 cx1 = glm::mix(c01, c11, tx);

        return glm::mix(cx0, cx1, ty);
    }

    glm::vec4 Sample(const glm::vec2& uv, bool bilinear = false) const
    {
        return bilinear ? SampleBilinear(uv) : SampleNearest(uv);
    }
};
