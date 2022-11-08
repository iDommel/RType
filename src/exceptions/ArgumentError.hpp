/*
** EPITECH PROJECT, 2022
** RType
** File description:
** ArgumentError
*/

#ifndef ARGUMENTERROR_HPP_
#define ARGUMENTERROR_HPP_

namespace ecs
{
    class ArgumentError
    {
    public:
        ArgumentError(const std::string &msg) : _msg(msg) {}
        virtual const char *what() const throw() { return _msg.c_str(); }

    private:
        std::string _msg;
    };
}

#endif /* !ARGUMENTERROR_HPP_ */
