#pragma once
#include "Enviroment.h"

class Object : virtual public Enviroment
{
public:
    virtual void Break() = 0;
    virtual void Use() = 0;
    virtual void Hold() = 0;
    virtual void TakeDamage() = 0;
    virtual void Draw() = 0;
private:
    bool pickedUp;
    bool used;
    bool broken;
};
