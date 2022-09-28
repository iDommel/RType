/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Grid.hpp
*/

#ifndef Grid_HPP_
#define Grid_HPP_

#include "Component.hpp"

struct Color;

namespace indie
{
    class Grid : public Component
    {
    public:
        /**
         * @brief Construct a new Grid object
         * @param slices radius of the Grid
         * @param spacing color of the Grid
         */
        Grid(int slices, float spacing);

        /// @brief Returns the slices of the Grid
        int &getSlices();
        /// @brief Rerturns the spacing of the Grid
        float getSpacing();

    private:
        int _slices;
        float _spacing;
    };
}

#endif /* !Grid_HPP_ */