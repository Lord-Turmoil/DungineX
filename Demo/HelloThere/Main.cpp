/**
 * This is a placeholder demo for DungineX. :P
 */

#define NAME          "Demo"
#define USE_CALLBACKS 0

#if USE_CALLBACKS

#define DGEX_USE_CALLBACKS

#include <DgeX/DgeX.h>
#include <DgeX/EntryPoint.h>

int DgeXOnInit(const CommandLineArgs& args, void** context)
{
    DGEX_LOG_INFO(NAME, "OnInit");
    return 0;
}

int DgeXOnUpdate(void* context)
{
    return 0;
}

int DgeXOnEvent(void* context)
{
    DGEX_LOG_INFO(NAME, "OnEvent");
    return 0;
}

int DgeXOnExit(void* context)
{
    DGEX_LOG_INFO(NAME, "OnExit");
    return 0;
}

#else

#include <DgeX/DgeX.h>
#include <DgeX/EntryPoint.h>

#include <SDL3/SDL.h>

int main(const CommandLineArgs& args)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Demo", 640, 480, SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    SDL_ShowWindow(window);
    SDL_SetRenderVSync(renderer, -1);

    bool isRunning = true;
    while (isRunning)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            DGEX_LOG_INFO(NAME, "Event: {0}", event.type);
            if (event.type == SDL_EVENT_QUIT)
            {
                isRunning = false;
            }
        }

        auto time = SDL_GetTicks() / 1000.f;
        auto red = (std::sin(time) + 1) / 2.0 * 255;
        auto green = (std::sin(time / 2) + 1) / 2.0 * 255;
        auto blue = (std::sin(time) * 2 + 1) / 2.0 * 255;

        SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}

#endif
