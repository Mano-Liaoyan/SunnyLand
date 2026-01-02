#include "Renderer.h"

#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>
#include <stdexcept>  // For std::runtime_error

#include "../resource/ResourceManager.h"
#include "Camera.h"
#include "Sprite.h"

namespace engine::render
{

    // Constructor: perform initialization, add ResourceManager
    Renderer::Renderer(SDL_Renderer* sdl_renderer, engine::resource::ResourceManager* resource_manager) : renderer_(sdl_renderer), resourceManager_(resource_manager)
    {
        spdlog::trace("Constructing Renderer...");

        if (!renderer_)
        {
            throw std::runtime_error("Renderer construction failed: Provided SDL_Renderer pointer is null.");
        }
        if (!resourceManager_)
        {
            throw std::runtime_error("Renderer construction failed: Provided ResourceManager pointer is null.");
        }
        setDrawColor(0, 0, 0, 255);
        spdlog::trace("Renderer constructed successfully.");
    }

    void Renderer::drawSprite(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scale, double angle)
    {
        auto texture = resourceManager_->getTexture(sprite.getTextureId());
        if (!texture)
        {
            spdlog::error("Unable to get texture for ID {}.", sprite.getTextureId());
            return;
        }

        auto src_rect = getSpriteSrcRect(sprite);
        if (!src_rect.has_value())
        {
            spdlog::error("Unable to get source rectangle for sprite, ID: {}", sprite.getTextureId());
            return;
        }

        // Apply camera transformation
        glm::vec2 position_screen = camera.worldToScreen(position);

        // Calculate destination rectangle, note that position is the top-left coordinate of the sprite
        float scaled_w = src_rect.value().w * scale.x;
        float scaled_h = src_rect.value().h * scale.y;
        SDL_FRect dest_rect = {position_screen.x, position_screen.y, scaled_w, scaled_h};

        if (!isRectInViewport(camera, dest_rect))
        {
            // Viewport culling: skip drawing if the sprite is outside the viewport
            // spdlog::info("精灵超出视口范围，ID: {}", sprite.getTextureId());
            return;
        }

        // Perform drawing (default rotation center is the center of the sprite)
        if (!SDL_RenderTextureRotated(renderer_, texture, &src_rect.value(), &dest_rect, angle, NULL, sprite.isFlipped() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE))
        {
            spdlog::error("Render rotated texture failed (ID: {}): {}", sprite.getTextureId(), SDL_GetError());
        }
    }

    void Renderer::drawParallax(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scroll_factor, const glm::bvec2& repeat,
                                const glm::vec2& scale)
    {
        auto texture = resourceManager_->getTexture(sprite.getTextureId());
        if (!texture)
        {
            spdlog::error("Unable to get texture for ID {}.", sprite.getTextureId());
            return;
        }

        auto src_rect = getSpriteSrcRect(sprite);
        if (!src_rect.has_value())
        {
            spdlog::error("Unable to get source rectangle for sprite, ID: {}", sprite.getTextureId());
            return;
        }

        // Apply camera transformation with parallax effect
        glm::vec2 position_screen = camera.worldToScreenWithParallax(position, scroll_factor);

        // Calculate scaled texture size
        float scaled_tex_w = src_rect.value().w * scale.x;
        float scaled_tex_h = src_rect.value().h * scale.y;

        glm::vec2 start, stop;
        glm::vec2 viewport_size = camera.getViewportSize();

        if (repeat.x)
        {
            // use glm::mod to find the starting x position within one texture width
            start.x = glm::mod(position_screen.x, scaled_tex_w) - scaled_tex_w;
            stop.x = viewport_size.x;
        }
        else
        {
            start.x = position_screen.x;
            stop.x = glm::min(position_screen.x + scaled_tex_w, viewport_size.x);  // The stop point is one texture width later, but not exceeding viewport width
        }
        if (repeat.y)
        {
            start.y = glm::mod(position_screen.y, scaled_tex_h) - scaled_tex_h;
            stop.y = viewport_size.y;
        }
        else
        {
            start.y = position_screen.y;
            stop.y = glm::min(position_screen.y + scaled_tex_h, viewport_size.y);  // The stop point is one texture height later, but not exceeding viewport height
        }

        for (float y = start.y; y < stop.y; y += scaled_tex_h)
        {
            for (float x = start.x; x < stop.x; x += scaled_tex_w)
            {
                SDL_FRect dest_rect = {x, y, scaled_tex_w, scaled_tex_h};
                if (!SDL_RenderTexture(renderer_, texture, nullptr, &dest_rect))
                {
                    spdlog::error("Render parallax texture failed (ID: {}): {}", sprite.getTextureId(), SDL_GetError());
                    return;
                }
            }
        }
    }

