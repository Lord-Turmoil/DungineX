#pragma once

#include "doctest.h"

#define CHECK_NULL(EXP)     CHECK_EQ((EXP), nullptr)
#define CHECK_NON_NULL(EXP) CHECK_NE((EXP), nullptr)

#define CHECK_ZERO(EXP)     CHECK_EQ((EXP), 0)
#define CHECK_NON_ZERO(EXP) CHECK_NE((EXP), 0)
