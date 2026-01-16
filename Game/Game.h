#pragma once
#include <raylib.h>
#include <raymath.h>

#include <functional>
#include <string>
#include <vector>
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
    
    //items

    //entities
    Player *player;
    HUD *_HUD;
    std::vector<Skeleton*> enemies;
    std::vector<Entity*> entities;
    std::vector<Object*> objects;
     
    //other values
    unsigned int FPS = 60;
    
    std::vector<std::string> welcoming_options{"Load Alias", "Start New", "Settings", "Exit"};
    std::vector<Texture2D> temp_textures;
    
    //methods
    void CheckKeysPressed();
    void OpenMenu(std::vector<std::string> options, std::vector<Image> textures, Color primaryColor, Color secondaryColor, Color inactiveColor, unsigned int orientation);
    void LoadTextures();
    void LoadMap();
};
