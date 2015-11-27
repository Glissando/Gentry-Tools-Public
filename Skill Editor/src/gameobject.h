#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QString>

class GameObject
{
public:
    GameObject();
    virtual QString getName() = 0;
    virtual ~GameObject();
};

#endif // GAMEOBJECT_H
