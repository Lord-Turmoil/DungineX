#include "DgeX/Application/Application.h"
#include "DgeX/Application/Event/ApplicationEvent.h"
#include "DgeX/Application/Event/EventEmitter.h"
#include "DgeX/Application/Interface/Interface.h"
#include "DgeX/Renderer/RenderApi.h"
#include "DgeX/Utils/Easing.h"

DGEX_BEGIN

class SplashInterface : public Interface
{
public:
    SplashInterface() : _layer(nullptr)
    {
    }

    ~SplashInterface() override = default;

    void OnLoad() override;
    void OnUnload() override;

private:
    Layer* _layer;
};

class SplashLayer : public Layer
{
public:
    SplashLayer() : Layer("SplashLayer")
    {
    }

    void OnAttach() override;
    void OnUpdate(DeltaTime delta) override;
    void OnRender() override;
    void OnEvent(const Ref<Event>& event) override;

private:
    bool _OnResize(WindowResizeEvent& event);

    timestamp_t _elapsedTime = 0.0f;
    glm::vec2 _splashPosition = { 0.f, 0.f };
    glm::vec2 _splashSize = { 600.f, 400.f };
    float _alpha = 0.0f;
};

void SplashInterface::OnLoad()
{
    _layer = new SplashLayer();
    PushLayer(_layer);

    DGEX_CORE_INFO("SplashInterface loaded");
}

void SplashInterface::OnUnload()
{
    PopLayer(_layer);
    delete _layer;
    _layer = nullptr;

    DGEX_CORE_INFO("SplashInterface unloaded");
}

void SplashLayer::OnAttach()
{
    _elapsedTime = 0.0f;

    auto app = Application::GetInstance();

    _splashPosition.x = 0.5f * static_cast<float>(app->GetWidth());
    _splashPosition.y = 0.5f * static_cast<float>(app->GetHeight());

    DGEX_CORE_INFO("SplashLayer attached");
}

void SplashLayer::OnUpdate(DeltaTime delta)
{
    using namespace Utils::Easing;

    _elapsedTime += delta.Seconds();

#if defined(DGEX_RELEASE) || defined(DGEX_ALWAYS_SPLASH)
    if (_elapsedTime < 3.5f)
    {
        _alpha = Interpolate(0.0f, 1.0f, RangeToStep(.5f, 2.8f, _elapsedTime), Linear);
    }
    else
    {
        _alpha = Interpolate(1.0f, 0.0f, RangeToStep(3.8f, 6.0f, _elapsedTime), EaseOutQuart);
    }

    if (_elapsedTime > 6.28f)
    {
        DGEX_CORE_INFO("Prepare to launch Client interface");
        EventEmitter::Emit(CreateRef<InterfaceChangeEvent>("Main"));
    }
#else
    _alpha = 255.f;
    if (_elapsedTime > 1.f)
    {
        DGEX_CORE_INFO("Splash screen skipped");
        EventEmitter::Emit(CreateRef<InterfaceChangeEvent>("Main"));
    }
#endif
}

void SplashLayer::OnRender()
{
    // RenderApi::DrawQuad(_splashPosition, _splashSize, glm::vec4(1.f, 1.f, 0.f, _alpha));
    RenderApi::DrawRotatedFilledRect(_splashPosition, _splashSize * 0.8f, Math::QUARTER_PI<float>,
                                     glm::vec4(1.f, 1.f, 0.f, _alpha));
    RenderApi::DrawRect(_splashPosition, _splashSize, glm::vec4(1.f, 0.f, 0.f, _alpha));
}

void SplashLayer::OnEvent(const Ref<Event>& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(DGEX_BIND_EVENT_FN(SplashLayer::_OnResize));
}

bool SplashLayer::_OnResize(WindowResizeEvent& event)
{
    auto app = Application::GetInstance();

    _splashPosition.x = 0.5f * static_cast<float>(app->GetWidth());
    _splashPosition.y = 0.5f * static_cast<float>(app->GetHeight());

    return false;
}

Interface* GetSplashInterface()
{
    static Ref<SplashInterface> sSplashInterface;

    if (!sSplashInterface)
    {
        sSplashInterface = CreateRef<SplashInterface>();
    }
    return sSplashInterface.get();
}

DGEX_END
