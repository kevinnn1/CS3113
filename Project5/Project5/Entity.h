#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Map.h"
#include "util.h"

enum EntityType { PLAYER, ENEMY };
enum AIType { WALKER, JUMPER, ATTACKER  };
enum AIState { IDLE, WALKING, JUMPING, ATTACKING };
class Entity {
public:
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    float width = 1;
    float height = 1;
    float speed;
    EntityType entityType;
    AIType aiType;
    AIState aiState;
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    bool jump = false; 
    float jumpPower = 0;

    int *animRight = NULL;
    int *animLeft = NULL;
    int *animUp = NULL;
    int *animDown = NULL;

    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;

    int playerLives = 3;

    bool isActive = true;
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;

    bool collidedTopMap = false;
    bool collidedBottomMap = false;
    bool collidedLeftMap = false;
    bool collidedRightMap = false;
    
    Entity();
    bool checkCollision(Entity *other);
    void CheckCollisionsX(Entity*  pbjects, int objectCount);
    void CheckCollisionsY(Entity* pbjects, int objectCount);
    void CheckCollisionsX(Map* map); 
    void CheckCollisionsY(Map* map);
    void Update(float deltaTime, Entity* entity, Entity* objects, int objectCount, Map* map);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);

    void AI(Map* map);
    void AIWalker(Map* map);
    void AIJumper(Map* map);
    void AIAttacker(Map* map);
};
