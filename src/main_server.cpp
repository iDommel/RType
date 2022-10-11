/*
** EPITECH PROJECT, 2022
** RType
** File description:
** main_server
*/

#include <exception>
#include <iostream>

#include "Core.hpp"

#include "TestHeaders.hpp"

int main(void)
{
    ecs::Core core(
        {ecs::Core::SystemType::GAME,
         ecs::Core::SystemType::PARTICLE});

    try {
        core.mainLoop();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}