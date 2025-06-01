/**
 * This is a placeholder demo for DungineX. :P
 */

#include <DgeX/DgeX.h>

#define DGEX_CUSTOM_LOOP
#include <DgeX/EntryPoint.h>

#include <iostream>

#define NAME "Demo"

struct State
{
    int Count;
};

int DgeXOnInit(const CommandLineArgs& args, void** context)
{
    DGEX_LOG_INFO(NAME, "OnInit");

    State* state = new State();
    state->Count = 0;

    *context = state;

    return 0;
}

int DgeXOnEvent(void* context)
{
    DGEX_LOG_INFO(NAME, "OnEvent");
    return 0;
}

int DgeXOnUpdate(void* context)
{
    State* state = static_cast<State*>(context);
    state->Count++;
    if (state->Count % 100000 == 0)
    {
        DGEX_LOG_INFO(NAME, "Total {0} frames", state->Count);
    }

    if (state->Count > 10000000)
    {
        DGEX_LOG_WARN(NAME, "Over {0} frames, stopping", state->Count - 1);
        return 1;
    }

    return 0;
}

int DgeXOnExit(void* context, int result)
{
    DGEX_LOG_INFO(NAME, "OnExit");
    return 0;
}
