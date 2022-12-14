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

namespace ecs
{

    void AudioSystem::init(SceneManager &sceneManager)
    {
        std::cerr << "AudioSystem init" << std::endl;
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

        if (_musics.find(music->getValue()) == _musics.end())
            _musics.emplace(music->getValue(), std::make_unique<Music>(music->getValue()));
    }

    void AudioSystem::loadSound(std::shared_ptr<IEntity> entity)
    {
        auto sound = Component::castComponent<SoundComponent>((*entity)[IComponent::Type::SOUND]);

        if (_sounds.find(sound->getValue()) == _sounds.end())
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

    void AudioSystem::onEntityAdded(std::shared_ptr<IEntity> entity, IScene &)
    {
        if (entity->hasComponent(IComponent::Type::SOUND))
            loadSound(entity);
        else if (entity->hasComponent(IComponent::Type::MUSIC))
            loadMusic(entity);
    }

    void AudioSystem::onEntityRemoved(std::shared_ptr<IEntity>, IScene &)
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
        if (!soundComponent.shouldUpdate() || _sounds.find(soundComponent.getValue()) == _sounds.end())
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