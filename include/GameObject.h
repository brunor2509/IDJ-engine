//
// Created by bruno on 28/03/18.
//

#ifndef INC_140017658_IDJ_GAMEOBJECT_H
#define INC_140017658_IDJ_GAMEOBJECT_H

#include <vector>
#include "Component.h"
#include "Rect.h"

class GameObject {
public:
    GameObject();
    ~GameObject();

    Rect box;

    void Update(float dt);
    void Render();
    bool IsDead();
    void RequestDelete();
    void AddComponent(Component* cpt);
    void RemoveComponent(Component* cpt);
    Component* GetComponent(std::string type);

private:
    std::vector<Component*> components;
    bool isDead;
};


#endif //INC_140017658_IDJ_GAMEOBJECT_H