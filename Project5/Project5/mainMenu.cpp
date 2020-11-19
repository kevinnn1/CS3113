#pragma once
#include "mainMenu.h"
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define MENU_WIDTH 10
#define MENU_HEIGHT 5
#define MENU_ENEMY_COUNT 0

Mix_Music* menuSong;

unsigned int menu_data[] = {    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
                                1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
                                2, 2, 2, 2, 2, 2, 0, 0, 0, 0,
};

void mainMenu::Initialize() {
    state.nextScene = -1;
    menuSong = Mix_LoadMUS("dreams.mp3");
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_PlayMusic(menuSong, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(MENU_WIDTH, MENU_HEIGHT, menu_data, mapTextureID, 1.0f, 4, 1);
}


void mainMenu::Update(float deltaTime) {

}

void mainMenu::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font.png");
    Util::DrawText(program, fontTextureID, "Dreams", 1.5f, -0.25f, glm::vec3(1.5, 1, 0));
    Util::DrawText(program, fontTextureID, "Press Enter to Begin", 0.5f, -0.25f, glm::vec3(2.25, -1, 0));
    state.map->Render(program);
}