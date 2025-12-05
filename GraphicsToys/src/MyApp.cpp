#include "Application.hpp"
#include <Image.hpp>
#include <vector>
class MyLayer : public Flux::Layer
{
public:
    MyLayer()
        : m_Image(m_Width, m_Height)
    {
        Render();
    }

    void Render()
    {
        m_Data.resize(m_Width * m_Height * 4);
        for (int y = 0; y < m_Height; y++)
        {
            for (int x = 0; x < m_Width; x++)
            {
                int index = (y * m_Width + x) * 4;
                m_Data[index + 0] = uint8_t((float)x / m_Width * 255);
                m_Data[index + 1] = uint8_t((float)y / m_Height * 255);
                m_Data[index + 2] = 0;
                m_Data[index + 3] = 255;
            }
        }
        m_Image.SetData(m_Data.data());
    }
    virtual void OnAttach() override {}
    virtual void OnDetach() override {}
    virtual void OnUpdate(float dt) override {}
    virtual void OnRenderUI() override
    {
        ImGui::Begin("RayTracing");
        ImGui::Image(
            (ImTextureID)(uintptr_t)m_Image.GetColorAttachment(),
            ImVec2(static_cast<float>(m_Width), static_cast<float>(m_Height)));
        ImGui::End();
    }

private:
    int m_Width = 512;
    int m_Height = 512;

    Flux::Image m_Image;
    std::vector<uint8_t> m_Data;
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
            } });
    }

    virtual ~MyApp() override = default;
};
std::unique_ptr<Flux::Application> Flux::CreateApplication()
{
    return std::make_unique<MyApp>();
}