/*
** EPITECH PROJECT, 2022
** RType
** File description:
** MapGeneration
*/

#include "raylib.h"

#include <functional>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>

#include "GameSystem.hpp"
#include "EventSystem.hpp"
#include "CollideSystem.hpp"
#include "UIComponent.hpp"
#include "Camera3DComponent.hpp"
#include "GamepadStickCallbacks.hpp"
#include "Core.hpp"
#include "Entity.hpp"
#include "EventListener.hpp"
#include "HitboxComponent.hpp"
#include "AudioDevice.hpp"
#include "Player.hpp"
#include "AIPlayer.hpp"
#include "Radar.hpp"
#include "Position.hpp"
#include "Rect.hpp"
#include "Scene.hpp"
#include "Sprite.hpp"
#include "String.hpp"
#include "Bonus.hpp"
#include "Model3D.hpp"
#include "Sphere.hpp"
#include "Grid.hpp"
#include "Velocity.hpp"
#include "Bomb.hpp"
#include "Timer.hpp"
#include "Destructible.hpp"
#include "SoundComponent.hpp"
#include "MusicComponent.hpp"
#include "ParticleCloud.hpp"
#include "ModelAnim.hpp"
#include "Window.hpp"

namespace ecs
{
    static int SCALE = 64;

    std::shared_ptr<Entity> GameSystem::whichEntity(std::string mapAround, int x, int y)
    {
        int lastLine = 15;
        std::shared_ptr<Entity> entity = std::make_shared<Entity>();
        std::string path;

        if (mapAround[0] == 'a')
        {
            if (mapAround[1] == 'a')
            {
                if (mapAround[2] == 'a')
                {
                    if (mapAround[3] == 'a')
                        path = "assets/Blue Ground/Center.png";
                    else
                        path = "assets/Blue Ground/Top.png";
                }
                else
                {
                    if (mapAround[3] == 'a')
                        path = "assets/Blue Ground/Right.png";
                    else
                        path = "assets/Blue Ground/TopRight.png";
                }
            }
            else
            {
                if (mapAround[2] == 'a')
                {
                    if (mapAround[3] == 'a')
                        path = "assets/Blue Ground/Left.png";
                    else
                        path = "assets/Blue Ground/TopLeft.png";
                }
                else
                {
                    if (mapAround[3] == 'a')
                        path = "assets/Blue Ground/LeftRight.png";
                    else
                        path = "assets/Blue Ground/TopLeftRight.png";
                }
            }
        }
        else
        {
            if (mapAround[1] == 'a')
            {
                if (mapAround[2] == 'a')
                {
                    if (mapAround[3] == 'a')
                        path = "assets/Blue Ground/Bot.png";
                    else
                        path = "assets/Blue Ground/TopBot.png";
                }
                else
                {
                    if (mapAround[3] == 'a')
                        path = "assets/Blue Ground/BotRight.png";
                    else
                        path = "assets/Blue Ground/TopBotRight.png";
                }
            }
            else
            {
                if (mapAround[2] == 'a')
                {
                    if (mapAround[3] == 'a')
                        path = "assets/Blue Ground/BotLeft.png";
                    else
                        path = "assets/Blue Ground/TopBotLeft.png";
                }
                else
                {
                    if (mapAround[3] == 'a')
                        path = "assets/Blue Ground/BotLeftRight.png";
                    else
                        path = "assets/Blue Ground/TopBotLeftRight.png";
                }
            }
        }
        std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(path, 0.0f, 2.0f);
        std::shared_ptr<Position> position = std::make_shared<Position>(x * SCALE, (lastLine - y) * SCALE, 0);
        entity->addComponent(sprite).addComponent(position);
        return entity;
    }

    std::unique_ptr<IScene> GameSystem::ReadMap()
    {
        int firstRow = 0;
        int lastRow = 169;
        int firstLine = 0;
        int lastLine = 15;

        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createGameScene, this), SceneType::GAME);

        std::shared_ptr<Entity> BGentity1 = std::make_shared<Entity>();
        std::shared_ptr<Sprite> BGsprite1 = std::make_shared<Sprite>("assets/Background/Background1.png", 0.0f, 3.0f);
        std::shared_ptr<Position> BGposition1 = std::make_shared<Position>(0, 0, 0);
        BGentity1->addComponent(BGsprite1).addComponent(BGposition1);
        scene->addEntity(BGentity1);

        std::shared_ptr<Entity> BGentity2 = std::make_shared<Entity>();
        std::shared_ptr<Sprite> BGsprite2 = std::make_shared<Sprite>("assets/Background/Background2.png", 0.0f, 3.0f);
        std::shared_ptr<Position> BGposition2 = std::make_shared<Position>(0, 0, 0);
        BGentity2->addComponent(BGsprite2).addComponent(BGposition2);
        scene->addEntity(BGentity2);

        std::shared_ptr<Entity> BGentity3 = std::make_shared<Entity>();
        std::shared_ptr<Sprite> BGsprite3 = std::make_shared<Sprite>("assets/Background/Background3.png", 0.0f, 3.0f);
        std::shared_ptr<Position> BGposition3 = std::make_shared<Position>(0, 0, 0);
        BGentity3->addComponent(BGsprite3).addComponent(BGposition3);
        scene->addEntity(BGentity3);

        std::ifstream file;
        file.open("map/Map Test.txt");
        std::string lineOne = "****************";
        std::string lineTwo;
        std::string lineThree;
        std::string strCube;
        std::getline(file, lineTwo);
        std::getline(file, lineThree);
        for (int row = firstRow; row <= lastRow && lineThree != ""; row++)
        {
            if (lineTwo[firstLine] == 'a')
            {
                strCube.clear();
                strCube.push_back('a');
                strCube.push_back(lineOne[firstLine]);
                strCube.push_back(lineThree[firstLine]);
                strCube.push_back(lineTwo[firstLine + 1]);
                scene->addEntity(whichEntity(strCube, row, firstLine));
            }
            if (lineTwo[lastLine] == 'a')
            {
                strCube.clear();
                strCube.push_back(lineTwo[lastLine - 1]);
                strCube.push_back(lineOne[lastLine]);
                strCube.push_back(lineThree[lastLine]);
                strCube.push_back('a');
                scene->addEntity(whichEntity(strCube, row, lastLine));
            }

            for (int line = firstLine; line <= lastLine && line <= lineTwo.size(); line++)
            {
                if (lineTwo[line] == '*')
                    ;
                else if (line != firstLine && line != lastLine && lineTwo[line] == 'a')
                {
                    strCube.clear();
                    strCube.push_back(lineTwo[line - 1]);
                    strCube.push_back(lineOne[line]);
                    strCube.push_back(lineThree[line]);
                    strCube.push_back(lineTwo[line + 1]);
                    scene->addEntity(whichEntity(strCube, row, line));
                }
                else if (lineTwo[line] == 'P')
                    _playerSpawns.push_back({row * SCALE, (lastLine - line) * SCALE, 0});
                else if (lineTwo[line] == '1')
                {
                    std::shared_ptr<Entity> entity = std::make_shared<Entity>();
                    std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Enemies/RedEnemy1.png", 0.0f, 2.0f);
                    std::shared_ptr<Position> position = std::make_shared<Position>(row * SCALE, (lastLine - line) * SCALE, 0);
                    entity->addComponent(sprite).addComponent(position);
                    scene->addEntity(entity);
                }
            }
            lineOne = lineTwo;
            lineTwo = lineThree;
            std::getline(file, lineThree);
            if (lineThree == "" && lineTwo != "****************")
                lineThree = "****************";
        }
        return scene;
    }
}