/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Timer.hpp
*/

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include "Component.hpp"

namespace indie
{
    class Timer : public Component
    {
    public:
        Timer(int time) : Component(Type::TIMER), _time(time) {};

        int &getTime() { return _time; };

    private:
        int _time;
    };
}

#endif /* !TIMER_HPP_ */