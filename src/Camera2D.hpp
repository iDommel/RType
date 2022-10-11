/*
** EPITECH PROJECT, 2022
** RType
** File description:
** Camera2D
*/

#ifndef CAMERA2D_HPP_
#define CAMERA2D_HPP_

#include <memory>

struct Camera2D;
struct Vector2;

namespace indie {
/// @brief raylib camera encapsulation class
class Camera2D {
    public:
        /**
         * @brief Construct a new Camera2D object
         * @param target vector the cam point to
         */
        Camera2D(Vector2 target);
        /**
         * @brief Construct a new Camera2D object with target and position snaped to another entity
         * @param target vector the cam point to, owned by another entity
         */
        Camera2D(std::shared_ptr<Vector2> target);
        ~Camera2D();

        /**
         * @brief Set if camera target and position are snapped to another entity
         * @param snapMode true if camera target and position are snapped to another entity
         */
        void setSnapMode(bool snapMode);
        /**
         * @brief Set position
         * @param pos New position
         */
        void setTarget(Vector2 target);
        /**
         * @brief Set target coordinates, owned by another entity
         * @param target New target coordinates
         */
        void setSnapedTarget(std::shared_ptr<Vector2> target);
        /**
         * @brief Shift position, only if snap mode is off
         * @param shift Vector to add to position
         */
        
        int getId(void) const;

        /// @brief Update camera (target and position updated only if snap mode is on)
        void update(void);
        /// @brief Begin drawing scope (should be called before 3d drawing and after the clear)
        void beginDrawScope(void);
        /// @brief End drawing scope (should be called after 3d drawing)
        void endDrawScope(void);

    protected:
    private:
        /// @brief _snaped target of the camera (as a shared_ptr), used if _snapMode == true
        std::shared_ptr<Vector2> _targetSnap = nullptr;
        /// @brief _snaped position of the camera (as a shared_ptr), used if _snapMode == true
        std::shared_ptr<Vector2> _positionSnap = nullptr;
        /// @brief _snapMode, set at true if you want to use extern values as position and target to auto update them
        bool _snapMode = false;
        /// @brief _id, used to identify the camera
        int _id;
        std::shared_ptr<::Camera2D> _camera = nullptr;
};
}

#endif /* !CAMERA2D_HPP_ */
