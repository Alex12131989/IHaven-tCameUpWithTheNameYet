#pragma once
#include <cstdlib>
#include <ctime>

#include "Entity.h"
#include "GenericFunctions.h"

class Skeleton : public Entity
{
public:
    Skeleton(Vector2 levelRange);
    Skeleton(unsigned int level);
    Rectangle GetBody() override;
    void Spawn(Vector2 position) override;
    void Cooldown(int timeInMilliseconds) override;
    void Idle() override;
    void Attack() override;
    void Defend() override;
    void ChangeSpeed(float rate) override;
    void DrainStamina() override;
    void TakeDamage(int hp) override;
    void Heal(int hp) override;
    void Move(Vector2 movement) override;
    void Move(char direction) override;
    void Draw() override;
    void Die() override;
private:
    unsigned int hp;
    unsigned int damagePoints;
    unsigned int level;
    unsigned int experiencePoints;
    bool idle;
    bool alive;
    Rectangle body;
};