#include "GameApp.h"

#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>

#include "../render/Camera.h"
#include "../render/Renderer.h"
#include "../resource/ResourceManager.h"

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

        if (!initRenderer())
        {
            spdlog::error("Failed to initialize Renderer.");
            return false;
        }

        if (!initCamera())
        {
            spdlog::error("Failed to initialize Camera Component.");
            return false;
        }

        testResourceManager();

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

    void GameApp::update(float deltaTime) { testCamera(); }

    void GameApp::render()
    {
        renderer_->clearScreen();

        testRenderer();

        renderer_->present();
    }

    void GameApp::close()
    {
        spdlog::trace("Closing GameApp...");

        resourceManager_.reset();

        if (sdl_renderer_)
        {
            SDL_DestroyRenderer(sdl_renderer_);
            sdl_renderer_ = nullptr;
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

        sdl_renderer_ = SDL_CreateRenderer(window_, nullptr);
        if (nullptr == sdl_renderer_)
        {
            spdlog::error("Renderer could not be created! SDL_Error: {}", SDL_GetError());
            return false;
        }
        SDL_SetRenderLogicalPresentation(sdl_renderer_, 640, 360, SDL_LOGICAL_PRESENTATION_LETTERBOX);
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
            resourceManager_ = std::make_unique<resource::ResourceManager>(sdl_renderer_);
        }
        catch (const std::exception& e)
        {
            spdlog::error("Failed to create Resource Manager component: {}", e.what());
            return false;
        }

        return true;
    }
    bool GameApp::initRenderer()
    {
        try
        {
            renderer_ = std::make_unique<engine::render::Renderer>(sdl_renderer_, resourceManager_.get());
        }
        catch (const std::exception& e)
        {
            spdlog::error("Failed to initialize Renderer: {}", e.what());
            return false;
        }
        spdlog::trace("Renderer initialized successfully.");
        return true;
    }
    bool GameApp::initCamera()
    {
        try
        {
            camera_ = std::make_unique<engine::render::Camera>(glm::vec2(640, 360));
        }
        catch (const std::exception& e)
        {
            spdlog::error("Failed to initialize Camera: {}", e.what());
            return false;
        }
        spdlog::trace("Camera initialized successfully.");
        return true;
    }

    // --- Test Functions ---

    void GameApp::testResourceManager()
    {
        resourceManager_->getTexture("assets/textures/Actors/eagle-attack.png");
        resourceManager_->getFont("assets/fonts/VonwaonBitmap-16px.ttf", 16);
        resourceManager_->getSound("assets/audio/button_click.wav");

        resourceManager_->unloadTexture("assets/textures/Actors/eagle-attack.png");
        resourceManager_->unloadFont("assets/fonts/VonwaonBitmap-16px.ttf", 16);
        resourceManager_->unloadSound("assets/audio/button_click.wav");
    }

    void GameApp::testRenderer()
    {
        const render::Sprite sprite_world("assets/textures/Actors/frog.png");
        const render::Sprite sprite_ui("assets/textures/UI/buttons/Start1.png");
        const render::Sprite sprite_parallax("assets/textures/Layers/back.png");

        static float rotation = 0.0f;
        rotation += 0.1f;

        // Note the rendering order
        renderer_->drawParallax(*camera_, sprite_parallax, glm::vec2(100, 100), glm::vec2(0.5f, 0.5f), glm::bvec2(true, false));
        renderer_->drawSprite(*camera_, sprite_world, glm::vec2(200, 200), glm::vec2(1.0f, 1.0f), rotation);
        renderer_->drawUISprite(sprite_ui, glm::vec2(100, 100));
    }

    void GameApp::testCamera()
    {
        const auto key_state = SDL_GetKeyboardState(nullptr);
        if (key_state[SDL_SCANCODE_UP]) camera_->move(glm::vec2(0, -1));
        if (key_state[SDL_SCANCODE_DOWN]) camera_->move(glm::vec2(0, 1));
        if (key_state[SDL_SCANCODE_LEFT]) camera_->move(glm::vec2(-1, 0));
        if (key_state[SDL_SCANCODE_RIGHT]) camera_->move(glm::vec2(1, 0));
    }

}  // namespace engine::core
