#include "FontManager.h"

#include <spdlog/spdlog.h>
#include <stdexcept>

namespace engine::resource
{
    FontManager::FontManager()
    {
        if (!TTF_WasInit() && !TTF_Init())
        {
            throw std::runtime_error("FontManager could not initialize! TTF_Init Error: " + std::string(SDL_GetError()));
        }
        spdlog::trace("SDL_ttf initialized successfully.");
        spdlog::trace("FontManager constructed.");
    }

    FontManager::~FontManager()
    {
        if (!fonts_.empty())
        {
            spdlog::trace("FontManager destructor called, unloading {} loaded fonts.", fonts_.size());
            clearFonts();
        }

        if (TTF_WasInit()) TTF_Quit();

        spdlog::trace("FontManager destructed.");
    }

    TTF_Font* FontManager::loadFont(const std::string& file_path, int point_size)
    {
        if (point_size <= 0)
        {
            spdlog::error("Failed to load font '{}': point size {} <= 0 is invalid.", file_path, point_size);
            return nullptr;
        }

        FontKey key = std::make_pair(file_path, point_size);

        // Check if font is already loaded
        auto it = fonts_.find(key);
        if (it != fonts_.end())
        {
            spdlog::debug("Font '{}' ({}pt) already loaded, returning cached font.", file_path, point_size);
            return it->second.get();
        }

        // Load the font using SDL_ttf
        spdlog::debug("Loading font from file: {} with size {}", file_path, point_size);

        TTF_Font* rawFont = TTF_OpenFont(file_path.c_str(), point_size);

        if (!rawFont)
        {
            spdlog::error("Failed to load font '{}': {}", file_path, SDL_GetError());
            return nullptr;
        }

        // Store the font in the map with automatic memory management
        fonts_.emplace(key, std::unique_ptr<TTF_Font, SDLTTFFontDeleter>(rawFont));
        spdlog::debug("Font '{}' ({}pt) loaded and cached successfully.", file_path, point_size);

        return rawFont;
    }

    TTF_Font* FontManager::getFont(const std::string& file_path, int point_size)
    {
        FontKey key = std::make_pair(file_path, point_size);

        auto it = fonts_.find(key);
        if (it != fonts_.end())
        {
            return it->second.get();
        }

        spdlog::warn("Font '{}' ({}pt) not found in cache, attempting to load.", file_path, point_size);
        return loadFont(file_path, point_size);
    }

    void FontManager::unloadFont(const std::string& file_path, int point_size)
    {
        FontKey key = std::make_pair(file_path, point_size);

        auto it = fonts_.find(key);
        if (it != fonts_.end())
        {
            spdlog::debug("Unloading font '{}' ({}pt) from memory.", file_path, point_size);
            fonts_.erase(it);
        }
        else
        {
            spdlog::warn("Attempted to unload font '{}' ({}pt) which is not loaded.", file_path, point_size);
        }
    }

    void FontManager::clearFonts()
    {
        if (!fonts_.empty())
        {
            spdlog::debug("Clearing all {} loaded fonts from memory.", fonts_.size());
            fonts_.clear();
        }
    }
}  // namespace engine::resource
