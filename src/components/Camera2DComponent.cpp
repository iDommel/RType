/*
** EPITECH PROJECT, 2022
** RType
** File description:
** Camera2DComponent
*/

#include <raylib.h>
#include "Camera2DComponent.hpp"
#include "Position.hpp"

namespace ecs
{
    Camera2DComponent::Camera2DComponent(std::shared_ptr<Position> target) : Component(Type::CAMERA_2D), _camera(target)
    {
        _isInitialized = true;
    }

    Camera2D &Camera2DComponent::getCamera() { return _camera; }
}
