#include "Game.h"

bool Game::MainLoop()
{
    InitWindow(screenWidth, screenHeight, "Title");
    LoadTextures();
    //ToggleFullscreen();
    SetTargetFPS(FPS);
    currentSelection = 1;

    while (!WindowShouldClose())
    {

        //Drawing
        BeginDrawing();
        ClearBackground(menuBackgroundColor);
        
        Menu(welcoming_options, {}, RED, BLUE, GRAY, HORIZONTAL);
        DrawTexture(temp_textures[0], 25, 25, RAYWHITE);
        EndDrawing();
        
    }
    CloseWindow();
    return true;
}

void Game::Menu(std::vector<std::string> options, std::vector<Image> textures, Color primaryColor, Color secondaryColor, Color inactiveColor, unsigned int orientation)
{
    //I don't really want it THAT way, try vertical (MW kinda style)
    switch (orientation)
    {
    case VERTICAL:
        for (size_t i = 0; i < options.size(); ++i)
        {
            DrawRectangle
            (
                Int(screenWidth*(1 - VmenuOptionWPerc)/2, FLOOR),
                Int(screenHeight*(VmenuOptionHPerc + i*(VmenuOptionHPerc + VmenuGapPerc)), FLOOR),
                Int(screenWidth*VmenuOptionWPerc, FLOOR), Int(screenHeight*VmenuOptionHPerc, FLOOR), secondaryColor
            );
            Vector2 textSizes = MeasureTextEx(GetFontDefault(), options[i].data(), static_cast<float>(menuFontValue), 0);
            DrawText
            (
                options[i].data(), Int((screenWidth - textSizes.x)/2, FLOOR),
                Int(screenHeight*(VmenuOptionHPerc + i*(VmenuOptionHPerc + VmenuGapPerc) + (VmenuOptionHPerc - textSizes.y/screenHeight)/2), FLOOR),
                menuFontValue, primaryColor
            );
        }
        break;
    case HORIZONTAL:
        //options
        if (currentSelection)
        {
            DrawRectangle
            (
                Int(screenWidth*((1 - HmenuWidth)/2 - HmenuGapPerc - HmenuWidth), FLOOR), 
                Int(screenHeight*(1 - HmenuHeight)/2, FLOOR),
                Int(screenWidth*HmenuWidth, FLOOR), Int(screenHeight*HmenuHeight, FLOOR), secondaryColor
            );
            DrawRectangle
            (
                Int(screenWidth*((1 - HmenuWidth)/2 - HmenuGapPerc - HmenuWidth), FLOOR), 
                Int(screenHeight*(1 - HmenuHeight)/2, FLOOR),
                Int(screenWidth*HmenuWidth/2, FLOOR), Int(screenHeight*HmenuHeight, FLOOR), menuBackgroundColor
            );

        }

        DrawRectangle
        (
            Int(screenWidth*(1 - HmenuWidth)/2, FLOOR), Int(screenHeight*(1 - HmenuHeight)/2, FLOOR), 
            Int(screenWidth*HmenuWidth, FLOOR), Int(screenHeight*HmenuHeight, FLOOR), secondaryColor
        );

        if (currentSelection != options.size() - 1)
        {
            DrawRectangle
            (
                Int(screenWidth*((1 - HmenuWidth)/2 + HmenuGapPerc + HmenuWidth), FLOOR),
                Int(screenHeight*(1 - HmenuHeight)/2, FLOOR),
                Int(screenWidth*HmenuWidth, FLOOR), Int(screenHeight*HmenuHeight, FLOOR), secondaryColor
            );
            DrawRectangle
            (
                Int(screenWidth*((1 - HmenuWidth)/2 + HmenuGapPerc + 1.5*HmenuWidth), FLOOR),
                Int(screenHeight*(1 - HmenuHeight)/2, FLOOR),
                Int(screenWidth*HmenuWidth/2, FLOOR), Int(screenHeight*HmenuHeight, FLOOR), menuBackgroundColor
            );

        }

        //arrows
        if (!currentSelection)
            DrawArrow
            (
                { screenWidth*HarrowMargin, screenHeight*(1 - HarrowHeight)/2 },
                { screenWidth*HarrowWidth, screenHeight*HarrowHeight }, HarrowThickness, inactiveColor, LEFT
            );
        else
            DrawArrow
            (
                { screenWidth*HarrowMargin, screenHeight*(1 - HarrowHeight)/2 },
                { screenWidth*HarrowWidth, screenHeight*HarrowHeight }, HarrowThickness, 
                EditColor(secondaryColor, 0.8f), LEFT
            );
        if (currentSelection == options.size() - 1)
            DrawArrow
            (
                { screenWidth*(1 - HarrowWidth - HarrowMargin), screenHeight*(1 - HarrowHeight)/2 }, 
                { screenWidth*HarrowWidth, screenHeight*HarrowHeight }, HarrowThickness, inactiveColor, RIGHT
            );
        else
            DrawArrow
            (
                { screenWidth*(1 - HarrowWidth - HarrowMargin), screenHeight*(1 - HarrowHeight)/2 },
                { screenWidth*HarrowWidth, screenHeight*HarrowHeight }, HarrowThickness, 
                EditColor(secondaryColor, 0.8f), RIGHT
            );
        break;
    }
}

void Game::DrawArrow(Vector2 topLeft, Vector2 sizes, unsigned int thickness, Color color, unsigned int orientation)
{
    switch (orientation)
    {
    case LEFT:
        DrawLineEx({ topLeft.x + sizes.x, topLeft.y }, { topLeft.x, topLeft.y + sizes.y/2 }, static_cast<float>(thickness), color);
        DrawLineEx({ topLeft.x, topLeft.y + sizes.y/2 - thickness/2 }, { topLeft.x + sizes.x, topLeft.y + sizes.y }, static_cast<float>(thickness), color);
        break;
    case RIGHT:
        DrawLineEx({ topLeft.x, topLeft.y }, { topLeft.x + sizes.x, topLeft.y + sizes.y/2 }, static_cast<float>(thickness), color);
        DrawLineEx({ topLeft.x + sizes.x, topLeft.y + sizes.y/2 - thickness/2 }, { topLeft.x, topLeft.y + sizes.y }, static_cast<float>(thickness), color);
        break;
    case DOWNWARD:
        DrawLineEx({ topLeft.x, topLeft.y }, { topLeft.x + sizes.x/2, topLeft.y + sizes.y }, static_cast<float>(thickness), color);
        DrawLineEx({ topLeft.x + sizes.x/2 - thickness/2, topLeft.y + sizes.y}, { topLeft.x + sizes.x, topLeft.y}, static_cast<float>(thickness), color);
        break;
    case UPWARD:
        DrawLineEx({ topLeft.x, topLeft.y + sizes.y}, { topLeft.x + sizes.x/2, topLeft.y }, static_cast<float>(thickness), color);
        DrawLineEx({ topLeft.x + sizes.x/2 - thickness/2, topLeft.y }, { topLeft.x + sizes.x, topLeft.y + sizes.y}, static_cast<float>(thickness), color);
        break;
    default:
        throw new std::invalid_argument("Invalid orientation");
    }
}

void Game::LoadTextures()
{
    temp_textures = GetTextures("numbers.png", { 512, 512 }, { 256, 256 });
}
