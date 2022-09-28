/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Hitbox
*/

#ifndef HITBOX_HPP_
#define HITBOX_HPP_

#include "Component.hpp"

struct BoundingBox;
struct Model;
struct Mesh;
struct Vector2;
struct Vector3;
struct Rectangle;
struct Image;

namespace indie
{
    class AVector;
    class Hitbox : public Component
    {
    public:
        /**
         * @brief Construct a new Hitbox object using a bounding box retrieved from a model and his position
         * @param box Bounding box retrieve with model->getBBox()
         * @param pos Position of the model
         * @return Hitbox
         **/
        Hitbox(BoundingBox box, Vector3 pos);
        /**
         * @brief Construct a new Hitbox object using a bounding box already updated with a position
         * @param box Bounding box retrieve with CollideSystem::makeUpdatedBBox function
         * @return Hitbox
         **/
        Hitbox(BoundingBox box);
        /**
         * @brief Construct a new Hitbox object using a rect retrieved from a image/texture and his position
         * @param rect Rect retrieve with texture->getRect()
         * @param pos Position of the texture/image
         * @return Hitbox
         **/
        Hitbox(Rectangle rect, Vector2 pos);
        /**
         * @brief Construct a new Hitbox object using a rect already updated with a position
         * @param rect Rect retrieve with CollideSystem::makeUpdatedRect function
         * @return Hitbox
         **/
        Hitbox(Rectangle rect);
        /**
         * @brief Construct uninitialized Hitbox object
         * @param is3D True if the hitbox is 3D, false if it is 2D
         */
        Hitbox(bool is3D);
        ~Hitbox();

        /// @brief Shift the 3D hitbox
        Hitbox &operator+=(const Vector3 shift);
        /// @brief Shift the 3D hitbox
        Hitbox &operator-=(const Vector3 shift);
        /// @brief Shift the 3D hitbox
        Hitbox &operator*=(const Vector3 shift);
        /// @brief Shift the 3D hitbox
        Hitbox &operator/=(const Vector3 shift);

        /// @brief Shift the 2D Rectangle
        Hitbox &operator+=(const Vector2 shift);
        /// @brief Shift the 2D Rectangle
        Hitbox &operator-=(const Vector2 shift);
        /// @brief Shift the 2D Rectangle
        Hitbox &operator*=(const Vector2 shift);
        /// @brief Shift the 2D Rectangle
        Hitbox &operator/=(const Vector2 shift);

        /// @brief Shift the 3D hitbox
        Hitbox &operator+=(const AVector shift);
        /// @brief Shift the 3D hitbox
        Hitbox &operator-=(const AVector shift);

        /// @brief Test if the hitbox is a 3d Type one
        bool is3D(void) const;
        /// @brief Get the 3D BoundingBox (WARNING-> use 3d funtions on 3d hitbox only)
        BoundingBox getBBox(void) const;
        /// @brief Get the 2D Rectangle (WARNING-> use 2d funtions on 2d hitbox only)
        Rectangle getRect(void) const;
        void setBBox(BoundingBox box);

    protected:
    private:
        std::unique_ptr<BoundingBox> _box = nullptr;
        bool _is3D = false;
        std::unique_ptr<Rectangle> _rect = nullptr;
    };
}

#endif /* !HITBOX_HPP_ */
