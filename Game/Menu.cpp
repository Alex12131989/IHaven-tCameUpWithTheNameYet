#include "Menu.h"
#include "CONSTANTS.h"
#include "GenericFunctions.h"
#include "Shapes.h"

Menu::Menu(Vector2 screen, std::vector<std::string> options, std::vector<Image> textures, Color primaryColor, Color secondaryColor, Color inactiveColor, unsigned int orientation)
{
    //I don't really want it THAT way, try vertical (MW kinda style)
    switch (orientation)
    {
    case VERTICAL:
        for (size_t i = 0; i < options.size(); ++i)
        {
            DrawRectangle(Int(screen.x*(1 - VmenuOptionWPerc)/2, FLOOR), Int(screen.y*(VmenuOptionHPerc + i*(VmenuOptionHPerc + VmenuGapPerc)), FLOOR), Int(screen.x*VmenuOptionWPerc, FLOOR), Int(screen.y*VmenuOptionHPerc, FLOOR), secondaryColor);
            Vector2 textSizes = MeasureTextEx(GetFontDefault(), options[i].data(), static_cast<float>(menuFontValue), 0);
            DrawText(options[i].data(), Int((screen.x - textSizes.x)/2, FLOOR),Int(screen.y*(VmenuOptionHPerc + i*(VmenuOptionHPerc + VmenuGapPerc) + (VmenuOptionHPerc - textSizes.y/screen.y)/2), FLOOR), menuFontValue, primaryColor);
        }
        break;
    case HORIZONTAL:
        //options
        if (currentSelection)
        {
            DrawRectangle(Int(screen.x*((1 - HmenuWidth)/2 - HmenuGapPerc - HmenuWidth), FLOOR), Int(screen.y*(1 - HmenuHeight)/2, FLOOR), Int(screen.x*HmenuWidth, FLOOR), Int(screen.y*HmenuHeight, FLOOR), secondaryColor);
            DrawRectangle(Int(screen.x*((1 - HmenuWidth)/2 - HmenuGapPerc - HmenuWidth), FLOOR), Int(screen.y*(1 - HmenuHeight)/2, FLOOR), Int(screen.x*HmenuWidth/2, FLOOR), Int(screen.y*HmenuHeight, FLOOR), menuBackgroundColor);

        }

        DrawRectangle(Int(screen.x*(1 - HmenuWidth)/2, FLOOR), Int(screen.y*(1 - HmenuHeight)/2, FLOOR),Int(screen.x*HmenuWidth, FLOOR), Int(screen.y*HmenuHeight, FLOOR), secondaryColor);

        if (currentSelection != options.size() - 1)
        {
            DrawRectangle(Int(screen.x*((1 - HmenuWidth)/2 + HmenuGapPerc + HmenuWidth), FLOOR), Int(screen.y*(1 - HmenuHeight)/2, FLOOR), Int(screen.x*HmenuWidth, FLOOR), Int(screen.y*HmenuHeight, FLOOR), secondaryColor);
            DrawRectangle(Int(screen.x*((1 - HmenuWidth)/2 + HmenuGapPerc + 1.5*HmenuWidth), FLOOR), Int(screen.y*(1 - HmenuHeight)/2, FLOOR), Int(screen.x*HmenuWidth/2, FLOOR), Int(screen.y*HmenuHeight, FLOOR), menuBackgroundColor);

        }

        //arrows
        if (!currentSelection)
            DrawArrow({ screen.x*HarrowMargin, screen.y*(1 - HarrowHeight)/2 }, { screen.x*HarrowWidth, screen.y*HarrowHeight }, HarrowThickness, inactiveColor, LEFTBOUND);
        else
            DrawArrow({ screen.x*HarrowMargin, screen.y*(1 - HarrowHeight)/2 }, { screen.x*HarrowWidth, screen.y*HarrowHeight }, HarrowThickness, EditColor(secondaryColor, 0.8f), LEFTBOUND);
        if (currentSelection == options.size() - 1)
            DrawArrow({ screen.x*(1 - HarrowWidth - HarrowMargin), screen.y*(1 - HarrowHeight)/2 }, { screen.x*HarrowWidth, screen.y*HarrowHeight }, HarrowThickness, inactiveColor, RIGHTBOUND);
        else
            DrawArrow({ screen.x*(1 - HarrowWidth - HarrowMargin), screen.y*(1 - HarrowHeight)/2 }, { screen.x*HarrowWidth, screen.y*HarrowHeight }, HarrowThickness, EditColor(secondaryColor, 0.8f), RIGHTBOUND);
        break;
    }
}