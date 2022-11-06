/*
** EPITECH PROJECT, 2022
** RType
** File description:
** Camera2D
*/

#ifndef CAMERA2D_HPP_
#define CAMERA2D_HPP_

#include <memory>
#include "Position.hpp"

struct Camera2D;
struct Vector2;

namespace ecs
{
    /// @brief raylib camera encapsulation class
    class Camera2D
    {
    public:
        /**
         * @brief Construct a new Camera2D object
         * @param target vector the cam point to
         */
        Camera2D(Vector2 target);
        /**
         * @brief Construct a new Camera2D object with target snaped to another entity
         * @param target vector the cam point to, owned by another entity
         */
        Camera2D(std::shared_ptr<Position> target);
        ~Camera2D();

        /**
         * @brief Set if camera target are snapped to another entity
         * @param snapMode true if camera target are snapped to another entity
         */
        void setSnapMode(bool snapMode);
        /**
         * @brief Set target
         * @param target New target
         */
        void setTarget(Vector2 target);
        /**
         * @brief Set target coordinates, owned by another entity
         * @param target New target coordinates
         */
        void setSnapedTarget(std::shared_ptr<Position> target);

        int getId(void) const;

        /// @brief Update camera (target updated only if snap mode is on)
        void update(void);
        /// @brief Begin drawing scope (should be called before 2d drawing and after the clear)
        void beginDrawScope(void);
        /// @brief End drawing scope (should be called after 2d drawing)
        void endDrawScope(void);

    protected:
    private:
        /// @brief _snaped target of the camera (as a shared_ptr), used if _snapMode == true
        std::shared_ptr<Position> _targetSnap = nullptr;
        /// @brief _snapMode, set at true if you want to use extern value target to auto update it
        bool _snapMode = false;
        /// @brief _id, used to identify the camera
        int _id;
        std::shared_ptr<::Camera2D> _camera = nullptr;
    };
}

#endif /* !CAMERA2D_HPP_ */
