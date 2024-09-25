#include "Firework.h"
#include "FireworkApp.h"

#include <DgeX/EntryPoint.h>

Application* DgeX::CreateApplication(ApplicationCommandLineArgs args)
{
    ApplicationSpecification specification;
    specification.Name = "Firework";
    specification.Width = 1280;
    specification.Height = 720;
    specification.Flags = Default | DisableResize;
    specification.CommandLineArgs = args;

    return new Application(specification);
}
