/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** MusicError
*/

#ifndef MUSICERROR_HPP_
#define MUSICERROR_HPP_

#include <stdexcept>

namespace ecs
{
    class MusicError : public std::exception
    {
    public:
        MusicError(const std::string &msg) : _msg(msg) {}
        virtual const char *what() const throw() { return _msg.c_str(); }

    private:
        std::string _msg;
    };
}

#endif /* !MUSICERROR_HPP_ */
