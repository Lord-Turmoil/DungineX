/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Error.h                                   *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 2, 2025                              *
 *                                                                            *
 *                    Last Update : June 2, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Error code definitions.                                                    *
 ******************************************************************************/

#pragma once

#define DGEX_SUCCESS 0

#define ERROR_GRAPHICS(NO) (100 + (NO))
#define ERROR_AUDIO(NO)    (200 + (NO))
#define ERROR_RESOURCE(NO) (400 + (NO))

#define ERROR_SDL_INIT      (ERROR_GRAPHICS(1))
#define ERROR_WINDOW_INIT   (ERROR_GRAPHICS(2))
#define ERROR_RENDERER_INIT (ERROR_GRAPHICS(3))
#define ERROR_CUSTOM_INIT   (ERROR_GRAPHICS(4))
