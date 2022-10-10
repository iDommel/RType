/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** AudioSystem.cpp
*/

#include "raylib.h"

#include <iostream>

#include "AudioSystem.hpp"
#include "Sound.hpp"
#include "Music.hpp"
#include "Component.hpp"
#include "MusicComponent.hpp"
#include "AudioDevice.hpp"

namespace indie
{

    void AudioSystem::init(SceneManager &sceneManager)
    {
        if (AudioDevice::isReady()) {
            AudioDevice::setVolume(100);
        }
        for (auto &scene : sceneManager.getScenes()) {
            for (auto &entity : (*scene.second)[IEntity::Tags::MUSIC])
                loadMusic(entity);
            for (auto &e : (*scene.second)[IEntity::Tags::SOUND])
                loadSound(e);
        }
    }

    void AudioSystem::loadMusic(std::shared_ptr<IEntity> entity)
    {
        auto music = Component::castComponent<MusicComponent>((*entity)[IComponent::Type::MUSIC]);

        _musics.emplace(music->getValue(), std::make_unique<Music>(music->getValue()));
    }

    void AudioSystem::loadSound(std::shared_ptr<IEntity> entity)
    {
        auto sound = Component::castComponent<SoundComponent>((*entity)[IComponent::Type::SOUND]);

        _sounds.emplace(sound->getValue(), std::make_unique<Sound>(sound->getValue()));
    }

    void AudioSystem::update(SceneManager &sceneManager, uint64_t)
    {
        if (!AudioDevice::isReady())
            return;
        for (auto &e : sceneManager.getCurrentScene()[IEntity::Tags::AUDIBLE]) {
            if ((*e)[IComponent::Type::MUSIC] == nullptr)
                continue;
            auto music = Component::castComponent<MusicComponent>((*e)[IComponent::Type::MUSIC]);
            manageMusic(*music);
        }
        for (auto &e : sceneManager.getCurrentScene()[IEntity::Tags::AUDIBLE]) {
            if ((*e)[IComponent::Type::SOUND] == nullptr)
                continue;
            auto sound = Component::castComponent<SoundComponent>((*e)[IComponent::Type::SOUND]);
            manageSound(*sound);
        }
    }

    void AudioSystem::destroy()
    {
        std::cerr << "AudioSystem::destroy" << std::endl;
    }

    void AudioSystem::loadEntity(std::shared_ptr<IEntity>)
    {
    }

    void AudioSystem::unloadEntity(std::shared_ptr<IEntity>)
    {
    }

    void AudioSystem::manageMusic(MusicComponent &musicComponent)
    {
        _musics[musicComponent.getValue()]->updateMusic();
        if (musicComponent.getMusicState() == _musics[musicComponent.getValue()]->getMusic())
            return;
        if (musicComponent.getMusicState() == Music::MusicState::PLAY) {
            _musics[musicComponent.getValue()]->playMusic();
        } else if (musicComponent.getMusicState() == Music::MusicState::PAUSE)
            _musics[musicComponent.getValue()]->pauseMusic();
        else if (musicComponent.getMusicState() == Music::MusicState::STOP)
            _musics[musicComponent.getValue()]->stopMusic();
        _musics[musicComponent.getValue()]->getMusic() = musicComponent.getMusicState();
    }

    void AudioSystem::manageSound(SoundComponent &soundComponent)
    {
        if (!soundComponent.shouldUpdate())
            return;
        if (soundComponent.getSoundState() == Sound::SoundState::PLAYING)
            _sounds[soundComponent.getValue()]->play();
        else if (soundComponent.getSoundState() == Sound::SoundState::PAUSED)
            _sounds[soundComponent.getValue()]->pause();
        else if (soundComponent.getSoundState() == Sound::SoundState::STOPPED)
            _sounds[soundComponent.getValue()]->stop();
        soundComponent.justUpdated();
    }
}