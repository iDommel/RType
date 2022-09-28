/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** generateMap.cpp
*/

#include "raylib.h"

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <array>

#include "IScene.hpp"
#include "Entity.hpp"
#include "Position.hpp"
#include "Sprite.hpp"
#include "GameSystem.hpp"
#include "Model3D.hpp"
#include "Cube.hpp"
#include "Destructible.hpp"
#include "GameSystem.hpp"

namespace indie
{
    static const std::string tilesFilepath = "assets/ground_asset/sand_asset_basic/basicBeach";
    static const std::string indestructibleBordersFile = "assets/wall asset/plamier_wall/palmier_wall_1";
    static const std::string indestructibleWallFile = "assets/wall asset/plamier_wall/palmier_wall_1";
    static const std::string wallFilepath = "assets/wall asset/breakable_wall/wood_box_breakable";

    static std::shared_ptr<IEntity> createWall(int x, int y)
    {
        float halfSize = GAME_TILE_SIZE / 2;
        std::shared_ptr<Entity> wall = std::make_shared<Entity>();
        BoundingBox boundingBox = {{x - halfSize, -halfSize, y - halfSize}, {x + halfSize, 0, y + halfSize}};

        wall->addComponent(std::make_shared<Position>(x, 0, y))
            .addComponent(std::make_shared<Hitbox>(boundingBox))
            .addComponent(std::make_shared<Destructible>())
            .addComponent(std::make_shared<Model3D>(wallFilepath + ".obj", wallFilepath + ".png"));
        return wall;
    }

    static std::shared_ptr<IEntity> createIndestructibleWall(int x, int y, const std::string &filename)
    {
        float halfSize = GAME_TILE_SIZE / 2;
        std::shared_ptr<Entity> wall = std::make_shared<Entity>();
        BoundingBox boundingBox = {{x - halfSize, -halfSize, y - halfSize}, {x + halfSize, 0, y + halfSize}};

        wall->addComponent(std::make_shared<Position>(x * 1.0f, 0, y * 1.0f))
            .addComponent(std::make_shared<Model3D>(filename + ".obj", filename + ".png"))
            .addComponent(std::make_shared<Hitbox>(boundingBox));
        return wall;
    }

    int createRandomWalls(IScene &scene, std::array<std::array<char, GAME_MAP_WIDTH>, GAME_MAP_HEIGHT> &map)
    {
        for (int n = 0; n < GAME_NB_INDESTRUCTIBLE_WALL; n++) {
            int x = std::rand() % (GAME_MAP_WIDTH - 4) + 2;
            int y = std::rand() % (GAME_MAP_HEIGHT - 4) + 2;
            if (map[y][x] != ' ') {
                n--;
                continue;
            }
            map[y][x] = 'H';
            scene.addEntity(createIndestructibleWall(x * GAME_TILE_SIZE, y * GAME_TILE_SIZE, indestructibleWallFile));
        }
        for (int n = 0; n < GAME_NB_DESTRUCTIBLE_WALL; n++) {
            int x = std::rand() % (GAME_MAP_WIDTH - 2) + 1;
            int y = std::rand() % (GAME_MAP_HEIGHT - 2) + 1;
            if (map[y][x] != ' ' || (x <= 2 && (y <= 2 || y >= GAME_MAP_HEIGHT - 3)) || (x >= GAME_MAP_WIDTH - 3 && (y <= 2 || y >= GAME_MAP_HEIGHT - 3))) {
                n--;
                continue;
            }
            map[y][x] = 'D';
            scene.addEntity(createWall(x * GAME_TILE_SIZE, y * GAME_TILE_SIZE));
        }
        return 0;
    }

    void GameSystem::createSpawn(int x, int y, IScene &scene)
    {
        static unsigned int nb = 0;
        static int keys[4][5] = {
            {KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_RIGHT_CONTROL},
            {KEY_D, KEY_A, KEY_W, KEY_S, KEY_E},
            {KEY_L, KEY_J, KEY_I, KEY_K, KEY_O},
            {KEY_H, KEY_F, KEY_T, KEY_G, KEY_Y}};

        if (nb < nbr_player) {
            createPlayer(scene, keys[nb][0], keys[nb][1], keys[nb][2], keys[nb][3], keys[nb][4], nb + 1, {x * GAME_TILE_SIZE * 1.0f, 0.0f, y * GAME_TILE_SIZE * 1.0f});
            nb++;
        } else if (nb < 4) {
            createAIPlayer(scene, nb + 1, {x * GAME_TILE_SIZE * 1.0f, 0.0f, y * GAME_TILE_SIZE * 1.0f});
            nb++;
        } else {
            nb = 0;
            createSpawn(x, y, scene);
        }
    }

    static std::shared_ptr<IEntity> createGroundTile(int x, int y)
    {
        std::shared_ptr<Entity> groundTile = std::make_shared<Entity>();

        groundTile->addComponent(std::make_shared<Position>(x, 0, y))
            .addComponent(std::make_shared<Model3D>(tilesFilepath + ".obj", tilesFilepath + ".png"));
        return groundTile;
    }

    static std::vector<std::shared_ptr<IEntity>> createTiles()
    {
        std::vector<std::shared_ptr<IEntity>> tiles;
        std::srand(std::time(nullptr));

        for (int y = 0; y < GAME_MAP_HEIGHT; y++) {
            for (int x = 0; x < GAME_MAP_WIDTH; x++) {
                tiles.push_back(createGroundTile(x * GAME_TILE_SIZE, y * GAME_TILE_SIZE));
            }
        }
        return tiles;
    }

    void GameSystem::generateMap(const std::string &filename, IScene &scene)
    {
        std::ifstream file(filename);
        std::string line;
        std::array<std::array<char, GAME_MAP_WIDTH>, GAME_MAP_HEIGHT> map;
        int y = -1;

        if (!file.is_open())
            throw std::runtime_error("Could not open map file");
        for (auto &line : map)
            line.fill(' ');
        scene.addEntities(createTiles());
        while (!file.eof()) {
            y++;
            std::getline(file, line);
            for (size_t i = 0; i < line.size(); i++) {
                if (line[i] == '#') {
                    scene.addEntity(createIndestructibleWall(i * GAME_TILE_SIZE, y * GAME_TILE_SIZE, indestructibleBordersFile));
                    map[y][i] = line[i];
                } else if (line[i] == 'H') {
                    scene.addEntity(createIndestructibleWall(i * GAME_TILE_SIZE, y * GAME_TILE_SIZE, indestructibleWallFile));
                    map[y][i] = line[i];
                } else if (line[i] == 'o')
                    createSpawn(i, y, scene);
            }
        }
        file.close();
        createRandomWalls(scene, map);
    }

}