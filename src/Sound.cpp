/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Sound.cpp
*/

#include "raylib.h"

#include <iostream>

#include "Sound.hpp"
#include "exceptions/SoundError.hpp"

namespace indie
{

    Sound::Sound(const std::string &filename)
    {
        _soundRaylib = std::make_unique<::Sound>();
        *_soundRaylib = LoadSound(filename.c_str());
        if (_soundRaylib->frameCount == 0)
            throw SoundError("Sound file not found");
    }

    Sound::~Sound()
    {
        if (!_soundRaylib)
            return;
        UnloadSound(*_soundRaylib);
        delete _soundRaylib.release();
    }

    void Sound::play()
    {
        PlaySoundMulti(*_soundRaylib);
    }

    void Sound::stop()
    {
        StopSound(*_soundRaylib);
    }

    void Sound::pause()
    {
        PauseSound(*_soundRaylib);
    }

    void Sound::resume()
    {
        ResumeSound(*_soundRaylib);
    }

    void Sound::setVolume(float volume)
    {
        SetSoundVolume(*_soundRaylib, volume);
    }

}
