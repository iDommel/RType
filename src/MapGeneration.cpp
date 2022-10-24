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
        std::shared_ptr<Entity> entity = std::make_shared<Entity>();
        std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundCenter.png", 0.0f, 2.0f);
        entity->addComponent(sprite);
        std::shared_ptr<Position> position = std::make_shared<Position>(x * SCALE, (15 - y) * SCALE, 0);
        entity->addComponent(position);
        return entity;
    }

    std::unique_ptr<IScene> GameSystem::ReadMap()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createGameScene, this));

        std::shared_ptr<Entity> BGentity1 = std::make_shared<Entity>();
        std::shared_ptr<Sprite> BGsprite1 = std::make_shared<Sprite>("assets/Background/Background1.png", 0.0f, 3.0f);
        BGentity1->addComponent(BGsprite1);
        std::shared_ptr<Position> BGposition1 = std::make_shared<Position>(0, 0, 0);
        BGentity1->addComponent(BGposition1);
        scene->addEntity(BGentity1);

        std::shared_ptr<Entity> BGentity2 = std::make_shared<Entity>();
        std::shared_ptr<Sprite> BGsprite2 = std::make_shared<Sprite>("assets/Background/Background2.png", 0.0f, 3.0f);
        BGentity2->addComponent(BGsprite2);
        std::shared_ptr<Position> BGposition2 = std::make_shared<Position>(0, 0, 0);
        BGentity2->addComponent(BGposition2);
        scene->addEntity(BGentity2);

        std::shared_ptr<Entity> BGentity3 = std::make_shared<Entity>();
        std::shared_ptr<Sprite> BGsprite3 = std::make_shared<Sprite>("assets/Background/Background3.png", 0.0f, 3.0f);
        BGentity3->addComponent(BGsprite3);
        std::shared_ptr<Position> BGposition3 = std::make_shared<Position>(0, 0, 0);
        BGentity3->addComponent(BGposition3);
        scene->addEntity(BGentity3);

        std::ifstream file;
        file.open("map/Map Test.txt");
        std::string lineOne = "****************";
        std::string lineTwo;
        std::string lineThree;
        std::string strCube;
        std::getline(file, lineTwo);
        std::getline(file, lineThree);
        for (int row = 0; row < 170 && lineThree != ""; row++)
        {
            if (lineTwo[0] == 'a')
            {
                strCube.clear();
                strCube = "***";
                strCube = strCube + lineOne[0] + lineTwo[0] + lineThree[0] + lineOne[1] + lineTwo[1] + lineThree[1];
                scene->addEntity(whichEntity(strCube, row, 0));
            }
            if (lineTwo[15] == 'a')
            {
                strCube.clear();
                strCube = lineOne[14] + lineTwo[14] + lineThree[14] + lineOne[15] + lineTwo[15] + lineThree[15];
                strCube.append("***");
                scene->addEntity(whichEntity(strCube, row, 15));
            }

            for (int line = 0; line < 16 && line <= lineTwo.size(); line++)
            {
                if (lineTwo[line] == '*')
                    ;
                else if (line != 0 && line != 15 && lineTwo[line] == 'a')
                {
                    strCube.clear();
                    strCube[0] = lineOne[line - 1] + lineTwo[line - 1] + lineThree[line - 1] + lineOne[line] + lineTwo[line] + lineThree[line] + lineOne[line + 1] + lineTwo[line + 1] + lineThree[line + 1];
                    scene->addEntity(whichEntity(strCube, row, line));
                }
                else if (lineTwo[line] == 'P')
                {
                    createPlayer(*scene, KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_RIGHT_CONTROL, 1, {row * SCALE, (15 - line) * SCALE, 0});
                }
                else if (lineTwo[line] == '1')
                {
                    std::shared_ptr<Entity> entity = std::make_shared<Entity>();
                    std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Enemies/RedEnemy1.png", 0.0f, 2.0f);
                    entity->addComponent(sprite);
                    std::shared_ptr<Position> position = std::make_shared<Position>(row * SCALE, (15 - line) * SCALE, 0);
                    entity->addComponent(position);
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

// for (int row = 0; row < 170 && lineTwo != ""; row++)
// {
//     for (int line = 0; line < 17 && line <= lineTwo.size(); line++)
//     {
//         if (lineTwo[line] == '*')
//             ;
//         else if (lineTwo[line] == 'a')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundTop.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'b')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundBot.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'c')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundLeft.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'd')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundRight.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'e')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundTopLeft.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'f')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundTopRight.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'g')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundBotLeft.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'h')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundBotRight.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'i')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundCenter.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'j')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundTopBotLeft.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'k')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundTopBotRight.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'l')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundTopLeftRight.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'm')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundBotLeftRight.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'n')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundInAngleTopLeft.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'o')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundInAngleTopRight.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'p')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundInAngleBotLeft.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'q')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundInAngleBotRight.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'r')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundOutAngleTopLeft.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 's')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundOutAngleTopRight.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 't')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundOutAngleBotLeft.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'u')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundOutAngleBotRight.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'v')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundRightLeft.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'w')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundTopBot.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'x')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Blue Ground/BlueGroundTopBotLeftRight.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//         else if (lineTwo[line] == 'P')
//         {
//             createPlayer(*scene, KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_RIGHT_CONTROL, 1, {row * HscaleSpacing, (15 - line) * VscaleSpacing, 0});
//         }
//         else if (lineTwo[line] == '1')
//         {
//             std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//             std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Enemies/RedEnemy1.png", 0.0f, 2.0f);
//             entity->addComponent(sprite);
//             std::shared_ptr<Position> position = std::make_shared<Position>(row * HscaleSpacing, (15 - line) * VscaleSpacing, 0);
//             entity->addComponent(position);
//             // Need to add hitbox but no idea of the size of the grounds in the window
//             scene->addEntity(entity);
//         }
//     }
//     std::getline(file, lineTwo);
// }