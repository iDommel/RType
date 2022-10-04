/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** main
*/

#include <exception>
#include <iostream>

#include "Core.hpp"

int main(void)
{
    ecs::Core core;

    try {
        core.mainLoop();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
// #include <SFML/Graphics.hpp>

// int main()
// {
//     auto window = sf::RenderWindow{{1920u, 1080u}, "CMake SFML Project"};
//     window.setFramerateLimit(144);

//     while (window.isOpen()) {
//         for (auto event = sf::Event{}; window.pollEvent(event);) {
//             if (event.type == sf::Event::Closed) {
//                 window.close();
//             }
//         }

//         window.clear();
//         window.display();
//     }
// }