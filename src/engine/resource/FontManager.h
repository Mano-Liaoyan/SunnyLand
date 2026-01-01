#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <functional>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>
#include <utility>

namespace engine::resource
{
    using FontKey = std::pair<std::string, int>;  // pair of file path and point size

    struct FontKeyHash
    {
        std::size_t operator()(const FontKey& k) const { return std::hash<std::string>()(k.first) ^ std::hash<int>()(k.second); }
    };

    class FontManager
    {
        friend class ResourceManager;

      private:
        struct SDLTTFFontDeleter
        {
            void operator()(TTF_Font* font) const
            {
                if (font)
                {
                    TTF_CloseFont(font);
                }
            }
        };

        // the map storing TTF_Font with automatic memory management
        std::unordered_map<FontKey, std::unique_ptr<TTF_Font, SDLTTFFontDeleter>, FontKeyHash> fonts_;

      public:
        FontManager();
        ~FontManager();

        // Delete copy and move constructors and assignment operators
        FontManager(const FontManager&) = delete;
        FontManager& operator=(const FontManager&) = delete;
        FontManager(FontManager&&) = delete;
        FontManager& operator=(FontManager&&) = delete;

      private:
        TTF_Font* loadFont(const std::string& file_path, int point_size);  ///< @brief Load font from file with specific point size
        TTF_Font* getFont(const std::string& file_path, int point_size);   ///< @brief try to get the pointer of loaded font from cache, if not found, try to load it
        void unloadFont(const std::string& file_path, int point_size);     ///< @brief Unload font from memory
        void clearFonts();                                                 ///< @brief Clear all loaded fonts from memory
    };
}  // namespace engine::resource