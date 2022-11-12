/*
** EPITECH PROJECT, 2022
** RType
** File description:
** Animation2D
*/

#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_
#include "Component.hpp"
namespace ecs
{
    class Animation2D : public Component
    {
    public:
        enum class AnimationType {
            LOOP,
            ONCE,
            PINGPONG,
            FIXED
        };
        /// @brief Construct a new Animation2D object
        /// @param nbFrame number of frames in the spriteSheet
        /// @param frameSpeed number of frame to wait before changing frame
        /// @param type type of animation
        Animation2D(int nbFrame, int _frameSpeed = 1, AnimationType type = AnimationType::LOOP);

        /// @brief Set current frame, the first frame is 1
        void setCurrentFrame(int currentFrame);

        /// @brief Get the total number of frames
        int getNbFrames() const;

        /// @brief Get the number of the current frame, the first frame is 1
        int getCurrentFrame() const;

        /// @brief Get the interval between each animation update, in frames
        int getFrameSpeed() const;
        /// @brief Get the animation type
        AnimationType getAnimationType() const;

        /// @brief Increment the current animation
        void increment();

    private:
        int _nbFrames;
        int _currentFrame;
        int _frameSpeed;
        AnimationType _animationType;
        int _frameCounter;
    };
}

#endif /* !ANIMATION_HPP_ */
