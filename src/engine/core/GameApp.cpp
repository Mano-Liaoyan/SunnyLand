#include "GameApp.h"

#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>

namespace engine::core
{
    GameApp::GameApp() = default;

    GameApp::~GameApp()
    {
        if (isRunning_)
        {
            spdlog::warn("GameApp destructor called while application is still running. Calling close().");
            close();
        }
    }

    void GameApp::run()
    {
        if (!init())
        {
            spdlog::error("Failed to initialize GameApp.");
            return;
        }

        while (isRunning_)
        {
            float deltaTime = 0.01f;
            handleEvents();
            update(deltaTime);
            render();
        }

        close();
    }

    bool GameApp::init()
    {
        spdlog::trace("Initializing GameApp...");

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            spdlog::error("SDL could not initialize! SDL_Error: {}", SDL_GetError());
            return false;
        }

        window_ = SDL_CreateWindow("Sunny Land", 1280, 720, SDL_WINDOW_RESIZABLE);
        if (nullptr == window_)
        {
            spdlog::error("Window could not be created! SDL_Error: {}", SDL_GetError());
            return false;
        }

        renderer_ = SDL_CreateRenderer(window_, nullptr);
        if (nullptr == renderer_)
        {
            spdlog::error("Renderer could not be created! SDL_Error: {}", SDL_GetError());
            return false;
        }

        isRunning_ = true;
        spdlog::info("GameApp initialized successfully.");
        return true;
    }

    void GameApp::handleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                isRunning_ = false;
            }
        }
    }

    void GameApp::update(float deltaTime) {}

    void GameApp::render() {}

    void GameApp::close()
    {
        spdlog::trace("Closing GameApp...");
        if (renderer_)
        {
            SDL_DestroyRenderer(renderer_);
            renderer_ = nullptr;
        }
        if (window_)
        {
            SDL_DestroyWindow(window_);
            window_ = nullptr;
        }
        SDL_Quit();
        isRunning_ = false;
        spdlog::info("GameApp closed ...");
    }
}  // namespace engine::core
