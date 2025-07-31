/**
 * This is a placeholder demo for DungineX. :P
 */

#define NAME "Demo"

#include <DgeX/DgeX.h>

#define DGEX_USE_CALLBACKS
#include <DgeX/EntryPoint.h>

using namespace DgeX;

struct State
{
    Ref<Renderer> DirectRenderer;
    Ref<Renderer> OrderedRenderer;
    Ref<Texture> Image;
    Ref<Texture> Canvas;
    int Count = 0;
    bool Quit = false;
};

int OnInit(const CommandLineArgs& args, void** context)
{
    DGEX_LOG_INFO(NAME, "OnInit");

    *context = new State();

    SetWindowPropertiesHint({ "Hello There", 640, 480, DgexWindowResizable });

    return 0;
}

int OnStart(void* context)
{
    auto state = static_cast<State*>(context);

    state->DirectRenderer = CreateRenderer({ false });
    state->OrderedRenderer = CreateRenderer({ true });
    state->Image = LoadTexture("gs_tiger.svg");
    state->Canvas = CreateTexture(300, 300);

    SetFont(LoadFont("Arial"));
    SetFontSize(36.0f);

    DGEX_LOG_INFO(NAME, "Image size: {0}x{0}", state->Image->GetWidth(), state->Image->GetHeight());

    return 0;
}

int OnUpdate(void* context)
{
    auto state = static_cast<State*>(context);

    ClearDevice();

    DrawTexture(state->Image, 20, 20);

    {
        USE_RENDERER(state->DirectRenderer);
        USE_RENDER_TARGET(state->Canvas);

        Color oldClearColor = GetClearColor();
        SetClearColor(Color::White);
        ClearDevice();
        SetClearColor(oldClearColor);

        SetFillColor(Color::Yellow);
        DrawFilledRect(0, 0, 200, 100);
        SetFillColor(Color::Red);
        DrawFilledRect(40, 40, 200, 100); // on top of yellow
        state->DirectRenderer->Render();
    }

    TextureStyle styleA;
    styleA.Alpha = 220;
    styleA.Scale = 0.9f;

    TextureStyle styleB;
    styleB.Alpha = 220;
    styleB.Degree = 30.0f;
    styleB.Scale = 0.9f;

    TextureAnchor anchor = { 0, 0 };

    DrawTexture(state->Canvas, styleA, 10, 10);
    DrawTexture(state->Canvas, styleB, anchor, 10, 10);

    {
        USE_RENDERER(state->OrderedRenderer);
        SetFillColor(Color::Yellow);
        DrawFilledRect(310, 10, 200, 100, 10); // on top of red
        SetFillColor(Color::Red);
        DrawFilledRect(350, 50, 200, 100);
        state->OrderedRenderer->Render();
    }

    SetLineColor(Color::Blue);
    DrawLine(0, 0, 640, 480);

    SetFontColor(Color::LightMagenta);
    DrawText("Hello there!", 600, 10, L(TextFlag::AlignRight));

    if (IsKeyPressed(L(KeyCodes::A)))
    {
        SetFontColor(Color::LightGreen);
        DrawText("Key A is pressed!", 10, 10, L(TextFlag::AlignLeft));
    }

    if (IsMousePressed(L(MouseCodes::Left)))
    {
        FPoint pos = GetMousePosition();
        SetFontColor(Color::LightBlue);
        DrawText("Mouse Left Clicked!", static_cast<int>(pos.X), static_cast<int>(pos.Y), L(TextFlag::AlignRight));
    }

    FlushDevice();

    if (IsKeyPressed(L(KeyCodes::Escape)))
    {
        state->Quit = true;
    }

    return state->Quit ? 1 : 0;
}

int OnEvent(void* context, Ref<Event> event)
{
    auto state = static_cast<State*>(context);
    int value = (state->Count++) % 255;

    uint8_t r = static_cast<uint8_t>(value);
    uint8_t g = static_cast<uint8_t>((value * 2) % 255);
    uint8_t b = static_cast<uint8_t>((value * 3) % 255);

    SetClearColor(Color(r, g, b));

    DGEX_LOG_DEBUG(NAME, "OnEvent: {0}", event->ToString());

    DispatchEvent<WindowCloseEvent>(event, [&](const WindowCloseEvent& e) {
        DGEX_LOG_INFO(NAME, "Window close event received");
        state->Quit = true;
        return true;
    });

    return 0;
}

int OnExit(void* context)
{
    DGEX_LOG_INFO(NAME, "OnExit");
    delete static_cast<State*>(context);

    return 0;
}
