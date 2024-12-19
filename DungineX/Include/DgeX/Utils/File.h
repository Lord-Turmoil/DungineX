#pragma once

#include "DgeX/Common/Macros.h"

#include <string>

DGEX_BEGIN

namespace Utils::File
{

std::string GetFileExtension(const std::string& filepath);

std::string GetFileName(const std::string& filepath);

} // namespace Utils::File

DGEX_END
