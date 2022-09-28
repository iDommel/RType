/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Music
*/

#include "raylib.h"

#include <iostream>

#include "Music.hpp"
#include "exceptions/MusicError.hpp"
#include "AudioDevice.hpp"

namespace indie
{
    Music::Music(const std::string &fileName)
    {
        _music = std::make_unique<::Music>();
        *_music = LoadMusicStream(fileName.c_str());
        if (_music->frameCount == 0)
            throw MusicError("Music file not found");
    }

    Music::~Music()
    {
        if (!_music)
            return;
        UnloadMusicStream(*_music);
    }

    void Music::playMusic()
    {
        if (IsMusicStreamPlaying(*_music) == false)
            _state = MusicState::STOP;
        PlayMusicStream(*_music);
        _state = MusicState::PLAY;
    }

    void Music::stopMusic()
    {
        if (_state == MusicState::STOP)
            return;
        StopMusicStream(*_music);
        _state = MusicState::STOP;
    }

    void Music::pauseMusic()
    {
        if (_state == MusicState::PAUSE)
            return;
        PauseMusicStream(*_music);
        _state = MusicState::PAUSE;
    }

    void Music::resumeMusic()
    {
        if (_state != MusicState::PAUSE)
            return;
        ResumeMusicStream(*_music);
        _state = MusicState::PLAY;
    }

    void Music::setVolumeMusic(float volume)
    {
        SetMusicVolume(*_music, volume);
    }

    void Music::updateMusic()
    {
        UpdateMusicStream(*_music);
    }

    float Music::getTimeLengthMusic()
    {
        return GetMusicTimeLength(*_music);
    }

    float Music::getTimePlayedMusic()
    {
        return GetMusicTimePlayed(*_music);
    }

    bool Music::isMusicPlaying()
    {
        return IsMusicStreamPlaying(*_music);
    }
}