#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

struct SDL_Renderer;
struct SDL_Texture;
struct Mix_Chunk;
struct Mix_Music;
struct TTF_Font;

namespace engine::resource
{
    class TextureManager;
    class AudioManager;
    class FontManager;

    class ResourceManager
    {
       private:
        std::unique_ptr<TextureManager> textureManager_;
        std::unique_ptr<AudioManager> audioManager_;
        std::unique_ptr<FontManager> fontManager_;

       public:
        /**
         * @brief Constructs a ResourceManager which manages textures, audio, and fonts.
         * @param renderer The SDL_Renderer pointer passed for its child managers. Can NOT be null.
         */
        explicit ResourceManager(SDL_Renderer* renderer);

        ~ResourceManager();

        void clear();

        // Delete copy and move constructors and assignment operators
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;

        // --- Unify the resource access interface ---

        // -- Texture --
        SDL_Texture* loadTexture(const std::string& file_path);  ///< @brief Load texture resource
        SDL_Texture* getTexture(const std::string& file_path);   ///< @brief Try to get a pointer to a loaded texture, or try to load it if not loaded
        void unloadTexture(const std::string& file_path);        ///< @brief Unload a specific texture resource
        glm::vec2 getTextureSize(const std::string& file_path);  ///< @brief Get the size of a specific texture
        void clearTextures();                                    ///< @brief Clear all texture resources
        // -- Sound Effects (Chunks) --
        Mix_Chunk* loadSound(const std::string& file_path);  ///< @brief Load sound effect resource
        Mix_Chunk* getSound(const std::string& file_path);   ///< @brief Try to get a pointer to a loaded sound effect, or try to load it if not loaded
        void unloadSound(const std::string& file_path);      ///< @brief Unload a specific sound effect resource
        void clearSounds();                                  ///< @brief Clear all sound effect resources

        // -- Music --
        Mix_Music* loadMusic(const std::string& file_path);  ///< @brief Load music resource
        Mix_Music* getMusic(const std::string& file_path);   ///< @brief Try to get a pointer to a loaded music, or try to load it if not loaded
        void unloadMusic(const std::string& file_path);      ///< @brief Unload a specific music resource
        void clearMusic();                                   ///< @brief Clear all music resources

        // -- Fonts --
        TTF_Font* loadFont(const std::string& file_path, int point_size);  ///< @brief Load font resource
        TTF_Font* getFont(const std::string& file_path, int point_size);   ///< @brief Try to get a pointer to a loaded font, or try to load it if not loaded
        void unloadFont(const std::string& file_path, int point_size);     ///< @brief Unload a specific font resource
        void clearFonts();                                                 ///< @brief Clear all font resources
    };

}  // namespace engine::resource
