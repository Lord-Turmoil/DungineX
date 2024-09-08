#pragma once

#include "DgeX/dgexpch.h"

DGEX_BEGIN

namespace Utils::String
{

/**
 * @brief Convert char* to wchar_t*
 * @param dest converted wide string address
 * @param src source string address
 * @return the number of characters converted
 */
int CharToWChar(wchar_t* dest, const char* src);

/**
 * @brief Convert char* to wchar_t*
 * @warning This function is not thread-safe as it uses global buffer.
 * @param src source string address
 * @return address of the converted string
 */
wchar_t* CharToWChar(const char* src);

/**
 * @brief Convert wchar_t* to char*
 * @param dest converted string address
 * @param src source wide string address
 * @return the number of characters converted
 */
int WCharToChar(char* dest, const wchar_t* src);

/**
 * @brief Convert wchar_t* to char*
 * @warning This function is not thread-safe as it uses global buffer.
 * @param src source wide string address
 * @return address of the converted string
 */
char* WCharToChar(const wchar_t* src);

/**
 * @brief Convert bad string to std::string.
 * @param str the original bad string
 * @return A std::string
 */
std::string ToString(const unsigned char* str);

} // namespace Utils::String

DGEX_END
