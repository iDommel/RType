/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Hitbox
*/

#include "raylib.h"

#include "HitboxComponent.hpp"
#include "AVector.hpp"

namespace indie
{
    Hitbox::Hitbox(BoundingBox box, Vector3 pos) : Component(Type::HITBOX),
    _box(std::make_unique<BoundingBox>(box)), _is3D(true)
    {
        _box->min = {pos.x + _box->min.x, pos.y + _box->min.y, pos.z + _box->min.z};
        _box->max = {pos.x + _box->max.x, pos.y + _box->max.y, pos.z + _box->max.z};
        _isInitialized = true;
    }

    Hitbox::Hitbox(BoundingBox box) : Component(Type::HITBOX), _box(std::make_unique<BoundingBox>(box)), _is3D(true)
    {
        _isInitialized = true;
    }

    Hitbox::Hitbox(Rectangle rect, Vector2 pos) : Component(Type::HITBOX), _is3D(false), _rect(std::make_unique<Rectangle>(rect))
    {
        _rect->x = pos.x;
        _rect->y = pos.y;
        _isInitialized = true;
    }

    Hitbox::Hitbox(Rectangle rect) : Component(Type::HITBOX), _is3D(false), _rect(std::make_unique<Rectangle>(rect))
    {
        _isInitialized = true;
    }

    Hitbox::Hitbox(bool is3D)
    : Component(Type::HITBOX), _is3D(is3D)
    {
        _isInitialized = false;
    }

    bool Hitbox::is3D(void) const
    {
        return _is3D;
    }

    BoundingBox Hitbox::getBBox(void) const
    {
        if (_is3D)
            return *_box;
        else
            throw std::runtime_error("3d getter used on none 3d Hitbox");
    }

    void Hitbox::setBBox(BoundingBox box)
    {
        if (_is3D)
            _box = std::make_unique<BoundingBox>(box);
        else
            throw std::runtime_error("3d setter used on none 3d Hitbox");
    }

    Rectangle Hitbox::getRect(void) const
    {
        if (!_is3D)
            return *_rect;
        else
            throw std::runtime_error("2d getter used on none 2d Hitbox");
    }

    Hitbox &Hitbox::operator+=(const AVector shift)
    {
        if (_box) {
            _box->min.x += shift.x;
            _box->min.y += shift.y;
            _box->min.z += shift.z;
            _box->max.x += shift.x;
            _box->max.y += shift.y;
            _box->max.z += shift.z;
        } else if (_rect) {
            _rect->x += shift.x;
            _rect->y += shift.y;
        }
        return (*this);
    }

    Hitbox &Hitbox::operator-=(const AVector shift)
    {
        if (_box) {
            _box->min.x -= shift.x;
            _box->min.y -= shift.y;
            _box->min.z -= shift.z;
            _box->max.x -= shift.x;
            _box->max.y -= shift.y;
            _box->max.z -= shift.z;
        } else if (_rect) {
            _rect->x -= shift.x;
            _rect->y -= shift.y;
        }
        return (*this);
    }

    Hitbox::~Hitbox()
    {
        _box.release();
        _rect.release();
    }

}