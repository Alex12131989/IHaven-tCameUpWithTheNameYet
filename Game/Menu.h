#pragma once
#include <raylib.h>
#include <vector>
#include <string>

class Menu
{
public:
    Menu(Vector2 screen, std::vector<std::string> options, std::vector<Image> textures, Color primaryColor, Color secondaryColor, Color inactiveColor, unsigned int orientation);

private:
    //sizes
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
    unsigned int menuFontValue = 20;
    unsigned int currentSelection = 0;
    Color menuBackgroundColor = BLACK;
};