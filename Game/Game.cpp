#include "Game.h"

bool Game::MainLoop()
{
    InitWindow(screenWidth, screenHeight, "Title");
    //ToggleFullscreen();
    SetTargetFPS(FPS);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Menu(welcoming_options);
    
        EndDrawing();
        
    }
    CloseWindow();
    return true;
}

void Game::Menu(std::vector<std::string> options)
{
    //I don't really want it THAT way, try vertical (MW kinda style)
    for (size_t i = 0; i < options.size(); ++i)
    {
        DrawRectangle
        (
            Int(screenWidth*(1 - menuOptionWPerc)/2, 2), 
            Int(screenHeight*(menuOptionHPerc + i*(menuOptionHPerc + menuGapPerc)), 2), 
            Int(screenWidth * menuOptionWPerc, 2), 
            Int(screenHeight * menuOptionHPerc, 2), 
            LIME
        );
        Vector2 textSizes = MeasureTextEx(GetFontDefault(), options[i].data(), static_cast<float>(menuFontValue), 0);
        DrawText(
            options[i].data(), 
            Int((screenWidth - textSizes.x)/2, 2), 
            Int(screenHeight*(menuOptionHPerc + i*(menuOptionHPerc + menuGapPerc) + (menuOptionHPerc - textSizes.y/screenHeight)/2), 2),
            menuFontValue, 
            BLACK);
    }
}