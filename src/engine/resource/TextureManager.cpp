#include "TextureManager.h"

#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace engine::resource
{
    TextureManager::TextureManager(SDL_Renderer* renderer) : renderer_(renderer)
    {
        if (!renderer_)
        {
            throw std::runtime_error("TextureManager build failed: SDL_Renderer pointer is null.");
        }

        spdlog::trace("TextureManager constructed successfully.");
    }

    SDL_Texture* TextureManager::loadTexture(const std::string& file_path)
    {
        // Check if texture is already loaded
        auto it = textures_.find(file_path);
        if (it != textures_.end())
        {
            spdlog::debug("Texture '{}' already loaded, returning cached texture.", file_path);
            return it->second.get();
        }

        // Load the texture using SDL_image
        SDL_Texture* rawTexture = IMG_LoadTexture(renderer_, file_path.c_str());

        if (!rawTexture)
        {
            spdlog::error("Failed to load texture '{}': {}", file_path, SDL_GetError());
            return nullptr;
        }

        // Store the texture in the map with automatic memory management
        textures_.emplace(file_path, std::unique_ptr<SDL_Texture, SDLTextureDeleter>(rawTexture));
        spdlog::debug("Texture '{}' loaded and cached successfully.", file_path);

        return rawTexture;
    }

    SDL_Texture* TextureManager::getTexture(const std::string& file_path)
    {
        auto it = textures_.find(file_path);
        if (it != textures_.end())
        {
            return it->second.get();
        }

        spdlog::warn("Texture '{}' not found in cache, attempting to load.", file_path);
        return loadTexture(file_path);
    }

    glm::vec2 TextureManager::getTextureSize(const std::string& file_path)
    {
        SDL_Texture* texture = getTexture(file_path);
        if (!texture)
        {
            spdlog::error("Cannot get size for texture '{}': texture not found.", file_path);
            return glm::vec2(0);
        }

        glm::vec2 size;
        if (!SDL_GetTextureSize(texture, &size.x, &size.y))
        {
            spdlog::error("Failed to check texture size for '{}'", file_path);
            spdlog::error("{}", SDL_GetError());
            return glm::vec2(0.0f, 0.0f);
        }

        return size;
    }

    void TextureManager::unloadTexture(const std::string& file_path)
    {
        auto it = textures_.find(file_path);
        if (it != textures_.end())
        {
            spdlog::debug("Unloading Texture '{}' from memory.", file_path);
            textures_.erase(it);
        }
        else
        {
            spdlog::warn("Attempted to unload texture '{}' which is not loaded.", file_path);
        }
    }

    void TextureManager::clearTextures()
    {
        if (!textures_.empty())
        {
            spdlog::debug("Clearing all {} loaded textures from memory.", textures_.size());
            textures_.clear();
        }
        else
        {
            spdlog::debug("No textures to clear; texture cache is already empty.");
        }
    }
}  // namespace engine::resource