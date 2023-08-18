#include "Game.h"
#include<fmt/core.h>
#include<fmt/color.h>
Game::Game():
    mWindowWidth(1024), mWindowHeight(768),
    mIsRunning(true),mUpdatingActors(false), mWindow(nullptr), mRenderer(nullptr),
    mTicksCount(0)
{

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

    //更新Actor
    mUpdatingActors = true;
    for (auto actor : mActors) {
        actor->Update(deltaTime);
    }
    //从mPendingActors添加待添加的Actor
    mUpdatingActors = false;
    for (auto actor : mPendingActors) {
        mActors.emplace_back(actor);
    }
    mPendingActors.clear();

    //移除待移除的Actor
    std::vector<Actor*> deadActors;
    for (auto actor : mActors) {
        if (actor->GetState() == Actor::EDead) {
            deadActors.emplace_back(actor);
        }
    }
    for (auto actor : deadActors) {
        delete actor;
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


    //显示缓冲区
    SDL_RenderPresent(mRenderer);

}

void Game::AddActor(Actor* actor)
{
    //如果正在更新Actor，将其添加到待添加的Actor列表中
    if (mUpdatingActors) {
        mPendingActors.emplace_back(actor);
    }
    else {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor* actor)
{
}
