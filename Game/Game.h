#pragma once
#include <raylib.h>
#include <raymath.h>

#include <functional>
#include <string>
#include <array>
#include <cmath>

#include "CONSTANTS.h"
#include "KeysBound.h"
#include "GenericFunctions.h"
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
    //size
    Vector2 screen = { 640, 360 };

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
     
    
    std::array<std::string, 4> welcoming_options{"Load Alias", "Start New", "Settings", "Exit"};
    //std::array<Texture2D, 1> temp_textures;
    
    //methods
    void CheckKeysPressed();
    void CheckCollisions();
    void OpenMenu(std::vector<std::string> options, std::vector<Image> textures, Color primaryColor, Color secondaryColor, Color inactiveColor, unsigned int orientation);
    void LoadTextures();
    void LoadMap();
};
