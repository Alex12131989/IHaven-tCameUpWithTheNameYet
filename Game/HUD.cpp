#include "HUD.h"

HUD::HUD(Player* player, Vector2 screen)
{
    this->screen = screen;
    hp = player->Health();
    maxHp = player->MaxHealth();
    stamina = player->Stamina();
    maxStamina = player->MaxStamina();
}

void HUD::Display() const
{
    //health bar
    DrawRectangle(Int(screen.x*healthGap.x, FLOOR), Int(screen.y*healthGap.y, FLOOR), Int(screen.x*healthBar.x, FLOOR), Int(screen.y*healthBar.y, FLOOR), DARKBLUE);
    DrawRectangle(Int(screen.x*healthGap.x, FLOOR) + 1, Int(screen.y*healthGap.y, FLOOR) + 1, Int(screen.x*healthBar.x*(hp/maxHp), FLOOR) - 2, Int(screen.y*healthBar.y, FLOOR) - 2, RED);
    //stamina bar
    DrawRectangle(Int(screen.x*healthGap.x, FLOOR), Int(screen.y*(1.3*healthGap.y + healthBar.y), FLOOR), Int(screen.x*healthBar.x, FLOOR), Int(screen.y*healthBar.y, FLOOR), DARKBLUE);
    DrawRectangle(Int(screen.x*healthGap.x, FLOOR) + 1, Int(screen.y*(1.3*healthGap.y + healthBar.y), FLOOR) + 1, Int(screen.x*healthBar.x*(stamina/maxStamina), FLOOR) - 2, Int(screen.y*healthBar.y, FLOOR) - 2, SKYBLUE);

}

void HUD::Hide()
{

}
