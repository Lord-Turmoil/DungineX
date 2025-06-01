/**
 * This is a placeholder demo for DungineX. :P
 */

#include <DgeX/DgeX.h>
#include <DgeX/EntryPoint.h>

#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "Hello there!" << std::endl;
    std::cout << "DungineX " << DgeX::GetDgeXVersion() << "!" << std::endl;

    std::cout << "Press Enter to quit..." << std::endl;
    std::cin.get();

    return 0;
}
