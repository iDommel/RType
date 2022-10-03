/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** AudioDevice
*/

#include "raylib.h"

#include "AudioDevice.hpp"
#include "exceptions/AudioDeviceError.hpp"

rtype::AudioDevice::AudioDevice()
{
    InitAudioDevice();
    if (isReady() != true)
        throw AudioDeviceError("AudioDevice already initialized");
    setVolume(50);
}

rtype::AudioDevice::~AudioDevice()
{
    CloseAudioDevice();
}

bool rtype::AudioDevice::isReady()
{
    return IsAudioDeviceReady();
}

void rtype::AudioDevice::setVolume(float volume)
{
    SetMasterVolume(volume);
}