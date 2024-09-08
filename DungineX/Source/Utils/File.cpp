#include <fstream>

#include "DgeX/Utils/File.h"

DGEX_BEGIN

namespace Utils::File
{

std::string GetFileExtension(const std::string& filepath)
{
    auto lastSlash = filepath.find_last_of("/\\"); // last '/' or '\'
    auto lastDot = filepath.rfind('.');

    if (lastSlash == std::string::npos)
    {
        return (lastDot == std::string::npos) ? "" : filepath.substr(lastDot + 1);
    }
    if ((lastDot == std::string::npos) || (lastDot < lastSlash))
    {
        return "";
    }
    return filepath.substr(lastSlash + 1, lastDot - lastSlash - 1);
}

std::string GetFileName(const std::string& filepath)
{
    auto lastSlash = filepath.find_last_of("/\\"); // last '/' or '\'
    auto lastDot = filepath.rfind('.');

    if (lastSlash == std::string::npos)
    {
        return (lastDot == std::string::npos) ? filepath : filepath.substr(0, lastDot);
    }
    if (lastDot == std::string::npos)
    {
        return filepath.substr(lastSlash + 1);
    }
    if (lastDot < lastSlash)
    {
        return filepath.substr(lastSlash + 1);
    }
    return filepath.substr(lastSlash + 1, lastDot - lastSlash - 1);
}

} // namespace Utils::File

DGEX_END
