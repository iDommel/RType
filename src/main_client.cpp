/*
** EPITECH PROJECT, 2022
** rtypeStudio
** File description:
** main
*/

#include <exception>
#include <iostream>

#include "Core.hpp"
#include "NetworkSystem.hpp"

#include "TestHeaders.hpp"

int main(int ac, char **av)
{
    ecs::Core core(ac, av,
        {ecs::Core::SystemType::GAME,
        //  ecs::Core::SystemType::NETWORK,
         ecs::Core::SystemType::EVENT,
         ecs::Core::SystemType::AUDIO,
         ecs::Core::SystemType::GRAPHIC,
         ecs::Core::SystemType::PARTICLE},
        ecs::NetworkRole::CLIENT);

    try {
        core.run();
        return core.exec();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}