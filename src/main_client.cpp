/*
** EPITECH PROJECT, 2022
** rtypeStudio
** File description:
** main
*/

#include <exception>
#include <iostream>

#include "Core.hpp"
#include "ANetworkSystem.hpp"

#include "TestHeaders.hpp"
#include "ArgumentError.hpp"

int main(int ac, char **av)
{
    char *defaultAv[] = {av[0], (char *)"127.0.0.1", (char *)"8080"};
    if (ac != 1 && !ecs::checkArguments(ac, av))
        return 84;
    ecs::Core core(ac, ac == 1 ? defaultAv : av,
                   {ecs::Core::SystemType::GAME,
                    ecs::Core::SystemType::NETWORK,
                    ecs::Core::SystemType::EVENT,
                    ecs::Core::SystemType::AUDIO,
                    ecs::Core::SystemType::GRAPHIC,
                    ecs::Core::SystemType::PARTICLE},
                   ecs::NetworkRole::CLIENT);
    try {
        core.setEventNetwork();
        core.run();
        return core.exec();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}