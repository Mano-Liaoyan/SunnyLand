#pragma once

#include <SDL3/SDL_stdinc.h>

namespace engine::core
{
    /**
     * @brief The Time class provides functionality for tracking elapsed time and calculating delta time between frames.
     *
     * Use SDL for high-resolution timing for accurate time measurements.
     * Provide methods to give scaled and non-scaled delta time, with the ability to set a time scale factor.
     */
    class Time final
    {
       private:
        Uint64 lastTick_ = 0;        ///< @brief The last recorded tick count. Used to calculate delta time.
        Uint64 frameStartTime_ = 0;  ///< @brief The start time of the current frame. Used for frame limiting.
        double deltaTime_ = 0.0;     ///< @brief None scaled delta time between frames in seconds.
        double timeScale_ = 1.0;     ///< @brief Time scale factor.

        // Frame limiting
        int targetFPS_ = 0;             ///< @brief Target frames per second. 0 for unlimited.
        double targetFrameTime_ = 0.0;  ///< @brief Target duration of each frame in seconds.

       public:
        Time();

        // Delete copy and move constructors and assignment operators
        Time(const Time&) = delete;
        Time& operator=(const Time&) = delete;
        Time(Time&&) = delete;
        Time& operator=(Time&&) = delete;

        /**
         * @brief Called at the beginning of each frame. Update the internal time status and calculate delta time.
         */
        void update();

        /**
         * @brief Gets the delta time in seconds after scaling.
         *
         * @return Scaled delta time in seconds.
         */
        float getDeltaTime() const;

        /**
         * @brief Gets the delta time in seconds without scaling.
         *
         * @return Unscaled delta time in seconds.
         */
        float getUnscaledDeltaTime() const;

        /**
         * @brief Sets the time scale factor.
         *
         * @param scale The new time scale factor. 1.0 is normal speed, < 1.0 is slow motion, > 1.0 is fast forward.
         */
        void setTimeScale(double scale);

        /**
         * @brief Gets the current time scale factor.
         *
         * @return The current time scale factor.
         */
        float getTimeScale() const;

        /**
         * @brief Sets the target frames per second (FPS)
         *
         * @param fps The target FPS. Set to 0 for unlimited FPS. Negative values are clamped to 0.
         */
        void setTargetFPS(int fps);

        /**
         * @brief Gets the target frames per second (FPS)
         *
         * @return The target FPS. 0 means unlimited FPS.
         */
        int getTargetFPS() const;

       private:
        /**
         * @brief Used in update() to limit the frame rate to the target FPS.
         * If the current frame time is less than the target frame time, a delay is introduced using SDL_Delay.
         *
         * @param currentDeltaTime The execution time of the current frame in seconds.
         */
        void limitFrameRate(float currentDeltaTime);
    };
}  // namespace engine::core