//
// Created by bruno on 08/04/18.
//

#include <Sprite.h>
#include "Bullet.h"

Bullet::Bullet(GameObject &associated, float angle, float speed, int damage, float maxDistance, std::string sprite) :
        Component(associated),
        speed(speed,0),
        speedMagnitude(speed),
        distanceLeft(maxDistance),
        damage(damage){

    associated.AddComponent(new Sprite(associated, sprite));

    associated.angleDeg = (180/PI)*angle;

    this->speed = this->speed.Rotate(angle);
}

void Bullet::Update(float dt) {
    distanceLeft -= speedMagnitude*dt;

    if(distanceLeft <= 0){
        associated.RequestDelete();
        return;
    }

    associated.box += speed*dt;
}

void Bullet::Render() {

}

bool Bullet::Is(std::string type) {
    return type == "Bullet";
}

int Bullet::GetDamage() {
    return damage;
}
