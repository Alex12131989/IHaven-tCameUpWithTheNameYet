#pragma once
#include <raylib.h>
#include <raymath.h>

#include <functional>
#include <string>
#include <array>
#include <cmath>

#include "KeysBound.h"
#include "Player.h"
#include "Skeleton.h"
#include "Object.h"
#include "HUD.h"

class Game
{
public:
    Game();
    bool MainLoop();
    
private:
    //methods
    void CheckKeysPressed();
    void CheckCollisions();
    void OpenMenu(std::vector<std::string> options, std::vector<Image> textures, Color primaryColor, Color secondaryColor, Color inactiveColor, unsigned int orientation);
    void LoadTextures();
    void LoadMap();
    void UpdateWorld();
    std::array<char, 2> ChooseCorrectTile(Vector2 targetPosition, char focus);//orientation_type, material_type
    void DrawTile(Vector2 targetPosition, char focus);

    //sizes
    Vector2 screen = { 640, 360 };
    Vector2 renderMap = { 32, 32 };
    Vector2 tile = { 32, 32 };

    //other values
    Camera2D camera = { 0 };
    unsigned int FPS = 60;
    unsigned int entitiesRedering = 0;
    static const unsigned int maxTrivialEnemies = 89;
    static const unsigned int maxSpecialEntities = 36;
    static const unsigned int maxRenderingObjects = 87;
    
    //items

    //entities
    Player *player;
    HUD *_HUD;
    std::array<Skeleton*, maxTrivialEnemies> enemies;
    std::array<Entity*, maxTrivialEnemies + maxSpecialEntities + 1> entities;
    std::array<Object*, maxRenderingObjects> objects;
     
    //arrays
    std::array<std::string, 4> welcoming_options{"Load Alias", "Start New", "Settings", "Exit"};
    
    //assets
    Image wholePhysicalMapImage;

    //colors
    Color* colorToTexture;
    Color* renderMapColors;

    //textures
    char tileTypes = 4;
    //0: dirt, 1: grass, 2: water, 3: sand
    Color* tileColors;

    Texture2D wholePhysicalMap;
    std::array<Texture2D, TILEMAP_SIZE> grassTiles;
    std::array<Texture2D, TILEMAP_SIZE> sandTiles;
};
