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
    float _x = 0.0f;
    float _y = 0.0f;
    float _width = 600.0f;
    float _height = 400.0f;
    float _alpha = 0.0f;
};

void SplashInterface::OnLoad()
{
    RenderApi::SetClearColor(Color::From255(50.f, 5.f, 60.f));

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

    _x = 0.5f * (static_cast<float>(app->GetWidth()) - _width);
    _y = 0.5f * (static_cast<float>(app->GetHeight()) - _height);

    DGEX_CORE_INFO("SplashLayer attached");
}

void SplashLayer::OnUpdate(DeltaTime delta)
{
    using namespace Utils::Easing;

    _elapsedTime += delta.Seconds();

#ifdef _PUBLISH
    if (_elapsedTime < 4.f)
    {
        _alpha = Interpolate(0.0f, 255.0f, RangeToStep(.5f, 3.f, _elapsedTime), EaseInOutCubic);
    }
    else
    {
        _alpha = Interpolate(255.0f, 0.0f, RangeToStep(5.f, 7.7f, _elapsedTime), EaseOutQuart);
        float step = RangeToStep(5.f, 7.5f, _elapsedTime);
        RenderApi::SetClearColor(Color::From255(Interpolate(50.f, 0.f, step, EaseInOutSine),
                                                Interpolate(5.f, 0.f, step, EaseInOutSine),
                                                Interpolate(60.f, 0.f, step, EaseInOutSine), 255.f));
    }

    if (_elapsedTime > 8.0f)
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
    RenderApi::SetColor(Color::From255(200.f, 0.f, 200.f, _alpha));
    RenderApi::DrawRectangle(_x, _y, _width, _height);
}

void SplashLayer::OnEvent(const Ref<Event>& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(DGEX_BIND_EVENT_FN(SplashLayer::_OnResize));
}

bool SplashLayer::_OnResize(WindowResizeEvent& event)
{
    auto app = Application::GetInstance();

    _x = 0.5f * (static_cast<float>(app->GetWidth()) - _width);
    _y = 0.5f * (static_cast<float>(app->GetHeight()) - _height);

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
