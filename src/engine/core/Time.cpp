#include <SDL3/SDL_timer.h>
#include <spdlog/spdlog.h>

#include "Time.h"

namespace engine::core
{
    Time::Time()
    {
        // Initialize lastTick_ and frameStartTime_ to the current SDL tick to avoid large delta time on the first update
        lastTick_ = SDL_GetTicksNS();
        frameStartTime_ = lastTick_;
        spdlog::trace("Time initialized. lastTick_: {}", lastTick_);
    }

    void Time::update()
    {
        frameStartTime_ = SDL_GetTicksNS();  // record the time enters the frame

        auto currentDeltaTime = static_cast<float>(frameStartTime_ - lastTick_) / 1'000'000'000.0;

        if (targetFrameTime_ > 0)  // if frame limiting is enabled, apply it
        {
            limitFrameRate(currentDeltaTime);
        }
        else  // otherwise, just let deltaTime_ = currentDeltaTime
        {
            deltaTime_ = currentDeltaTime;
        }

        lastTick_ = SDL_GetTicksNS();  // record the time leaves the frame
    }

    float Time::getDeltaTime() const { return deltaTime_ * timeScale_; }

    float Time::getUnscaledDeltaTime() const { return deltaTime_; }

    void Time::setTimeScale(double scale)
    {
        if (scale < 0.0)
        {
            spdlog::warn("Time scale cannot be negative. Clamping to 0.0.");
            scale = 0.0;
        }

        timeScale_ = scale;
        spdlog::info("Time Scale setted to {}", timeScale_);
    }

    void Time::setTargetFPS(int fps)
    {
        if (fps < 0)
        {
            spdlog::warn("Target FPS cannot be negative. Clamping to 0.");
            targetFPS_ = 0;
        }
        else
        {
            targetFPS_ = fps;
        }

        if (targetFPS_ > 0)
        {
            targetFrameTime_ = 1.0 / static_cast<double>(targetFPS_);
            spdlog::info("Target FPS setted to {} (Frame time: {:.6f}s)", targetFPS_, targetFrameTime_);
        }
        else
        {
            targetFrameTime_ = 0.0;
            spdlog::info("Target FPS setted to: Unlimited");
        }
    }

    int Time::getTargetFPS() const { return targetFPS_; }

    void Time::limitFrameRate(float currentDeltaTime)
    {
        // If the current frame time is less than the target frame time, introduce a delay
        if (currentDeltaTime < targetFrameTime_)
        {
            auto timeToWait = static_cast<Uint64>((targetFrameTime_ - currentDeltaTime) * 1'000'000'000.0);
            SDL_DelayNS(timeToWait);
            deltaTime_ = static_cast<double>(SDL_GetTicksNS() - lastTick_) / 1'000'000'000.0f;
        }
    }
}  // namespace engine::core