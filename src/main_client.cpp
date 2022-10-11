/*
** EPITECH PROJECT, 2022
** rtypeStudio
** File description:
** main
*/

#include <exception>
#include <iostream>

#include "Core.hpp"

#include "TestHeaders.hpp"

int main(void)
{
    rtype::Core core(
        {rtype::Core::SystemType::GAME,
         rtype::Core::SystemType::EVENT,
         rtype::Core::SystemType::AUDIO,
         rtype::Core::SystemType::GRAPHIC,
         rtype::Core::SystemType::PARTICLE});

    try {
        core.mainLoop();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}