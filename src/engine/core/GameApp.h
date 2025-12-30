#pragma once

#include <memory>

#include "Time.h"

// Forward declarations for SDL structures
struct SDL_Window;
struct SDL_Renderer;

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
        SDL_Renderer* renderer_ = nullptr;
        bool isRunning_ = false;

        // Engine components
        std::unique_ptr<Time> time_;

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
    };
}  // namespace engine::core
