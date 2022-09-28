/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** UIComponent
*/

#ifndef UICOMPONENT_HPP_
#define UICOMPONENT_HPP_

#include "Player.hpp"
#include "String.hpp"

namespace indie
{

    class UIComponent : public Component
    {
    public:
        UIComponent(std::shared_ptr<IEntity> player, std::shared_ptr<IEntity> text);
        ~UIComponent();
        void update();
        bool shouldBeDestroyed() const;
        std::shared_ptr<IEntity> getTextToDestroy();

    protected:
    private:
        std::shared_ptr<Player> _player;
        std::shared_ptr<String> _text;
        std::shared_ptr<IEntity> _textToDestroy;
        bool _shouldBeDestroyed = false;
    };
}

#endif /* !UICOMPONENT_HPP_ */
