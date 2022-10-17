/*
** EPITECH PROJECT, 2022
** RType
** File description:
** main_server
*/

#include <exception>
#include <iostream>

#include "Core.hpp"
#include "NetworkSystem.hpp"

#include "TestHeaders.hpp"

int main(int ac, char **av)
{
    ecs::Core core(ac, av, {
        // {ecs::Core::SystemType::GAME,
         ecs::Core::SystemType::NETWORK,},
        //  ecs::Core::SystemType::PARTICLE},
        ecs::NetworkRole::SERVER);

    try {
        core.run();
        return core.exec();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}