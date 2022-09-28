/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** AudioSystem.hpp
*/

#ifndef AUDIO_SYSTEM_HPP
#define AUDIO_SYSTEM_HPP

#include "ISystem.hpp"
#include "SceneManager.hpp"
#include "MusicComponent.hpp"
#include "SoundComponent.hpp"
#include "AudioDevice.hpp"

namespace indie
{
    class AudioSystem : public ISystem
    {
    public:
        void init(SceneManager &manager) final;
        void update(SceneManager &manager, uint64_t deltaTime) final;
        void destroy() final;

        /**
         * @brief The callback to be called when an entity is added to a scene
         * @param entity The Entity that was added
         */
        void loadEntity(std::shared_ptr<IEntity> entity) final;
        /**
         * @brief The callback to be called when an entity is removed from a scene
         * @param entity The Entity that was removed
         */
        void unloadEntity(std::shared_ptr<IEntity> entity) final;
        void manageMusic(MusicComponent &music);
        void manageSound(SoundComponent &sound);

    private:
        std::map<std::string, std::unique_ptr<Sound>> _sounds;
        std::map<std::string, std::unique_ptr<Music>> _musics;
        AudioDevice audioDevice;
    };

}

#endif /* AUDIO_SYSTEM_HPP */