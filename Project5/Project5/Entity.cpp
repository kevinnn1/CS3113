#include "Entity.h"
#include <chrono>
#include <thread>

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::checkCollision(Entity* other) {
    if (other == this)  return false; 
    std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
    if (isActive == false || other->isActive == false) {
        return false;
    }
    float xDist = fabs(position.x - other->position.x) - ((width + (other->width)) / 2.0f);
    float yDist = fabs(position.y - other->position.y) - ((height + (other->height)) / 2.0f);
    return (xDist < 0 && yDist < 0);
}

void Entity::CheckCollisionsY(Map* map) {    
    // Probes for tiles    
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);    
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);    
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);    
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);    
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);    
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);    
    float penetration_x = 0;    
    float penetration_y = 0;    
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {        
        position.y -= penetration_y;        
        velocity.y = 0;        
        collidedTop = true;    
    }    
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {        
        position.y -= penetration_y;        
        velocity.y = 0;        
        collidedTopMap = true;    
    }    
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {        
        position.y -= penetration_y;        
        velocity.y = 0;        
        collidedTopMap = true;    
    }
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) { 
        position.y += penetration_y;        
        velocity.y = 0;        
        collidedBottomMap = true; 
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) { 
        position.y += penetration_y;        
        velocity.y = 0;        
        collidedBottomMap = true; }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) { 
        position.y += penetration_y;        
        velocity.y = 0;        
        collidedBottomMap = true; 
    }
}

void Entity::CheckCollisionsX(Map* map) {    
    // Probes for tiles    
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);    
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);    
    float penetration_x = 0;    
    float penetration_y = 0;    
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {        
        position.x += penetration_x;        
        velocity.x = 0;        
        collidedLeftMap = true;    
    }    
    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {        
        position.x -= penetration_x;        
        velocity.x = 0;        
        collidedRightMap = true;    
    }
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount) {
    for (int i = 0; i < objectCount; i++) {
        Entity* object = &objects[i];
        if (checkCollision(object)) {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
            }
        }
    }
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount) { 
    for (int i = 0; i < objectCount; i++) { 
        Entity* object = &objects[i];        
        if (checkCollision(object)) { 
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) { 
                position.x -= penetrationX;
                velocity.x = 0; 
                collidedRight = true;
            } 
            else if (velocity.x < 0) { 
                position.x += penetrationX;
                velocity.x = 0; 
                collidedLeft = true;
            } 
        } 
    } 
}

int direction = -1;
void Entity::AIWalker(Map* map) {
    if (collidedLeftMap == true || collidedRightMap == true) {
        direction = direction*-1;
    }
    movement = glm::vec3(direction*-1, 0, 0);
}

int count = 0;
void Entity::AIJumper(Map* map) {
    movement = glm::vec3(0);
    if (collidedBottomMap) {
       jump = true;
    }
    if (jump) {
        jump = false;
        velocity.y += jumpPower + count;
    }
    ++count;
    if (count == 4) {
        count = 1;
    }
}

void Entity::AIAttacker(Map* map) {
    movement = glm::vec3(0);
    if (collidedLeftMap == true || collidedRightMap == true) {
        direction = direction * -1;
    }
    if (collidedBottomMap) {
        jump = true;
    }
    if (jump) {
        jump = false;
        velocity.y += jumpPower;
    }
    movement.x = direction;
    movement.y = -1.0f;
}

void Entity::AI(Map* map) {
    switch (aiType) {
        case WALKER:
            AIWalker(map);
            break;
        case JUMPER:
            AIJumper(map);
            break;
        case ATTACKER:
            AIAttacker(map);
            break;
    }
}

void Entity::Update(float deltaTime, Entity *entity, Entity* objects, int objectCount, Map* map) {
    if (isActive == false) {
        return;
    }
    if (position.y < -10) {
        isActive = false;
    }
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

    collidedTopMap = false;
    collidedBottomMap = false;
    collidedLeftMap = false;
    collidedRightMap = false;

    if (animIndices != NULL) {
        if (glm::length(movement) != 0) {
            animTime += deltaTime;
            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        } else {
            animIndex = 0;
        }
    }
    if (jump && entityType == PLAYER) {
        jump = false;
        velocity.y += jumpPower;
    }
    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;
    position.y += velocity.y * deltaTime;        // Move on Y    
    CheckCollisionsY(map);    
    CheckCollisionsY(objects, objectCount);    // Fix if needed    
    position.x += velocity.x * deltaTime;        // Move on X    
    CheckCollisionsX(map);    
    CheckCollisionsX(objects, objectCount);    // Fix if needed


    if (entityType == ENEMY) {
        AI(map);
        if (collidedTop) {
            isActive = false;
        }
    }
    if (entityType == PLAYER) {
        if (collidedLeft || collidedRight || collidedTop) {
            isActive = false;
        }
    }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;
    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    glBindTexture(GL_TEXTURE_2D, textureID);  
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);    
    glDrawArrays(GL_TRIANGLES, 0, 6);    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {
    if (isActive == false) return;
    program->SetModelMatrix(modelMatrix);    
    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };    
    glBindTexture(GL_TEXTURE_2D, textureID);    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);    
    glDrawArrays(GL_TRIANGLES, 0, 6);   
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}