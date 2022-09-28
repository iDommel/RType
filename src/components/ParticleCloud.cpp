/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** ParticleCloud
*/

#include "raylib.h"

#include <iostream>

#include "ParticleCloud.hpp"

namespace indie {
ParticleCloud::ParticleCloud(Vector3 start, Vector3 end, double nbParticles, int curvature, double, float lifeTime):
Component(Type::PARTICLES), _timeToLive(lifeTime)
{
    Vector3 pos = {0, 0, 0};
    Vector3 lenght = {end.x - start.x, end.y - start.y, end.z - start.z};
    Vector3 chunkSize = {lenght.x / curvature, lenght.y / curvature, lenght.z / curvature};

    for (double i = 0; i < nbParticles; i++) {
        std::vector<Vector3> particleParams;
        particleParams.push_back(end);
        for (int j = 1; j < curvature * 2; j++) {
            Vector3 lastPos = particleParams[j - 1];
            if (j % 2 == 0) {
                pos.x = end.x - (chunkSize.x * (j / 2));
                pos.y = end.y - (chunkSize.y * (j / 2));
                pos.z = end.z - (chunkSize.z * (j / 2));
            } else {
                pos.x = random(end.x - (chunkSize.x * (1 + j / 2)), lastPos.x);
                pos.y = random(end.y - (chunkSize.y * (1 + j / 2)), lastPos.y);
                pos.z = random(end.z - (chunkSize.z * (1 + j / 2)), lastPos.z);
            }
            particleParams.push_back(pos);
        }
        particleParams.push_back(start);
        _bezierCurves.push_back(particleParams);
    }
}

static Vector3 getPt(Vector3 one, Vector3 two, int percent)
{
    Vector3 result = {0, 0, 0};
    float realPercent = (float)percent / 100;

    result.x = one.x + ((two.x - one.x) * realPercent);
    result.y = one.y + ((two.y - one.y) * realPercent);
    result.z = one.z + ((two.z - one.z) * realPercent);
    return (result);
}

static Vector3 computeBeziers(std::vector<Vector3> curveParams, float percent)
{
    float chunkSize = 100 / (curveParams.size() / 2);
    int actualChunk = ((int)(percent) / (int)(chunkSize)) * 2;
    int actualPercent = (int)((percent / chunkSize) * 100) % 100;
    size_t chunkCheck = actualChunk + 2;

    if (chunkCheck >= curveParams.size())
        return computeBeziers(curveParams, 99);
    Vector3 p1 = getPt(curveParams.at(actualChunk), curveParams.at(actualChunk + 1), actualPercent);
    Vector3 p2 = getPt(curveParams.at(actualChunk + 1), curveParams.at(actualChunk + 2), actualPercent);
    Vector3 pos = getPt(p1, p2, actualPercent);

    return (pos);
}

std::vector<Vector3> ParticleCloud::getPos(void) const
{
    std::vector<Vector3> positions;

    for (auto &curve : _bezierCurves)
        positions.push_back(computeBeziers(curve, ((_lifeTime * 1.0f) / (_timeToLive * 1.0f)) * 100.0f));
    return positions;
}

bool ParticleCloud::isAlive(void) const
{
    return _timeToLive >= _lifeTime;
}

void ParticleCloud::addTime(uint64_t time)
{
    _lifeTime += time;
}

ParticleCloud::~ParticleCloud()
{
}
}
