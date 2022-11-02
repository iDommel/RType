/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Camera3DComponent.hpp
*/

#ifndef CAMERA_COMPONENT_HPP_
#define CAMERA_COMPONENT_HPP_

#include "Component.hpp"
#include "Camera3D.hpp"

namespace ecs
{
    class Camera3DComponent : public Component
    {
    public:
        /**
         * @brief Construct a new Camera Component
         * @param target Vector the camera point to
         * @param position Vector where the camera is
         */
        Camera3DComponent(Vector3 target, Vector3 position);

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