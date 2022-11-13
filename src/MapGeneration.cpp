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

    void GameSystem::createEnemy(IScene &scene, Enemy::EnemyType mobId, int x, int y, QUuid id)
    {
        if (quint8(mobId) >= quint8(Enemy::EnemyType::NB))
            throw std::invalid_argument("Enemy factory: invalid enemy type");

        std::shared_ptr<Entity> entity = std::make_shared<Entity>(id);
        std::shared_ptr<Position> position = std::make_shared<Position>(x, y, 0);
        Rectangle rect = {position->x + SCALE / 2, position->y + SCALE / 2, SCALE, SCALE};
        std::shared_ptr<Hitbox> hitbox = std::make_shared<Hitbox>(rect);
        std::shared_ptr<Enemy> enemyComponent = nullptr;
        std::shared_ptr<Sprite> sprite = nullptr;
        std::shared_ptr<Trajectory> trajectory = nullptr;
        std::shared_ptr<Animation2D> animation = nullptr;

        if (mobId == Enemy::EnemyType::REDSCOUT) {
            sprite = std::make_shared<Sprite>("assets/Enemies/RedEnemy1/RedEnemy1SS.png", 180.0f, 2.0f);
            animation = std::make_shared<Animation2D>(10, 6, Animation2D::AnimationType::LOOP);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_HOMING_RED1, 5500);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return -a; }),
                                                      std::function<float(float)>([](float a) { return std::sin(a / 10) * 50; }),
                                                      position);
        } else if (mobId == Enemy::EnemyType::REDFIGHTER) {
            sprite = std::make_shared<Sprite>("assets/Enemies/RedEnemy2/RedEnemy2SS.png", 180.0f, 2.0f);
            animation = std::make_shared<Animation2D>(10, 6, Animation2D::AnimationType::LOOP);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_RED2, 3000);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return -a; }),
                                                      std::function<float(float)>([](float a) { return 0; }),
                                                      position);
        } else if (mobId == Enemy::EnemyType::REDTORPEDO) {
            sprite = std::make_shared<Sprite>("assets/Enemies/RedEnemy3/RedEnemy3SS.png", 180.0f, 2.0f);
            animation = std::make_shared<Animation2D>(10, 6, Animation2D::AnimationType::LOOP);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_RED3, 4000, 3);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return -a; }),
                                                      std::function<float(float)>([](float a) { return 0; }),
                                                      position);
        } else if (mobId == Enemy::EnemyType::REDFRIGATE) {
            sprite = std::make_shared<Sprite>("assets/Enemies/RedEnemy4/RedEnemy4SS.png", 180.0f, 2.0f);
            animation = std::make_shared<Animation2D>(12, 6, Animation2D::AnimationType::LOOP);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_HOMING_RED4, 6000, 8);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return -a; }),
                                                      std::function<float(float)>([](float a) { return 0; }),
                                                      position);
        } else if (mobId == Enemy::EnemyType::REDTURRET) {
            sprite = std::make_shared<Sprite>("assets/Enemies/RedEnemy5/RedEnemy5.png", 0.0f, 2.0f);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_HOMING_RED5, 5000);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return 0; }),
                                                      std::function<float(float)>([](float a) { return 0; }),
                                                      position);
        } else if (mobId == Enemy::EnemyType::BROWNSCOUT) {
            sprite = std::make_shared<Sprite>("assets/Enemies/BrownEnemy1/BrownEnemy1SS.png", 180.0f, 2.0f);
            animation = std::make_shared<Animation2D>(8, 6, Animation2D::AnimationType::LOOP);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_BROWN1, 8000);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return -a; }),
                                                      std::function<float(float)>([](float a) { return std::sin(a / 10) * 50; }),
                                                      position);
        } else if (mobId == Enemy::EnemyType::BROWNFIGHTER) {
            sprite = std::make_shared<Sprite>("assets/Enemies/BrownEnemy2/BrownEnemy2SS.png", 180.0f, 2.0f);
            animation = std::make_shared<Animation2D>(8, 6, Animation2D::AnimationType::LOOP);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_BROWN2, 5000);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return -a; }),
                                                      std::function<float(float)>([](float a) { return 0; }),
                                                      position);
        } else if (mobId == Enemy::EnemyType::BROWNTORPEDO) {
            sprite = std::make_shared<Sprite>("assets/Enemies/BrownEnemy3/BrownEnemy3SS.png", 180.0f, 2.0f);
            animation = std::make_shared<Animation2D>(8, 6, Animation2D::AnimationType::LOOP);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_HOMING_BROWN3, 4000);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return -a; }),
                                                      std::function<float(float)>([](float a) { return 0; }),
                                                      position);
        } else if (mobId == Enemy::EnemyType::BROWNFRIGATE) {
            sprite = std::make_shared<Sprite>("assets/Enemies/BrownEnemy4/BrownEnemy4SS.png", 180.0f, 2.0f);
            animation = std::make_shared<Animation2D>(8, 6, Animation2D::AnimationType::LOOP);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_BROWN4, 8000, 8);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return -a; }),
                                                      std::function<float(float)>([](float a) { return 0; }),
                                                      position);
        } else if (mobId == Enemy::EnemyType::BROWNTURRET) {
            sprite = std::make_shared<Sprite>("assets/Enemies/BrownEnemy5/BrownEnemy5.png", 0.0f, 2.0f);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_HOMING_BROWN5, 5000);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return 0; }),
                                                      std::function<float(float)>([](float a) { return 0; }),
                                                      position);
        } else if (mobId == Enemy::EnemyType::GREENSCOUT) {
            sprite = std::make_shared<Sprite>("assets/Enemies/GreenEnemy1/GreenEnemy1SS.png", 180.0f, 2.0f);
            animation = std::make_shared<Animation2D>(8, 6, Animation2D::AnimationType::LOOP);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_HOMING_GREEN1, 8000);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return -a; }),
                                                      std::function<float(float)>([](float a) { return std::sin(a / 10) * 50; }),
                                                      position);
        } else if (mobId == Enemy::EnemyType::GREENFIGHTER) {
            sprite = std::make_shared<Sprite>("assets/Enemies/GreenEnemy2/GreenEnemy2SS.png", 180.0f, 2.0f);
            animation = std::make_shared<Animation2D>(8, 6, Animation2D::AnimationType::LOOP);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_HOMING_GREEN2, 5000);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return -a; }),
                                                      std::function<float(float)>([](float a) { return 0; }),
                                                      position);
        } else if (mobId == Enemy::EnemyType::GREENTORPEDO) {
            sprite = std::make_shared<Sprite>("assets/Enemies/GreenEnemy3/GreenEnemy3SS.png", 180.0f, 2.0f);
            animation = std::make_shared<Animation2D>(8, 6, Animation2D::AnimationType::LOOP);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_HOMING_GREEN3, 4000);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return -a; }),
                                                      std::function<float(float)>([](float a) { return 0; }),
                                                      position);
        } else if (mobId == Enemy::EnemyType::GREENFRIGATE) {
            sprite = std::make_shared<Sprite>("assets/Enemies/GreenEnemy4/GreenEnemy4SS.png", 180.0f, 2.0f);
            animation = std::make_shared<Animation2D>(8, 6, Animation2D::AnimationType::LOOP);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_HOMING_GREEN4, 8000, 8);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return -a; }),
                                                      std::function<float(float)>([](float a) { return 0; }),
                                                      position);
        } else if (mobId == Enemy::EnemyType::GREENTURRET) {
            sprite = std::make_shared<Sprite>("assets/Enemies/GreenEnemy5/GreenEnemy5.png", 0.0f, 2.0f);
            enemyComponent = std::make_shared<Enemy>(mobId, Missile::MissileType::E_HOMING_GREEN5, 5000);
            trajectory = std::make_shared<Trajectory>(std::function<float(float)>([](float a) { return 0; }),
                                                      std::function<float(float)>([](float a) { return 0; }),
                                                      position);
        }
        entity->addComponent(position)
            .addComponent(enemyComponent)
            .addComponent(hitbox)
            .addComponent(sprite)
            .addComponent(trajectory);
        if (animation)
            entity->addComponent(animation);
        scene.addEntity(entity);
        std::cout << "Enemy created" << std::endl;
    }

    std::shared_ptr<Entity> GameSystem::whichWall(std::string mapAround, int x, int y)
    {
        int lastLine = 16;
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
        int lastLine = 16;

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
                    playerSpawns.push_back({(float)row * (float)SCALE, (lastLine - line) * (float)SCALE, 0});
                else if (lineTwo[line] >= '0' && lineTwo[line] <= '9') {
                    // std::shared_ptr<Entity> entity = whichEnemy(lineTwo[line] - '0', row * SCALE, (lastLine - line) * SCALE);
                    // scene->addEntity(entity);
                    GameSystem::enemies.push_back(std::make_pair(Enemy::EnemyType(lineTwo[line] - '0'), Position(row * SCALE, (lastLine - line) * SCALE)));
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