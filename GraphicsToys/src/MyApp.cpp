// MyLayer.hpp
#pragma once
#include "Application.hpp"
#include "Renderer/Rasterization/RasterizationRenderer.hpp"
#include "Renderer/Mesh.hpp"
#include "Renderer/Camera.hpp"
#include <imgui.h>

class MyLayer : public Flux::Layer
{
public:
    MyLayer(uint32_t width = 256, uint32_t height = 256)
        : m_Width(width), m_Height(height), m_Renderer(width, height)
    {
        m_Renderer.setClearColor({0.1f, 0.1f, 0.1f, 1.f});


        gty::Vertex3 r0({-50.f, -50.f, 0}, {1, 0, 0, 1});
        gty::Vertex3 r1({50.f, -50.f, 0}, {1, 0, 0, 1});
        gty::Vertex3 r2({0.f, 50.f, 0}, {1, 0, 0, 1});
        m_RedMesh.triangles.push_back({r0, r1, r2});
        m_RedMesh.translation = {0.f, 0.f, 0.f};

       
        gty::Vertex3 g0({0.f, -50.f, 0}, {0, 1, 0, 1});
        gty::Vertex3 g1({100.f, -50.f, 0}, {0, 1, 0, 1});
        gty::Vertex3 g2({50.f, 50.f, 0}, {0, 1, 0, 1});
        m_GreenMesh.triangles.push_back({g0, g1, g2});
        m_GreenMesh.translation = {0.f, 0.f, -20.f};

        m_Camera.position = {0.f, 0.f, 300.f};
        m_Camera.target = {0.f, 0.f, 0.f};
        m_Camera.up = {0.f, 1.f, 0.f};
        m_Camera.screenWidth = m_Width;
        m_Camera.screenHeight = m_Height;
    }

    virtual void OnUpdate(float dt) override
    {
        m_Renderer.Clear();
        m_RedMesh.UpdateModelMatrix();
        m_GreenMesh.UpdateModelMatrix();

        glm::mat4 VP = m_Camera.GetVPMatrix();

        m_Renderer.DrawMesh(m_RedMesh, VP);
        m_Renderer.DrawMesh(m_GreenMesh, VP);

        m_Renderer.Render();
    }

    virtual void OnRenderUI() override
    {
        ImGui::Begin("Mesh Renderer");
        ImGui::Image(m_Renderer.GetTextureRef(), ImVec2(600, 600));
        ImGui::End();
        ImGui::Begin("Camera & Mesh");
        ImGui::SeparatorText("Red Mesh");
        ImGui::SliderFloat3("Translation##Red", &m_RedMesh.translation.x, -200.f, 200.f);
        ImGui::SliderFloat3("Rotation##Red", &m_RedMesh.rotation.x, 0.f, 360.f);
        ImGui::SliderFloat("Scale##Red", &m_RedMesh.scale, 0.1f, 5.f);

        ImGui::SeparatorText("Green Mesh");
        ImGui::SliderFloat3("Translation##Green", &m_GreenMesh.translation.x, -200.f, 200.f);
        ImGui::SliderFloat3("Rotation##Green", &m_GreenMesh.rotation.x, 0.f, 360.f);
        ImGui::SliderFloat("Scale##Green", &m_GreenMesh.scale, 0.1f, 5.f);

        ImGui::SeparatorText("Camera");
        ImGui::SliderFloat3("Camera Pos", &m_Camera.position.x, -500.f, 500.f);
        ImGui::SliderFloat3("Camera Target", &m_Camera.target.x, -500.f, 500.f);
        ImGui::SliderFloat("FOV", &m_Camera.fov, 10.f, 120.f);
        ImGui::End();
    }

private:
    uint32_t m_Width, m_Height;
    gty::RasterizationRenderer m_Renderer;
    gty::Camera m_Camera;

    gty::Mesh m_RedMesh;
    gty::Mesh m_GreenMesh;
};

class MyApp : public Flux::Application
{
public:
    MyApp()
    {
        PushLayer(std::make_unique<MyLayer>());
        SetMenubarCallback([this]()
                           {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit")) Close();
                ImGui::EndMenu();
            } });
    }
    virtual ~MyApp() override = default;
};

std::unique_ptr<Flux::Application> Flux::CreateApplication()
{
    return std::make_unique<MyApp>();
}
