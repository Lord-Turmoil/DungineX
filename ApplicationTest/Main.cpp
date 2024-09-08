#include <DgeX/DgeX.h>

class DemoApplication final : public DgeX::Application
{
public:
    DemoApplication(const DgeX::ApplicationSpecification& specification) : Application(specification)
    {
    }
};

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

    void OnRender() override
    {
        DgeX::RenderApi::SetColor(DgeX::Color::FromUInt32(0xFFF00000));
        DgeX::RenderApi::DrawRectangle(600, 300, 80, 60);

        DgeX::RenderApi::SetColor(DgeX::Color::FromUInt32(0xFF0F0000));
        DgeX::RenderApi::DrawRectangle(700, 400, 80, 60);

        DgeX::RenderApi::SetColor(DgeX::Color::FromUInt32(0xA00F0000));
        DgeX::RenderApi::DrawRectangle(600, 400, 80, 60);
    }

private:
    static bool _OnKeyPressed(DgeX::KeyPressedEvent& event)
    {
        if (event.GetKeyCode() == DgeX::Key::Escape)
        {
            DgeX::EventEmitter::Emit(DgeX::CreateRef<DgeX::WindowCloseEvent>());
        }
        return true;
    }
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
        DgeX::RenderApi::SetClearColor(DgeX::Color::FromUInt32(0xFFFFFFFF));
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
    return new DemoApplication(specification);
}
