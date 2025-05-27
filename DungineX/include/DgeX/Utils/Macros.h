/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Macros.h                                  *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : May 25, 2025                              *
 *                                                                            *
 *                    Last Update : May 25, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Utility macro definitions.                                                 *
 ******************************************************************************/

// All macros are prefixed with DGEX_ to avoid name collisions.

#define DGEX_EXPAND_MACRO(x)    x
#define DGEX_STRINGIFY_MACRO(x) #x
#define DGEX_STRINGIFY(x)       DGEX_STRINGIFY_MACRO(x)

#define DGEX_BIT(x) (1 << (x))
