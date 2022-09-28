/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Position.hpp
*/

#ifndef POSITION_HPP
#define POSITION_HPP

#include <tuple>

#include "AVector.hpp"

namespace indie
{

    class Position : public AVector
    {
    public:
        Position(float x, float y, float z = 0);
        Position(const Position &other);
        Position(const Position &&other);
        void operator=(const Position &other);
        Position(const AVector &other);
        void operator=(const AVector &other);
    };
}

#endif /* !POSITION_HPP */
