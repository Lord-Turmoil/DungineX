/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : RenderCommand.h                           *
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
 * Render command is each single render action.                               *
 ******************************************************************************/

#include "Device/Graphics/RenderCommand.h"

DGEX_BEGIN

RenderCommand::RenderCommand(int order) : _order(order)
{
}

int RenderCommand::GetOrder() const
{
    return _order;
}

DGEX_END
