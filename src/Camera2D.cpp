/*
** EPITECH PROJECT, 2022
** RType
** File description:
** Camera2D
*/

#include "raylib.h"

#include <memory>
#include <iostream>
#include "Camera2D.hpp"

namespace ecs
{
    Camera2D::Camera2D(Vector2 target)
    {
        _camera = std::make_shared<::Camera2D>();
        _camera->offset = {0.0f, 0.0f};
        _camera->rotation = 0.0f;
        _camera->target = target;
        _camera->zoom = 1.0f;
    }

    Camera2D::Camera2D(std::shared_ptr<Position> target) : _targetSnap(target), _snapMode(true)
    {
        Vector2 pos = {target->x, target->y};
        _camera = std::make_shared<::Camera2D>();
        _camera->offset = {0.0f, 0.0f};
        _camera->rotation = 0.0f;
        _camera->target = pos;
        _camera->zoom = 1.0f;
    }

    void Camera2D::setSnapMode(bool snapMode)
    {
        _snapMode = snapMode;
    }

    void Camera2D::setTarget(Vector2 target)
    {
        _camera->target = target;
    }

    void Camera2D::setSnapedTarget(std::shared_ptr<Position> target)
    {
        Vector2 pos = {target->x, target->y};
        _snapMode = true;
        _targetSnap = target;
        _camera->target = pos;
    }

    void Camera2D::update(void)
    {
        if (_snapMode) {
            Vector2 pos = {_targetSnap->x, _targetSnap->y};
            _camera->target = pos;
        }
    }

    void Camera2D::beginDrawScope(void)
    {
        BeginMode2D(*(_camera.get()));
    }

    void Camera2D::endDrawScope(void)
    {
        EndMode2D();
    }

    Camera2D::~Camera2D()
    {
    }
}
