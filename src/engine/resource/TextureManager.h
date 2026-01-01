#pragma once

#include <SDL3/SDL_render.h>
#include <glm/glm.hpp>
#include <memory>
#include <spdlog/spdlog.h>
#include <unordered_map>

namespace engine::resource
{
    class TextureManager
    {
        friend class ResourceManager;

      private:
        struct SDLTextureDeleter
        {
            void operator()(SDL_Texture* texture) const
            {
                if (texture)
                {
                    SDL_DestroyTexture(texture);
                    spdlog::trace("SDL_Texture has been destroyed.");
                }
            }
        };

        // the map storing textures with automatic memory management
        std::unordered_map<std::string, std::unique_ptr<SDL_Texture, SDLTextureDeleter>> textures_;

        // pointer to the SDL_Renderer, not owned
        SDL_Renderer* renderer_ = nullptr;

      public:
        explicit TextureManager(SDL_Renderer* renderer);

        // Delete copy and move constructors and assignment operators
        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;
        TextureManager(TextureManager&&) = delete;
        TextureManager& operator=(TextureManager&&) = delete;

      private:
        SDL_Texture* loadTexture(const std::string& file_path);  ///< @brief Load texture from file
        SDL_Texture* getTexture(const std::string& file_path);   ///< @brief try to get the pointer of loaded texture from cache, if not found, try to load it
        glm::vec2 getTextureSize(const std::string& file_path);  ///< @brief Get texture size
        void unloadTexture(const std::string& file_path);        ///< @brief Unload texture from memory
        void clearTextures();                                    ///< @brief Clear all loaded textures from memory
    };
}  // namespace engine::resource
