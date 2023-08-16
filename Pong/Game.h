#pragma once
#include<SDL2/SDL.h>
struct Vector2
{
    float x;
    float y;
};
struct Ball
{
    Vector2 mPosition;
    Vector2 mVelocity;
};
struct Paddle
{
    enum Direction {
        UP = -1,
        DOWN = 1
    };
    Vector2 mPosition;
    float mVelocity;
    float mHeight;
    
};
constexpr int thickness = 15;
class Game
{
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();
private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Ball* mBall;
    Paddle* mPaddle;
    bool mIsRunning;
    int mPaddleDir;
    size_t mWindowWidth;
    size_t mWindowHeight;
    Uint32 mTicksCount;
};

