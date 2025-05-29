/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : EntryPoint.cpp                            *
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
 * Entry point implementation.                                                *
 ******************************************************************************/

// Prevent inclusion of `main` function.
#define DGEX_ENTRYPOINT_INTERNAL
#include "DgeX/EntryPoint.h"

#include <SDL3/SDL.h>

DGEX_EXTERN_C int DgeXMainImpl(int argc, char* argv[], DgeXEntryPoint entryPoint)
{
    SDL_Log("DungineX stated");
    int ret = entryPoint(argc, argv);
    SDL_Log("DungineX finished with return code: %d", ret);

    return ret;
}
