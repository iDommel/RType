/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** AudioDevice
*/

#ifndef AUDIODEVICE_HPP_
#define AUDIODEVICE_HPP_

namespace indie
{
    class AudioDevice
    {
    public:
        ///  @brief Constructor a new audio device
        AudioDevice();
        ///  @brief Destruct the audio device:: close the audio device
        ~AudioDevice();
        /// @brief Init audio device
        void initAudio();
        ///  @brief isReady:: Check if the audio device is playing
        static bool isReady();
        ///  @brief Set music volume
        static void setVolume(float volume);

        static float &getMasterVolume() {static float masterVolume; return masterVolume;};
    protected:
    private:
        static float masterVolume;
    };
}

#endif /* !AUDIODEVICE_HPP_ */
