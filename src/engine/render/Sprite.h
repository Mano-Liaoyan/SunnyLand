#pragma once
#include <SDL3/SDL_rect.h>  // 用于 SDL_FRect
#include <optional>         // 用于 std::optional 表示可选的源矩形
#include <string>

namespace engine::render
{

    /**
     * @brief Repersent the data of the visual Sprite that is going to be draw on the screen
     *
     * contains texture identifier, portion of the texture to draw (source rectangle), and flip state.
     * positions, scaling and rotation are identified externally (e.g., by SpriteComponent).
     * rendering work is done by Renderer class (taking Sprite as parameter).
     */
    class Sprite final
    {
      private:
        std::string textureId_;                ///< @brief identifier of the texture resource
        std::optional<SDL_FRect> sourceRect_;  ///< @brief optional: portion of the texture to draw
        bool isFlipped_ = false;               ///< @brief whether it is flipped horizontally

      public:
        /**
         * @brief construct a sprite
         *
         * @param texture_id identifier of the texture resource. Must not be empty.
         * @param source_rect optional source rectangle (SDL_FRect) defining the portion of the texture to use. If std::nullopt, the entire texture is used.
         * @param is_flipped whether it is flipped horizontally
         */
        Sprite(const std::string& texture_id, const std::optional<SDL_FRect>& source_rect = std::nullopt, bool is_flipped = false)
            : textureId_(texture_id), sourceRect_(source_rect), isFlipped_(is_flipped)
        {
        }

        // --- getters and setters ---
        const std::string& getTextureId() const { return textureId_; }                 ///< @brief get the texture ID
        const std::optional<SDL_FRect>& getSourceRect() const { return sourceRect_; }  ///< @brief get the source rectangle (if using entire texture, it is std::nullopt)
        bool isFlipped() const { return isFlipped_; }                                  ///< @brief get whether it is flipped horizontally

        void setTextureId(const std::string& texture_id) { textureId_ = texture_id; }  ///< @brief set the texture ID
        void setSourceRect(const std::optional<SDL_FRect>& source_rect)
        {
            sourceRect_ = source_rect;
        }  ///< @brief set the source rectangle (if using entire texture, it is std::nullopt)
        void setFlipped(bool flipped) { isFlipped_ = flipped; }  ///< @brief set whether it is flipped horizontally
    };

}  // namespace engine::render
