/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** IComponent.hpp
*/

#ifndef ICOMPONENT_HPP
#define ICOMPONENT_HPP

namespace indie
{

    class IEntity;
    class IComponent
    {
    public:
        enum class Type {
            POSITION,
            VELOCITY,
            TRANSFORM,
            ROTATION,
            TEXT,
            SPRITE,
            MODEL,
            HITBOX,
            MUSIC,
            SOUND,
            EVT_LISTENER,
            BONUS,
            PLAYER,
            RECT,
            CAMERA,
            SPHERE,
            CUBE,
            GRID,
            ANIMATION,
            PARTICLES,
            BOMB,
            TIMER,
            DESTRUCTIBLE,
            UI,
            AI,
            RADAR,
            TYPE_NB
        };
        virtual const Type &getType() const = 0;
        virtual bool isInitialized() const = 0;
    };

}

#endif /* ICOMPONENT_HPP */