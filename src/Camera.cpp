/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Camera
*/

#include "raylib.h"

#include <memory>
#include <iostream>

#include "Camera.hpp"

static int getNewId(void)
{
    static int idCounter = 0;

    return (idCounter++);
}

namespace indie {
Camera::Camera(Vector3 target, Vector3 position)
{
    Vector3 up = {0.0f, 1.0f, 0.0f};
    _camera = std::make_shared <Camera3D>();
    _camera->target = target;
    _camera->position = position;
    _camera->up = up;
    _camera->fovy = 45.0f;
    _camera->projection = CAMERA_PERSPECTIVE;
    _id = getNewId();
}

Camera::Camera(std::shared_ptr<Vector3> target, std::shared_ptr<Vector3> position)
: _targetSnap(target), _positionSnap(position), _snapMode(true)
{
    Vector3 up = {10.0f, 3.0f, 0.0f};
    _camera = std::make_shared <Camera3D>();
    SetCameraMode(*(_camera.get()), CAMERA_CUSTOM);
    _camera->target = *(_targetSnap.get());
    _camera->position = *(_positionSnap.get());
    _camera->up = up;	
    _camera->fovy = 100.0f;
    _camera->projection = CAMERA_PERSPECTIVE;
    _id = getNewId();
}

void Camera::setSnapMode(bool snapMode)
{
    _snapMode = snapMode;
}

void Camera::setPosition(Vector3 pos)
{
    _camera->position = pos;
}

void Camera::setSnapedPosition(std::shared_ptr<Vector3> pos)
{
    _positionSnap = pos;
    _camera->position = *(_positionSnap.get());
}

void Camera::setTarget(Vector3 target)
{
    _camera->target = target;
}

void Camera::setSnapedTarget(std::shared_ptr<Vector3> target)
{
    _targetSnap = target;
    _camera->target = *(_targetSnap.get());
}

void Camera::shiftPosition(Vector3 shift)
{
    _camera->position.x += shift.x;
    _camera->position.y += shift.y;
    _camera->position.z += shift.z;
}

int Camera::getId(void) const
{
    return _id;
}

void Camera::update(void)
{
    if (_snapMode) {
        _camera->target = *_targetSnap;
        _camera->position = *_positionSnap;
    }
    UpdateCamera(_camera.get());
}

void Camera::beginDrawScope(void)
{
    BeginMode3D(*(_camera.get()));
}

void Camera::endDrawScope(void)
{
    EndMode3D();
}

Camera::~Camera()
{
}
}
