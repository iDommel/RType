/*
** EPITECH PROJECT, 2022
** Indie
** File description:
** player
*/

#ifndef AIPLAYER_HPP_
#define AIPLAYER_HPP_

#include "Component.hpp"
#include "EventListener.hpp"
#include "SceneManager.hpp"

struct Vector3;

namespace indie
{
    class Velocity;
    class Position;
    class Bonus;

    class AIPlayer : public Component
    {
    public:

        AIPlayer(int id);

        /**
         * @brief Handle the various bonuses
         * @param bonus The Bonus that was given to the player
         */
        void handleBonus(const Bonus &bonus);
        /**
         * @brief Generate a bomb and add it to the entities list
         * @param manager The scene manager
         * @param pos The position of the player
         */
        void generateBomb(SceneManager &manager, Vector3 &pos);
        void updateBombsVec();

        ///@brief gets the player ID
        int getId() const;

        ///@brief gets the current player speed
        int getSpeed() const;

        ///@brief gets the current maximum number of bomb a player can drop
        int getNbBomb() const;
        ///@brief Sets the current maximum number of bomb a player can drop
        void setNbBomb(int newNbBomb);

        void setRadar(std::shared_ptr<IEntity> radar);
        std::shared_ptr<IEntity> getRadar() const;

        /// @brief sets the velocity of the player to its speed value to the right
        void moveRight(std::shared_ptr<IEntity> entity);
        /// @brief sets the velocity of the player to 0 to the right
        void stopRight(std::shared_ptr<IEntity> entity);
        /// @brief sets the velocity of the player to its speed value to the left
        void moveLeft(std::shared_ptr<IEntity> entity);
        /// @brief sets the velocity of the player to 0 to the left
        void stopLeft(std::shared_ptr<IEntity> entity);
        /// @brief sets the velocity of the player to its speed value upwards
        void moveUp(std::shared_ptr<IEntity> entity);
        /// @brief sets the velocity of the player to 0 upwards
        void stopUp(std::shared_ptr<IEntity> entity);
        /// @brief sets the velocity of the player to its speed value downwards
        void moveDown(std::shared_ptr<IEntity> entity);
        /// @brief sets the velocity of the player to 0 downwards
        void stopDown(std::shared_ptr<IEntity> entity);

        bool _isUp = false;
        bool _isDown = false;
        bool _isLeft = false;
        bool _isRight = false;
        int tileX;
        int tileY;

    protected:
    private:
        void move(std::shared_ptr<Velocity> vel);
        size_t _nbBombMax = 1;
        int _blastPower = 3;
        int _speed = 60;
        int _id;
        std::vector<std::shared_ptr<IEntity>> _bombs;
        std::shared_ptr<IEntity> _radar;
    };

}

#endif /* !PLAYER_HPP_ */
