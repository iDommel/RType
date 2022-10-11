/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** main
*/

#include <exception>
#include <iostream>

#include "Core.hpp"

#include "TestHeaders.hpp"

int main(void)
{
    indie::Core core;

    try {
        core.mainLoop();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}