#pragma once
#include "Application.hpp"
#include "Renderer/Rasterization/RasterizationRenderer.hpp"
#include "Renderer/Mesh.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/OBJLoader.hpp"
#include <imgui.h>
#include <iostream>

class MyLayer : public Flux::Layer
{
public:
    MyLayer(uint32_t width = 512, uint32_t height = 512)
        : m_Width(width), m_Height(height), m_Renderer(width, height)
    {
        m_Renderer.setClearColor({0.0f, 0.0f, 0.0f, 1.f});

        gty::Mesh model;
        if (!LoadObjToMesh("F:\\Beisent\\GraphicsToy\\GraphicsToys\\obj\\Cubone.obj", model))
        {
            std::cerr << "Failed to load obj\n";
        }
        model.translation = {0.f, 0.f, 0.f};
        model.scale = 1.f;
        m_Model = model;

        m_Camera.position = {-300.f, 300.f, 500.f};
        m_Camera.target = {0.f, 0.f, 0.f};
        m_Camera.up = {0.f, 1.f, 0.f};
        m_Camera.screenWidth = m_Width;
        m_Camera.screenHeight = m_Height;

        m_Light.direction = {-1.f, 1.f, 1.f};
        m_Light.color = {1.f, 1.f, 1.f};
        m_Light.intensity = 1.f;

        m_Material.ambient = {0.1f, 0.1f, 0.1f};
        m_Material.diffuse = {1.f, 1.f, 1.f};
        m_Material.specular = {1.f, 1.f, 1.f};
        m_Material.shininess = 32.f;
    }

    virtual void OnUpdate(float dt) override
    {
        m_Renderer.Clear();
        m_Model.UpdateModelMatrix();

        glm::mat4 VP = m_Camera.GetVPMatrix();
        m_Renderer.DrawMesh(m_Model, VP, m_Camera.position, m_Light, m_Material);
        m_Renderer.Render();
    }

    virtual void OnRenderUI() override
    {
        ImGui::Begin("Mesh Renderer");
        ImGui::Image(m_Renderer.GetTextureRef(), ImVec2(600, 600));
        ImGui::End();

        ImGui::Begin("Settings");
        ImGui::SeparatorText("Mesh");
        ImGui::SliderFloat3("Translation", &m_Model.translation.x, -200.f, 200.f);
        ImGui::SliderFloat3("Rotation", &m_Model.rotation.x, 0.f, 360.f);
        ImGui::SliderFloat("Scale", &m_Model.scale, 0.1f, 100.f);

        ImGui::SeparatorText("Camera");
        ImGui::SliderFloat3("Camera Pos", &m_Camera.position.x, -500.f, 500.f);
        ImGui::SliderFloat3("Camera Target", &m_Camera.target.x, -500.f, 500.f);
        ImGui::SliderFloat("FOV", &m_Camera.fov, 10.f, 120.f);

        ImGui::SeparatorText("Light");
        ImGui::SliderFloat3("Direction", &m_Light.direction.x, -1.f, 1.f);
        ImGui::ColorEdit3("Color", &m_Light.color.x);
        ImGui::SliderFloat("Intensity", &m_Light.intensity, 0.f, 100.f);

        ImGui::SeparatorText("Material (Global Multipliers)");
        ImGui::ColorEdit3("Ambient Scale", &m_Material.ambient.x);
        ImGui::ColorEdit3("Diffuse Scale", &m_Material.diffuse.x);
        ImGui::ColorEdit3("Specular Scale", &m_Material.specular.x);
        ImGui::SliderFloat("Shininess Scale", &m_Material.shininess, 0.1f, 4.f);

        ImGui::End();
    }

private:
    uint32_t m_Width, m_Height;
    gty::RasterizationRenderer m_Renderer;
    gty::Camera m_Camera;
    gty::Mesh m_Model;

    DirectionalLight m_Light;
    Material m_Material;
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
