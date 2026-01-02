#pragma once

#include <memory>

#include "../resource/ResourceManager.h"
#include "Time.h"

// Forward declarations for SDL structures
struct SDL_Window;
struct SDL_Renderer;

namespace engine::resource
{
    class ResourceManager;
}

namespace engine::render
{
    class Renderer;
    class Camera;
}  // namespace engine::render

namespace engine::core
{
    /**
     * * @brief The GameApp class is responsible for initializing and managing the
     * main application window and renderer using SDL3.
     */
    class GameApp final
    {
      private:
        SDL_Window* window_ = nullptr;
        SDL_Renderer* sdl_renderer_ = nullptr;
        bool isRunning_ = false;

        // Engine components
        std::unique_ptr<Time> time_;
        std::unique_ptr<resource::ResourceManager> resourceManager_;
        std::unique_ptr<render::Renderer> renderer_;
        std::unique_ptr<render::Camera> camera_;

      public:
        GameApp();
        ~GameApp();

        /***
         * @brief Starts the main application, call init(), and then enters the main
         * loop. Calls close() on exit.
         */
        void run();

        // Delete copy and move constructors and assignment operators
        GameApp(const GameApp&) = delete;

        GameApp& operator=(const GameApp&) = delete;

        GameApp(GameApp&&) = delete;

        GameApp& operator=(GameApp&&) = delete;

      private:
        [[nodiscard]] bool init();

        void handleEvents();

        void update(float deltaTime);

        void render();

        void close();

        [[nodiscard]] bool initSDL();

        [[nodiscard]] bool initTime();

        [[nodiscard]] bool initResourceManager();

        [[nodiscard]] bool initRenderer();

        [[nodiscard]] bool initCamera();

        void testResourceManager();

        void testRenderer();

        void testCamera();
    };
}  // namespace engine::core
