/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** ParticleCloud
*/

#ifndef PARTICLECLOUD_HPP_
#define PARTICLECLOUD_HPP_

#include <vector>
#include <random>

#include "Component.hpp"
#include "Position.hpp"

struct Vector3;

namespace indie {

/**
 * @brief Uniformly distributed random number generator
 * @tparam Any ord type (floating point will be truncated)
 * @return A random number between range_from and range_to as a long int
**/
template<typename T>
long int random(T range_from, T range_to) {
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    if (range_from > range_to)
        std::swap(range_from, range_to);
    std::uniform_int_distribution<long int>  distr((long int)range_from, (long int)range_to);
    return distr(generator);
}

class ParticleCloud : public Component {
    public:
        /**
         * @brief Construct a new Particle Cloud object
         * @param start The start position of the cloud
         * @param end The end position of the cloud
         * @param nbParticles The number of particles in the cloud
         * @param curvature The number of bend of each particule (0=straight, 1=curve, 2+=curves)
         * @param dispersion The dispersion of the cloud, like a radius around the followed curve
         * @param lifeTime The lifetime of the cloud, each particule will reach the end vector after this time
        **/
        ParticleCloud(Vector3 start, Vector3 end, double nbParticles, int curvature, double dispersion, float lifeTime);
        ~ParticleCloud();
        
        /**
         * @brief Update the cloud lifeTime
         * @param deltaTime The time since the last update
        **/
        void addTime(uint64_t time);
        /**
         * @brief Get positions of particules to print
         * @return std::vector<Vector3> The positions of particules to print
        **/
        std::vector<Vector3> getPos(void) const;
        /**
         * @brief Ask if the cloud lifeTime is over
         * @return true If the cloud lifeTime is over
         */
        bool isAlive(void) const;
    protected:
    private:
        std::vector<std::vector<Vector3>> _bezierCurves;
        uint64_t _lifeTime = 0;
        uint64_t _timeToLive;
};
}

#endif /* !PARTICLECLOUD_HPP_ */
