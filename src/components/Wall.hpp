/*
** EPITECH PROJECT, 2022
** RType
** File description:
** Wall
*/

#ifndef WALL_HPP_
#define WALL_HPP_

#include "Component.hpp"
namespace ecs
{
    class Wall : public Component
    {
    public:
        /// @brief Construct a new Wall object
        Wall();

        /// @brief Returns the path to the correct wall texture depending on the
        /// surrounding map layout
        /// @param mapLayout the map layout
        /// @return the path to the correct wall texture
        static std::string getCorrespondingPath(const std::string &mapAround, int level);

    private:
    };
}

#endif /* !WALL_HPP_ */
