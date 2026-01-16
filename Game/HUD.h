#pragma once
#include <raylib.h>

#include "CONSTANTS.h"
#include "GenericFunctions.h"
#include "Player.h"

class HUD
{
public:
    HUD(Player* player, Vector2 screen);
    void Display() const;
    void Hide();
private:
    //information
    Vector2 screen;
    unsigned int hp;
    unsigned int maxHp;
    unsigned int stamina;
    unsigned int maxStamina;
    unsigned int damagePoints;
    char level;
    unsigned int experiencePoints;
    unsigned int rankExperiencePoint;
    char faction;
    float speed;
    bool idle;
    bool alive;

    //sizes
    Vector2 healthBar = { 0.10f, 0.03f };
    Vector2 healthGap = { 0.03f, 0.03f };

};