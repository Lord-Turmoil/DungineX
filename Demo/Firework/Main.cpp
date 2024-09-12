#include "Firework.h"
#include "FireworkApp.h"

#include <DgeX/EntryPoint.h>

Application* DgeX::CreateApplication(ApplicationCommandLineArgs args)
{
    ApplicationSpecification specification;
    specification.Name = "Firework";
    specification.Width = 800;
    specification.Height = 600;
    specification.Flags = Default | DisableResize;
    specification.CommandLineArgs = args;
    return new FireworkApp(specification);
}
