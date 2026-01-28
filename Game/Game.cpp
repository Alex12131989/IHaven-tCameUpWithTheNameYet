#include "Game.h"
//#include <iostream>

Game::Game()
{
    player = new Player(0);
    player->Spawn({400,100});
    camera.target = player->GetBodyCenter();

    camera.offset = { screen.x/2.0f, screen.y/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    _HUD = new HUD(player, screen);
    entities[0] = player;
    ++entitiesRedering;
    for (size_t i = 1; i <= 5; ++i)
    {
        auto enemy = new Skeleton(13);
        enemy->Spawn({(i + 1)*100.0f,200});
        enemies[i] = enemy;
        entities[i] = enemy;
        ++entitiesRedering;
    }
}

bool Game::MainLoop()
{
    InitWindow(Int(screen.x, FLOOR), Int(screen.y, FLOOR), "Title");

    LoadTextures();
    SetTargetFPS(FPS);

    while (!WindowShouldClose())
    {
        //Processing
        CheckKeysPressed();
        CheckCollisions();
        //Drawing
        BeginDrawing();
            ClearBackground(GRAY);
            BeginMode2D(camera);
                for (size_t i = 0; i < entitiesRedering; ++i)
                    entities[i]->Draw();
            EndMode2D();
            _HUD->Display();
        EndDrawing();
    }
    CloseWindow();
    return true;
}

void Game::CheckKeysPressed()
{
    bool changedPlayerSpeed = false;
    if ((IsKeyDown(UP) || IsKeyDown(UP2) || IsKeyDown(DOWN) || IsKeyDown(DOWN2)) && (IsKeyDown(LEFT) || IsKeyDown(LEFT2) || IsKeyDown(RIGHT) || IsKeyDown(RIGHT2)))
    {
        changedPlayerSpeed = true;
        player->ChangeSpeed(static_cast<float>(1/sqrt(2)));
    }
    if (IsKeyDown(UP) || IsKeyDown(UP2)) player->Move(UPWARD);
    else if (IsKeyDown(DOWN) || IsKeyDown(DOWN2)) player->Move(DOWNWARD);
    if (IsKeyDown(LEFT) || IsKeyDown(LEFT2)) player->Move(LEFTBOUND);
    else if (IsKeyDown(RIGHT) || IsKeyDown(RIGHT2)) player->Move(RIGHTBOUND);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) player->Attack();

    camera.target = player->GetBodyCenter();
    


    if (changedPlayerSpeed) player->ChangeSpeed(static_cast<float>(sqrt(2)));
}

void Game::CheckCollisions()
{
    //if (player)
}

void Game::OpenMenu(std::vector<std::string> options, std::vector<Image> textures, Color primaryColor, Color secondaryColor, Color inactiveColor, unsigned int orientation)
{
    
}

void Game::LoadTextures()
{
    //temp_textures = GetTextures("numbers.png", { 512, 512 }, { 256, 256 });
}

void Game::LoadMap()
{

}
