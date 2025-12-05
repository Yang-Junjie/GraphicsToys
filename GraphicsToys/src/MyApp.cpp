#include "Application.hpp"
#include "Renderer/Rasterization/RasterizationRenderer.hpp"
#include "Renderer/Triangle.hpp"
#include <imgui.h>

class MyLayer : public Flux::Layer
{
public:
    MyLayer(uint32_t width = 256, uint32_t height = 256)
        : m_Width(width), m_Height(height), m_Renderer(width, height)
    {
        m_Renderer.setClearColor({0.1f, 0.1f, 0.1f, 1.f});

        gty::Vertex3 v0({-0.5f, -0.5f, 0}, {1, 0, 0, 1});
        gty::Vertex3 v1({0.5f, -0.5f, 0}, {0, 1, 0, 1});
        gty::Vertex3 v2({0.0f, 0.5f, 0}, {0, 0, 1, 1});
        m_Triangle = gty::Triangle(v0, v1, v2);
    }

    virtual void OnUpdate(float dt) override
    {
        m_Renderer.Clear();

        m_Triangle.UpdateModelMatrix(m_Width, m_Height);

        m_Renderer.DrawTriangle(m_Triangle);
        m_Renderer.Render();
    }

    virtual void OnRenderUI() override
    {
        ImGui::Begin("Triangle");
        ImGui::Image(m_Renderer.GetTextureRef(), ImVec2(550, 550));
        ImGui::End();
        ImGui::Begin("Settings");
        ImGui::SliderFloat("Scale", &m_Triangle.scale, 0.1f, 2.f);
        ImGui::SliderAngle("Rotation", &m_Triangle.rotation);
        ImGui::SliderFloat("Offset X", &m_Triangle.offsetX, -float(m_Width) / 2.f, float(m_Width) / 2.f);
        ImGui::SliderFloat("Offset Y", &m_Triangle.offsetY, -float(m_Height) / 2.f, float(m_Height) / 2.f);
        ImGui::End();
    }

private:
    uint32_t m_Width, m_Height;
    gty::RasterizationRenderer m_Renderer;
    gty::Triangle m_Triangle;
};

class MyApp : public Flux::Application
{
public:
    MyApp()
    {
        PushLayer(std::make_unique<MyLayer>());
        SetMenubarCallback([this]()
                           {
            if(ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("Exit")) Close();
                ImGui::EndMenu();
            } });
    }
    virtual ~MyApp() override = default;
};

std::unique_ptr<Flux::Application> Flux::CreateApplication()
{
    return std::make_unique<MyApp>();
}
