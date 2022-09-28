/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** AComponent.cpp
*/

#include "Component.hpp"

namespace indie
{
    Component::Component(const Type &type) : _type(type)
    {
    }

    const Component::Type &Component::getType() const
    {
        return _type;
    }

}