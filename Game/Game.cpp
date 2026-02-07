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
    wholePhysicalMapImage = LoadImage("PhysicalMap.png");
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
    for (int y = 0; y < renderMap.y; ++y)
        for (int x = 0; x < renderMap.x; ++x)
        {
            DrawTile({ y*tile.x, x*tile.y }, BACKGROUND);
            DrawTile({ y*tile.x, x*tile.y }, FOREGROUND);
        }
}

std::array<char, 2> Game::ChooseCorrectTile(Vector2 targetPosition, char focus)
{
    Color currentTileColor = renderMapColors[static_cast<int>(targetPosition.x/tile.x + targetPosition.y*renderMap.y/tile.y)];
    char key = 0;
    for (short int type = 0; type < tileTypes*tileTypes; ++type)
        if (currentTileColor == tileColors[type])
        {
            if (focus == FOREGROUND) key = type % tileTypes;
            else key = type % tileTypes - Int(type/tileTypes, FLOOR);
            if (key < 0) key += tileTypes;

            if (type < tileTypes && focus == BACKGROUND)
                return std::array<char, 2>{-1, -1};
            type = tileTypes*tileTypes;
        }

    char values[3][3]{ 0 };
    char valueCount = 0;
    for (float y = targetPosition.y - tile.y; y <= targetPosition.y + tile.y; y += tile.y)
        for (float x = targetPosition.x - tile.x; x <= targetPosition.x + tile.x; x += tile.x)
        {
            short int currentDiagonalIndex = key;
            for (char type = 0; type < tileTypes; ++type)
            {
                if ((tileTypes - 1 - key == type) && (key != 0))
                    currentDiagonalIndex += 1;
                else
                    currentDiagonalIndex += tileTypes + 1;

                if (currentDiagonalIndex >= tileTypes*tileTypes)
                    currentDiagonalIndex = key;

                if (x >= 0 && y >= 0 && x < renderMap.x*tile.x && y < renderMap.y*tile.y)
                    if ((tileColors[key + type*tileTypes] == renderMapColors[static_cast<int>(x/tile.x + y/tile.y*tile.x)]) ||
                        (tileColors[currentDiagonalIndex] == renderMapColors[static_cast<int>(x/tile.x + y/tile.y*tile.x)]))
                    {
                        values[static_cast<char>((y - targetPosition.y)/tile.y) + 1][static_cast<char>((x - targetPosition.x)/tile.x) + 1] = 1;
                        ++valueCount;
                        type = tileTypes;
                    }

            }
        }
    std::array<char, 2> returnArray{};
    switch (valueCount)
    {
    //16=20, 18=21, 0=12, 2=13; 14=30, 15=31, 22=38, 23=39, 9=57
    case 1:
        returnArray[0] = 7;
        break;
    case 2:
        if (values[0][0] || values[0][2] || values[2][0] || values[2][2])
            returnArray[0] = 7;
        else if (values[0][1]) returnArray[0] = 19;
        else if (values[1][0]) returnArray[0] = 6;
        else if (values[1][2]) returnArray[0] = 4;
        else returnArray[0] = 3;
        break;
    case 3:
        if (values[0][0] || values[0][2] || values[2][0] || values[2][2])
        {
            if (values[0][1]) returnArray[0] = 19;
            else if (values[1][0]) returnArray[0] = 6;
            else if (values[1][2]) returnArray[0] = 4;
            else returnArray[0] = 3;
        }
        else if (values[0][1] && values[1][0]) returnArray[0] = 44;
        else if (values[0][1] && values[1][2]) returnArray[0] = 43;
        else if (values[0][1] && values[2][1]) returnArray[0] = 11;
        else if (values[1][0] && values[1][2]) returnArray[0] = 5;
        else if (values[1][0] && values[2][1]) returnArray[0] = 31;
        else if (values[1][2] && values[2][1]) returnArray[0] = 30;
        break;
    case 4:
        if (values[0][0] && values[0][1] && values[0][2]) returnArray[0] = 19;//++
        else if (values[0][0] && values[0][1] && values[1][0]) returnArray[0] = 18;//++
        else if (values[0][0] && values[0][1] && values[2][0]) returnArray[0] = 19;//++
        else if (values[0][0] && values[0][1] && values[2][1]) returnArray[0] = 11;//++
        else if (values[0][0] && values[0][1] && values[2][2]) returnArray[0] = 19;//++
        else if (values[0][0] && values[0][2] && values[1][0]) returnArray[0] = 6;//++
        else if (values[0][0] && values[0][2] && values[1][2]) returnArray[0] = 4;//++
        else if (values[0][0] && values[0][2] && values[2][0]) returnArray[0] = 7;//++
        else if (values[0][0] && values[0][2] && values[2][1]) returnArray[0] = 3;//++
        else if (values[0][0] && values[0][2] && values[2][2]) returnArray[0] = 7;//++
        else if (values[0][0] && values[1][0] && values[1][2]) returnArray[0] = 5;//++
        else if (values[0][0] && values[1][0] && values[2][0]) returnArray[0] = 6;//++
        else if (values[0][0] && values[1][0] && values[2][1]) returnArray[0] = 31;//++
        else if (values[0][0] && values[1][0] && values[2][2]) returnArray[0] = 6;//++
        else if (values[0][0] && values[1][2] && values[2][0]) returnArray[0] = 4;//++
        else if (values[0][0] && values[1][2] && values[2][1]) returnArray[0] = 30;//++
        else if (values[0][0] && values[1][2] && values[2][2]) returnArray[0] = 4;//++
        else if (values[0][0] && values[2][0] && values[2][1]) returnArray[0] = 3;//++
        else if (values[0][0] && values[2][0] && values[2][2]) returnArray[0] = 7;//++
        else if (values[0][0] && values[2][1] && values[2][1]) returnArray[0] = 3;//++
        else if (values[0][1] && values[0][2] && values[1][0]) returnArray[0] = 39;//++
        else if (values[0][1] && values[0][2] && values[1][2]) returnArray[0] = 16;//++
        else if (values[0][1] && values[0][2] && values[2][0]) returnArray[0] = 19;//++
        else if (values[0][1] && values[0][2] && values[2][1]) returnArray[0] = 11;//++
        else if (values[0][1] && values[0][2] && values[2][2]) returnArray[0] = 19;//++
        else if (values[0][1] && values[1][0] && values[1][2]) returnArray[0] = 36;//++
        else if (values[0][1] && values[1][0] && values[2][0]) returnArray[0] = 39;//++
        else if (values[0][1] && values[1][0] && values[2][1]) returnArray[0] = 37;//++
        else if (values[0][1] && values[1][0] && values[2][2]) returnArray[0] = 39;//++
        else if (values[0][1] && values[1][2] && values[2][0]) returnArray[0] = 38;//++
        else if (values[0][1] && values[1][2] && values[2][1]) returnArray[0] = 28;//++
        else if (values[0][1] && values[1][2] && values[2][2]) returnArray[0] = 38;//++
        else if (values[0][1] && values[2][0] && values[2][1]) returnArray[0] = 11;//++
        else if (values[0][1] && values[2][0] && values[2][2]) returnArray[0] = 19;//++
        else if (values[0][1] && values[2][1] && values[2][2]) returnArray[0] = 11;//++
        else if (values[0][2] && values[1][0] && values[1][2]) returnArray[0] = 5;//++
        else if (values[0][2] && values[1][0] && values[2][0]) returnArray[0] = 6;//++
        else if (values[0][2] && values[1][0] && values[2][1]) returnArray[0] = 31;//++
        else if (values[0][2] && values[1][0] && values[2][2]) returnArray[0] = 6;//++
        else if (values[0][2] && values[1][2] && values[2][0]) returnArray[0] = 4;//++
        else if (values[0][2] && values[1][2] && values[2][1]) returnArray[0] = 30;//++
        else if (values[0][2] && values[1][2] && values[2][2]) returnArray[0] = 4;//++
        else if (values[0][2] && values[2][0] && values[2][1]) returnArray[0] = 3;//++
        else if (values[0][2] && values[2][0] && values[2][2]) returnArray[0] = 7;//++
        else if (values[0][2] && values[2][1] && values[2][2]) returnArray[0] = 3;//++
        else if (values[1][0] && values[1][2] && values[2][0]) returnArray[0] = 5;//++
        else if (values[1][0] && values[1][2] && values[2][1]) returnArray[0] = 29;//++
        else if (values[1][0] && values[1][2] && values[2][2]) returnArray[0] = 5;//++
        else if (values[1][0] && values[2][0] && values[2][1]) returnArray[0] = 2;//++
        else if (values[1][0] && values[2][0] && values[2][2]) returnArray[0] = 6;//++
        else if (values[1][0] && values[2][1] && values[0][2]) returnArray[0] = 31;//++
        else if (values[1][2] && values[2][0] && values[2][1]) returnArray[0] = 30;//++
        else if (values[1][2] && values[2][0] && values[2][2]) returnArray[0] = 4;//++
        else if (values[1][2] && values[2][1] && values[2][2]) returnArray[0] = 0;//++
        else if (values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 3;//++
        break;
    case 5:
        if (values[0][0] && values[0][1] && values[0][2] && values[1][0]) returnArray[0] = 18;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][2]) returnArray[0] = 16;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[2][0]) returnArray[0] = 19;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[2][1]) returnArray[0] = 11;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[2][2]) returnArray[0] = 19;//++
        else if (values[0][0] && values[0][1] && values[1][0] && values[1][2]) returnArray[0] = 50;//++
        else if (values[0][0] && values[0][1] && values[1][0] && values[2][0]) returnArray[0] = 18;//++
        else if (values[0][0] && values[0][1] && values[1][0] && values[2][1]) returnArray[0] = 41;//++
        else if (values[0][0] && values[0][1] && values[1][0] && values[2][2]) returnArray[0] = 18;//++
        else if (values[0][0] && values[0][1] && values[1][2] && values[2][0]) returnArray[0] = 38;//++
        else if (values[0][0] && values[0][1] && values[1][2] && values[2][1]) returnArray[0] = 28;//++
        else if (values[0][0] && values[0][1] && values[1][2] && values[2][2]) returnArray[0] = 38;//++
        else if (values[0][0] && values[0][1] && values[2][0] && values[2][1]) returnArray[0] = 11;//++
        else if (values[0][0] && values[0][1] && values[2][0] && values[2][2]) returnArray[0] = 19;//++
        else if (values[0][0] && values[0][1] && values[2][1] && values[2][2]) returnArray[0] = 11;//++
        else if (values[0][0] && values[0][2] && values[1][0] && values[1][2]) returnArray[0] = 5;//++
        else if (values[0][0] && values[0][2] && values[1][0] && values[2][0]) returnArray[0] = 6;//++
        else if (values[0][0] && values[0][2] && values[1][0] && values[2][1]) returnArray[0] = 31;//++
        else if (values[0][0] && values[0][2] && values[1][0] && values[2][2]) returnArray[0] = 6;//++
        else if (values[0][0] && values[0][2] && values[1][2] && values[2][0]) returnArray[0] = 4;//++
        else if (values[0][0] && values[0][2] && values[1][2] && values[2][1]) returnArray[0] = 30;//++
        else if (values[0][0] && values[0][2] && values[1][2] && values[2][2]) returnArray[0] = 4;//++
        else if (values[0][0] && values[0][2] && values[2][0] && values[2][1]) returnArray[0] = 3;//++
        else if (values[0][0] && values[0][2] && values[2][0] && values[2][2]) returnArray[0] = 7;//++
        else if (values[0][0] && values[0][1] && values[2][1] && values[2][2]) returnArray[0] = 3;//++
        else if (values[0][0] && values[1][0] && values[1][2] && values[2][0]) returnArray[0] = 5;//++
        else if (values[0][0] && values[1][0] && values[1][2] && values[2][1]) returnArray[0] = 29;//++
        else if (values[0][0] && values[1][0] && values[1][2] && values[2][2]) returnArray[0] = 5;//++
        else if (values[0][0] && values[1][0] && values[2][0] && values[2][1]) returnArray[0] = 2;//++
        else if (values[0][0] && values[1][0] && values[2][0] && values[2][2]) returnArray[0] = 6;//++
        else if (values[0][0] && values[1][0] && values[2][1] && values[2][2]) returnArray[0] = 31;//++
        else if (values[0][0] && values[1][2] && values[2][0] && values[2][1]) returnArray[0] = 30;//++
        else if (values[0][0] && values[1][2] && values[2][0] && values[2][2]) returnArray[0] = 4;//++
        else if (values[0][0] && values[1][2] && values[2][1] && values[2][2]) returnArray[0] = 0;//++
        else if (values[0][0] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 3;//++
        else if (values[0][1] && values[0][2] && values[1][0] && values[1][2]) returnArray[0] = 49;//++
        else if (values[0][1] && values[0][2] && values[1][0] && values[2][0]) returnArray[0] = 39;//++
        else if (values[0][1] && values[0][2] && values[1][0] && values[2][1]) returnArray[0] = 37;//++
        else if (values[0][1] && values[0][2] && values[1][0] && values[2][2]) returnArray[0] = 39;//++
        else if (values[0][1] && values[0][2] && values[1][2] && values[2][0]) returnArray[0] = 16;//++
        else if (values[0][1] && values[0][2] && values[1][2] && values[2][1]) returnArray[0] = 42;//++
        else if (values[0][1] && values[0][2] && values[1][2] && values[2][2]) returnArray[0] = 16;//++
        else if (values[0][1] && values[0][2] && values[2][0] && values[2][1]) returnArray[0] = 11;//++
        else if (values[0][1] && values[0][2] && values[2][0] && values[2][2]) returnArray[0] = 19;//++
        else if (values[0][1] && values[0][2] && values[2][1] && values[2][2]) returnArray[0] = 19;//++
        else if (values[0][1] && values[1][0] && values[1][2] && values[2][0]) returnArray[0] = 36;//++
        else if (values[0][1] && values[1][0] && values[1][2] && values[2][1]) returnArray[0] = 56;//++
        else if (values[0][1] && values[1][0] && values[1][2] && values[2][2]) returnArray[0] = 36;//++
        else if (values[0][1] && values[1][0] && values[2][0] && values[2][1]) returnArray[0] = 51;//++
        else if (values[0][1] && values[1][0] && values[2][0] && values[2][2]) returnArray[0] = 39;//++
        else if (values[0][1] && values[1][0] && values[2][1] && values[2][2]) returnArray[0] = 37;//++
        else if (values[0][1] && values[1][2] && values[2][0] && values[2][1]) returnArray[0] = 28;//++
        else if (values[0][1] && values[1][2] && values[2][0] && values[2][2]) returnArray[0] = 38;//++
        else if (values[0][1] && values[1][2] && values[2][1] && values[2][2]) returnArray[0] = 48;//++
        else if (values[0][1] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 11;//++
        else if (values[0][2] && values[1][0] && values[1][2] && values[2][0]) returnArray[0] = 5;//++
        else if (values[0][2] && values[1][0] && values[1][2] && values[2][1]) returnArray[0] = 29;//++
        else if (values[0][2] && values[1][0] && values[1][2] && values[2][2]) returnArray[0] = 5;//++
        else if (values[0][2] && values[1][0] && values[2][0] && values[2][1]) returnArray[0] = 2;//++
        else if (values[0][2] && values[1][0] && values[2][0] && values[2][2]) returnArray[0] = 6;//++
        else if (values[0][2] && values[1][0] && values[2][1] && values[2][2]) returnArray[0] = 31;//++
        else if (values[0][2] && values[1][2] && values[2][0] && values[2][1]) returnArray[0] = 30;//++
        else if (values[0][2] && values[1][2] && values[2][0] && values[2][2]) returnArray[0] = 4;//++
        else if (values[0][2] && values[1][2] && values[2][1] && values[2][2]) returnArray[0] = 0;//++
        else if (values[0][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 3;//++
        else if (values[1][0] && values[1][2] && values[2][0] && values[2][1]) returnArray[0] = 40;//++
        else if (values[1][0] && values[1][2] && values[2][0] && values[2][2]) returnArray[0] = 5;//++
        else if (values[1][0] && values[1][2] && values[2][1] && values[2][2]) returnArray[0] = 43;//++
        else if (values[1][0] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 2;//++
        else if (values[1][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 0;//++
        break;
    case 6:
        if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[1][2]) returnArray[0] = 17;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[2][0]) returnArray[0] = 18;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[2][1]) returnArray[0] = 41;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[2][2]) returnArray[0] = 18;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][2] && values[2][0]) returnArray[0] = 16;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][2] && values[2][1]) returnArray[0] = 42;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][2] && values[2][2]) returnArray[0] = 16;
        else if (values[0][0] && values[0][1] && values[0][2] && values[2][0] && values[2][1]) returnArray[0] = 11;
        else if (values[0][0] && values[0][1] && values[0][2] && values[2][0] && values[2][2]) returnArray[0] = 19;
        else if (values[0][0] && values[0][1] && values[0][2] && values[2][1] && values[2][2]) returnArray[0] = 11;
        else if (values[0][0] && values[0][1] && values[1][0] && values[1][2] && values[2][0]) returnArray[0] = 50;
        else if (values[0][0] && values[0][1] && values[1][0] && values[1][2] && values[2][1]) returnArray[0] = 33;
        else if (values[0][0] && values[0][1] && values[1][0] && values[1][2] && values[2][2]) returnArray[0] = 50;
        else if (values[0][0] && values[0][1] && values[1][0] && values[2][0] && values[2][1]) returnArray[0] = 10;
        else if (values[0][0] && values[0][1] && values[1][0] && values[2][0] && values[2][2]) returnArray[0] = 18;
        else if (values[0][0] && values[0][1] && values[1][0] && values[2][1] && values[2][2]) returnArray[0] = 41;
        else if (values[0][0] && values[0][1] && values[1][2] && values[2][0] && values[2][1]) returnArray[0] = 52;
        else if (values[0][0] && values[0][1] && values[1][2] && values[2][0] && values[2][2]) returnArray[0] = 38;
        else if (values[0][0] && values[0][1] && values[1][2] && values[2][1] && values[2][2]) returnArray[0] = 48;
        else if (values[0][0] && values[0][1] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 11;
        else if (values[0][0] && values[0][2] && values[1][0] && values[1][2] && values[2][0]) returnArray[0] = 5;
        else if (values[0][0] && values[0][2] && values[1][0] && values[1][2] && values[2][1]) returnArray[0] = 29;
        else if (values[0][0] && values[0][2] && values[1][0] && values[1][2] && values[2][2]) returnArray[0] = 5;
        else if (values[0][0] && values[0][2] && values[1][0] && values[2][0] && values[2][1]) returnArray[0] = 2;
        else if (values[0][0] && values[0][2] && values[1][0] && values[2][0] && values[2][2]) returnArray[0] = 6;
        else if (values[0][0] && values[0][2] && values[1][0] && values[2][1] && values[2][2]) returnArray[0] = 31;
        else if (values[0][0] && values[0][2] && values[1][2] && values[2][0] && values[2][1]) returnArray[0] = 30;
        else if (values[0][0] && values[0][2] && values[1][2] && values[2][0] && values[2][2]) returnArray[0] = 4;
        else if (values[0][0] && values[0][2] && values[1][2] && values[2][1] && values[2][2]) returnArray[0] = 0;
        else if (values[0][0] && values[0][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 3;
        else if (values[0][0] && values[1][0] && values[1][2] && values[2][0] && values[2][1]) returnArray[0] = 40;
        else if (values[0][0] && values[1][0] && values[1][2] && values[2][0] && values[2][2]) returnArray[0] = 5;
        else if (values[0][0] && values[1][0] && values[1][2] && values[2][1] && values[2][2]) returnArray[0] = 43;
        else if (values[0][0] && values[1][0] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 2;
        else if (values[0][0] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 0;
        else if (values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][0]) returnArray[0] = 49;
        else if (values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][1]) returnArray[0] = 32;
        else if (values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][2]) returnArray[0] = 49;
        else if (values[0][1] && values[0][2] && values[1][0] && values[2][0] && values[2][1]) returnArray[0] = 51;
        else if (values[0][1] && values[0][2] && values[1][0] && values[2][0] && values[2][2]) returnArray[0] = 39;
        else if (values[0][1] && values[0][2] && values[1][0] && values[2][1] && values[2][2]) returnArray[0] = 37;
        else if (values[0][1] && values[0][2] && values[1][2] && values[2][0] && values[2][1]) returnArray[0] = 42;
        else if (values[0][1] && values[0][2] && values[1][2] && values[2][0] && values[2][2]) returnArray[0] = 16;
        else if (values[0][1] && values[0][2] && values[1][2] && values[2][1] && values[2][2]) returnArray[0] = 8;
        else if (values[0][1] && values[0][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 11;
        else if (values[0][1] && values[1][0] && values[1][2] && values[2][0] && values[2][1]) returnArray[0] = 25;
        else if (values[0][1] && values[1][0] && values[1][2] && values[2][0] && values[2][2]) returnArray[0] = 36;
        else if (values[0][1] && values[1][0] && values[1][2] && values[2][1] && values[2][2]) returnArray[0] = 24;
        else if (values[0][1] && values[1][0] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 51;
        else if (values[0][1] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 48;
        else if (values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][1]) returnArray[0] = 40;
        else if (values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][2]) returnArray[0] = 5;
        else if (values[0][2] && values[1][0] && values[1][2] && values[2][1] && values[2][2]) returnArray[0] = 43;
        else if (values[0][2] && values[1][0] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 2;
        else if (values[0][2] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 0;//++
        else if (values[1][0] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 1;
        break;
    case 7:
        if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][0]) returnArray[0] = 17;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][1]) returnArray[0] = 44;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][2]) returnArray[0] = 17;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[2][0] && values[2][1]) returnArray[0] = 10;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[2][0] && values[2][2]) returnArray[0] = 18;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[2][1] && values[2][2]) returnArray[0] = 41;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][2] && values[2][0] && values[2][1]) returnArray[0] = 42;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][2] && values[2][0] && values[2][2]) returnArray[0] = 16;
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][2] && values[2][1] && values[2][2]) returnArray[0] = 8;
        else if (values[0][0] && values[0][1] && values[0][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 11;
        else if (values[0][0] && values[0][1] && values[1][0] && values[1][2] && values[2][0] && values[2][1]) returnArray[0] = 52;
        else if (values[0][0] && values[0][1] && values[1][0] && values[1][2] && values[2][0] && values[2][2]) returnArray[0] = 50;
        else if (values[0][0] && values[0][1] && values[1][0] && values[1][2] && values[2][1] && values[2][2]) returnArray[0] = 47;
        else if (values[0][0] && values[0][1] && values[1][0] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 10;
        else if (values[0][0] && values[0][1] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 48;
        else if (values[0][0] && values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][1]) returnArray[0] = 40;
        else if (values[0][0] && values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][2]) returnArray[0] = 5;
        else if (values[0][0] && values[0][2] && values[1][0] && values[1][2] && values[2][1] && values[2][2]) returnArray[0] = 43;
        else if (values[0][0] && values[0][2] && values[1][0] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 2;
        else if (values[0][0] && values[0][2] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 0;
        else if (values[0][0] && values[1][0] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 1;
        else if (values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][1]) returnArray[0] = 46;
        else if (values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][2]) returnArray[0] = 49;
        else if (values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][1] && values[2][2]) returnArray[0] = 45;
        else if (values[0][1] && values[0][2] && values[1][0] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 51;
        else if (values[0][1] && values[0][2] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 8;
        else if (values[0][1] && values[1][0] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 53;
        else if (values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 1;
        break;
    case 8:
        if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][1]) returnArray[0] = 26;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][2]) returnArray[0] = 17;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][1] && values[2][2]) returnArray[0] = 27;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][0] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 10;//++
        else if (values[0][0] && values[0][1] && values[0][2] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 8;//++
        else if (values[0][0] && values[0][1] && values[1][0] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 34;//++
        else if (values[0][0] && values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 1;//++
        else if (values[0][1] && values[0][2] && values[1][0] && values[1][2] && values[2][0] && values[2][1] && values[2][2]) returnArray[0] = 35;//++
        break;
    default:
        returnArray[0] = 9;
        break;
    }
    returnArray[1] = key;
    return returnArray;
}

void Game::DrawTile(Vector2 targetPosition, char focus)
{
    if (targetPosition.x == 0 && targetPosition.y == 352)
        int stop_point = 0;
    std::array<char, 2> tileInfo;
    tileInfo = ChooseCorrectTile({ targetPosition.x, targetPosition.y }, focus);
    if (tileInfo[0] == -1 || tileInfo[1] == -1) return;
    switch (tileInfo[1])
    {
    case 0:
        DrawRectangleRec({ targetPosition.x, targetPosition.y, tile.x, tile.y }, BROWN);
        break;
    case 1:
        DrawTexture(grassTiles[tileInfo[0]], Int(targetPosition.x), Int(targetPosition.y), WHITE);
        break;
    case 2:
        DrawRectangleRec({ targetPosition.x, targetPosition.y, tile.x, tile.y }, BLUE);
        break;
    case 3:
        DrawTexture(sandTiles[tileInfo[0]], Int(targetPosition.x), Int(targetPosition.y), WHITE);
        break;
    default:
        throw new std::invalid_argument("Tile type doesn't exist");
    }

}
