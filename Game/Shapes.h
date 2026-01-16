#pragma once
#include "CONSTANTS.h"
#include <raylib.h>

void DrawArrow(Vector2 topLeft, Vector2 sizes, unsigned int thickness, Color color, unsigned int orientation)
{
    switch (orientation)
    {
    case LEFTBOUND:
        DrawLineEx({ topLeft.x + sizes.x, topLeft.y }, { topLeft.x, topLeft.y + sizes.y/2 }, static_cast<float>(thickness), color);
        DrawLineEx({ topLeft.x, topLeft.y + sizes.y/2 - thickness/2 }, { topLeft.x + sizes.x, topLeft.y + sizes.y }, static_cast<float>(thickness), color);
        break;
    case RIGHTBOUND:
        DrawLineEx({ topLeft.x, topLeft.y }, { topLeft.x + sizes.x, topLeft.y + sizes.y/2 }, static_cast<float>(thickness), color);
        DrawLineEx({ topLeft.x + sizes.x, topLeft.y + sizes.y/2 - thickness/2 }, { topLeft.x, topLeft.y + sizes.y }, static_cast<float>(thickness), color);
        break;
    case DOWNWARD:
        DrawLineEx({ topLeft.x, topLeft.y }, { topLeft.x + sizes.x/2, topLeft.y + sizes.y }, static_cast<float>(thickness), color);
        DrawLineEx({ topLeft.x + sizes.x/2 - thickness/2, topLeft.y + sizes.y }, { topLeft.x + sizes.x, topLeft.y }, static_cast<float>(thickness), color);
        break;
    case UPWARD:
        DrawLineEx({ topLeft.x, topLeft.y + sizes.y }, { topLeft.x + sizes.x/2, topLeft.y }, static_cast<float>(thickness), color);
        DrawLineEx({ topLeft.x + sizes.x/2 - thickness/2, topLeft.y }, { topLeft.x + sizes.x, topLeft.y + sizes.y }, static_cast<float>(thickness), color);
        break;
    default:
        throw new std::invalid_argument("Invalid orientation");
    }
}
