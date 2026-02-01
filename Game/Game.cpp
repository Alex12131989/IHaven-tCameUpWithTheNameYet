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

    LoadMap();
    LoadTextures();
    SetTargetFPS(FPS);

    while (!WindowShouldClose())
    {
        //Processing
        CheckKeysPressed();
        CheckCollisions();
        //Drawing
        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(camera);
                UpdateWorld();
                 
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
    GetTextures("Assets/grass.png", { 256, 256 }, { 32, 32 }, grassTiles);
    GetTextures("Assets/sand.png", { 256, 256 }, { 32, 32 }, sandTiles);
    Image tileTypes = LoadImage("Assets/tile types.png");
    tileColors = LoadImageColors(tileTypes);
    UnloadImage(tileTypes);
}

void Game::LoadMap()
{
    Image renderMapImage = LoadImage("Assets/test map.png");
    renderMapColors = LoadImageColors(renderMapImage);
}

void Game::UpdateWorld()
{
    for (size_t y = 0; y < renderMap.y; ++y)
    {
        for (size_t x = 0; x < renderMap.x; ++x)
        {
            if (x == 21 && y == 3)
                int i = 0; 
            Color currentTileColor = renderMapColors[x + y*static_cast<int>(renderMap.x)];
            size_t tileType = ChooseCorrectTile({ static_cast<float>(x*tile.x), static_cast<float>(y*tile.y)});
            for (char type = 0; type < tileTypes; ++type)
                if (currentTileColor == tileColors[type*tileTypes])
                {
                    if (type) DrawRectangle(x*tile.x, y*tile.y, tile.x, tile.y, tileColors[type % tileTypes]);
                    DrawRectangle(x*tile.x, y*tile.y, tile.x, tile.y, currentTileColor);
                    type = tileTypes;
                }
                else if (currentTileColor == tileColors[type*tileTypes + 1])
                {
                    if (type) DrawRectangle(x*tile.x, y*tile.y, tile.x, tile.y, tileColors[(1 - type + tileTypes) % tileTypes]);
                    DrawTexture(grassTiles[tileType], x*tile.x, y*tile.y, WHITE);
                    type = tileTypes;
                }
                else if (currentTileColor == tileColors[type*tileTypes + 2])
                {
                    if (type) DrawRectangle(x*tile.x, y*tile.y, tile.x, tile.y, tileColors[(2 - type + tileTypes) % tileTypes]);
                    DrawRectangle(x*tile.x, y*tile.y, tile.x, tile.y, currentTileColor);
                    type = tileTypes;
                }
                else if (currentTileColor == tileColors[type*tileTypes + 3])
                {
                    if (type) DrawRectangle(x*tile.x, y*tile.y, tile.x, tile.y, tileColors[(3 - type + tileTypes) % tileTypes]);
                    DrawTexture(sandTiles[tileType], x*tile.x, y*tile.y, WHITE);
                    type = tileTypes;
                }
        }
    }
}

