#include "Game.h"
#include<fmt/core.h>
#include<fmt/color.h>
Game::Game():
    mWindowWidth(1024), mWindowHeight(768),
    mIsRunning(true),mWindow(nullptr),mRenderer(nullptr),
    mBall(nullptr),mPaddle(nullptr),mTicksCount(0),mPaddleDir(0)
{
    mBall = new Ball();
    mPaddle = new Paddle();
    mBall->mPosition.x = static_cast<float>(mWindowWidth) / 2.0f;
    mBall->mPosition.y = static_cast<float>(mWindowHeight) / 2.0f;
    mPaddle->mPosition.x = 30.0f;
    mPaddle->mPosition.y = static_cast<float>(mWindowHeight) / 2.0f;
    
}
bool Game::Initialize(){
    fmt::print("Game::Start Initialize\n");
    //��ʼ��SDL
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if(sdlResult != 0){
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    //��ʼ������
    mWindow = SDL_CreateWindow(
        "Game Programming",
        100, 100,
        mWindowWidth, mWindowHeight,
        0);
    if(!mWindow){
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    //��ʼ����Ⱦ��
    mRenderer = SDL_CreateRenderer(
        mWindow,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    return true;
}

void Game::RunLoop(){
    fmt::print("Game::Start Run Loop\n");
    while(mIsRunning){
            ProcessInput();
            UpdateGame();
            GenerateOutput();
    }
}

void Game::Shutdown(){
    fmt::print("Game::Start Shutdown\n");
    //�ͷ���Դ
    delete mPaddle;
    delete mBall;

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            mIsRunning = false;
            break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) {
        fmt::print("Game::ProcessInput keyboard Escape\n");
        mIsRunning = false;
    }

    if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
        fmt::print("Game::ProcessInput Paddle move up\n");
        mPaddleDir -= 1;
    }

    if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]) {
        fmt::print("Game::ProcessInput Paddle move down\n");
        mPaddleDir += 1;
    }
}

void Game::UpdateGame()
{
    //����֡�ʣ��ȴ�16ms
    while(!SDL_TICKS_PASSED(SDL_GetTicks(),mTicksCount + 16)){

    }
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f; //����֡���
    mTicksCount = SDL_GetTicks();
    if(deltaTime > 0.05f) { //�������֡���,��ֹ����
        deltaTime = 0.05f;
    }
    fmt::print("Game::Updating {:.4}s\n", deltaTime);
    //��������λ��
    mPaddle->mPosition.y += mPaddleDir * 300.0f * deltaTime;
    //��������λ��
    mPaddle->mPosition.y = std::max(mPaddle->mPosition.y, static_cast<float>(50+ thickness));
    mPaddle->mPosition.y = std::min(mPaddle->mPosition.y, static_cast<float>(mWindowHeight - 50 - thickness));
    //���������ƶ�����
    mPaddleDir = 0;
}

void Game::GenerateOutput()
{
    //���ñ�����ɫ
    SDL_SetRenderDrawColor(
        mRenderer,
        0, 0, 0, 0);


    //��ջ�����
    SDL_RenderClear(mRenderer);

    //����ǽ��
    SDL_SetRenderDrawColor(
        mRenderer,
        255, 255, 255, 255);
    
    SDL_Rect wall_top = { 0,0,mWindowWidth,thickness };
    SDL_RenderFillRect(mRenderer, &wall_top);
    
    SDL_Rect wall_right = { mWindowWidth -thickness,0,mWindowHeight - thickness,mWindowHeight };
    SDL_RenderFillRect(mRenderer, &wall_right);

    SDL_Rect wall_button = {0,mWindowHeight-thickness,mWindowWidth,mWindowHeight };
    SDL_RenderFillRect(mRenderer, &wall_button);
    //������
    SDL_Rect ball_rect = {
        static_cast<int>(mBall->mPosition.x - thickness / 2),
        static_cast<int>(mBall->mPosition.y - thickness / 2),
        thickness,thickness };
    SDL_RenderFillRect(mRenderer, &ball_rect);
    //���ư���
    SDL_Rect paddle_rect = {
        static_cast<int>(mPaddle->mPosition.x - thickness / 2),
        static_cast<int>(mPaddle->mPosition.y - 100 / 2),
        thickness,100};
    SDL_RenderFillRect(mRenderer, &paddle_rect);

    //��ʾ������
    SDL_RenderPresent(mRenderer);

}
