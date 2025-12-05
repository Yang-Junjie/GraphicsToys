#include "Application.hpp"
#include "Renderer/Rasterization/RasterizationRenderer.hpp"
#include "Renderer/Triangle.hpp"
#include "Renderer/Camera.hpp"
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

        gty::Vertex3 v3({-0.3f, -0.3f, 0.5f}, {1, 1, 0, 1});
        gty::Vertex3 v4({0.7f, -0.3f, 0.5f}, {0, 1, 1, 1});
        gty::Vertex3 v5({0.2f, 0.7f, 0.5f}, {1, 0, 1, 1});
        m_Triangle2 = gty::Triangle(v3, v4, v5);
    }

    virtual void OnUpdate(float dt) override
    {
        m_Renderer.Clear();
        m_Triangle.UpdateModelMatrix();
        glm::mat4 MVP1 = m_Camera.GetVPMatrix() * m_Triangle.modelMatrix;
        glm::mat4 MVP2 = m_Camera.GetVPMatrix() * m_Triangle2.modelMatrix;
        m_Renderer.DrawTriangle(m_Triangle, MVP1);
        m_Renderer.DrawTriangle(m_Triangle2, MVP2);

        m_Renderer.Render();
    }

    virtual void OnRenderUI() override
    {
        ImGui::Begin("Triangle");
        ImGui::Image(m_Renderer.GetTextureRef(), ImVec2(550, 550));
        ImGui::End();
        ImGui::Begin("Settings");
        ImGui::SeparatorText("Object Transform");
        ImGui::SliderFloat("Scale", &m_Triangle.scale, 0.1f, 2.f);
        ImGui::SliderAngle("Rotation", &m_Triangle.rotation);
        ImGui::SliderFloat("Offset X", &m_Triangle.offsetX, -float(m_Width) / 2.f, float(m_Width) / 2.f);
        ImGui::SliderFloat("Offset Y", &m_Triangle.offsetY, -float(m_Height) / 2.f, float(m_Height) / 2.f);
        ImGui::SeparatorText("Camera");
        ImGui::SliderFloat3("Camera Pos", &m_Camera.position.x, -10.f, 10.f);
        ImGui::SliderFloat3("Camera Target", &m_Camera.target.x, -10.f, 10.f);
        ImGui::SliderFloat("FOV", &m_Camera.fov, 10.f, 120.f);
        ImGui::End();
    }

private:
    uint32_t m_Width, m_Height;
    gty::RasterizationRenderer m_Renderer;
    gty::Triangle m_Triangle;
    gty::Triangle m_Triangle2;
    gty::Camera m_Camera;
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
