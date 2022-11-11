/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** AudioDevice
*/

#include "raylib.h"

#include "AudioDevice.hpp"
#include "exceptions/AudioDeviceError.hpp"
#include   <iostream>

float ecs::AudioDevice::masterVolume = 1.0f;
float ecs::AudioDevice::oldVolume = 1.0f;
bool ecs::AudioDevice::isMute = false;

ecs::AudioDevice::AudioDevice()
{
    InitAudioDevice();
    if (isReady() != true)
        throw AudioDeviceError("AudioDevice already initialized");
    setVolume(.5);
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
    AudioDevice::oldVolume = AudioDevice::masterVolume;
    std::cerr << "Volume: " << volume << std::endl;
    SetMasterVolume(volume);
    AudioDevice::masterVolume = volume;
}