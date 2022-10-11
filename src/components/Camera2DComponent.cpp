/*
** EPITECH PROJECT, 2022
** RType
** File description:
** Camera2DComponent
*/

#include <raylib.h>
#include "Camera2DComponent.hpp"

namespace rtype
{
    Camera2DComponent::Camera2DComponent(Vector2 target):
    Component(Type::CAMERA_2D), _camera(target)
    {
        _isInitialized = true;
    }

    Camera2D &Camera2DComponent::getCamera() { return _camera; }
}
