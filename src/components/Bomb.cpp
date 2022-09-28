/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Bomb
*/

#include "raylib.h"

#include "Bomb.hpp"
#include "CollideSystem.hpp"
#include "GameSystem.hpp"
#include "Entity.hpp"
#include "HitboxComponent.hpp"
#include "Timer.hpp"
#include "ParticleCloud.hpp"
#include "SoundComponent.hpp"
#include "Radar.hpp"

#define BOMB_TIMER      3000
#define EXPLOSION_TIMER 500

namespace indie {

Bomb::Bomb(int blastPower): Component(Type::BOMB)
{
    _countdown = BOMB_TIMER;
    _blastPower = blastPower;
}

Bomb::~Bomb()
{
}

void Bomb::setTimer(uint64_t seconds)
{
    if (seconds > _countdown)
        _countdown = 0;
    else
        _countdown = seconds;
}

uint64_t Bomb::getTimer() const
{
    return _countdown;
}

static void createParticlesHorizontal(SceneManager &sceneManager, Vector3 pos, int blastPower)
{
    float range = (((blastPower - 2) * GAME_TILE_SIZE - 2) + (GAME_TILE_SIZE / 2));

    pos.z -= (GAME_TILE_SIZE / 2);
    for (int i = GAME_TILE_SIZE; i > 0; i--) {
        Vector3 targetEast = {(pos.x + range), 5.0f, pos.z + i};
        Vector3 targetWest = {(pos.x - range), 5.0f, pos.z + i};
        Vector3 actualPos = {pos.x, 5.0f, pos.z + i};
        std::shared_ptr<Entity> boom1 = std::make_shared<Entity>();
        std::shared_ptr<Entity> boom2 = std::make_shared<Entity>();

        boom1->addComponent(std::make_shared<ParticleCloud>(targetEast, actualPos, 1, 2, -1, EXPLOSION_TIMER));
        boom2->addComponent(std::make_shared<ParticleCloud>(targetWest, actualPos, 1, 2, -1, EXPLOSION_TIMER));
        sceneManager.getCurrentScene().addEntities({boom1, boom2});
    }
}

static void createParticlesVertical(SceneManager &sceneManager, Vector3 pos, int blastPower)
{
    float range = (((blastPower - 2) * GAME_TILE_SIZE - 2) + (GAME_TILE_SIZE / 2));

    pos.x -= (GAME_TILE_SIZE / 2);
    for (int i = GAME_TILE_SIZE; i > 0; i--) {
        Vector3 targetSouth = {pos.x + i, 5.0f, (pos.z + range)};
        Vector3 targetNorth = {pos.x + i, 5.0f, (pos.z - range)};
        Vector3 actualPos = {pos.x + i, 5.0f, pos.z};
        std::shared_ptr<Entity> boom1 = std::make_shared<Entity>();
        std::shared_ptr<Entity> boom2 = std::make_shared<Entity>();

        boom1->addComponent(std::make_shared<ParticleCloud>(targetSouth, actualPos, 1, 2, -1, EXPLOSION_TIMER));
        boom2->addComponent(std::make_shared<ParticleCloud>(targetNorth, actualPos, 1, 2, -1, EXPLOSION_TIMER));
        sceneManager.getCurrentScene().addEntities({boom1, boom2});
    }
}

void Bomb::explode(SceneManager &sceneManager, Vector3 pos)
{
    std::shared_ptr<Entity> explosion = std::make_shared<Entity>();
    std::shared_ptr<Entity> explosion2 = std::make_shared<Entity>();
    std::shared_ptr<Entity> sonicBoom = std::make_shared<Entity>();

    calculateExplosionRadius(sceneManager.getCurrentScene(), pos);
    sonicBoom->addComponent(std::make_shared<SoundComponent>("sound_expl"));
    sceneManager.getCurrentScene().addEntities({sonicBoom});
    createParticlesHorizontal(sceneManager, pos, _blastPower);
    createParticlesVertical(sceneManager, pos, _blastPower);
}

void Bomb::calculateExplosionRadius(IScene &scene, Vector3 pos)
{
    float range = GAME_TILE_SIZE * _blastPower - 2;
    Vector3 radarSize = {range, GAME_TILE_SIZE, range};
    Vector3 radarPos = {pos.x - radarSize.x / 2, pos.y, pos.z - radarSize.z / 2};
    std::shared_ptr<Hitbox> radarBox = std::make_shared<Hitbox>(CollideSystem::makeBBoxFromSizePos(radarSize, radarPos));
    std::shared_ptr<Entity> radar = std::make_shared<Entity>();
    std::shared_ptr<Radar> radarR = std::make_shared<Radar>();

    float demiTile = GAME_TILE_SIZE / 2;
    Vector3 sizeX = {range, 5.0f, GAME_TILE_SIZE - 2};
    Vector3 posX = {pos.x - (range - GAME_TILE_SIZE)/2 - demiTile, pos.y, pos.z - demiTile};
    Vector3 sizeZ = {GAME_TILE_SIZE - 2, 5.0f, range};
    Vector3 posZ = {pos.x - demiTile, pos.y, pos.z - (range - GAME_TILE_SIZE)/2 - demiTile};

    radar->addComponent(radarR)
        .addComponent(radarBox);
    scene.addEntity(radar);
    createExplosion(posX, sizeX, scene);
    createExplosion(posZ, sizeZ, scene);
    scene.removeEntity(radar);
}

void Bomb::createExplosion(Vector3 pos, Vector3 size, IScene &scene)
{
    std::shared_ptr<Entity> explosion = std::make_shared<Entity>();
    auto c = std::make_shared<Hitbox>(CollideSystem::makeBBoxFromSizePos(size, pos));
    auto timer = std::make_shared<Timer>(EXPLOSION_TIMER);

    explosion->addComponents({c, timer});
    scene.addEntity(explosion);
}

}