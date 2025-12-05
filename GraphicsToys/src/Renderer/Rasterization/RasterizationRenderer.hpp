#include "Image.hpp"
#include "../Triangle.hpp"
#include <vector>
#include <imgui.h>
#include <glm/glm.hpp>
namespace gty
{
    class RasterizationRenderer
    {
    public:
        RasterizationRenderer(uint32_t width, uint32_t height)
            : m_Width(width), m_Height(height),
              m_BufferImage(width, height),
              m_Image(width, height),
              m_ClearColor(0.0f, 0.0f, 0.0f, 1.0f)
        {
            m_Data.resize(m_Width * m_Height * 4);
            Clear();
        }

        void Render();

        void SetPixel(glm::vec2 pos, glm::vec4 color);
        void DrawLine(glm::vec2 start, glm::vec2 end, glm::vec4 color);
        void DrawTriangle(const gty::Triangle &tri);

        void setClearColor(glm::vec4 color);
        void Clear();

        void SwapBuffers();

        ImTextureRef GetTextureRef() const
        {
            return ImTextureRef((ImTextureID)(uintptr_t)m_Image.GetColorAttachment());
        }

    private:
        uint32_t m_Width;
        uint32_t m_Height;

        std::vector<uint8_t> m_Data;
        Flux::Image m_BufferImage;
        Flux::Image m_Image;

        glm::vec4 m_ClearColor;
    };
}