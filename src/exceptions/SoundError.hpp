/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** SoundError.hpp
*/

#ifndef SOUND_ERROR_HPP_
#define SOUND_ERROR_HPP_

#include <stdexcept>

namespace indie {
    class SoundError: public std::exception {
        public:
            SoundError(const std::string &msg): _msg(msg) {}
            virtual const char *what() const throw() { return _msg.c_str(); }
        private:
            std::string _msg;
    };
}

#endif /* SOUND_ERROR_HPP_ */