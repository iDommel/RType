/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** AudioDeviceError
*/

#ifndef AUDIODEVICEERROR_HPP_
#define AUDIODEVICEERROR_HPP_

#include <string>

namespace ecs
{
    class AudioDeviceError
    {
    public:
        AudioDeviceError(const std::string &msg) : _msg(msg) {}
        virtual const char *what() const throw() { return _msg.c_str(); }

    private:
        std::string _msg;
    };
}

#endif /* !AUDIODEVICEERROR_HPP_ */