#pragma once

#include "DgeX/dgexpch.h"

DGEX_BEGIN

namespace Utils::File
{

std::string GetFileExtension(const std::string& filepath);

std::string GetFileName(const std::string& filepath);

} // namespace Utils::File

DGEX_END
