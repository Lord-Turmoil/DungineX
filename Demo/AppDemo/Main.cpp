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
        DgeX::RenderApi::SetLineWidth(5.0f);

        DgeX::RenderApi::DrawRotatedFilledRect({ GetWidth() / 2.0f, GetHeight() / 2.0f }, { 200.0f, 200.0f },
                                               _elapsedTime * DgeX::Math::PI<float>,
                                               DgeX::Color::FromUInt32(0xFF9900FF).ToVec4());
        DgeX::RenderApi::DrawRotatedRect({ GetWidth() / 2.0f, GetHeight() / 2.0f }, { 250.0f, 250.0f },
                                         -_elapsedTime * DgeX::Math::PI<float>,
                                         DgeX::Color::FromUInt32(0xFFFF0000).ToVec4());

        DgeX::RenderApi::DrawCircle({ GetWidth() * 0.75f, GetHeight() * 0.75f }, 100.0f,
                                    DgeX::Color::FromUInt32(0xFF99FF00).ToVec4());
        DgeX::RenderApi::DrawFilledCircle({ GetWidth() * 0.75f, GetHeight() * 0.25f }, 100.0f,
                                          DgeX::Color::FromUInt32(0xFFFF00FF).ToVec4());

        _RenderText();
    }

private:
    void _RenderText() const;

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

void MainLayer::_RenderText() const
{
    using namespace DgeX::RenderApi;

    static const std::string text = "Greetings from DgeX!";
    static const std::string block = "Greetings from DgeX!\nI mean it.";
    SetFontFamily(48);
    SetLineWidth(1.0f);

    float width, height;
    DrawString(text, &width, &height);
    float lineWidth = width + 10.f;
    float lineHeight = height + 10.f;
    float radian = DgeX::Math::PI<float> / 36.0f;

    glm::vec2 position = { lineWidth, 0.0f };
    glm::vec4 textColor = { 1.0f, 1.0f, 0.2f, 1.0f };
    glm::vec4 lineColor = { 1.0f, 0.2f, 1.0f, 1.0f };

    DrawLine(position, { position.x, position.y + lineHeight * 8 }, lineColor);

    // Single line.
    position.y += lineHeight;
    DrawString(text, position, textColor, SA_Left | SA_Bottom);
    DrawLine({ position.x - lineWidth, position.y }, { position.x + lineWidth, position.y }, lineColor);
    position.y += lineHeight;
    DrawString(text, position, textColor, SA_Right | SA_Middle);
    DrawLine({ position.x - lineWidth, position.y }, { position.x + lineWidth, position.y }, lineColor);
    position.y += lineHeight;
    DrawString(text, position, textColor, SA_Center | SA_Top);
    DrawLine({ position.x - lineWidth, position.y }, { position.x + lineWidth, position.y }, lineColor);

    position.y += lineHeight;
    DrawLine({ position.x - lineWidth, position.y }, { position.x + lineWidth, position.y }, lineColor);

    // Rotated single line.
    position.y += lineHeight;
    DrawRotatedString(text, position, radian, textColor, SA_Left | SA_Bottom);
    DrawLine({ position.x - lineWidth, position.y }, { position.x + lineWidth, position.y }, lineColor);
    position.y += lineHeight;
    DrawRotatedString(text, position, DgeX::Math::Sin(_elapsedTime * 3) * radian, textColor, SA_Right | SA_Middle);
    DrawLine({ position.x - lineWidth, position.y }, { position.x + lineWidth, position.y }, lineColor);
    position.y += lineHeight;
    DrawRotatedString(text, position, -radian, textColor, SA_Center | SA_Top);
    DrawLine({ position.x - lineWidth, position.y }, { position.x + lineWidth, position.y }, lineColor);

    position.y += lineHeight;
    DrawLine({ position.x - lineWidth, position.y }, { position.x + lineWidth, position.y }, lineColor);

    // Multiple lines.
    DrawString(block, &width, &height);
    lineWidth = width + 10.f;
    lineHeight = height + 10.f;
    DrawLine(position, { position.x, position.y + lineHeight * 2.5f }, lineColor);

    position.y += lineHeight;
    DrawRotatedString(block, position, DgeX::Math::Sin(_elapsedTime * 3) * radian, textColor,
                      SA_Center | SA_Bottom | SA_MultiLine);
    DrawLine({ position.x - lineWidth, position.y }, { position.x + lineWidth, position.y }, lineColor);
    position.y += lineHeight;
    DrawString(block, position, textColor, SA_Right | SA_Middle | SA_MultiLine);
    DrawLine({ position.x - lineWidth, position.y }, { position.x + lineWidth, position.y }, lineColor);
}

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
