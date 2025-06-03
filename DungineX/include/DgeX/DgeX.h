/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : DgeX.h                                    *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : May 26, 2025                              *
 *                                                                            *
 *                    Last Update : June 1, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Alas, here is the all-in-one header file for DungineX.                     *
 * -------------------------------------------------------------------------- *
 * ABOUT:                                                                     *
 *                                                                            *
 * DungineX is a 2D game engine made for fun, so I can enjoy my favorite      *
 * programming language, which apparently, is C++. This engine might not be   *
 * that powerful or performant, but it strives for the best practice.         *
 *                                                                            *
 * You may wonder, why "DungineX"? Initially, I made a rouge-like game called *
 * Dungeon, in which I implemented a simple game engine. By then, I found the *
 * combination of "Dungeon" and "Engine" quite natural, so I eventually named *
 * the underlying engine Dungine, i.e. "DUNgeon enGINE". And now, with more   *
 * experience, I wish to make it better, so here it is, DungineX. As for X, I *
 * prefer not to say.                                                         *
 * -------------------------------------------------------------------------- *
 * TRIVIA:                                                                    *
 *                                                                            *
 * This is quite an old-fashioned comment block for a file, which originally  *
 * comes from my favorite game series Command & Conquer by Westwood Studios.  *
 *                                                                            *
 * I know, as mentioned by Code Complete, that this is a bad practice as any  *
 * modification to the comment requires careful adjustment to alignment and   *
 * spacing, resulting in excessive effort. However, C&C has always been my    *
 * love, so I decided to use this comment style here as a tribute.            *
 * -------------------------------------------------------------------------- *
 * ACKNOWLEDGEMENT:                                                           *
 *                                                                            *
 * DungineX is built with many wonderful open-source libraries, and I would   *
 * like to say thank you to:                                                  *
 * - SDL3 (https://www.libsdl.org/) and all its components                    *
 * - spdlog (https://github.com/gabime/spdlog)                                *
 * -------------------------------------------------------------------------- *
 *                                                                            *
 *               "This is the moment, when all I've done,                     *
 *         all of my dreaming, scheming and screaming become one."            *
 *                                                                            *
 *                                                         - Jekyll & Hyde    *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"
#include "DgeX/Version.h"

#include "DgeX/Device/Graphics/Graphics.h"
#include "DgeX/Device/Graphics/Renderer.h"
#include "DgeX/Device/Graphics/Window.h"

#include "DgeX/Renderer/Color.h"
#include "DgeX/Renderer/RenderApi.h"
#include "DgeX/Renderer/Texture.h"

#include "DgeX/Utils/Assert.h"
#include "DgeX/Utils/Log.h"
#include "DgeX/Utils/Macros.h"
#include "DgeX/Utils/Math.h"
#include "DgeX/Utils/Types.h"
