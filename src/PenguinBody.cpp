//
// Created by bruno on 21/04/18.
//

#include <Sprite.h>
#include <Game.h>
#include <InputManager.h>
#include "PenguinBody.h"
#include "PenguinCannon.h"

PenguinBody* PenguinBody::player = nullptr;

PenguinBody::PenguinBody(GameObject &associated) :
        Component(associated),
        pcannon(),
        speed(1,0),
        linearSpeed(0),
        angle(0),
        hp(100){

    player = this;

    auto sprite = new Sprite(associated, "./assets/img/penguin.png");
    associated.AddComponent(sprite);
    associated.angleDeg = angle * 180/PI;
}

PenguinBody::~PenguinBody() {
    player = nullptr;
}

void PenguinBody::Start() {
    auto cannonGO = new GameObject();
    auto cannon = new PenguinCannon(*cannonGO, Game::GetInstance().GetState().GetObjectPtr(&associated));
    cannonGO->AddComponent(cannon);

    pcannon = Game::GetInstance().GetState().AddObject(cannonGO);
}

void PenguinBody::Update(float dt) {
    auto inputManager = InputManager::GetInstance();
    double angleVariation = 0;

    if(hp <= 0){
        associated.RequestDelete();
        pcannon.lock()->RequestDelete();
        return;
    }

    double accelSpeedGain = PENGUIN_ACCELERATION*dt;

    if(inputManager.IsKeyDown(W_KEY) && (PENGUIN_MAX_LINEAR_SPEED-abs(linearSpeed) > accelSpeedGain)){
        linearSpeed += accelSpeedGain;
    }
    else if(inputManager.IsKeyDown(S_KEY) && (PENGUIN_MAX_LINEAR_SPEED-abs(linearSpeed) > accelSpeedGain)){
        linearSpeed -= accelSpeedGain;
    }

    if(inputManager.IsKeyDown(A_KEY)){
        angleVariation = PENGUIN_ANGULAR_SPEED*dt;
        angle -= angleVariation;
        associated.angleDeg = angle * 180/PI;
        speed = speed.Rotate(-angleVariation);
    }
    else if(inputManager.IsKeyDown(D_KEY)){
        angleVariation = PENGUIN_ANGULAR_SPEED*dt;
        angle += angleVariation;
        associated.angleDeg = angle * 180/PI;
        speed = speed.Rotate(angleVariation);
    }

    double atrictSpeedLoss = PENGUIN_ATRICT*dt;

    if(abs(linearSpeed) > atrictSpeedLoss){
        linearSpeed -= (linearSpeed < 0)? -1*atrictSpeedLoss : atrictSpeedLoss;
        associated.box += speed*linearSpeed*dt;
    }
    else{
        linearSpeed = 0;
    }
}

void PenguinBody::Render() {

}

bool PenguinBody::Is(std::string type) {
    return type == "PenguinBody";
}

