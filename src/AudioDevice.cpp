/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** AudioDevice
*/

#include "raylib.h"

#include "AudioDevice.hpp"
#include "exceptions/AudioDeviceError.hpp"

indie::AudioDevice::AudioDevice()
{
    InitAudioDevice();
    if (isReady() != true)
        throw AudioDeviceError("AudioDevice already initialized");
    setVolume(50);
}

indie::AudioDevice::~AudioDevice()
{
    CloseAudioDevice();
}

bool indie::AudioDevice::isReady()
{
    return IsAudioDeviceReady();
}

void indie::AudioDevice::setVolume(float volume)
{
    SetMasterVolume(volume);
}