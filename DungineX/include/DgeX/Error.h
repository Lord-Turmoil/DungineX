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

typedef int dgex_error_t;

#define DGEX_SUCCESS 0

#define DGEX_ERROR_GRAPHICS(NO) (100 + (NO))
#define DGEX_ERROR_AUDIO(NO)    (200 + (NO))
#define DGEX_ERROR_RESOURCE(NO) (300 + (NO))
#define DGEX_ERROR_CUSTOM(NO)   (400 + (NO))

#define DGEX_ERROR_SDL_INIT          (DGEX_ERROR_GRAPHICS(1))
#define DGEX_ERROR_SDL_TTF_INIT      (DGEX_ERROR_GRAPHICS(2))
#define DGEX_ERROR_WINDOW_INIT       (DGEX_ERROR_GRAPHICS(3))
#define DGEX_ERROR_RENDERER_INIT     (DGEX_ERROR_GRAPHICS(4))
#define DGEX_ERROR_RENDERER_API_INIT (DGEX_ERROR_GRAPHICS(5))

#define DGEX_ERROR_CUSTOM_INIT  (DGEX_ERROR_CUSTOM(1))
#define DGEX_ERROR_CUSTOM_START (DGEX_ERROR_CUSTOM(2))
