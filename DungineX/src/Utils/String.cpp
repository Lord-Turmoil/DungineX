#include "DgeX/Utils/String.h"

#include <cstring>

DGEX_BEGIN

namespace Utils::String
{

static constexpr int WCHAR_BUFFER_SIZE = 256;
static constexpr int CHAR_BUFFER_SIZE = 256;

int CharToWChar(wchar_t* dest, const char* src)
{
    size_t converted;
    const errno_t err = mbstowcs_s(&converted, dest, strlen(src) + 1, src, WCHAR_BUFFER_SIZE);
    return err ? -1 : static_cast<int>(converted);
}

wchar_t* CharToWChar(const char* src)
{
    static wchar_t sWidenBuffer[WCHAR_BUFFER_SIZE];

    const errno_t err = mbstowcs_s(nullptr, sWidenBuffer, strlen(src) + 1, src, WCHAR_BUFFER_SIZE);
    return err ? nullptr : sWidenBuffer;
}

int WCharToChar(char* dest, const wchar_t* src)
{
    size_t converted;
    const errno_t err = wcstombs_s(&converted, dest, wcslen(src) + 1, src, CHAR_BUFFER_SIZE) ? false : true;
    return err ? -1 : static_cast<int>(converted);
}

char* WCharToChar(const wchar_t* src)
{
    static char sNarrowBuffer[WCHAR_BUFFER_SIZE];

    const errno_t err = wcstombs_s(nullptr, sNarrowBuffer, wcslen(src) + 1, src, CHAR_BUFFER_SIZE);

    return err ? nullptr : sNarrowBuffer;
}

std::string ToString(const unsigned char* str)
{
    return reinterpret_cast<const char*>(str);
}

void Copy(char* dest, const char* src, size_t count)
{
    strncpy_s(dest, count, src, count);
}

bool Equals(const char* str1, const char* str2)
{
    return strcmp(str1, str2) == 0;
}

size_t Length(const char* str)
{
    return strlen(str);
}

bool Contains(const char* str, char ch)
{
    return strchr(str, ch) != nullptr;
}

const char* Find(const char* str, char ch)
{
    return strchr(str, ch);
}

const char* Find(const char* begin, const char* end, char ch)
{
    const char* pos = std::find(begin, end, ch);
    return pos != end ? pos : nullptr;
}

} // namespace Utils::String

DGEX_END
