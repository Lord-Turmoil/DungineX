#include <DgeX/EntryPoint.h>

#include "CarApp.h"

DgeX::Ref<CarInterface> interface;

DgeX::Interface* DgeX::GetInterface(const std::string& name)
{
    if (!interface)
    {
        interface = DgeX::CreateRef<CarInterface>();
    }
    return interface.get();
}

DgeX::Application* DgeX::CreateApplication(ApplicationCommandLineArgs args)
{
    ApplicationSpecification specification;
    specification.Name = "Car";
    specification.Width = 1280;
    specification.Height = 720;
    specification.Flags = Default | DisableResize;
    specification.CommandLineArgs = args;

    return new Application(specification);
}
