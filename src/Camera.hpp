/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Camera
*/

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <memory>

struct Camera3D;
struct Vector3;

namespace indie {
/// @brief raylib camera encapsulation class
class Camera {
    public:
        /**
         * @brief Construct a new Camera object
         * @param target vector the cam point to
         * @param position vector where the cam is
         */
        Camera(Vector3 target, Vector3 position);
        /**
         * @brief Construct a new Camera object with target and position snaped to another entity
         * @param target vector the cam point to, owned by another entity
         * @param position vector where the cam is, owned by another entity
         */
        Camera(std::shared_ptr<Vector3> target, std::shared_ptr<Vector3> position);
        ~Camera();

        /**
         * @brief Set if camera target and position are snapped to another entity
         * @param snapMode true if camera target and position are snapped to another entity
         */
        void setSnapMode(bool snapMode);
        /**
         * @brief Set position
         * @param pos New position
         */
        void setPosition(Vector3 pos);
        /**
         * @brief Set position, owned by another entity
         * @param pos New position
         */
        void setSnapedPosition(std::shared_ptr<Vector3> pos);
        /**
         * @brief Set target coordinates
         * @param target New target coordinates
         */
        void setTarget(Vector3 target);
        /**
         * @brief Set target coordinates, owned by another entity
         * @param target New target coordinates
         */
        void setSnapedTarget(std::shared_ptr<Vector3> target);
        /**
         * @brief Shift position, only if snap mode is off
         * @param shift Vector to add to position
         */
        
        void shiftPosition(Vector3 shift);
        /**
         * @brief Get ID
         * @return integer descriptor of the camera
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
        std::shared_ptr<Vector3> _targetSnap = nullptr;
        /// @brief _snaped position of the camera (as a shared_ptr), used if _snapMode == true
        std::shared_ptr<Vector3> _positionSnap = nullptr;
        /// @brief _snapMode, set at true if you want to use extern values as position and target to auto update them
        bool _snapMode = false;
        /// @brief _id, used to identify the camera
        int _id;
        std::shared_ptr<Camera3D> _camera = nullptr;
};
}

#endif /* !CAMERA_HPP_ */
