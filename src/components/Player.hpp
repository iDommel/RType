/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** player
*/

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "Component.hpp"
#include "EventListener.hpp"
#include "SceneManager.hpp"
#include <QtCore>

namespace ecs
{
    class Velocity;
    class Bonus;

    class Player : public Component
    {
    public:
        enum class Keys {
            UP,
            DOWN,
            LEFT,
            RIGHT,
            BOMB
        };

        Player(int id, int _up, int _down, int _left, int _right, int _bomb, int module);
        ~Player();

        /**
         * @brief Handle the various bonuses
         * @param bonus The Bonus that was given to the player
         */
        void handleBonus(const Bonus &bonus);

        ///@brief gets the player ID
        int getId() const;

        ///@brief gets the current player speed
        int getSpeed() const;

        ///@brief gets the current blast power
        int getBlastPower() const;

        /// @brief Gets the ptr to the space module associated, null otherwise
        std::shared_ptr<IEntity> getSpaceModule();
        /// @brief Associates spaceModule to the player
        void setSpaceModule(std::shared_ptr<IEntity> spaceModule);

        /// @brief sets the velocity of the player to its speed value to the right
        void moveRight(SceneManager &manager, std::shared_ptr<IEntity> entity, float dT);
        /// @brief sets the velocity of the player to 0 to the right
        void stopRight(SceneManager &manager, std::shared_ptr<IEntity> entity, float dT);
        /// @brief sets the velocity of the player to its speed value to the left
        void moveLeft(SceneManager &manager, std::shared_ptr<IEntity> entity, float dT);
        /// @brief sets the velocity of the player to 0 to the left
        void stopLeft(SceneManager &manager, std::shared_ptr<IEntity> entity, float dT);
        /// @brief sets the velocity of the player to its speed value upwards
        void moveUp(SceneManager &manager, std::shared_ptr<IEntity> entity, float dT);
        /// @brief sets the velocity of the player to 0 upwards
        void stopUp(SceneManager &manager, std::shared_ptr<IEntity> entity, float dT);
        /// @brief sets the velocity of the player to its speed value downwards
        void moveDown(SceneManager &manager, std::shared_ptr<IEntity> entity, float dT);
        /// @brief sets the velocity of the player to 0 downwards
        void stopDown(SceneManager &manager, std::shared_ptr<IEntity> entity, float dT);
        // @brief horizontal movement for gamepad sticks
        void moveHorizontal(SceneManager &manager, std::shared_ptr<IEntity> entity, float value);
        /// @brief verticcal movement for gamepad sticks
        void moveVertical(SceneManager &manager, std::shared_ptr<IEntity> entity, float value);
        void bindModule(std::shared_ptr<IEntity> entity);
        /// @brief Set internal clock to now
        void startClock();
        /// @return Returns a reference to the internal clock, if not previously started clock will be null
        QTime &getShootTimer();
        /// @return Returns a reference to the cooldown timer
        QTimer &getShootCooldownTimer();
        /// @return Returns true if the timer has timed out or has not started, false otherwise
        bool hasCooldownTimedOut() const;
        /// @brief Starts (or restarts) the shoot cooldown timer
        /// @param msecs Cooldown time in milliseconds
        void startShootCooldownTimer(std::chrono::milliseconds msecs = std::chrono::milliseconds(250));

        void kill();
        bool isDead() const;
        std::string getUp();
        std::string getDown();
        std::string getLeft();
        std::string getRight();
        std::string getBomb();
        std::string getModule();
        int getTagUp();
        int getTagDown();
        int getTagLeft();
        int getTagRight();
        int getTagBomb();
        int getTagModule();
        void setUP(std::string _up);
        void setDOWN(std::string _down);
        void setLEFT(std::string _left);
        void setRIGHT(std::string _right);
        void setBOMB(std::string _bomb);
        void setModule(std::string module);

        int changeUp;
        int changeDown;
        int changeLeft;
        int changeRight;
        int changeBomb;
        static const int _defaultSpeed;
        static const float maxBoundingDist;

    protected:
    private:
        void move(std::shared_ptr<Velocity> vel);

        int _blastPower;
        int _speed;
        int _id;
        QTime _shootTimer;
        QTimer _shootCooldownTimer;
        bool _isDead = false;
        bool _isUp = false;
        bool _isDown = false;
        bool _isLeft = false;
        bool _isRight = false;
        bool _isModuleBound = false;
        static const int _defaultBlastPower = 3;
        std::string UP;
        std::string DOWN;
        std::string LEFT;
        std::string RIGHT;
        std::string BOMB;
        std::string MODULE;

        std::shared_ptr<IEntity> _spaceModule = nullptr;
    };

}

#endif /* !PLAYER_HPP_ */