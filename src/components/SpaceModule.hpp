/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** SpaceModule.hpp
*/

#ifndef SPACE_MODULE_HPP
#define SPACE_MODULE_HPP

#include "Component.hpp"
#include "IEntity.hpp"

namespace ecs
{
    class SpaceModule : public Component
    {
    public:
        enum class BoundMode {
            NONE,
            FRONT,
            BACK
        };

        SpaceModule(std::shared_ptr<IEntity> player) : Component(Type::SPACE_MODULE), _player(player)
        {
            _isInitialized = true;
        };

        std::shared_ptr<IEntity> getPlayer() { return _player; };

        BoundMode getBoundMode() const { return _bound; }
        void setAttached(BoundMode boundMode) { _bound = boundMode; }

    private:
        std::shared_ptr<IEntity> _player = nullptr;
        BoundMode _bound = BoundMode::FRONT;
    };
}

#endif /* SPACE_MODULE_HPP */