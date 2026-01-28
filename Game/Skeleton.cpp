#include "Skeleton.h"

Skeleton::Skeleton(Vector2 levelRange)
{
    srand(static_cast<unsigned int>(time(0)));
    level = Int(levelRange.x, FLOOR) + rand() % Int(levelRange.y, FLOOR);
}

Skeleton::Skeleton(unsigned int level)
{
    this->level = level;
}

Rectangle Skeleton::GetBody()
{
    return body;
}

void Skeleton::Spawn(Vector2 position)
{
    body.x = position.x;
    body.y = position.y;

    //temp
    body.width = 25;
    body.height = 50;
}

void Skeleton::Cooldown(int timeInMilliseconds)
{

}

void Skeleton::Idle()
{

}

void Skeleton::Attack()
{

}

void Skeleton::Defend()
{

}

void Skeleton::ChangeSpeed(float rate)
{

}

void Skeleton::DrainStamina()
{

}

void Skeleton::TakeDamage(int hp)
{

}

void Skeleton::Heal(int hp)
{

}

void Skeleton::Move(Vector2 movement)
{

}

void Skeleton::Move(char direction)
{

}

void Skeleton::Draw()
{
    DrawRectangleRec(body, RED);
}

void Skeleton::Die()
{

}
