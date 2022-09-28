/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Bomb
*/

#ifndef BOMB_HPP_
#define BOMB_HPP_

#include "Component.hpp"
#include "SceneManager.hpp"

struct Vector3;

namespace indie {

class Bomb: public Component{
    public:
        Bomb(int blastPower = 1);
        ~Bomb();

        ///@brief Explode the bomb after a certain amount of time
        void explode(SceneManager &sceneManager, Vector3 position);

        /**
         * @brief Set the timer of the bomb
         * @param time The time in seconds
         */
        void setTimer(uint64_t time);

        /**
         * @brief Get the timer of the bomb
         * @return The time in seconds
         */
        uint64_t getTimer() const;
    protected:
    private:
        void calculateExplosionRadius(IScene &scene, Vector3 pos);
        void createExplosion(Vector3 pos, Vector3 size, IScene &scene);

        uint64_t _countdown;
        int _blastPower;
};

}

#endif /* !BOMB_HPP_ */