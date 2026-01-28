#include "Player.h"

Player::Player(char faction)
{
    SetFaction(faction);
}

void Player::SetFaction(char faction)
{
    switch (faction)
    {
    case Spaith:
        hp = 1000;
        maxHp = 1000;
        stamina = 100;
        maxStamina = 100;
        damagePoints = 50;
        speed = 2;
        break;
    case Farfall:
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

Rectangle Player::GetBody()
{
    return body;
}

Vector2 Player::GetBodyCenter()
{
    return { body.x + body.width/2, body.y + body.height/2 };
}

void Player::Spawn(Vector2 position)
{
    body.x = position.x;
    body.y = position.y;

    //temp
    body.width = 25;
    body.height = 50;
}

void Player::Cooldown(int timeInMilliseconds)
{
    if (!timeInMilliseconds)
        return;
    if (timeInMilliseconds > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(timeInMilliseconds));
    else
        throw new std::invalid_argument("Invalid time");
}

void Player::Idle()
{
    rectColor = YELLOW;
}

void Player::Attack()
{
    rectColor = BLACK;
}

void Player::Defend()
{
    rectColor = BLUE;
}

void Player::TakeDamage(int hp)
{
    this->hp -= hp;
    rectColor = RED;
}

void Player::Heal(int hp) 
{
    this->hp += hp;
    rectColor = GREEN;
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
    if (alive)
        DrawRectangleRec(body, rectColor);
    else
        DrawCircle(Int(body.x + body.width/2, 2), Int(body.y + body.height/2, 2), body.width/2, MAROON);
}

void Player::Die()
{
    alive = false;
    rectColor = MAROON;
}
