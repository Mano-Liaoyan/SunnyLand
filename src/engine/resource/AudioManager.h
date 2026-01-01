#pragma once

#include <SDL3_mixer/SDL_mixer.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <unordered_map>

namespace engine::resource
{
    class AudioManager
    {
        friend class ResourceManager;

      private:
        struct SDLMixChunkDeleter
        {
            void operator()(Mix_Chunk* chunk) const
            {
                if (chunk)
                {
                    Mix_FreeChunk(chunk);
                    spdlog::trace("Mix_Chunk has been freed.");
                }
            }
        };
        struct SDLMixMusicDeleter
        {
            void operator()(Mix_Music* music) const
            {
                if (music)
                {
                    Mix_FreeMusic(music);
                    spdlog::trace("Mix_Music has been freed.");
                }
            }
        };

        // the map storing Mix_Chunk with automatic memory management
        std::unordered_map<std::string, std::unique_ptr<Mix_Chunk, SDLMixChunkDeleter>> sounds_;
        // the map storing Mix_Music with automatic memory management
        std::unordered_map<std::string, std::unique_ptr<Mix_Music, SDLMixMusicDeleter>> musics_;

      public:
        AudioManager();
        ~AudioManager();

        // Delete copy and move constructors and assignment operators
        AudioManager(const AudioManager&) = delete;
        AudioManager& operator=(const AudioManager&) = delete;
        AudioManager(AudioManager&&) = delete;
        AudioManager& operator=(AudioManager&&) = delete;

      private:
        Mix_Chunk* loadSound(const std::string& file_path);  ///< @brief Load sound from file
        Mix_Chunk* getSound(const std::string& file_path);   ///< @brief try to get the pointer of loaded sound from cache, if not found, try to load it
        Mix_Music* loadMusic(const std::string& file_path);  ///< @brief Load music from file
        Mix_Music* getMusic(const std::string& file_path);   ///< @brief try to get the pointer of loaded music from cache, if not found, try to load it
        void unloadSound(const std::string& file_path);      ///< @brief Unload sound from memory
        void unloadMusic(const std::string& file_path);      ///< @brief Unload music from memory
        void clearSounds();                                  ///< @brief Clear all loaded sounds from memory
        void clearMusics();                                  ///< @brief Clear all loaded musics from memory
        void clearAudio();                                   ///< @brief Clear all loaded audio from memory
    };
}  // namespace engine::resource
