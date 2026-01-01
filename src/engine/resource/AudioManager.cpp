#include "AudioManager.h"

#include <SDL3_mixer/SDL_mixer.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace engine::resource
{
    AudioManager::AudioManager()
    {
        MIX_InitFlags flags = MIX_INIT_MP3 | MIX_INIT_OGG;

        if ((Mix_Init(flags) & flags) != flags)
        {
            throw std::runtime_error("AudioManager could not initialize! Mix_Init Error: " + std::string(SDL_GetError()));
        }

        // Initialize SDL_mixer with desired audio format
        // Default: 44100Hz default format, stereo, 2048 sampling rate
        if (Mix_OpenAudio(0, nullptr) < 0)
        {
            Mix_Quit();

            throw std::runtime_error("AudioManager could not initialize! Mix_OpenAudio Error: " + std::string(SDL_GetError()));
        }

        spdlog::trace("AudioManager constructed and SDL_mixer initialized successfully.");
    };

    AudioManager::~AudioManager()
    {
        Mix_HaltChannel(-1);  // Halt all channels
        Mix_HaltMusic();      // Halt music playback

        clearSounds();  // Clear all loaded sounds from memory
        clearMusics();  // Clear all loaded musics from memory

        Mix_CloseAudio();  // Close the audio device

        Mix_Quit();  // Quit SDL_mixer subsystem

        spdlog::trace("AudioManager destructed and SDL_mixer quit successfully.");
    };

    Mix_Chunk* AudioManager::loadSound(const std::string& file_path)
    {
        // Check if sound is already loaded
        auto it = sounds_.find(file_path);
        if (it != sounds_.end())
        {
            spdlog::debug("Sound '{}' already loaded, returning cached sound.", file_path);
            return it->second.get();
        }

        // Load the sound using SDL_mixer
        spdlog::debug("Loading chunk from file: {}", file_path);

        Mix_Chunk* rawSound = Mix_LoadWAV(file_path.c_str());

        if (!rawSound)
        {
            spdlog::error("Failed to load sound '{}': {}", file_path, SDL_GetError());
            return nullptr;
        }

        // Store the sound in the map with automatic memory management
        sounds_.emplace(file_path, std::unique_ptr<Mix_Chunk, SDLMixChunkDeleter>(rawSound));
        spdlog::debug("Sound '{}' loaded and cached successfully.", file_path);

        return rawSound;
    }

    Mix_Chunk* AudioManager::getSound(const std::string& file_path)
    {
        auto it = sounds_.find(file_path);
        if (it != sounds_.end())
        {
            return it->second.get();
        }

        spdlog::warn("Sound '{}' not found in cache, attempting to load.", file_path);
        return loadSound(file_path);
    }

    void AudioManager::unloadSound(const std::string& file_path)
    {
        auto it = sounds_.find(file_path);
        if (it != sounds_.end())
        {
            spdlog::debug("Unloading sound '{}' from memory.", file_path);
            sounds_.erase(it);
        }
        else
        {
            spdlog::warn("Attempted to unload sound '{}' which is not loaded.", file_path);
        }
    }

    void AudioManager::clearSounds()
    {
        if (!sounds_.empty())
        {
            spdlog::debug("Clearing all {} loaded sounds from memory.", sounds_.size());
            sounds_.clear();
        }
    }

    Mix_Music* AudioManager::loadMusic(const std::string& file_path)
    {
        // Check if music is already loaded
        auto it = musics_.find(file_path);
        if (it != musics_.end())
        {
            spdlog::debug("Music '{}' already loaded, returning cached music.", file_path);
            return it->second.get();
        }

        // Load the music using SDL_mixer
        spdlog::debug("Loading music from file: {}", file_path);

        Mix_Music* rawMusic = Mix_LoadMUS(file_path.c_str());

        if (!rawMusic)
        {
            spdlog::error("Failed to load music '{}': {}", file_path, SDL_GetError());
            return nullptr;
        }

        // Store the music in the map with automatic memory management
        musics_.emplace(file_path, std::unique_ptr<Mix_Music, SDLMixMusicDeleter>(rawMusic));
        spdlog::debug("Music '{}' loaded and cached successfully.", file_path);

        return rawMusic;
    }

    Mix_Music* AudioManager::getMusic(const std::string& file_path)
    {
        auto it = musics_.find(file_path);
        if (it != musics_.end())
        {
            return it->second.get();
        }

        spdlog::warn("Music '{}' not found in cache, attempting to load.", file_path);
        return loadMusic(file_path);
    }

    void AudioManager::unloadMusic(const std::string& file_path)
    {
        auto it = musics_.find(file_path);
        if (it != musics_.end())
        {
            spdlog::debug("Unloading music '{}' from memory.", file_path);
            musics_.erase(it);
        }
        else
        {
            spdlog::warn("Attempted to unload music '{}' which is not loaded.", file_path);
        }
    }

    void AudioManager::clearMusics()
    {
        if (!musics_.empty())
        {
            spdlog::debug("Clearing all {} loaded musics from memory.", musics_.size());
            musics_.clear();
        }
    }

    void AudioManager::clearAudio()
    {
        clearSounds();
        clearMusics();
    }

}  // namespace engine::resource
