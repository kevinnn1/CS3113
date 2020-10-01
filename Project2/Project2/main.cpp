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

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix, leftPaddle, rightPaddle, ball;
GLuint pongTextureID, ballTextureID;

glm::vec3 leftPaddleMovement = glm::vec3(0, 0, 0);
glm::vec3 rightPaddleMovement = glm::vec3(0, 0, 0);
glm::vec3 leftPaddlePosition = glm::vec3(-5.0f, 0, 0);
glm::vec3 rightPaddlePosition = glm::vec3(5.0f, 0, 0);
glm::vec3 ballMovement = glm::vec3(4, 2, 0);
glm::vec3 ballPosition = glm::vec3(0, 0, 0);


float paddleSpeed = 1.0f;
float ballSpeed = 1.0f;


GLuint LoadTexture(const char* filePath) {
    int w, h, n;    
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);    
    if (image == NULL) { 
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false); 
    }    
    GLuint textureID; 
    glGenTextures(1, &textureID); 
    glBindTexture(GL_TEXTURE_2D, textureID); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    stbi_image_free(image);    
    return textureID; }


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    leftPaddle = glm::mat4(1.0f);
    leftPaddle = glm::translate(leftPaddle, glm::vec3(-5.0f, 0.0f, 0.0f));
    rightPaddle = glm::mat4(1.0f);
    rightPaddle = glm::translate(rightPaddle, glm::vec3(5.0f, 0.0f, 0.0f));
    ball = glm::mat4(1.0f);
    ball = glm::translate(ball, glm::vec3(0.0f, 0.0f, 0.0f));
    ball = glm::scale(ball, glm::vec3(0.25f, 0.25f, 0.0f));
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.9f, 0.7f);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    pongTextureID = LoadTexture("pong.png");
    ballTextureID = LoadTexture("ball.png");

}

void ProcessInput() {
    leftPaddleMovement = glm::vec3(0);
    rightPaddleMovement = glm::vec3(0);

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
                    case SDLK_SPACE:                        
                        break;                
                }                
                break;        
        }    
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL); 
    if (keys[SDL_SCANCODE_W] && leftPaddlePosition.y < 2.75f ) { 
        leftPaddleMovement.y = 4.0f;
    }
    else if (keys[SDL_SCANCODE_S] && leftPaddlePosition.y > -2.75f) { 
        leftPaddleMovement.y = -4.0f;
    }
    if (keys[SDL_SCANCODE_UP] && rightPaddlePosition.y < 2.75f) {
        rightPaddleMovement.y = 4.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN] && rightPaddlePosition.y > -2.75f) {
        rightPaddleMovement.y = -4.0f;
    }
    if (keys[SDL_SCANCODE_SPACE] && ballPosition.x == 0 && ballPosition.y == 0) {
        ballMovement.x = 4;
        ballMovement.y = 2;
    }
}

float lastTicks = 0;
float ballRotate = 0;

void Shutdown() {
    SDL_Quit();
}

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    float leftPaddlePositionX = leftPaddlePosition.x;
    float leftPaddlePositionY = leftPaddlePosition.y;
    float rightPaddlePositionX = rightPaddlePosition.x;
    float rightPaddlePositionY = rightPaddlePosition.y;
    float ballX = ballPosition.x;
    float ballY = ballPosition.y;

    float xdistLeftPaddleAndBall = fabs(ballX - leftPaddlePositionX) - ((0.125 + 0.5) / 2.0f);
    float ydistLeftPaddleAndBall = fabs(ballY - leftPaddlePositionY) - ((0.125 + 2) / 2.0f);
    if (xdistLeftPaddleAndBall < 0 && ydistLeftPaddleAndBall < 0) {
        ballMovement.x *= -1;
        ballMovement.y *= 1;
    }

    float xdistRightPaddleAndBall = fabs(ballX - rightPaddlePositionX) - ((0.125 + 0.5) / 2.0f);
    float ydistRightPaddleAndBall = fabs(ballY - rightPaddlePositionY) - ((0.125 + 2) / 2.0f);
    if (xdistRightPaddleAndBall < 0 && ydistRightPaddleAndBall < 0) {
        ballMovement.x *= -1;
        ballMovement.y *= 1;
    }

    leftPaddlePosition += leftPaddleMovement * paddleSpeed * deltaTime;
    leftPaddle = glm::mat4(1.0f);
    leftPaddle = glm::translate(leftPaddle, leftPaddlePosition);

    rightPaddlePosition += rightPaddleMovement * paddleSpeed * deltaTime;
    rightPaddle = glm::mat4(1.0f);
    rightPaddle = glm::translate(rightPaddle, rightPaddlePosition);


    ballPosition += ballMovement * ballSpeed * deltaTime;
    ball = glm::mat4(1.0f);
    ball = glm::translate(ball, ballPosition);
    if (ballPosition.y >= 3.75f) {
        ballMovement.x *= 1;
        ballMovement.y *= -1;
    }
    if (ballPosition.y <= -3.75f) {
        ballMovement.x *= 1;
        ballMovement.y *= -1;
    }
    if (ballPosition.x >= 5.0f || ballPosition.x <= -5.0f) {
        ballPosition.x = 0;
        ballPosition.y = 0;
        ballMovement.x = 0;
        ballMovement.y = 0;
    }

}

void draw(glm::mat4 model, GLuint textureID) {
    program.SetModelMatrix(model);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Render() { 
    glClear(GL_COLOR_BUFFER_BIT);       

    float vertices[] = { -0.25, -1.0, 0.25, -1.0, 0.25, 1.0, 
                         -0.25, -1.0, 0.25, 1.0, -0.25, 1.0 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 
                          0.0, 1.0, 1.0, 0.0, 0.0, 0.0 }; 

    float ballVertices[] = { -0.125, -0.125, 0.125, -0.125, 0.125, 0.125,
                             -0.125, -0.125, 0.125, 0.125, -0.125, 0.125 };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices); 
    glEnableVertexAttribArray(program.positionAttribute); 

    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords); 
    glEnableVertexAttribArray(program.texCoordAttribute); 

    draw(leftPaddle, pongTextureID);
    draw(rightPaddle, pongTextureID);

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ballVertices);
    glEnableVertexAttribArray(program.positionAttribute);

    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    draw(ball, ballTextureID);

    glDisableVertexAttribArray(program.positionAttribute);    
    glDisableVertexAttribArray(program.texCoordAttribute); 
    SDL_GL_SwapWindow(displayWindow);


}



int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}
