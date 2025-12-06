#pragma once
#include "Image.hpp"
#include <vector>
#include <imgui.h>
#include <glm/glm.hpp>
#include "Triangle.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

namespace gty
{
    class RasterizationRenderer
    {
    public:
        RasterizationRenderer(uint32_t width, uint32_t height)
            : m_Width(width), m_Height(height),
              m_Image(width, height)
        {
            m_Data.resize(m_Width * m_Height * 4, 0);
            m_DepthBuffer.resize(m_Width * m_Height, std::numeric_limits<float>::infinity());
            m_ClearColor = {0, 0, 0, 255};
        }

        void Render();
        void Clear();

        void SetPixel(glm::vec2 pos, glm::vec4 color);
        void DrawTriangle(const Triangle &tri, const glm::mat4 &MVP);
        void DrawTriangle(const Triangle &tri,
                          const glm::mat4 &MVP,
                          const glm::vec3 &cameraPos,
                          const DirectionalLight &light,
                          const Material &material);
        void DrawMesh(const Mesh &mesh, const glm::mat4 &VP);
        void DrawMesh(const Mesh &mesh,
                      const glm::mat4 &VP,
                      const glm::vec3 &cameraPos,
                      const DirectionalLight &light,
                      const Material &material);

        void setClearColor(glm::vec4 color)
        {
            m_ClearColor = glm::clamp(color * 255.f, 0.f, 255.f);
        }

        ImTextureRef GetTextureRef() const
        {
            return ImTextureRef((ImTextureID)(uintptr_t)m_Image.GetColorAttachment());
        }

    private:
        uint32_t m_Width;
        uint32_t m_Height;
        std::vector<uint8_t> m_Data;
        std::vector<float> m_DepthBuffer;
        Flux::Image m_Image;

        glm::vec4 m_ClearColor;
    };
}
