/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** CameraComponent.cpp
*/

#include "raylib.h"

#include "CameraComponent.hpp"

namespace indie
{
    CameraComponent::CameraComponent(Vector3 target, Vector3 position):
    Component(Type::CAMERA), _camera(target, position)
    {
        _isInitialized = true;
    }

    Camera &CameraComponent::getCamera() { return _camera; }
}