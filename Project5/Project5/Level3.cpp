#pragma once
#include "Level3.h"
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define LEVEL3_WIDTH 70
#define LEVEL3_HEIGHT 8
#define LEVEL3_ENEMY_COUNT 6

Mix_Music* level3Music;

unsigned int level3_data[] = {  0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
                                0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
                                0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
                                0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
                                0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
                                0, 0, 0, 3, 3, 3, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
                                0, 0, 0, 3, 3, 3, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3,
                                0, 0, 0, 3, 3, 3, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3
};

void Level3::Initialize() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    state.nextScene = -1;
    level3Music = Mix_LoadMUS("zombie.mp3");
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_PlayMusic(level3Music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 8);

    GLuint fontTextureID = Util::LoadTexture("font.png");
    GLuint mapTextureID = Util::LoadTexture("tilesetlevel3.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
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

    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("flyingman.png");
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; ++i) {
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
    state.enemies[0].position = glm::vec3(15, -2, 0);

    state.enemies[1].aiType = JUMPER;
    state.enemies[1].aiState = JUMPING;
    state.enemies[1].jumpPower = 3.5f;
    state.enemies[1].speed = 1.4f;
    state.enemies[1].position = glm::vec3(25, -2, 0);

    state.enemies[2].aiType = JUMPER;
    state.enemies[2].aiState = JUMPING;
    state.enemies[2].jumpPower = 6.5f;
    state.enemies[2].speed = 1.4f;
    state.enemies[2].position = glm::vec3(31, -2, 0);

    state.enemies[3].aiType = JUMPER;
    state.enemies[3].aiState = JUMPING;
    state.enemies[3].jumpPower = 6.5f;
    state.enemies[3].speed = 1.4f;
    state.enemies[3].position = glm::vec3(38, -2, 0);

    state.enemies[4].aiType = JUMPER;
    state.enemies[4].aiState = JUMPING;
    state.enemies[4].jumpPower = 6.5f;
    state.enemies[4].speed = 1.4f;
    state.enemies[4].position = glm::vec3(43, -2, 0);

    state.enemies[5].aiType = JUMPER;
    state.enemies[5].aiState = JUMPING;
    state.enemies[5].jumpPower = 3.5f;
    state.enemies[5].speed = 1.4f;
    state.enemies[5].position = glm::vec3(48, -2, 0);


}

bool Level3::allEnemiesDead(GameState state) {
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; ++i) {
        if (state.enemies[i].isActive == true) {
            return false;
        }
    }
    return true;
}

void Level3::Update(float deltaTime) {
    state.enemies->Update(deltaTime, state.enemies, state.player, LEVEL3_ENEMY_COUNT, state.map);
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.enemies, state.player, LEVEL3_ENEMY_COUNT, state.map);
    }
    if (state.player->isActive == false) {
        if (currentLives > 0) {
            currentLives--;
        }
        if (currentLives > 0) {
            state.player->isActive = true;
            state.player->position = glm::vec3(7, -3, 0);
            for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
                state.enemies[i].isActive = true;
            }
        }
    }
}

void Level3::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font.png");
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; ++i) {
        state.enemies[i].Render(program);
    }
    state.player->Render(program);
    state.map->Render(program);
    if (state.player->position.x >= 55 && state.player->isActive == true) {
        Util::DrawText(program, fontTextureID, "You Win", 1.0f, -0.25f, glm::vec3(state.player->position.x, -2.5, 0));
    }

    Util::DrawText(program, fontTextureID, "Lives: " + std::to_string(currentLives), 0.5f, -0.25f, glm::vec3(state.player->position.x - 2, -0.5, 0));
    if (currentLives <= 0) {
        state.player->isActive = false;
        Util::DrawText(program, fontTextureID, "You Lose", 1.0f, -0.25f, glm::vec3(state.player->position.x, -2.5, 0));
    }

}