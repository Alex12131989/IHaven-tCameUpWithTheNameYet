#include "Game.h"

Game::Game()
{
    player = new Player(0);
    _HUD = new HUD(player, screen);
    entities.push_back(player);
    for (size_t i = 0; i < 5; ++i)
    {
        auto enemy = new Skeleton(13);
        enemies.push_back(enemy);
        entities.push_back(enemy);
    }
}

bool Game::MainLoop()
{
    InitWindow(Int(screen.x, FLOOR), Int(screen.y, FLOOR), "Title");
    player->Spawn({400,100});
    for(size_t i = 0; i < enemies.size(); ++i)
        enemies[i]->Spawn({(i + 1)*100.0f,200});

    LoadTextures();
    SetTargetFPS(FPS);

    while (!WindowShouldClose())
    {
        CheckKeysPressed();
        //Drawing
        BeginDrawing();
        ClearBackground(GRAY);
        
        for (auto entity : entities)
            entity->Draw();
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





    if (changedPlayerSpeed) player->ChangeSpeed(static_cast<float>(sqrt(2)));
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