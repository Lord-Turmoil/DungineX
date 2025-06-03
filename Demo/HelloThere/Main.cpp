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
    int Count = 0;
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
    State* state = static_cast<State*>(context);

    state->DirectRenderer = CreateRenderer({ false });
    state->OrderedRenderer = CreateRenderer({ true });
    state->Image = LoadTexture("gs_tiger.svg");

    DGEX_LOG_INFO(NAME, "Image size: {0}x{0}", state->Image->GetWidth(), state->Image->GetHeight());

    return 0;
}

int OnUpdate(void* context)
{
    State* state = static_cast<State*>(context);

    ClearDevice();

    DrawTexture(state->Image, 20, 20);

    auto texture = Texture::Create(300, 300);
    {
        UseRenderer(state->DirectRenderer);
        UseRenderTarget(texture);

        {
            UseClearColor(Color::White);
            ClearDevice();
        }

        SetFillColor(Color::Yellow);
        DrawFilledRect(0, 0, 200, 100);
        SetFillColor(Color::Red);
        DrawFilledRect(40, 40, 200, 100); // on top of yellow
        state->DirectRenderer->Render();
    }
    DrawTextureBegin(texture, 10, 10).Alpha(220).Rotate(30).Scale(0.9f).Submit();

    {
        UseRenderer(state->OrderedRenderer);
        SetFillColor(Color::Yellow);
        DrawFilledRect(310, 10, 200, 100, 10); // on top of red
        SetFillColor(Color::Red);
        DrawFilledRect(350, 50, 200, 100);
        state->OrderedRenderer->Render();
    }

    SetLineColor(Color::Blue);
    DrawLine(0, 0, 640, 480);

    FlushDevice();

    return 0;
}

int OnEvent(void* context)
{
    State* state = static_cast<State*>(context);
    int value = (state->Count++) % 255;

    uint8_t r = static_cast<uint8_t>(value);
    uint8_t g = static_cast<uint8_t>((value * 2) % 255);
    uint8_t b = static_cast<uint8_t>((value * 3) % 255);

    SetClearColor(Color(r, g, b));

    return 0;
}

int OnExit(void* context)
{
    DGEX_LOG_INFO(NAME, "OnExit");
    delete static_cast<State*>(context);

    return 0;
}
