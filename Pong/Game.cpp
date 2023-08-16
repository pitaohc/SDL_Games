#include "Game.h"
#include<fmt/core.h>
#include<fmt/color.h>
Game::Game():
    mWindowWidth(1024), mWindowHeight(768),
    mIsRunning(true),mWindow(nullptr),mRenderer(nullptr),
    mBall(nullptr),mPaddle(nullptr),mTicksCount(0),mPaddleDir(0)
{
    //初始化球
    mBall = new Ball();
    mBall->mPosition.x = static_cast<float>(mWindowWidth) / 2.0f;
    mBall->mPosition.y = static_cast<float>(mWindowHeight) / 2.0f;
    mBall->mVelocity.x = 400.0f;
    mBall->mVelocity.y = 235.0f;
    //初始化球拍
    mPaddle = new Paddle();
    mPaddle->mPosition.x = 30.0f;
    mPaddle->mPosition.y = static_cast<float>(mWindowHeight) / 2.0f;
    mPaddle->mHeight = 100.0f;
}
bool Game::Initialize(){
    fmt::print("Game::Start Initialize\n");
    //初始化SDL
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if(sdlResult != 0){
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    //初始化窗口
    mWindow = SDL_CreateWindow(
        "Game Programming",
        (1920 - mWindowWidth) / 2, (1080 - mWindowHeight) / 2,
        mWindowWidth, mWindowHeight,
        0);
    if(!mWindow){
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    //初始化渲染器
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
    //释放资源
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
    //限制帧率，等待16ms
    while(!SDL_TICKS_PASSED(SDL_GetTicks(),mTicksCount + 16)){

    }
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f; //计算帧间隔
    mTicksCount = SDL_GetTicks();
    if(deltaTime > 0.05f) { //限制最大帧间隔,防止卡顿
        deltaTime = 0.05f;
    }
    fmt::print("Game::Updating {:.4}s\n", deltaTime);
    //更新球拍位置
    mPaddle->mPosition.y += mPaddleDir * 300.0f * deltaTime;
    //限制球拍位置
    mPaddle->mPosition.y = std::max(mPaddle->mPosition.y, static_cast<float>(mPaddle->mHeight/2+ thickness));
    mPaddle->mPosition.y = std::min(mPaddle->mPosition.y, static_cast<float>(mWindowHeight - mPaddle->mHeight / 2 - thickness));
    //重置球拍移动方向
    mPaddleDir = 0;

    //更新球的位置
    mBall->mPosition.x += mBall->mVelocity.x * deltaTime;
    mBall->mPosition.y += mBall->mVelocity.y * deltaTime;
    //检测球是否碰到墙壁
    if (mBall->mPosition.x - thickness / 2 <= thickness && mBall->mVelocity.x < 0.0f) {
        //Game Over
        fmt::print(fg(fmt::color::red), "GAME OVER!!!\n");
        mIsRunning = false;
    }
    else if (mBall->mPosition.x + thickness/2 >= mWindowWidth - thickness && mBall->mVelocity.x > 0.0f) {
        fmt::print(fg(fmt::color::yellow), "Ball hits the right wall\n");
        mBall->mVelocity.x *= -1;
    }
    else if (mBall->mPosition.y - thickness / 2 <= thickness && mBall->mVelocity.y < 0.0f) {
        fmt::print(fg(fmt::color::yellow), "Ball hits the top wall\n");
        mBall->mVelocity.y *= -1;
    }
    else if (mBall->mPosition.y + thickness / 2 >= mWindowHeight - thickness && mBall->mVelocity.y > 0.0f) {
        fmt::print(fg(fmt::color::yellow), "Ball hits the button wall\n");
        mBall->mVelocity.y *= -1;
    }

    //检查球与球拍的碰撞
    float diff_y = abs(mPaddle->mPosition.y - mBall->mPosition.y);
    float diff_x = mBall->mPosition.x - mPaddle->mPosition.x;
    if (diff_y < mPaddle->mHeight / 2 &&
        diff_x < thickness &&
        mBall->mVelocity.x < 0.0f) {
        fmt::print(fg(fmt::color::green), "Ball hits the paddle\n");
        mBall->mVelocity.x *= -1;
    }
}

void Game::GenerateOutput()
{
    //设置背景颜色
    SDL_SetRenderDrawColor(
        mRenderer,
        0, 0, 0, 0);


    //清空缓冲区
    SDL_RenderClear(mRenderer);

    //绘制球
    SDL_SetRenderDrawColor(
        mRenderer,
        255, 0, 0, 255);
    SDL_Rect ball_rect = {
        static_cast<int>(mBall->mPosition.x - thickness / 2),
        static_cast<int>(mBall->mPosition.y - thickness / 2),
        thickness,thickness };
    SDL_RenderFillRect(mRenderer, &ball_rect);
    //绘制板子
    SDL_SetRenderDrawColor(
        mRenderer,
        0, 0, 255, 255);

    SDL_Rect paddle_rect = {
        static_cast<int>(mPaddle->mPosition.x - thickness / 2),
        static_cast<int>(mPaddle->mPosition.y - mPaddle->mHeight / 2),
        thickness,mPaddle->mHeight};
    SDL_RenderFillRect(mRenderer, &paddle_rect);
    //绘制墙壁
    SDL_SetRenderDrawColor(
        mRenderer,
        255, 255, 255, 255);

    SDL_Rect wall_top = { 0,0,mWindowWidth,thickness };
    SDL_RenderFillRect(mRenderer, &wall_top);

    SDL_Rect wall_right = { mWindowWidth - thickness,0,mWindowHeight - thickness,mWindowHeight };
    SDL_RenderFillRect(mRenderer, &wall_right);

    SDL_Rect wall_button = { 0,mWindowHeight - thickness,mWindowWidth,mWindowHeight };
    SDL_RenderFillRect(mRenderer, &wall_button);

    //显示缓冲区
    SDL_RenderPresent(mRenderer);

}
