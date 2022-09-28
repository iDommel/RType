/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Music
*/

#ifndef MUSIC_HPP_
#define MUSIC_HPP_

#include <string>
#include <memory>

struct Music;

namespace indie {
    class Music {
        public:
            ///  @brief Constructor a new music:: load music from file
            Music(const std::string &fileName);
            ///  @brief Destruct the music:: unload music
            ~Music();

            enum class MusicState {
                PLAY,
                PAUSE,
                STOP
            };

            ///  @brief Start music
            void playMusic();
            ///  @brief Stop music
            void stopMusic();
            ///  @brief Pause music
            void pauseMusic();
            ///  @brief Resume playing paused music
            void resumeMusic();
            ///  @brief Set volume for music (1.0 is max level)
            void setVolumeMusic(float volume);
            ///  @brief Updates buffers for music
            void updateMusic();
            ///  @brief Get music time length (in seconds)
            float getTimeLengthMusic();
            ///  @brief Get current music time played (in seconds)
            float getTimePlayedMusic();
            /// @brief Check if music is playing
            bool isMusicPlaying();

            MusicState &getMusic() { return _state; };

        protected:
        private:
            MusicState _state = MusicState::PAUSE;
            std::unique_ptr<::Music> _music;
    };
}

#endif /* !MUSIC_HPP_ */
