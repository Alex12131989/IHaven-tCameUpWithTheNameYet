#pragma once
#include <raylib.h>
#include <raymath.h>

#include <functional>
#include <string>
#include <vector>

#include "CONSTANTS.h"
#include "GenericFunctions.h"

class Game
{
public:
    bool MainLoop();
    
private:
    //sizes
    unsigned int screenWidth = 640;
    unsigned int screenHeight = 360;
    static constexpr float VmenuGapPerc = 0.05f;
    static constexpr float VmenuOptionWPerc = 0.60f;
    static constexpr float VmenuOptionHPerc = 0.10f;
    static constexpr float HmenuGapPerc = 0.15f;
    static constexpr float HmenuWidth = 0.30f;
    static constexpr float HmenuHeight = 0.50f;
    static constexpr float HarrowWidth = 0.05f;
    static constexpr float HarrowHeight = 0.10f;
    static constexpr float HarrowMargin = 0.05f;
    static constexpr unsigned int HarrowThickness = 5;
    
    //other values
    unsigned int FPS = 60;
    unsigned int menuFontValue = 20;
    unsigned int currentSelection = 0;
    Color menuBackgroundColor = BLACK;
    
    std::vector<std::string> welcoming_options{"Load Alias", "Start New", "Settings", "Exit"};
    std::vector<Texture2D> temp_textures;
    
    //methods
    void Menu(std::vector<std::string> options, std::vector<Image> textures, Color primaryColor, Color secondaryColor, Color inactiveColor, unsigned int orientation);
    void DrawArrow(Vector2 topLeft, Vector2 sizes, unsigned int thickness, Color color, unsigned int orientation);
    void LoadTextures();
};
