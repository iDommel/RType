/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Sound.hpp
*/

#ifndef SOUND_HPP_
#define SOUND_HPP_

#include <string>
#include <memory>

struct Sound;

namespace indie
{

    class Sound
    {
    public:

        /**
         * @brief Construct a new Sound object
         * @param filename Path to the sound file
         */
        Sound(const std::string &filename);
        ~Sound();

        enum class SoundState
        {
            PLAYING,
            STOPPED,
            PAUSED
        };

        /// @brief Play the sound (multiple playing allowed)
        void play();
        /// @brief Stop the sound
        void stop();
        /// @brief Pause the sound
        void pause();
        /// @brief Resume the sound
        void resume();

        /**
         * @brief Set sound volume
         * @param volume Volume value
         */
        void setVolume(float volume);

    private:
        std::unique_ptr<::Sound> _soundRaylib;
    };

}

#endif /* SOUND_HPP_ */