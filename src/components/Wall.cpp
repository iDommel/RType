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

    std::string Wall::getCorrespondingPath(const std::string &mapAround)
    {
        std::string path;
        if (mapAround[0] == 'a') {
            if (mapAround[1] == 'a') {
                if (mapAround[2] == 'a') {
                    if (mapAround[3] == 'a')
                        path = "assets/Blue Ground/Center.png";
                    else
                        path = "assets/Blue Ground/Top.png";
                } else {
                    if (mapAround[3] == 'a')
                        path = "assets/Blue Ground/Right.png";
                    else
                        path = "assets/Blue Ground/TopRight.png";
                }
            } else {
                if (mapAround[2] == 'a') {
                    if (mapAround[3] == 'a')
                        path = "assets/Blue Ground/Left.png";
                    else
                        path = "assets/Blue Ground/TopLeft.png";
                } else {
                    if (mapAround[3] == 'a')
                        path = "assets/Blue Ground/LeftRight.png";
                    else
                        path = "assets/Blue Ground/TopLeftRight.png";
                }
            }
        } else {
            if (mapAround[1] == 'a') {
                if (mapAround[2] == 'a') {
                    if (mapAround[3] == 'a')
                        path = "assets/Blue Ground/Bot.png";
                    else
                        path = "assets/Blue Ground/TopBot.png";
                } else {
                    if (mapAround[3] == 'a')
                        path = "assets/Blue Ground/BotRight.png";
                    else
                        path = "assets/Blue Ground/TopBotRight.png";
                }
            } else {
                if (mapAround[2] == 'a') {
                    if (mapAround[3] == 'a')
                        path = "assets/Blue Ground/BotLeft.png";
                    else
                        path = "assets/Blue Ground/TopBotLeft.png";
                } else {
                    if (mapAround[3] == 'a')
                        path = "assets/Blue Ground/BotLeftRight.png";
                    else
                        path = "assets/Blue Ground/TopBotLeftRight.png";
                }
            }
            return path;
        }
    }
}  // namespace ecs
