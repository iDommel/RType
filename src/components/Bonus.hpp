/*
** EPITECH PROJECT, 2022
** src
** File description:
** Bonus
*/

#ifndef BONUS_HPP_
#define BONUS_HPP_

#include "Component.hpp"

namespace indie {

class Bonus: public Component {
    public:
        enum class Type {
            BOMB,
            SPEED,
            POWER
        };

        Bonus(const Type &type): Component(IComponent::Type::BONUS), _type(type) {};

        /**
         * @brief Get bonus type
         * @return Returns bonus type
         */
        const Type &getBonusType() const { return _type; };

    protected:
    private:
        Type _type;
};

}

#endif /* !BONUS_HPP_ */
