#pragma once
#include <raylib.h>

class Entity
{
public:
    virtual Rectangle GetBody() = 0;
    virtual void Spawn(Vector2 position) = 0;
    virtual void Cooldown(int timeInMilliseconds) = 0;
    virtual void Idle() = 0;
    virtual void Attack() = 0;
    virtual void Defend() = 0;
    virtual void ChangeSpeed(float rate) = 0;
    virtual void DrainStamina() = 0;
    virtual void TakeDamage(int hp) = 0;
    virtual void Heal(int hp) = 0;
    virtual void Move(Vector2 movement) = 0;
    virtual void Move(char direction) = 0;
    virtual void Draw() = 0;
    virtual void Die() = 0;
private:
    unsigned int hp;
    unsigned int maxHp;
    unsigned int damagePoints;
    unsigned int stamina;
    unsigned int maxStamina;
    unsigned int level;
    unsigned int experiencePoints;
    bool idle;
    bool alive;
    bool actionCooldown;
    Rectangle body;
};
