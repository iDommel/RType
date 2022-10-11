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
    indie::Core core(
        {indie::Core::SystemType::GAME,
         indie::Core::SystemType::EVENT,
         indie::Core::SystemType::AUDIO,
         indie::Core::SystemType::GRAPHIC,
         indie::Core::SystemType::PARTICLE});

    try {
        core.mainLoop();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}