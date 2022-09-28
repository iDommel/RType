/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** CameraComponent.hpp
*/

#ifndef CAMERA_COMPONENT_HPP_
#define CAMERA_COMPONENT_HPP_

#include "Component.hpp"
#include "Camera.hpp"

namespace indie
{
    class CameraComponent : public Component
    {
    public:
        /**
         * @brief Construct a new Camera Component
         * @param target Vector the camera point to
         * @param position Vector where the camera is
         */
        CameraComponent(Vector3 target, Vector3 position);

        /**
         * @brief Get camera object
         * @return Returns a reference to the camera object
         */
        Camera &getCamera();

    private:
        Camera _camera;
    };
}

#endif /* !CAMERA_COMPONENT_HPP_ */