    void Renderer::drawUISprite(const Sprite& sprite, const glm::vec2& position, const std::optional<glm::vec2>& size)
    {
        auto texture = resourceManager_->getTexture(sprite.getTextureId());
        if (!texture)
        {
            spdlog::error("Unable to get texture for ID {}.", sprite.getTextureId());
            return;
        }

        auto src_rect = getSpriteSrcRect(sprite);
        if (!src_rect.has_value())
        {
            spdlog::error("Unable to get source rectangle for sprite, ID: {}", sprite.getTextureId());
            return;
        }

        SDL_FRect dest_rect = {position.x, position.y, 0, 0};  // First determine the top-left corner of the destination rectangle
        if (size.has_value())
        {  // If size is provided, use the provided size
            dest_rect.w = size.value().x;
            dest_rect.h = size.value().y;
        }
        else
        {  // If size is not provided, use the original texture size
            dest_rect.w = src_rect.value().w;
            dest_rect.h = src_rect.value().h;
        }

        // Perform drawing (no UI rotation considered here)
        if (!SDL_RenderTextureRotated(renderer_, texture, &src_rect.value(), &dest_rect, 0.0, nullptr, sprite.isFlipped() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE))
        {
            spdlog::error("Render UI Sprite failed (ID: {}): {}", sprite.getTextureId(), SDL_GetError());
        }
    }

    void Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        if (!SDL_SetRenderDrawColor(renderer_, r, g, b, a))
        {
            spdlog::error("Set render draw color failed: {}", SDL_GetError());
        }
    }

    void Renderer::setDrawColorFloat(float r, float g, float b, float a)
    {
        if (!SDL_SetRenderDrawColorFloat(renderer_, r, g, b, a))
        {
            spdlog::error("Set render draw color failed: {}", SDL_GetError());
        }
    }

    void Renderer::clearScreen()
    {
        if (!SDL_RenderClear(renderer_))
        {
            spdlog::error("Clear renderer failed: {}", SDL_GetError());
        }
    }

    void Renderer::present() { SDL_RenderPresent(renderer_); }

    std::optional<SDL_FRect> Renderer::getSpriteSrcRect(const Sprite& sprite)
    {
        SDL_Texture* texture = resourceManager_->getTexture(sprite.getTextureId());
        if (!texture)
        {
            spdlog::error("Unable to get texture for ID {}.", sprite.getTextureId());
            return std::nullopt;
        }

        auto src_rect = sprite.getSourceRect();
        if (src_rect.has_value())
        {  // If sprite has a specified source rectangle, check if the dimensions are valid
            if (src_rect.value().w <= 0 || src_rect.value().h <= 0)
            {
                spdlog::error("Source rectangle size is invalid, ID: {}", sprite.getTextureId());
                return std::nullopt;
            }
            return src_rect;
        }
        else
        {  // Otherwise get the texture size and return the entire texture size
            SDL_FRect result = {0, 0, 0, 0};
            if (!SDL_GetTextureSize(texture, &result.w, &result.h))
            {
                spdlog::error("Unable to get texture size, ID: {}", sprite.getTextureId());
                return std::nullopt;
            }
            return result;
        }
    }

    bool Renderer::isRectInViewport(const Camera& camera, const SDL_FRect& rect)
    {
        glm::vec2 viewport_size = camera.getViewportSize();
        return rect.x + rect.w >= 0 && rect.x <= viewport_size.x &&  // Equivalent to AABB collision detection
               rect.y + rect.h >= 0 && rect.y <= viewport_size.y;
    }

}  // namespace engine::render