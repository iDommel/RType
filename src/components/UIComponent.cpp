/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** UIComponent
*/

#include "raylib.h"

#include "UIComponent.hpp"
#include "Position.hpp"
#include "Window.hpp"

namespace indie
{

    UIComponent::UIComponent(std::shared_ptr<IEntity> player, std::shared_ptr<IEntity> text) : Component(Type::UI)
    {
        _player = Component::castComponent<Player>((*player)[Component::Type::PLAYER]);
        _text = Component::castComponent<String>((*text)[IComponent::Type::TEXT]);
        _textToDestroy = text;
        int id = _player->getId();
        int nbBombs = _player->getNbBomb();
        int speed = _player->getSpeed();
        int power = _player->getBlastPower();
        std::string displayString = "Player " + std::to_string(id) + ":\nBombs : " + std::to_string(nbBombs) + "\nSpeed : " + std::to_string(speed) + "\nBlast Power : " + std::to_string(power);
        _text->getValue() = displayString;
    }

    UIComponent::~UIComponent()
    {
    }

    void UIComponent::update()
    {
        const int offset = 20;
        const int textWidth = 150;
        const Position uiPos[4] = {
            Position(offset, offset),
            Position(Window::getScreenWidth() - (textWidth + offset), offset),
            Position(offset, Window::getScreenHeight() - (textWidth + offset)),
            Position(Window::getScreenWidth() - (textWidth + offset), Window::getScreenHeight() - (textWidth + offset))};

        if (_player && _text && !_player->isDead()) {
            auto textPos = Component::castComponent<Position>((*_textToDestroy)[IComponent::Type::POSITION]);
            int id = _player->getId();
            *textPos = uiPos[id - 1];
            int nbBombs = _player->getNbBomb();
            int speed = _player->getSpeed();
            int power = _player->getBlastPower();
            std::string displayString = "Player " + std::to_string(id) + ":\nBombs : " + std::to_string(nbBombs) + "\nSpeed : " + std::to_string(speed) + "\nBlast Power : " + std::to_string(power);
            _text->getValue() = displayString;
        } else {
            _shouldBeDestroyed = true;
        }
    }

    bool UIComponent::shouldBeDestroyed() const
    {
        return _shouldBeDestroyed;
    }

    std::shared_ptr<IEntity> UIComponent::getTextToDestroy()
    {
        return _textToDestroy;
    }
} 
