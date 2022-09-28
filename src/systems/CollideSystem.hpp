/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Collide
*/

#ifndef COLLIDE_HPP_
#define COLLIDE_HPP_

#include "SceneManager.hpp"
#include "HitboxComponent.hpp"
#include "Model.hpp"
#include "ISystem.hpp"

struct BoundingBox;
struct Model;
struct Mesh;
struct Vector2;
struct Vector3;
struct Rectangle;
struct Image;

namespace indie
{
    /// @brief CollideSystem class provide collision detection functions for 2D and 3D shapes. (does not handle collision between 3D and 2D)
    class CollideSystem : public ISystem
    {
    public:
        void init(SceneManager &manager) final;
        void update(SceneManager &, uint64_t) final{};
        void destroy(void) final;

        /**
         * @brief The callback to be called when an entity is added to a scene
         * @param entity The Entity that was added
         */
        void loadEntity(std::shared_ptr<IEntity> entity) final;
        /**
         * @brief The callback to be called when an entity is removed from a scene
         * @param entity The Entity that was removed
         */
        void unloadEntity(std::shared_ptr<IEntity> entity) final;

        /**
         * @brief Search for every collisions between the given entity and all the other entities in the scene
         * @param entity The Entity to check
         * @return List of entities that collide with the entity
         */
        std::vector<std::shared_ptr<IEntity>> getColliders(std::shared_ptr<IEntity> entity) const;
        /// ------- 3D -------

        /**
         * @brief Make a bounding box from a size and a position
         * @param size Size of the bounding box (vector3){width, height, depth}
         * @param position Position of the bounding box corner used as origin
         * @return Newly created BoundingBox
         **/
        static BoundingBox makeBBoxFromSizePos(const Vector3 &size, const Vector3 &pos);

        /**
         * @brief Update a bounding box from a model with his position
         * @param box Bounding box to update
         * @param pos Position of the model
         * @return Updated BoundingBox
         **/
        BoundingBox &UpdateBBox(BoundingBox &box, const Vector3 &pos) const;

        /**
         * @brief Create Updated bounding box from a model with his position
         * @param box Bounding box to update
         * @param pos Position of the model
         * @return New updated BoundingBox
         **/
        static BoundingBox makeUpdatedBBox(const BoundingBox &box, const Vector3 &pos);

        /** brief Check if an hitbox is colliding with another one
         * @param box1 First hitbox
         * @param box2 Second hitbox
         * @return True if the bounding boxes are colliding, false otherwise
         **/
        bool check3DCollision(std::shared_ptr<Hitbox> box1, std::shared_ptr<Hitbox> box2) const;

        /**
         * @brief Check collision between two bounding boxes
         * @param box1 First bounding box
         * @param box2 Second bounding box
         * @return True if collision, false otherwise
         **/
        bool check3DCollision(const BoundingBox &box1, const BoundingBox &box2) const;

        /**
         * @brief Check collision between a bounding box and a sphere
         * @param box Bounding box
         * @param center Sphere center
         * @param radius Sphere radius
         * @return True if collision, false otherwise
         **/
        bool check3DCollision(const BoundingBox &box, const Vector3 &center, float radius) const;
        /**
         * @brief Check collision between a sphere and a bounding box
         * @param center Sphere center
         * @param radius Sphere radius
         * @param box Bounding box
         * @return True if collision, false otherwise
         **/
        bool check3DCollision(const Vector3 &center, float radius, const BoundingBox &box) const;

        /**
         * @brief Check collision between two spheres
         * @param center1 First sphere center
         * @param radius1 First sphere radius
         * @param center2 Second sphere center
         * @param radius2 Second sphere radius
         * @return True if collision, false otherwise
         **/
        bool check3DCollision(const Vector3 &center1, float radius1, const Vector3 &center2, float radius2) const;

        /// ------- 2D -------

        /**
         * @brief Create updated rect from an image with his position
         * @param rect Rect to update (retrieved from getRectangleOf(image))
         * @param pos Position of the rect
         * @return New updated Rect
         **/
        Rectangle makeUpdatedRect(const Rectangle &rect, const Vector2 &pos) const;

        /**
         * @brief Update a rect from an image with his position
         * @param rect Rect to update (retrieved from getRectangleOf(image))
         * @param pos Position of the rect
         * @return Updated Rect
         **/
        Rectangle &updateRect(Rectangle &rect, const Vector2 &pos) const;

        /**
         * @brief Retrieve rectangle of an image
         * @param image Image to retrieve rectangle from
         * @param threshold Level of transparency to consider in the image
         * @return Rectangle bounding the image
         **/
        Rectangle getRectangleOf(const Image &image, float threshold) const;

        /**
         * @brief Check collision between two hitboxes
         * @param rect1 First hibox
         * @param rect2 Second hitbox
         * @return True if collision, false otherwise
         **/
        bool check2DCollision(std::shared_ptr<Hitbox> box1, std::shared_ptr<Hitbox> box2) const;

        /**
         * @brief Check if two rectangles are colliding
         * @param rect1 First rectangle
         * @param rect2 Second rectangle
         * @return True if collision, false otherwise
         **/
        bool check2DCollision(const Rectangle &rect1, const Rectangle &rect2) const;

        /**
         * @brief Check if a rectangle is colliding with a circle
         * @param rect Rectangle
         * @param center Circle center
         * @param radius Circle radius
         * @return True if collision, false otherwise
         **/
        bool check2DCollision(const Rectangle &rect, const Vector2 &center, float radius) const;
        /**
         * @brief Check if a circle is colliding with a rectangle
         * @param center Circle center
         * @param radius Circle radius
         * @param rect Rectangle
         * @return True if collision, false otherwise
         */
        bool check2DCollision(const Vector2 &center, float radius, const Rectangle &rect) const;

        /**
         * @brief Check if two circles are colliding
         * @param center1 First circle center
         * @param radius1 First circle radius
         * @param center2 Second circle center
         * @param radius2 Second circle radius
         * @return True if collision, false otherwise
         **/
        bool check2DCollision(const Vector2 &center1, float radius1, const Vector2 &center2, float radius2) const;

        /**
         * @brief Check if a point is inside a rectangle
         * @param point Point
         * @param rect Rectangle
         **/
        bool check2DCollision(const Vector2 &point, const Rectangle &rect) const;
        /**
         * @brief Check if a rectangle is on a point
         * @param rect Rectangle
         * @param point Point
         **/
        bool check2DCollision(const Rectangle &rect, const Vector2 &point) const;

        /**
         * @brief Check if a point is inside a circle
         * @param point Point
         * @param center Circle center
         * @param radius Circle radius
         * @return True if collision, false otherwise
         **/
        bool check2DCollision(const Vector2 &point, const Vector2 &center, float radius) const;
        /**
         * @brief Check if a circle is on a point
         * @param center Circle center
         * @param radius Circle radius
         * @param point Point
         * @return True if collision, false otherwise
         **/
        bool check2DCollision(const Vector2 &center, float radius, const Vector2 &point) const;

        void preInit(IScene &scene);

        void reloadCollidables3D(SceneManager &sceneManager);

    private:
        std::vector<std::pair<std::shared_ptr<IEntity>, std::shared_ptr<Hitbox>>> _collidables3D;
        std::vector<std::pair<std::shared_ptr<IEntity>, std::shared_ptr<Hitbox>>> _collidables2D;
    };
}

#endif /* !COLLIDE_HPP_ */
