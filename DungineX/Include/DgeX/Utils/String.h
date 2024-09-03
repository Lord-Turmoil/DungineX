#pragma once

#include "DgeX/Common/Base.h"

DGEX_BEGIN

namespace Utils
{

/**
 * @brief Convert char* to wchar_t*
 * @param dest converted wide string address
 * @param src source string address
 * @return the number of characters converted
 */
int widen(wchar_t* dest, const char* src);

/**
 * @brief Convert char* to wchar_t*
 * @warning This function is not thread-safe as it uses global buffer.
 * @param src source string address
 * @return address of the converted string
 */
wchar_t* widen(const char* src);

/**
 * @brief Convert wchar_t* to char*
 * @param dest converted string address
 * @param src source wide string address
 * @return the number of characters converted
 */
int narrow(char* dest, const wchar_t* src);

/**
 * @brief Convert wchar_t* to char*
 * @warning This function is not thread-safe as it uses global buffer.
 * @param src source wide string address
 * @return address of the converted string
 */
char* narrow(const wchar_t* src);

}

DGEX_END
