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

static int getNewId(void)
{
    static int idCounter = 0;

    return (idCounter++);
}

namespace indie {
Camera2D::Camera2D(Vector2 target)
{
    _camera = std::make_shared <::Camera2D>();
    _camera->offset = {0.0f, 0.0f};
    _camera->rotation = 0.0f;
    _camera->target = target;
    _camera->zoom = 1.0f;
    _id = getNewId();
}

Camera2D::Camera2D(std::shared_ptr<Vector2> target): _targetSnap(target), _snapMode(true)
{
    _camera = std::make_shared <::Camera2D>();
    _camera->offset = {0.0f, 0.0f};
    _camera->rotation = 0.0f;
    _camera->target = *(_targetSnap.get());
    _camera->zoom = 1.0f;
    _id = getNewId();
}

void Camera2D::setSnapMode(bool snapMode)
{
    _snapMode = snapMode;
}

void Camera2D::setTarget(Vector2 target)
{
    _camera->target = target;
}

void Camera2D::setSnapedTarget(std::shared_ptr<Vector2> target)
{
    _targetSnap = target;
    _camera->target = *(_targetSnap.get());
}

int Camera2D::getId(void) const
{
    return _id;
}

void Camera2D::update(void)
{
    if (_snapMode)
        _camera->target = *_targetSnap;
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
