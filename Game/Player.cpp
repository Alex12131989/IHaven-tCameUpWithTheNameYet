#include "Player.h"

Player::Player(char faction)
{
    SetFaction(faction);
}

void Player::SetFaction(char faction)
{
    switch (faction)
    {
    case Tank:
        hp = 1000;
        maxHp = 1000;
        stamina = 100;
        maxStamina = 100;
        damagePoints = 50;
        speed = 2;
        break;
    case Sourcerer:
        hp = 200;
        maxHp = 200;
        stamina = 100;
        maxStamina = 100;
        damagePoints = 25;
        speed = 4;
        break;
    default:
        throw new std::invalid_argument("No such faction");
    }   
}

void Player::ChangeSpeed(float rate)
{
    if (rate < 0)
        throw new std::invalid_argument("negative speed");
    speed *= rate;
}

void Player::DrainStamina()
{

}

void Player::Spawn(Vector2 position)
{
    body.x = position.x;
    body.y = position.y;

    //temp
    body.width = 25;
    body.height = 50;
}

void Player::Attack()
{

}

void Player::Defend()
{

}

void Player::TakeDamage(int hp)
{

}

void Player::Heal(int hp) 
{

}

void Player::Move(Vector2 movement)
{
    body.x += movement.x;
    body.y += movement.y;
}

void Player::Move(char direction)
{
    switch (direction)
    {
    case UPWARD:
        Move({0,-speed});
        break;
    case DOWNWARD:
        Move({0,speed});
        break;
    case LEFTBOUND:
        Move({-speed,0});
        break;
    case RIGHTBOUND:
        Move({speed,0});
        break;
    default:
        throw new std::invalid_argument("Invalid direction");
    }
}

void Player::Draw() 
{
    DrawRectangleRec(body, GREEN);
}

void Player::Die()
{

}
