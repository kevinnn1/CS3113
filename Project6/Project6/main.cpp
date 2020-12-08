#pragma once
#define GL_SILENCE_DEPRECATION
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Map.h"
#include "Entity.h"
#include "Util.h"
#include "Scene.h"
#include "Level1.h"
#include <vector>

#include "mainMenu.h"

Mix_Chunk* start;
Mix_Music* menuSong;
Mix_Music* level1Music;

GameState state;
SDL_Window* displayWindow;
bool gameIsRunning = true;
ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene* currentScene; 
Scene* sceneList[1];

int currentLives = 4;

void SwitchToScene(Scene* scene) { 
    currentScene = scene;    
    currentScene->Initialize(); 
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Phantoms", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    start = Mix_LoadWAV("start.wav");

    menuSong = Mix_LoadMUS("title.mp3");
    level1Music = Mix_LoadMUS("main.mp3");
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_PlayMusic(menuSong, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 8);

#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(0, 0, 640, 480);
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);



    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sceneList[0] = new mainMenu();
    sceneList[1] = new Level1();
    SwitchToScene(sceneList[0]);

    projectionMatrix = glm::ortho(-0.5f, 9.5f, -7.75f, -0.25f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
}

void ProcessInput() {
    if (currentScene != sceneList[0]) {
        currentScene->state.player->movement = glm::vec3(0);
    }
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                break;
            case SDLK_RIGHT:
                break;
            case SDLK_RETURN:
                if (currentScene == sceneList[0]) {
                    SwitchToScene(sceneList[1]);
                    Mix_PlayChannel(-1, start, 0);
                    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
                    Mix_PlayMusic(level1Music, -1);
                    Mix_VolumeMusic(MIX_MAX_VOLUME / 8);

                }
            case SDLK_SPACE:
                if (currentScene != sceneList[0]) {
                }
                break;
            }
            break; // SDL_KEYDOWN
        }
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_LEFT] && currentScene != sceneList[0]) {
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
        currentScene->state.player->movement.x = -3.0f;
    }
    else if (keys[SDL_SCANCODE_RIGHT] && currentScene != sceneList[0]) {
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
        currentScene->state.player->movement.x = 3.0f;
    }
    else if (keys[SDL_SCANCODE_UP] && currentScene != sceneList[0]) {
        currentScene->state.player->animIndices = currentScene->state.player->animUp;
        currentScene->state.player->movement.y = 3.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN] && currentScene != sceneList[0]) {
        currentScene->state.player->animIndices = currentScene->state.player->animDown;
        currentScene->state.player->movement.y = -3.0f;
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
void Update() {    
    GLuint fontTextureID = Util::LoadTexture("font.png");
    float ticks = (float)SDL_GetTicks() / 1000.0f;    
    float deltaTime = ticks - lastTicks;    
    lastTicks = ticks;    
    deltaTime += accumulator;    
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;        
        return;    
    }    
    while (deltaTime >= FIXED_TIMESTEP) {   
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;    
    }

    accumulator = deltaTime;
    viewMatrix = glm::mat4(1.0f); 
    viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x+4.5, -currentScene->state.player->position.y-4, 0));
}

void Render() {
    program.SetViewMatrix(viewMatrix);
    currentScene->Render(&program);
    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    Mix_FreeChunk(start); 
    Mix_FreeMusic(menuSong);
    Mix_FreeMusic(level1Music);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    while (gameIsRunning) {
        ProcessInput();
        if (currentScene != sceneList[0]) {
            Update();
        }
        Render();
    }
    Shutdown();
    return 0;
}
