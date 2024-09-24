#include <DgeX/EntryPoint.h>

#include "PlatformApp.h"

Ref<PlatformInterface> interface;

Interface* DgeX::GetInterface(const std::string& name)
{
    if (!interface)
    {
        interface = CreateRef<PlatformInterface>();
    }
    return interface.get();
}

Application* DgeX::CreateApplication(ApplicationCommandLineArgs args)
{
    ApplicationSpecification specification;
    specification.Name = "Platform";
    specification.Width = 1280;
    specification.Height = 720;
    specification.Flags = Default | DisableResize;
    specification.CommandLineArgs = args;

    auto app = new Application(specification);
    app->SetFixedRefreshRate(144);

    return app;
}
