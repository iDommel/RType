/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Camera3DComponent.cpp
*/

#include <raylib.h>

#include "Camera3DComponent.hpp"

namespace ecs
{
    Camera3DComponent::Camera3DComponent(Vector3 target, Vector3 position) : Component(Type::CAMERA_3D), _camera(target, position)
    {
        _isInitialized = true;
    }

    Camera &Camera3DComponent::getCamera() { return _camera; }
}