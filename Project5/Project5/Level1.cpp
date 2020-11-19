#pragma once
#include "Level1.h"
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define LEVEL1_WIDTH 70
#define LEVEL1_HEIGHT 8
#define LEVEL1_ENEMY_COUNT 3

Mix_Music* level1Music;

unsigned int level1_data[] = {  0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
                                0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
                                0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
                                0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
                                0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
                                0, 0, 0, 3, 3, 3, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
                                0, 0, 0, 3, 3, 3, 0, 1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
                                0, 0, 0, 3, 3, 3, 1, 2, 2, 2, 2, 2, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

void Level1::Initialize() {
    state.nextScene = -1;
    level1Music = Mix_LoadMUS("linger.mp3");
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_PlayMusic(level1Music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 8);

    GLuint mapTextureID = Util::LoadTexture("tileset.png");    
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);    
    // Move over all of the player and enemy code from initialization.
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(7, -3, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 1.5f;
    state.player->jumpPower = 7.0f;
    state.player->textureID = Util::LoadTexture("player.png");
    state.player->width = 0.5;
    state.player->height = 0.5;

    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("flyingman.png");
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; ++i) {
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].width = 1;
        state.enemies[i].height = 0.5;
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].movement = glm::vec3(0);
        state.enemies[i].acceleration = glm::vec3(0, -9.81f, 0);
    }

    state.enemies[0].aiType = JUMPER;
    state.enemies[0].aiState = JUMPING;
    state.enemies[0].jumpPower = 3.5f;
    state.enemies[0].speed = 1.4f;
    state.enemies[0].position = glm::vec3(10, -2, 0);

    state.enemies[1].aiType = WALKER;
    state.enemies[1].aiState = WALKING;
    state.enemies[1].jumpPower = 3.5f;
    state.enemies[1].speed = 1.4f;
    state.enemies[1].position = glm::vec3(7, -1, 0);

    state.enemies[2].aiType = ATTACKER;
    state.enemies[2].aiState = ATTACKING;
    state.enemies[2].jumpPower = 6.5f;
    state.enemies[2].speed = 1.4f;
    state.enemies[2].position = glm::vec3(4, -2, 0);


}

bool Level1::allEnemiesDead(GameState state) {
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; ++i) {
        if (state.enemies[i].isActive == true) {
            return false;
        }
    }
    return true;
}

void Level1::Update(float deltaTime) {    
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    state.enemies->Update(deltaTime, state.enemies, state.player, LEVEL1_ENEMY_COUNT, state.map);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.enemies, state.player, LEVEL1_ENEMY_COUNT, state.map);
    }
    if (state.player->position.x >= 60) {
        state.nextScene = 1;
    }

    if (state.player->isActive == false) {
        if (state.player->playerLives != 0) {
            state.player->isActive = true;
            state.player->position = glm::vec3(7, -3, 0);
        }
    }
}

void Level1::Render(ShaderProgram *program) { 
    GLuint fontTextureID = Util::LoadTexture("font.png");
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; ++i) {
        state.enemies[i].Render(program);
    }
    state.player->Render(program);
    state.map->Render(program);
    Util::DrawText(program, fontTextureID, "Lives: " + std::to_string(state.player->playerLives), 0.5f, -0.25f, glm::vec3(state.player->position.x-2, -0.5, 0));
    if (state.player->playerLives == 0) {
        state.player->isActive = false;
        Util::DrawText(program, fontTextureID, "You Lose", 1.0f, -0.25f, glm::vec3(state.player->position.x, -2.5, 0));
    }
}