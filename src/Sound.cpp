//
// Created by bruno on 28/03/18.
//

#include "Sound.h"

Sound::Sound(GameObject &associated) : Component(associated) {
    chunk = nullptr;
}

Sound::Sound(GameObject &associated, std::string file) : Component(associated) {
    Open(file);
}

Sound::~Sound() {
    if(chunk != nullptr){
        Mix_HaltChannel(channel);
        Mix_FreeChunk(chunk);
    }
};

void Sound::Play(int times) {
    channel = Mix_PlayChannel(-1, chunk, times-1);
    if(channel < 0){
        std::cerr << "\n\n EXECUTION ERROR: COULD NOT PLAY SOUND: " << Mix_GetError();
        exit(1);
    }
}

void Sound::Stop() {
    if(chunk != nullptr){
        Mix_HaltChannel(channel);
    }
}

void Sound::Open(std::string file) {
    chunk = Mix_LoadWAV(file.c_str());

    if(!chunk){
        std::cerr << "\n\n EXECUTION ERROR: OPEN SOUND FILE: " << Mix_GetError();
        exit(1);
    }
}

bool Sound::IsOpen() {
    return chunk != nullptr;
}

void Sound::Update(float dt) {
    //todo?
}

void Sound::Render() {
    //todo?
}

bool Sound::Is(std::string type) {
    return type == "Sound";
}








