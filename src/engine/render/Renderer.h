#pragma once

#include <SDL3/SDL_stdinc.h>
#include <glm/glm.hpp>
#include <optional>

#include "Sprite.h"

struct SDL_Renderer;
struct SDL_FRect;

namespace engine::resource
{
    class ResourceManager;
}

namespace engine::render
{
    class Camera;

    /**
     * @brief encapsulate the rendering behavior of SDL3
     * encapsulate SDL_Renderer and provide methods to clear the screen, draw sprites, and present the final image.
     * Initialize during construction. Depends on a valid SDL_Renderer and ResourceManager.
     * If either pointer is null, throws std::runtime_error.
     */
    class Renderer final
    {
      private:
        SDL_Renderer* renderer_ = nullptr;                              ///< @brief Non owning pointer to SDL_Renderer
        engine::resource::ResourceManager* resourceManager_ = nullptr;  ///< @brief Non owning pointer to ResourceManager
      public:
        /**
         * @brief Construct a new Renderer object
         * @param sdl_renderer points to a valid SDL_Renderer*, could not be null
         * @param resource_manager points to a valid ResourceManager*, could not be null
         * @throws std::runtime_error if either pointer is nullptr。
         */
        Renderer(SDL_Renderer* sdl_renderer, engine::resource::ResourceManager* resource_manager);

        /**
         * @brief Draw a sprite on the screen with position, scale and rotation angle
         *
         * @param camera The camera used to control the renderer
         * @param sprite A Sprite object contains texture_id, matrix and the status of flipping
         * @param position the top left position inside world coordinate
         * @param scale scaling factor
         * @param angle rotation angles in degree
         */
        void drawSprite(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scale = {1.0f, 1.0f}, double angle = 0.0f);

        /**
         * @brief Draw a parallax scrolling background
         *
         * @param camera The camera used to control the renderer
         * @param sprite A Sprite object contains texture_id, source_rect and the status of flipping.
         * @param position world coordinate.
         * @param scroll_factor scroll factor.
         * @param scale scaling factor.
         */
        void drawParallax(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scroll_factor, const glm::bvec2& repeat = {true, true},
                          const glm::vec2& scale = {1.0f, 1.0f});

        /**
         * @brief Draw a UI sprite directly in screen coordinates.
         *
         * @param sprite A Sprite object contains texture_id, source_rect and the status of flipping.
         * @param position The top-left position in screen coordinates.
         * @param size Optional: target rectangle size. If std::nullopt, use the original size of the sprite.
         */
        void drawUISprite(const Sprite& sprite, const glm::vec2& position, const std::optional<glm::vec2>& size = std::nullopt);

        void present();      ///< @brief Update screen, wrap SDL_RenderPresent function
        void clearScreen();  ///< @brief Clear screen, wrap SDL_RenderClear function

        void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);              ///< @brief Set draw color, wrap SDL_SetRenderDrawColor function, use Uint8 type
        void setDrawColorFloat(float r, float g, float b, float a = 1.0f);        ///< @brief Set draw color, wrap SDL_SetRenderDrawColorFloat function, use float type
        [[nodiscard]] SDL_Renderer* getSDLRenderer() const { return renderer_; }  ///< @brief Get the underlying SDL_Renderer pointer

        // Disable copy and move semantics
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

      private:
        std::optional<SDL_FRect> getSpriteSrcRect(
            const Sprite& sprite);  ///< @brief get the source rectangle of a sprite, used for actual drawing. If error occurs, return std::nullopt and skip drawing.
        bool isRectInViewport(const Camera& camera, const SDL_FRect& rect);  ///< @brief check if a rectangle is in the viewport, used for viewport clipping
    };
}  // namespace engine::render
// engine
