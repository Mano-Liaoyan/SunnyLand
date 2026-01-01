#include "ResourceManager.h"

#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#include "AudioManager.h"
#include "FontManager.h"
#include "TextureManager.h"


namespace engine::resource
{

    ResourceManager::~ResourceManager() = default;

    ResourceManager::ResourceManager(SDL_Renderer* renderer)
    {
        // --- 初始化各个子系统 --- (如果出现错误会抛出异常，由上层捕获)
        textureManager_ = std::make_unique<TextureManager>(renderer);
        audioManager_ = std::make_unique<AudioManager>();
        fontManager_ = std::make_unique<FontManager>();

        spdlog::trace("ResourceManager 构造成功。");
        // RAII: 构造成功即代表资源管理器可以正常工作，无需再初始化，无需检查指针是否为空
    }

    void ResourceManager::clear()
    {
        fontManager_->clearFonts();
        audioManager_->clearAudio();
        textureManager_->clearTextures();
        spdlog::trace("ResourceManager 中的资源通过 clear() 清空。");
    }

    // --- 纹理接口实现 ---
    SDL_Texture* ResourceManager::loadTexture(const std::string& file_path)
    {
        // 构造函数已经确保了 texture_manager_ 不为空，因此不需要再进行if检查，以免性能浪费
        return textureManager_->loadTexture(file_path);
    }

    SDL_Texture* ResourceManager::getTexture(const std::string& file_path) { return textureManager_->getTexture(file_path); }

    glm::vec2 ResourceManager::getTextureSize(const std::string& file_path) { return textureManager_->getTextureSize(file_path); }

    void ResourceManager::unloadTexture(const std::string& file_path) { textureManager_->unloadTexture(file_path); }

    void ResourceManager::clearTextures() { textureManager_->clearTextures(); }

    // --- 音频接口实现 ---
    Mix_Chunk* ResourceManager::loadSound(const std::string& file_path) { return audioManager_->loadSound(file_path); }

    Mix_Chunk* ResourceManager::getSound(const std::string& file_path) { return audioManager_->getSound(file_path); }

    void ResourceManager::unloadSound(const std::string& file_path) { audioManager_->unloadSound(file_path); }

    void ResourceManager::clearSounds() { audioManager_->clearSounds(); }

    Mix_Music* ResourceManager::loadMusic(const std::string& file_path) { return audioManager_->loadMusic(file_path); }

    Mix_Music* ResourceManager::getMusic(const std::string& file_path) { return audioManager_->getMusic(file_path); }

    void ResourceManager::unloadMusic(const std::string& file_path) { audioManager_->unloadMusic(file_path); }

    void ResourceManager::clearMusic() { audioManager_->clearMusics(); }

    // --- 字体接口实现 ---
    TTF_Font* ResourceManager::loadFont(const std::string& file_path, int point_size) { return fontManager_->loadFont(file_path, point_size); }

    TTF_Font* ResourceManager::getFont(const std::string& file_path, int point_size) { return fontManager_->getFont(file_path, point_size); }

    void ResourceManager::unloadFont(const std::string& file_path, int point_size) { fontManager_->unloadFont(file_path, point_size); }

    void ResourceManager::clearFonts() { fontManager_->clearFonts(); }

}  // namespace engine::resource