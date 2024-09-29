#include <DgeX/DgeX.h>
#include <DgeX/EntryPoint.h>

class MainLayer final : public DgeX::Layer
{
public:
    MainLayer() : Layer("MainLayer")
    {
    }

    void OnEvent(const DgeX::Ref<DgeX::Event>& event) override
    {
        DgeX::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<DgeX::KeyPressedEvent>(DGEX_BIND_EVENT_FN(_OnKeyPressed));
    }

    void OnUpdate(DgeX::DeltaTime delta) override
    {
        _elapsedTime += delta;
    }

    void OnRender() override
    {
        DgeX::RenderApi::DrawRotatedFilledRect({ GetWidth() / 2.0f, GetHeight() / 2.0f }, { 200.0f, 200.0f },
                                               _elapsedTime * DgeX::Math::PI<float>,
                                               DgeX::Color::FromUInt32(0xFF9900FF).ToVec4());
        DgeX::RenderApi::DrawRotatedRect({ GetWidth() / 2.0f, GetHeight() / 2.0f }, { 250.0f, 250.0f },
                                         -_elapsedTime * DgeX::Math::PI<float>,
                                         DgeX::Color::FromUInt32(0xFFFFFF00).ToVec4());

        DgeX::RenderApi::DrawCircle({ GetWidth() / 4.0f, GetHeight() / 4.0f }, 100.0f,
                                    DgeX::Color::FromUInt32(0xFFFF3300).ToVec4());
        DgeX::RenderApi::DrawCircle({ GetWidth() * 0.75f, GetHeight() * 0.75f }, 100.0f,
                                    DgeX::Color::FromUInt32(0xFF99FF00).ToVec4());
        DgeX::RenderApi::DrawFilledCircle({ GetWidth() * 0.25f, GetHeight() * 0.75f }, 100.0f,
                                          DgeX::Color::FromUInt32(0xFF00CCFF).ToVec4());
        DgeX::RenderApi::DrawFilledCircle({ GetWidth() * 0.75f, GetHeight() * 0.25f }, 100.0f,
                                          DgeX::Color::FromUInt32(0xFFFF00FF).ToVec4());

        DgeX::RenderApi::SetFontFamily(72);
        DgeX::RenderApi::DrawString("Greetings from DgeX!\nI mean it.", { GetWidth() * 0.5f, GetHeight() * 0.15f },
                                    DgeX::RenderApi::StringAlignEnum::SA_Center |
                                        DgeX::RenderApi::StringAlignEnum::SA_VCenter);
    }

private:
    static bool _OnKeyPressed(DgeX::KeyPressedEvent& event)
    {
        if (event.GetKeyCode() == DgeX::Key::Escape)
        {
            DgeX::EventEmitter::Emit(DgeX::WindowCloseEvent::Create());
        }
        return true;
    }

    float _elapsedTime = 0.0f;
};

class MainInterface final : public DgeX::Interface
{
public:
    MainInterface() : Interface("Main")
    {
        PushLayer(&_layer);
    }

    void OnLoad() override
    {
        DGEX_LOG_INFO("MainInterface loaded");
    }

private:
    MainLayer _layer;
};

MainInterface mainInterface;

DgeX::Interface* DgeX::GetInterface(const std::string& name)
{
    if (name == "Main")
    {
        return &mainInterface;
    }
    return nullptr;
}

DgeX::Application* DgeX::CreateApplication(ApplicationCommandLineArgs args)
{
    ApplicationSpecification specification;
    specification.Name = "Demo Application";
    specification.Width = 1280;
    specification.Height = 720;
    specification.Flags = Default;
    specification.CommandLineArgs = args;
    return new Application(specification);
}
