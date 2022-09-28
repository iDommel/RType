/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** SoundComponent
*/

#ifndef SOUNDCOMPONENT_HPP_
#define SOUNDCOMPONENT_HPP_

#include <string>

#include "Component.hpp"
#include "Sound.hpp"

namespace indie
{
    class SoundComponent : public Component
    {
    public:
        SoundComponent(std::string str) : Component(Type::SOUND), sound(str) {};

        std::string getValue() const { return sound; };
        void setSoundState(Sound::SoundState state) { _newState = state; };
        Sound::SoundState getSoundState() { return _newState; };
        bool shouldUpdate() const { return _justRequested; };
        void justUpdated() { _justRequested = false; };

    private:
        Sound::SoundState _newState = Sound::SoundState::PLAYING;
        bool _justRequested = true;
        std::string sound;
    };
}

#endif /* !SOUNDCOMPONENT_HPP_ */
