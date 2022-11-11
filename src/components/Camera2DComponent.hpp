/*
** EPITECH PROJECT, 2022
** RType
** File description:
** Camera2DComponent
*/

#ifndef CAMERA2DCOMPONENT_HPP_
#define CAMERA2DCOMPONENT_HPP_

#include "Component.hpp"
#include "Camera2D.hpp"

namespace ecs
{
    class Camera2DComponent : public Component
    {
    public:
        /**
         * @brief Construct a new Camera Component
         * @param target Vector the camera point to
         */
        Camera2DComponent(std::shared_ptr<Position> target);

        /**
         * @brief Get camera object
         * @return Returns a reference to the camera object
         */
        Camera2D &getCamera();

    private:
        Camera2D _camera;
    };
}

#endif /* !CAMERA2DCOMPONENT_HPP_ */
