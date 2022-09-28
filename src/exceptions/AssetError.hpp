/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** AssetError
*/

#ifndef ASSETERROR_HPP_
#define ASSETERROR_HPP_

#include <exception>
#include <string>

namespace indie {
class AssetError: public std::exception {
    public:
        AssetError(const std::string &msg): _msg(msg) {}
        virtual const char *what() const throw() { return _msg.c_str(); }
    private:
        std::string _msg;
};
}

#endif /* !ASSETERROR_HPP_ */
