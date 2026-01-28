#pragma once
#include <string>
#include <chrono>
#include <thread>

#include "Entity.h"
#include "CONSTANTS.h"
#include "GenericFunctions.h"

class Player : public Entity
{
public:
    Player(char faction);
    void SetFaction(char faction);
    Rectangle GetBody() override;
    Vector2 GetBodyCenter();
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


    //getters
    unsigned int Health() const { return hp; }
    unsigned int MaxHealth() const { return maxHp; }
    unsigned int Stamina() const { return stamina; }
    unsigned int MaxStamina() const { return maxStamina; }
    unsigned int Damage() const { return damagePoints; }
    char Lebel() const { return level; }
    unsigned int Experience() const { return experiencePoints; }
    unsigned int XpToRank() const { return rankExperiencePoint; }
    char Faction() const { return faction; }
    float Speed() const { return speed; };
    bool Idle() const { return idle; }
    bool Alive() const { return alive; }
private:
    enum Factions 
    {
        Spaith,      //Spathi - sword (greek)                    
        Dorirni,    //Dory - spear (greek),  eirini - peace (greek)
        Pulee,      //Pouli - bird (greek)
        Farfall,    //Farfalla - butterfly (italian)
        Kreceed,    //Crecida - high water (spanish)
        Aphreich    // Apfel - apple (german), reich (rich)
    };
    unsigned int hp;
    unsigned int maxHp;
    unsigned int damagePoints;
    unsigned int stamina;
    unsigned int maxStamina;
    char level;
    unsigned int experiencePoints;
    unsigned int rankExperiencePoint;
    char faction;
    float speed;
    bool idle = true;
    bool alive = true;
    bool actionCooldown = false;
    Rectangle body = { 0, 0, 0, 0 };

    //temp
    Color rectColor = YELLOW;
};