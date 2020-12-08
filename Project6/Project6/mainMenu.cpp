#pragma once
#include "mainMenu.h"
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define MENU_WIDTH 10
#define MENU_HEIGHT 9
#define MENU_ENEMY_COUNT 0



unsigned int menu_data[] = {    1, 1, 1, 2, 2, 2, 2, 3, 3, 3,
                                1, 1, 1, 1, 2, 2, 3, 3, 3, 3,
                                2, 1, 1, 1, 1, 3, 3, 3, 3, 2,
                                2, 2, 1, 1, 1, 1, 3, 3, 2, 2,
                                2, 2, 2, 1, 1, 1, 1, 2, 2, 2,
                                2, 2, 3, 3, 1, 1, 1, 1, 2, 2,
                                2, 3, 3, 3, 3, 1, 1, 1, 1, 2,
                                3, 3, 3, 3, 2, 2, 1, 1, 1, 1,
                                3, 3, 3, 2, 2, 2, 2, 1, 1, 1,
};

void mainMenu::Initialize() {
    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(MENU_WIDTH, MENU_HEIGHT, menu_data, mapTextureID, 1.0f, 5, 1);
}


void mainMenu::Update(float deltaTime) {

}

void mainMenu::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font.png");
    state.map->Render(program);
    Util::DrawText(program, fontTextureID, "Phantoms", 1.5f, -0.75f, glm::vec3(2, -3, 0));
    Util::DrawText(program, fontTextureID, "Press Enter to Begin", 0.5f, -0.30f, glm::vec3(2.6, -5, 0));
}