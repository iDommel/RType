/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** ModelAnimation
*/

#include "raylib.h"

#include "ModelAnimation.hpp"

namespace indie {
ModelAnimation::ModelAnimation(const std::string &fileName)
{
    anims = LoadModelAnimations(fileName.c_str(), &_animsCount);
    _isLoaded = true;
}

ModelAnimation::~ModelAnimation()
{
    if (_isLoaded)
        unloadModelAnimation();
}

void ModelAnimation::updateModelAnimation(Model &model, int frame)
{
    ::UpdateModelAnimation(model.getModel(), anims[0], frame);
}

void ModelAnimation::unloadModelAnimation()
{
    for (unsigned int i = 0; i < _animsCount; ++i)
        UnloadModelAnimation(anims[i]);
    _isLoaded = false;
}

bool ModelAnimation::isValid(::Model model)
{
    return (IsModelAnimationValid(model, anims[0]));
}

int ModelAnimation::getFrameCount()
{
    if (anims != NULL)
        return (anims[0].frameCount);
    return (0);
}
}