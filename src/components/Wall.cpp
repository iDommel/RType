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

    std::string Wall::getCorrespondingPath(const std::string &mapAround, int level)
    {
        std::string path = "assets/Blue Ground/";

        if (level == 1) {
            path = "assets/Blue Ground/";
        } else if (level == 2) {
            path = "assets/Brown Ground/";
        } else if (level == 3) {
            path = "assets/Green Ground/";
        }
        if (mapAround[0] == 'a') {
            if (mapAround[1] == 'a') {
                if (mapAround[2] == 'a') {
                    if (mapAround[3] == 'a')
                        path += "Center.png";
                    else
                        path += "Top.png";
                } else {
                    if (mapAround[3] == 'a')
                        path += "Right.png";
                    else
                        path += "TopRight.png";
                }
            } else {
                if (mapAround[2] == 'a') {
                    if (mapAround[3] == 'a')
                        path += "Left.png";
                    else
                        path += "TopLeft.png";
                } else {
                    if (mapAround[3] == 'a')
                        path += "LeftRight.png";
                    else
                        path += "TopLeftRight.png";
                }
            }
        } else {
            if (mapAround[1] == 'a') {
                if (mapAround[2] == 'a') {
                    if (mapAround[3] == 'a')
                        path += "Bot.png";
                    else
                        path += "TopBot.png";
                } else {
                    if (mapAround[3] == 'a')
                        path += "BotRight.png";
                    else
                        path += "TopBotRight.png";
                }
            } else {
                if (mapAround[2] == 'a') {
                    if (mapAround[3] == 'a')
                        path += "BotLeft.png";
                    else
                        path += "TopBotLeft.png";
                } else {
                    if (mapAround[3] == 'a')
                        path += "BotLeftRight.png";
                    else
                        path += "TopBotLeftRight.png";
                }
            }
            return path;
        }
        return path;
    }
}  // namespace ecs
