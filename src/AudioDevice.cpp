/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** AudioDevice
*/

#include "raylib.h"

#include "AudioDevice.hpp"
#include "exceptions/AudioDeviceError.hpp"

ecs::AudioDevice::AudioDevice()
{
    InitAudioDevice();
    if (isReady() != true)
        throw AudioDeviceError("AudioDevice already initialized");
    setVolume(50);
}

ecs::AudioDevice::~AudioDevice()
{
    CloseAudioDevice();
}

bool ecs::AudioDevice::isReady()
{
    return IsAudioDeviceReady();
}

void ecs::AudioDevice::setVolume(float volume)
{
    SetMasterVolume(volume);
}