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
#include <regex>

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
#include "Trajectory.hpp"
#include "Enemy.hpp"
#include "Wall.hpp"

namespace ecs
{

    std::shared_ptr<Entity> GameSystem::whichEnemy(quint8 mobId, int x, int y)
    {
        if (mobId >= quint8(Enemy::EnemyType::NB))
            return nullptr;

        std::shared_ptr<Entity> entity = std::make_shared<Entity>();
        std::shared_ptr<Position> position = std::make_shared<Position>(x, y, 0);
        Rectangle rect = {position->x + SCALE / 2, position->y + SCALE / 2, SCALE, SCALE};
        std::shared_ptr<Hitbox> hitbox = std::make_shared<Hitbox>(rect);
        std::shared_ptr<Enemy> enemyComponent = nullptr;
        std::shared_ptr<Sprite> sprite = nullptr;
        std::shared_ptr<Trajectory> trajectory = nullptr;

        if (mobId == quint8(Enemy::EnemyType::SCOUT)) {
            sprite = std::make_shared<Sprite>("assets/Enemies/RedEnemy1.png", 0.0f, 2.0f);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_CLASSIC, 8000);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return -a; }),
                                                      std::function<float(float)>([](float a) { return std::sin(a / 10) * 50; }),
                                                      position);
        } else if (mobId == quint8(Enemy::EnemyType::FIGHTER)) {
            sprite = std::make_shared<Sprite>("assets/Enemies/RedEnemy2.png", 0.0f, 2.0f);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_HOMING_MISSILE, 5000);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return -a; }),
                                                      std::function<float(float)>([](float a) { return 0; }),
                                                      position);
        } else if (mobId == quint8(Enemy::EnemyType::TORPEDO)) {
            sprite = std::make_shared<Sprite>("assets/Enemies/RedEnemy3.png", 0.0f, 2.0f);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_SINUSOIDAL, 4000);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return -a; }),
                                                      std::function<float(float)>([](float a) { return 0; }),
                                                      position);
        } else if (mobId == quint8(Enemy::EnemyType::FRIGATE)) {
            sprite = std::make_shared<Sprite>("assets/Enemies/RedEnemy4.png", 0.0f, 2.0f);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_CLASSIC, 8000, 8);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return -a; }),
                                                      std::function<float(float)>([](float a) { return 0; }),
                                                      position);
        }
        entity->addComponent(position)
            .addComponent(enemyComponent)
            .addComponent(hitbox)
            .addComponent(sprite)
            .addComponent(trajectory);
        return entity;
    }

    std::shared_ptr<Entity> GameSystem::whichWall(std::string mapAround, int x, int y)
    {
        int lastLine = 15;
        std::shared_ptr<Entity> entity = std::make_shared<Entity>();
        std::string path = Wall::getCorrespondingPath(mapAround);

        std::shared_ptr<Wall> wallComponent = std::make_shared<Wall>();
        std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(path, 0.0f, 2.0f);
        std::shared_ptr<Position> position = std::make_shared<Position>(x * SCALE, (lastLine - y) * SCALE, 0);
        Rectangle rect = {position->x, position->y, SCALE, SCALE};
        std::shared_ptr<Hitbox> hitbox = std::make_shared<Hitbox>(rect);
        entity->addComponent(sprite)
            .addComponent(position)
            .addComponent(wallComponent)
            .addComponent(hitbox);
        return entity;
    }

    std::unique_ptr<IScene> GameSystem::ReadMap()
    {
        std::regex enemyRegex("[0-9]");
        int firstRow = 0;
        int lastRow = 169;
        int firstLine = 0;
        int lastLine = 15;

        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createGameScene, this), SceneType::GAME);

        std::shared_ptr<Entity> BGentity1 = std::make_shared<Entity>();
        std::shared_ptr<Sprite> BGsprite1 = std::make_shared<Sprite>("assets/Background/Background1.png", 0.0f, 3.0f);
        std::shared_ptr<Position> BGposition1 = std::make_shared<Position>(0, 180, 0);
        BGentity1->addComponent(BGsprite1).addComponent(BGposition1);
        scene->addEntity(BGentity1);

        std::shared_ptr<Entity> BGentity2 = std::make_shared<Entity>();
        std::shared_ptr<Sprite> BGsprite2 = std::make_shared<Sprite>("assets/Background/Background2.png", 0.0f, 3.0f);
        std::shared_ptr<Position> BGposition2 = std::make_shared<Position>(0, 180, 0);
        BGentity2->addComponent(BGsprite2).addComponent(BGposition2);
        scene->addEntity(BGentity2);

        std::shared_ptr<Entity> BGentity3 = std::make_shared<Entity>();
        std::shared_ptr<Sprite> BGsprite3 = std::make_shared<Sprite>("assets/Background/Background3.png", 0.0f, 3.0f);
        std::shared_ptr<Position> BGposition3 = std::make_shared<Position>(0, 180, 0);
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
        for (int row = firstRow; row <= lastRow && lineThree != ""; row++) {
            if (lineTwo[firstLine] == 'a') {
                strCube.clear();
                strCube.push_back('a');
                strCube.push_back(lineOne[firstLine]);
                strCube.push_back(lineThree[firstLine]);
                strCube.push_back(lineTwo[firstLine + 1]);
                scene->addEntity(whichWall(strCube, row, firstLine));
            }
            if (lineTwo[lastLine] == 'a') {
                strCube.clear();
                strCube.push_back(lineTwo[lastLine - 1]);
                strCube.push_back(lineOne[lastLine]);
                strCube.push_back(lineThree[lastLine]);
                strCube.push_back('a');
                scene->addEntity(whichWall(strCube, row, lastLine));
            }

            for (int line = firstLine; line <= lastLine && line <= lineTwo.size(); line++) {
                if (lineTwo[line] == '*')
                    ;
                else if (line != firstLine && line != lastLine && lineTwo[line] == 'a') {
                    strCube.clear();
                    strCube.push_back(lineTwo[line - 1]);
                    strCube.push_back(lineOne[line]);
                    strCube.push_back(lineThree[line]);
                    strCube.push_back(lineTwo[line + 1]);
                    scene->addEntity(whichWall(strCube, row, line));
                } else if (lineTwo[line] == 'P')
                    playerSpawns.push_back({(float)row * SCALE, (lastLine - line) * SCALE, 0});
                else if (lineTwo[line] >= '0' && lineTwo[line] <= '9') {
                    std::shared_ptr<Entity> entity = whichEnemy(lineTwo[line] - '0', row * SCALE, (lastLine - line) * SCALE);
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