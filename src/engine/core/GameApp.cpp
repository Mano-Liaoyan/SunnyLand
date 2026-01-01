#include "GameApp.h"

#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>

namespace engine::core
{
    GameApp::GameApp() {};

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

        time_->setTargetFPS(165);

        while (isRunning_)
        {
            time_->update();
            float deltaTime = time_->getDeltaTime();

            handleEvents();
            update(deltaTime);
            render();
        }

        close();
    }

    bool GameApp::init()
    {
        spdlog::trace("Initializing GameApp...");

        if (!initSDL())
        {
            spdlog::error("Failed to initialize SDL.");
            return false;
        }
        if (!initTime())
        {
            spdlog::error("Failed to initialize Time component.");
            return false;
        }
        if (!initResourceManager())
        {
            spdlog::error("Failed to initialize Resource Manager.");
            return false;
        }

        testReourceManager();

        isRunning_ = true;

        spdlog::info("GameApp initialized successfully.");
        spdlog::info("Using Video Driver: {}", SDL_GetCurrentVideoDriver());
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
            else if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                spdlog::debug("Window resized to {}x{}", event.window.data1, event.window.data2);
            }
        }
    }

    void GameApp::update(float deltaTime) {}

    void GameApp::render()
    {
        // You MUST do this every frame for Wayland to stay responsive
        SDL_SetRenderDrawColor(renderer_, 100, 149, 237, 255);
        SDL_RenderClear(renderer_);
        SDL_RenderPresent(renderer_);
    }

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

    bool GameApp::initSDL()
    {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        {
            spdlog::error("SDL could not initialize! SDL_Error: {}", SDL_GetError());
            return false;
        }

        window_ = SDL_CreateWindow("Sunny Land", 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
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

        SDL_SetRenderDrawColor(renderer_, 100, 149, 237, 255);
        SDL_RenderClear(renderer_);
        SDL_RenderPresent(renderer_);

        spdlog::trace("SDL initialized successfully.");
        return true;
    }

    bool GameApp::initTime()
    {
        try
        {
            time_ = std::make_unique<Time>();
        }
        catch (const std::exception& e)
        {
            spdlog::error("Failed to create Time component: {}", e.what());
            return false;
        }

        spdlog::trace("Time component initialized successfully.");
        return true;
    }

    bool GameApp::initResourceManager()
    {
        try
        {
            resource_manager_ = std::make_unique<resource::ResourceManager>(renderer_);
        }
        catch (const std::exception& e)
        {
            spdlog::error("Failed to create Resource Manager component: {}", e.what());
            return false;
        }

        return true;
    }

    void GameApp::testReourceManager()
    {
        resource_manager_->getTexture("assets/textures/Actors/eagle-attack.png");
        resource_manager_->getFont("assets/fonts/VonwaonBitmap-16px.ttf", 16);
        resource_manager_->getSound("assets/audio/button_click.wav");

        resource_manager_->unloadTexture("assets/textures/Actors/eagle-attack.png");
        resource_manager_->unloadFont("assets/fonts/VonwaonBitmap-16px.ttf", 16);
        resource_manager_->unloadSound("assets/audio/button_click.wav");
    }
}  // namespace engine::core
