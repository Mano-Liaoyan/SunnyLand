#include "Camera.h"

#include <spdlog/spdlog.h>

#include "../utils/Math.h"

namespace engine::render
{

    Camera::Camera(const glm::vec2& viewport_size, const glm::vec2& position, const std::optional<engine::utils::Rect> limit_bounds)
        : viewport_size_(viewport_size), position_(position), limit_bounds_(limit_bounds)
    {
        spdlog::trace("Camera initialized successfully, position: {},{}", position_.x, position_.y);
    }

    void Camera::setPosition(const glm::vec2& position)
    {
        position_ = position;
        clampPosition();
    }

    void Camera::update(float /* delta_time */)
    {
        // TODO Auto-follow target
    }

    void Camera::move(const glm::vec2& offset)
    {
        position_ += offset;
        clampPosition();
    }

    void Camera::setLimitBounds(const engine::utils::Rect& bounds)
    {
        limit_bounds_ = bounds;
        clampPosition();  // Apply limit immediately after setting boundaries
    }

    const glm::vec2& Camera::getPosition() const { return position_; }

    void Camera::clampPosition()
    {
        // Boundary check needs to ensure the camera view (position to position + viewport_size) is within limit_bounds
        if (limit_bounds_.has_value() && limit_bounds_->size.x > 0 && limit_bounds_->size.y > 0)
        {
            // Calculate the allowed camera position range
            glm::vec2 min_cam_pos = limit_bounds_->position;
            glm::vec2 max_cam_pos = limit_bounds_->position + limit_bounds_->size - viewport_size_;

            // Ensure max_cam_pos is not less than min_cam_pos (viewport might be larger than the world)
            max_cam_pos.x = std::max(min_cam_pos.x, max_cam_pos.x);
            max_cam_pos.y = std::max(min_cam_pos.y, max_cam_pos.y);

            position_ = glm::clamp(position_, min_cam_pos, max_cam_pos);
        }
        // If limit_bounds is invalid, no restriction is applied
    }

    glm::vec2 Camera::worldToScreen(const glm::vec2& world_pos) const
    {
        // Subtract the camera's top-left position from world coordinates
        return world_pos - position_;
    }

    glm::vec2 Camera::worldToScreenWithParallax(const glm::vec2& world_pos, const glm::vec2& scroll_factor) const
    {
        // Apply scroll factor to camera position
        return world_pos - position_ * scroll_factor;
    }

    glm::vec2 Camera::screenToWorld(const glm::vec2& screen_pos) const
    {
        // Add the camera's top-left position to screen coordinates
        return screen_pos + position_;
    }

    glm::vec2 Camera::getViewportSize() const { return viewport_size_; }

    std::optional<engine::utils::Rect> Camera::getLimitBounds() const { return limit_bounds_; }

}  // namespace engine::render