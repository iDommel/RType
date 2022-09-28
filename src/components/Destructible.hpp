/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Destructible.hpp
*/

#ifndef DESTRUCTIBLE_HPP
#define DESTRUCTIBLE_HPP

namespace indie
{
    class Destructible : public Component
    {
    public:
        Destructible() : Component(Type::DESTRUCTIBLE) {};
    };
}

#endif /* DESTRUCTIBLE_HPP */