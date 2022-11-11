/*
** EPITECH PROJECT, 2022
** RType
** File description:
** Animation2D
*/

#include "Animation2D.hpp"

namespace ecs
{
    Animation2D::Animation2D(int nbFrame, int frameSpeed, AnimationType type)
        : Component(Type::ANIMATION_2D), _nbFrames(nbFrame), _currentFrame(1), _frameSpeed(frameSpeed), _animationType(type), _frameCounter(1)
    {
    }

    void Animation2D::setCurrentFrame(int currentFrame)
    {
        _currentFrame = currentFrame;
    }

    int Animation2D::getNbFrames() const
    {
        return _nbFrames;
    }

    int Animation2D::getCurrentFrame() const
    {
        return _currentFrame;
    }

    int Animation2D::getFrameSpeed() const
    {
        return _frameSpeed;
    }

    Animation2D::AnimationType Animation2D::getAnimationType() const
    {
        return _animationType;
    }

    void Animation2D::increment()
    {
        if (_frameCounter >= _frameSpeed) {
            _currentFrame++;
            _frameCounter = 0;
        }
        if (_currentFrame >= _nbFrames) {
            if (_animationType == AnimationType::LOOP)
                _currentFrame = 0;
            else if (_animationType == AnimationType::PINGPONG) {
                _currentFrame = _nbFrames - 2;
            } else if (_animationType == AnimationType::ONCE) {
                _currentFrame = _nbFrames;
            }
        }
        _frameCounter++;
    }
}