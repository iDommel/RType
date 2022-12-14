/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** Music
*/

#ifndef MUSICCOMPONENT_HPP_
#define MUSICCOMPONENT_HPP_

#include <string>

#include "Component.hpp"
#include "Music.hpp"

namespace ecs
{
    class MusicComponent : public Component
    {
    public:
        MusicComponent(std::string str) : Component(Type::MUSIC), music(str){};

        std::string getValue() const { return music; };
        void setMusicState(Music::MusicState state) { _newState = state; };
        Music::MusicState getMusicState() { return _newState; };

    private:
        Music::MusicState _newState = Music::MusicState::PLAY;
        std::string music;
    };
}

#endif /* !MUSIC_HPP_ */
