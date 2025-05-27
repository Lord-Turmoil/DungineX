/**
 * This is a placeholder demo for DungineX. :P
 */

#include <DgeX/DgeX.h>

#include <iostream>

int main()
{
    std::cout << "Hello there!" << std::endl;
    std::cout << "DungineX " << DgeX::GetDgeXVersion() << "!" << std::endl;

    std::cout << std::endl << "Press Enter to quit..." << std::endl;
    std::cin.get();

    return 0;
}
