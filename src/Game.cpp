//
// Created by bruno on 16/03/18.
//

#include "Game.h"
#include <ctime>
#include <InputManager.h>
#include "Resources.h"

#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER

using std::cerr;

Game* Game::instance = nullptr;

Game& Game::GetInstance() {
    if(instance != nullptr){
        return *instance;
    }

    return *(new Game("140017658_BrunoRodrigues", 1024, 600));
}

Game::Game(std::string title, int width, int height) : frameStart(0), dt(0.0) {
    srand((unsigned int) (time(nullptr)));

    if(instance != nullptr){
        cerr << "\n\n EXECUTION ERROR: TRYED TO INSTANCIATE THE GAME, BUT A GAME INSTANCE WAS ALREADY RUNNING \n\n";
        exit(1);
    }

    instance = this;

    this->width = width;
    this->height = height;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)){
        cerr << "SDL_Init RETURNED ERROR: " << SDL_GetError();
        exit(1);
    }

    if(!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF)){
        cerr << "\n\n EXECUTION ERROR: SDL_IMAGE COULD NOT BE INITIALIZED \n\n";
        exit(1);
    }

    if(!Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3)){
        cerr << "\n\n EXECUTION ERROR: SDL_MIXER COULD NOT BE INITIALIZED \n\n";
        exit(1);
    }

    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024)){
        cerr << "\n\n EXECUTION ERROR: Mix_OpenAudio RETURNED ERROR \n\n";
        exit(1);
    }

    if(Mix_AllocateChannels(32) != 32){
        cerr << "\n\n EXECUTION ERROR: COULDN'T ALLOCATE 32 AUDIO CHANNELS \n\n";
        exit(1);
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if(!window){
        cerr << "\n\n EXECUTION ERROR: COULDN'T CREATE WINDOW \n\n";
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        cerr << "\n\n EXECUTION ERROR: COULDN'T CREATE RENDERER \n\n";
        exit(1);
    }

    storedState = nullptr;
}

Game::~Game() {
    if(storedState){
        delete storedState;
    }

    while(!stateStack.empty()){
        stateStack.pop();
    }

    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

State& Game::GetCurrentState() {
    return *(stateStack.top());
}

SDL_Renderer* Game::GetRenderer() {
    return renderer;
}

void Game::Push(State *state) {
    storedState = state;
}

void Game::Run() {
//    LIXO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//    auto inputManager = InputManager::GetInstance();

    if(!storedState){
        std::cout << "No initial state set. Quiting game...\n";
        return;
    }

    stateStack.emplace(storedState);
    storedState->Start();
    storedState = nullptr;

    while(!stateStack.top()->QuitRequested() && !stateStack.empty()){
        if(stateStack.top()->PopRequested()){
            stateStack.pop();
            if(stateStack.top()){
                stateStack.top()->Resume();
            }
        }

        if(storedState){
            stateStack.top()->Pause();
            stateStack.emplace(storedState);
            storedState->Start();
            storedState = nullptr;
        }

        CalculateDeltaTime();
        InputManager::GetInstance().Update();
        stateStack.top()->Update(dt);
        stateStack.top()->Render();
        SDL_RenderPresent(renderer);
        SDL_Delay(33);
    }

    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();
}

void Game::CalculateDeltaTime() {
    int aux = SDL_GetTicks();
    dt = (aux - frameStart) / (float)1000;
    frameStart = aux;
}

float Game::GetDeltaTime() {
    return dt;
}

int Game::GetWidth() {
    return width;
}

int Game::GetHeight() {
    return height;
}
