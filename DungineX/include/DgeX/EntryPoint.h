/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : EntryPoint.h                              *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : May 29, 2025                              *
 *                                                                            *
 *                    Last Update : May 29, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Entry point of the game.                                                   *
 * -------------------------------------------------------------------------- *
 * NOTE:                                                                      *
 *                                                                            *
 * This file should only be included in one source file.                      *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"

DGEX_EXTERN_C_BEGIN

typedef int (*DgeXEntryPoint)(int argc, char* argv[]);

/**
 * @brief Main entry point for the game built with DungineX.
 *
 * This entry should be defined in the client game code, and can't be
 * marked with DGEX_API. As the .dll target won't load it.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 */
extern int DgeXMain(int argc, char* argv[]);

extern DGEX_API int DgeXMainImpl(int argc, char* argv[], DgeXEntryPoint entryPoint);

DGEX_EXTERN_C_END

/**
 * Only include main implementation on client side, where
 * DGEX_ENTRYPOINT_INTERNAL is (hopefully) not defined.
 */
#ifndef DGEX_ENTRYPOINT_INTERNAL

#ifdef DGEX_USE_WINMAIN

#include <Windows.h>
#include <stdlib.h> // for __argc and __argv

/**
 * @brief Windows-specific entry point for the game.
 *
 * This is used when compiling for Windows with WinMain.
 *
 * @param hInstance Handle to the current instance of the application.
 * @param hPrevInstance Handle to the previous instance of the application (always NULL).
 * @param lpCmdLine Command line arguments as a string.
 * @param nCmdShow Controls how the window is to be shown.
 */
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    return DgeXMainImpl(__argc, __argv, DgeXMain);
}

#else

/**
 * @brief Standard entry point for the game.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 */
int main(int argc, char* argv[])
{
    return DgeXMainImpl(argc, argv, DgeXMain);
}

#endif // DGEX_USE_WINMAIN

/**
 * @brief An alias for the main entry point of the game.
 *
 * This should be defined in client game.
 */
#define main DgeXMain

#endif // DGEX_ENTRYPOINT_INTERNAL
