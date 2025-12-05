#include "Application.hpp"
#include <Image.hpp>
#include <vector>
#include "Renderer/Rasterization/RasterizationRenderer.hpp"
#include "Renderer/Triangle.hpp"

class MyLayer : public Flux::Layer
{
public:
    MyLayer() : m_Renderer(512, 512)
    {
        m_Renderer.setClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    }

    virtual void OnAttach() override {}
    virtual void OnDetach() override {}

    virtual void OnUpdate(float dt) override
    {
        m_Renderer.Clear();

        gty::Vertex3 v0(glm::vec3(128, 128, 0), glm::vec4(1,0,0,1)); // 红
        gty::Vertex3 v1(glm::vec3(384, 128, 0), glm::vec4(0,1,0,1)); // 绿
        gty::Vertex3 v2(glm::vec3(256, 384, 0), glm::vec4(0,0,1,1)); // 蓝

        gty::Triangle triangle(v0, v1, v2);
        m_Renderer.DrawTriangle(triangle);

        m_Renderer.Render();
        m_Renderer.SwapBuffers();
    }

    virtual void OnRenderUI() override
    {
        ImGui::Begin("Rasterization Test");
        ImGui::Image(m_Renderer.GetTextureRef(), ImVec2(512, 512));
        ImGui::End();
    }

private:
    gty::RasterizationRenderer m_Renderer;
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
                if (ImGui::MenuItem("Exit"))
                {
                    Close();
                }
                ImGui::EndMenu();
            }
        });
    }

    virtual ~MyApp() override = default;
};

std::unique_ptr<Flux::Application> Flux::CreateApplication()
{
    return std::make_unique<MyApp>();
}
