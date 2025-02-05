#pragma once

#include "DgeX/Core/Macros.h"

#include <string>

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

/**
 * @brief Copy characters from source to destination.
 * @param dest Destination buffer.
 * @param src Source buffer.
 * @param count Number of characters to copy.
 */
void Copy(char* dest, const char* src, size_t count);

/**
 * @brief Check if two strings are equal.
 * @param str1 String.
 * @param str2 String.
 * @return Whether two strings are equal or not.
 */
bool Equals(const char* str1, const char* str2);

/**
 * @brief Get the length of a string.
 * @param str String.
 * @return Length of the string.
 */
size_t Length(const char* str);

/**
 * @brief Check if the string contains the character.
 * @param str String.
 * @param ch Character to find.
 * @return Whether the string contains the character or not.
 */
bool Contains(const char* str, char ch);

/**
 * @brief Find the character in the given string.
 * @param str String.
 * @param ch Character to find.
 * @return First occurrence of the character. nullptr if not found.
 */
const char* Find(const char* str, char ch);

/**
 * @brief Find the character in the given string interval.
 * @param begin Begin of the string.
 * @param end End of the string.
 * @param ch Character to find.
 * @return First occurence of the character.
 */
const char* Find(const char* begin, const char* end, char ch);

} // namespace Utils::String

DGEX_END