size_t Game::ChooseCorrectTile(Vector2 targetPosition)
{
    Color currentTileColor = renderMapColors[static_cast<int>(targetPosition.x/tile.x + targetPosition.y*renderMap.y/tile.y)];
    char key;
    for (short int type = 0; type < tileTypes*tileTypes; ++type)
        if (currentTileColor == tileColors[type])
        {
            key = type % tileTypes;
            type = tileTypes*tileTypes;
        }

    char values[3][3]{ 0 };
    char valueCount = 0;
    for (float y = targetPosition.y - tile.y; y <= targetPosition.y + tile.y; y += tile.y)
        for (float x = targetPosition.x - tile.x; x <= targetPosition.x + tile.x; x += tile.x)
        {
            for (char type = 0; type < tileTypes; ++type)
                if (tileColors[key + type*tileTypes] == renderMapColors[static_cast<size_t>(x/tile.x + y/tile.y*tile.x)]
                    && x >= 0 && y >= 0 && x < renderMap.x*tile.x && y < renderMap.y*tile.y)
                {
                    values[static_cast<char>((y - targetPosition.y)/tile.y) + 1][static_cast<char>((x - targetPosition.x)/tile.x) + 1] = 1;
                    ++valueCount;
                    type = tileTypes;
                }
        }
    switch (valueCount)
    {
    //16=20, 18=21, 0=12, 2=13; 14=30, 15=31, 22=38, 23=39, 9=57
    case 1:
        return 7;
    case 2:
        if (values[0][0] || values[0][2] || values[2][0] || values[2][2])
            return 7;
        else if (values[0][1]) return 19;
        else if (values[1][0]) return 6;
        else if (values[1][2]) return 4;
        else return 3;
    case 3:
        if (values[0][0] || values[0][2] || values[2][0] || values[2][2])
        {
            if (values[0][1]) return 19;
            else if (values[1][0]) return 6;
            else if (values[1][2]) return 4;
            else return 3;
        }
        else if (values[0][1] && values[1][0]) return 44;
        else if (values[0][1] && values[1][2]) return 43;
        else if (values[0][1] && values[2][1]) return 11;
        else if (values[1][0] && values[1][2]) return 5;
        else if (values[1][0] && values[2][1]) return 31;
        else if (values[1][2] && values[2][1]) return 30;
    case 4:
        if (values[0][0] && values[0][1] && values[0][2]) return 19;//++
        else if (values[0][0] && values[0][1] && values[1][0]) return 18;//++
        else if (values[0][0] && values[0][1] && values[1][2]) return 38;//++
        else if (values[0][0] && values[0][1] && values[2][0]) return 19;//++
        else if (values[0][0] && values[0][1] && values[2][1]) return 11;//++
        else if (values[0][0] && values[0][1] && values[2][2]) return 19;//++
        else if (values[0][0] && values[0][2] && values[1][0]) return 6;//++
        else if (values[0][0] && values[0][2] && values[1][2]) return 4;//++
        else if (values[0][0] && values[0][2] && values[2][0]) return 7;//++
        else if (values[0][0] && values[0][2] && values[2][1]) return 3;//++
        else if (values[0][0] && values[0][2] && values[2][2]) return 7;//++
        else if (values[0][0] && values[1][0] && values[1][2]) return 5;//++
        else if (values[0][0] && values[1][0] && values[2][0]) return 6;//++
        else if (values[0][0] && values[1][0] && values[2][1]) return 31;//++
        else if (values[0][0] && values[1][0] && values[2][2]) return 6;//++
        else if (values[0][0] && values[1][2] && values[2][0]) return 4;//++
        else if (values[0][0] && values[1][2] && values[2][1]) return 30;//++
        else if (values[0][0] && values[1][2] && values[2][2]) return 4;//++
        else if (values[0][0] && values[2][0] && values[2][1]) return 3;//++
        else if (values[0][0] && values[2][0] && values[2][2]) return 7;//++
        else if (values[0][0] && values[2][1] && values[2][1]) return 3;//++
        else if (values[0][1] && values[0][2] && values[1][0]) return 39;//++
        else if (values[0][1] && values[0][2] && values[1][2]) return 16;//++
        else if (values[0][1] && values[0][2] && values[2][0]) return 19;//++
        else if (values[0][1] && values[0][2] && values[2][1]) return 11;//++
        else if (values[0][1] && values[0][2] && values[2][2]) return 19;//++
        else if (values[0][1] && values[1][0] && values[1][2]) return 36;//++
        else if (values[0][1] && values[1][0] && values[2][0]) return 39;//++
        else if (values[0][1] && values[1][0] && values[2][1]) return 37;//++
        else if (values[0][1] && values[1][0] && values[2][2]) return 39;//++
        else if (values[0][1] && values[1][2] && values[2][0]) return 38;//++
        else if (values[0][1] && values[1][2] && values[2][1]) return 28;//++
        else if (values[0][1] && values[1][2] && values[2][2]) return 38;//++
        else if (values[0][1] && values[2][0] && values[2][1]) return 11;//++
        else if (values[0][1] && values[2][0] && values[2][2]) return 19;//++
        else if (values[0][1] && values[2][1] && values[2][2]) return 11;//++
        else if (values[0][2] && values[1][0] && values[1][2]) return 5;//++
        else if (values[0][2] && values[1][0] && values[2][0]) return 6;//++
        else if (values[0][2] && values[1][0] && values[2][1]) return 31;//++
        else if (values[0][2] && values[1][0] && values[2][2]) return 6;//++
        else if (values[0][2] && values[1][2] && values[2][0]) return 4;//++
        else if (values[0][2] && values[1][2] && values[2][1]) return 30;//++
        else if (values[0][2] && values[1][2] && values[2][2]) return 4;//++
        else if (values[0][2] && values[2][0] && values[2][1]) return 3;//++
        else if (values[0][2] && values[2][0] && values[2][2]) return 7;//++
        else if (values[0][2] && values[2][1] && values[2][2]) return 3;//++
        else if (values[1][0] && values[1][2] && values[2][0]) return 5;//++
        else if (values[1][0] && values[1][2] && values[2][1]) return 29;//++
        else if (values[1][0] && values[1][2] && values[2][2]) return 5;//++
        else if (values[1][0] && values[2][0] && values[2][1]) return 2;//++
        else if (values[1][0] && values[2][0] && values[2][2]) return 6;//++
        else if (values[1][0] && values[2][1] && values[0][2]) return 31;//++
        else if (values[1][2] && values[2][0] && values[2][1]) return 30;//++
        else if (values[1][2] && values[2][0] && values[2][2]) return 4;//++
        else if (values[1][2] && values[2][1] && values[2][2]) return 0;//++

        else if (values[2][0] && values[2][1] && values[2][2]) return 3;//++
    case 5:
        if (values[0][0] && values[0][1] && values[0][2] && values[1][0]) return 18;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][2]) return 16;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[2][0]) return 19;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[2][1]) return 11;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[2][2]) return 19;//++
        else if (values[0][0] && values[0][1] && values[1][0] && values[1][2]) return 50;//++
        else if (values[0][0] && values[0][1] && values[1][0] && values[2][0]) return 18;//++
        else if (values[0][0] && values[0][1] && values[1][0] && values[2][1]) return 41;//++
        else if (values[0][0] && values[0][1] && values[1][0] && values[2][2]) return 18;//++
        else if (values[0][0] && values[0][1] && values[1][2] && values[2][0]) return 38;//++
        else if (values[0][0] && values[0][1] && values[1][2] && values[2][1]) return 28;//++
        else if (values[0][0] && values[0][1] && values[1][2] && values[2][2]) return 38;//++
        else if (values[0][0] && values[0][1] && values[2][0] && values[2][1]) return 11;//++
        else if (values[0][0] && values[0][1] && values[2][0] && values[2][2]) return 19;//++
        else if (values[0][0] && values[0][1] && values[2][1] && values[2][2]) return 11;//++
        else if (values[0][0] && values[0][2] && values[1][0] && values[1][2]) return 5;//++
        else if (values[0][0] && values[0][2] && values[1][0] && values[2][0]) return 6;//++
        else if (values[0][0] && values[0][2] && values[1][0] && values[2][1]) return 31;//++
        else if (values[0][0] && values[0][2] && values[1][0] && values[2][2]) return 6;//++
        else if (values[0][0] && values[0][2] && values[1][2] && values[2][0]) return 4;//++
        else if (values[0][0] && values[0][2] && values[1][2] && values[2][1]) return 30;//++
        else if (values[0][0] && values[0][2] && values[1][2] && values[2][2]) return 4;//++
        else if (values[0][0] && values[0][2] && values[2][0] && values[2][1]) return 3;//++
        else if (values[0][0] && values[0][2] && values[2][0] && values[2][2]) return 7;//++
        else if (values[0][0] && values[0][1] && values[2][1] && values[2][2]) return 3;//++
        else if (values[0][0] && values[1][0] && values[1][2] && values[2][0]) return 5;//++
        else if (values[0][0] && values[1][0] && values[1][2] && values[2][1]) return 29;//++
        else if (values[0][0] && values[1][0] && values[1][2] && values[2][2]) return 5;//++
        else if (values[0][0] && values[1][0] && values[2][0] && values[2][1]) return 2;//++
        else if (values[0][0] && values[1][0] && values[2][0] && values[2][2]) return 6;//++
        else if (values[0][0] && values[1][0] && values[2][1] && values[2][2]) return 31;//++
        else if (values[0][0] && values[1][2] && values[2][0] && values[2][1]) return 30;//++
        else if (values[0][0] && values[1][2] && values[2][0] && values[2][2]) return 4;//++
        else if (values[0][0] && values[1][2] && values[2][1] && values[2][2]) return 0;//++
        else if (values[0][0] && values[2][0] && values[2][1] && values[2][2]) return 3;//++
        else if (values[0][1] && values[0][2] && values[1][0] && values[1][2]) return 49;//++
        else if (values[0][1] && values[0][2] && values[1][0] && values[2][0]) return 39;//++
        else if (values[0][1] && values[0][2] && values[1][0] && values[2][1]) return 37;//++
        else if (values[0][1] && values[0][2] && values[1][0] && values[2][2]) return 39;//++
        else if (values[0][1] && values[0][2] && values[1][2] && values[2][0]) return 16;//++
        else if (values[0][1] && values[0][2] && values[1][2] && values[2][1]) return 42;//++
        else if (values[0][1] && values[0][2] && values[1][2] && values[2][2]) return 16;//++
        else if (values[0][1] && values[0][2] && values[2][0] && values[2][1]) return 11;//++
        else if (values[0][1] && values[0][2] && values[2][0] && values[2][2]) return 19;//++
        else if (values[0][1] && values[0][2] && values[2][1] && values[2][2]) return 19;//++
        else if (values[0][1] && values[1][0] && values[1][2] && values[2][0]) return 36;//++
        else if (values[0][1] && values[1][0] && values[1][2] && values[2][1]) return 56;//++
        else if (values[0][1] && values[1][0] && values[1][2] && values[2][2]) return 36;//++
        else if (values[0][1] && values[1][0] && values[2][0] && values[2][1]) return 51;//++
        else if (values[0][1] && values[1][0] && values[2][0] && values[2][2]) return 39;//++
        else if (values[0][1] && values[1][0] && values[2][1] && values[2][2]) return 37;//++
        else if (values[0][1] && values[1][2] && values[2][0] && values[2][1]) return 28;//++
        else if (values[0][1] && values[1][2] && values[2][0] && values[2][2]) return 38;//++
        else if (values[0][1] && values[1][2] && values[2][1] && values[2][2]) return 48;//++
        else if (values[0][1] && values[2][0] && values[2][1] && values[2][2]) return 11;//++
        else if (values[0][2] && values[1][0] && values[1][2] && values[2][0]) return 5;//++
        else if (values[0][2] && values[1][0] && values[1][2] && values[2][1]) return 29;//++
        else if (values[0][2] && values[1][0] && values[1][2] && values[2][2]) return 5;//++
        else if (values[0][2] && values[1][0] && values[2][0] && values[2][1]) return 2;//++
        else if (values[0][2] && values[1][0] && values[2][0] && values[2][2]) return 6;//++
        else if (values[0][2] && values[1][0] && values[2][1] && values[2][2]) return 31;//++
        else if (values[0][2] && values[1][2] && values[2][0] && values[2][1]) return 30;//++
        else if (values[0][2] && values[1][2] && values[2][0] && values[2][2]) return 4;//++
        else if (values[0][2] && values[1][2] && values[2][1] && values[2][2]) return 0;//++
        else if (values[0][2] && values[2][0] && values[2][1] && values[2][2]) return 3;//++
        else if (values[1][0] && values[1][2] && values[2][0] && values[2][1]) return 40;//++
        else if (values[1][0] && values[1][2] && values[2][0] && values[2][2]) return 5;//++
        else if (values[1][0] && values[1][2] && values[2][1] && values[2][2]) return 43;//++
        else if (values[1][0] && values[2][0] && values[2][1] && values[2][2]) return 2;//++
        else if (values[1][2] && values[2][0] && values[2][1] && values[2][2]) return 0;//++

    case 6:
        if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[1][2]) return 17;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[2][0]) return 18;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[2][1]) return 41;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[2][2]) return 18;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][2] && values[2][0]) return 16;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][2] && values[2][1]) return 42;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][2] && values[2][2]) return 16;
        else if (values[0][0] && values[0][1] && values[0][2] && values[2][0] && values[2][1]) return 11;
        else if (values[0][0] && values[0][1] && values[0][2] && values[2][0] && values[2][2]) return 19;
        else if (values[0][0] && values[0][1] && values[0][2] && values[2][1] && values[2][2]) return 11;
        else if (values[0][0] && values[0][1] && values[1][0] && values[1][2] && values[2][0]) return 50;
        else if (values[0][0] && values[0][1] && values[1][0] && values[1][2] && values[2][1]) return 33;
        else if (values[0][0] && values[0][1] && values[1][0] && values[1][2] && values[2][2]) return 50;
        else if (values[0][0] && values[0][1] && values[1][0] && values[2][0] && values[2][1]) return 10;
        else if (values[0][0] && values[0][1] && values[1][0] && values[2][0] && values[2][2]) return 18;
        else if (values[0][0] && values[0][1] && values[1][0] && values[2][1] && values[2][2]) return 41;
        else if (values[0][0] && values[0][1] && values[1][2] && values[2][0] && values[2][1]) return 52;
        else if (values[0][0] && values[0][1] && values[1][2] && values[2][0] && values[2][2]) return 38;
        else if (values[0][0] && values[0][1] && values[1][2] && values[2][1] && values[2][2]) return 48;
        else if (values[0][0] && values[0][1] && values[2][0] && values[2][1] && values[2][2]) return 11;
        else if (values[0][0] && values[0][2] && values[1][0] && values[1][2] && values[2][0]) return 5;
        else if (values[0][0] && values[0][2] && values[1][0] && values[1][2] && values[2][1]) return 29;
        else if (values[0][0] && values[0][2] && values[1][0] && values[1][2] && values[2][2]) return 5;
        else if (values[0][0] && values[0][2] && values[1][0] && values[2][0] && values[2][1]) return 2;
        else if (values[0][0] && values[0][2] && values[1][0] && values[2][0] && values[2][2]) return 6;
        else if (values[0][0] && values[0][2] && values[1][0] && values[2][1] && values[2][2]) return 31;
        else if (values[0][0] && values[0][2] && values[1][2] && values[2][0] && values[2][1]) return 30;
        else if (values[0][0] && values[0][2] && values[1][2] && values[2][0] && values[2][2]) return 4;
        else if (values[0][0] && values[0][2] && values[1][2] && values[2][1] && values[2][2]) return 0;
        else if (values[0][0] && values[0][2] && values[2][0] && values[2][1] && values[2][2]) return 3;
        else if (values[0][0] && values[1][0] && values[1][2] && values[2][0] && values[2][1]) return 40;
        else if (values[0][0] && values[1][0] && values[1][2] && values[2][0] && values[2][2]) return 5;
        else if (values[0][0] && values[1][0] && values[1][2] && values[2][1] && values[2][2]) return 43;
        else if (values[0][0] && values[1][0] && values[2][0] && values[2][1] && values[2][2]) return 2;
        else if (values[0][0] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) return 0;
        else if (values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][0]) return 49;
        else if (values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][1]) return 32;
        else if (values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][2]) return 49;
        else if (values[0][1] && values[0][2] && values[1][0] && values[2][0] && values[2][1]) return 51;
        else if (values[0][1] && values[0][2] && values[1][0] && values[2][0] && values[2][2]) return 39;
        else if (values[0][1] && values[0][2] && values[1][0] && values[2][1] && values[2][2]) return 37;
        else if (values[0][1] && values[0][2] && values[1][2] && values[2][0] && values[2][1]) return 42;
        else if (values[0][1] && values[0][2] && values[1][2] && values[2][0] && values[2][2]) return 16;
        else if (values[0][1] && values[0][2] && values[1][2] && values[2][1] && values[2][2]) return 8;
        else if (values[0][1] && values[0][2] && values[2][0] && values[2][1] && values[2][2]) return 11;
        else if (values[0][1] && values[1][0] && values[1][2] && values[2][0] && values[2][1]) return 25;
        else if (values[0][1] && values[1][0] && values[1][2] && values[2][0] && values[2][2]) return 36;
        else if (values[0][1] && values[1][0] && values[1][2] && values[2][1] && values[2][2]) return 24;
        else if (values[0][1] && values[1][0] && values[2][0] && values[2][1] && values[2][2]) return 51;
        else if (values[0][1] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) return 48;
        else if (values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][1]) return 40;
        else if (values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][2]) return 5;
        else if (values[0][2] && values[1][0] && values[1][2] && values[2][1] && values[2][2]) return 43;
        else if (values[0][2] && values[1][0] && values[2][0] && values[2][1] && values[2][2]) return 2;
        else if (values[0][2] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) return 0;//++
        else if (values[1][0] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) return 1;
    case 7:
        if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][0]) return 17;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][1]) return 44;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][2]) return 17;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[2][0] && values[2][1]) return 10;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[2][0] && values[2][2]) return 18;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[2][1] && values[2][2]) return 41;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][2] && values[2][0] && values[2][1]) return 42;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][2] && values[2][0] && values[2][2]) return 16;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][2] && values[2][1] && values[2][2]) return 8;
        else if (values[0][0] && values[0][1] && values[0][2] && values[2][0] && values[2][1] && values[2][2]) return 11;
        else if (values[0][0] && values[0][1] && values[1][0] && values[1][2] && values[2][0] && values[2][1]) return 52;
        else if (values[0][0] && values[0][1] && values[1][0] && values[1][2] && values[2][0] && values[2][2]) return 50;
        else if (values[0][0] && values[0][1] && values[1][0] && values[1][2] && values[2][1] && values[2][2]) return 47;
        else if (values[0][0] && values[0][1] && values[1][0] && values[2][0] && values[2][1] && values[2][2]) return 10;
        else if (values[0][0] && values[0][1] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) return 48;
        else if (values[0][0] && values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][1]) return 40;
        else if (values[0][0] && values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][2]) return 5;
        else if (values[0][0] && values[0][2] && values[1][0] && values[1][2] && values[2][1] && values[2][2]) return 43;
        else if (values[0][0] && values[0][2] && values[1][0] && values[2][0] && values[2][1] && values[2][2]) return 2;
        else if (values[0][0] && values[0][2] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) return 0;
        else if (values[0][0] && values[1][0] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) return 1;
        else if (values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][1]) return 46;
        else if (values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][2]) return 49;
        else if (values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][1] && values[2][2]) return 45;
        else if (values[0][1] && values[0][2] && values[1][0] && values[2][0] && values[2][1] && values[2][2]) return 51;
        else if (values[0][1] && values[0][2] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) return 8;
        else if (values[0][1] && values[1][0] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) return 53;
        else if (values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) return 1;
    case 8:
         
        if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][1]) return 26;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][2]) return 17;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][1] && values[2][2]) return 27;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[2][0] && values[2][1] && values[2][2]) return 10;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) return 8;//++
        else if (values[0][0] && values[0][1] && values[1][0] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) return 34;//++
        else if (values[0][0] && values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) return 1;//++
        else if (values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) return 35;//++
    default:
        return 9;
    }
}
