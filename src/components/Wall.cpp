/*
** EPITECH PROJECT, 2022
** RType
** File description:
** Wall
*/

#include "Wall.hpp"

namespace ecs
{
    Wall::Wall()
        : Component(Type::WALL)
    {
    }

    std::string Wall::getCorrespondingPath(const std::string &mapAround, char lettre)
    {
        std::string path = "assets/";
        if (lettre == 'a')
            path += "Blue Ground/";
        if (lettre == 'b')
            path += "Brown Ground/";
        if (lettre == 'c')
            path += "Green Ground/";
        if (mapAround[0] == lettre)
        {
            if (mapAround[1] == lettre)
            {
                if (mapAround[2] == lettre)
                {
                    if (mapAround[3] == lettre)
                        path += "Center.png";
                    else
                        path += "Top.png";
                }
                else
                {
                    if (mapAround[3] == lettre)
                        path += "Right.png";
                    else
                        path += "TopRight.png";
                }
            }
            else
            {
                if (mapAround[2] == lettre)
                {
                    if (mapAround[3] == lettre)
                        path += "Left.png";
                    else
                        path += "TopLeft.png";
                }
                else
                {
                    if (mapAround[3] == lettre)
                        path += "LeftRight.png";
                    else
                        path += "TopLeftRight.png";
                }
            }
        }
        else
        {
            if (mapAround[1] == lettre)
            {
                if (mapAround[2] == lettre)
                {
                    if (mapAround[3] == lettre)
                        path += "Bot.png";
                    else
                        path += "TopBot.png";
                }
                else
                {
                    if (mapAround[3] == lettre)
                        path += "BotRight.png";
                    else
                        path += "TopBotRight.png";
                }
            }
            else
            {
                if (mapAround[2] == lettre)
                {
                    if (mapAround[3] == lettre)
                        path += "BotLeft.png";
                    else
                        path += "TopBotLeft.png";
                }
                else
                {
                    if (mapAround[3] == lettre)
                        path += "BotLeftRight.png";
                    else
                        path += "TopBotLeftRight.png";
                }
            }
            return path;
        }
        return path;
    }
}
