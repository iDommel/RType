/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Scene.hpp
*/

#ifndef ISCENE_HPP
#define ISCENE_HPP

#include <map>
#include <memory>
#include <vector>
#include <functional>

#include "IEntity.hpp"

namespace indie
{
    class IScene
    {
    public:
        virtual ~IScene() = default;

        /// @brief Add entity to scene, you MUST call this function AFTER adding all the components to an entity
        virtual IScene &addEntity(std::shared_ptr<IEntity> entity) = 0;
        /// @brief add several entities at once to a scene
        virtual IScene &addEntities(std::vector<std::shared_ptr<IEntity>> entity) = 0;
        /// @brief Removes the given entity from scene
        virtual void removeEntity(std::shared_ptr<IEntity> entity) = 0;
        /**
         * @brief Inits the scene by calling its init function
         * @return Returns the new scene's ptr
         */
        virtual std::unique_ptr<IScene> initScene() = 0;

        /**
         * @brief Get entities with correcponding tags
         * @param tags Tags to search for
         * @return Returns a vector of entities
         */
        virtual std::map<IEntity::Tags, std::vector<std::shared_ptr<IEntity>>> getTaggedEntities(std::vector<IEntity::Tags> tags) = 0;

        /**
         * @brief Set the callback function to call when an entity is added
         * @param callback Callback function
         */
        virtual void setAddEntityCallback(std::function<void(std::shared_ptr<IEntity>)> callback) = 0;

        /**
         * @brief Set the callback function to call when an entity is removed
         * @param callback Callback function
         */
        virtual void setRemoveEntityCallback(std::function<void(std::shared_ptr<IEntity>)> callback) = 0;

        virtual std::function<void(std::shared_ptr<IEntity>)> getAddEntityCallback() = 0;

        virtual std::function<void(std::shared_ptr<IEntity>)> getRemoveEntityCallback() = 0;
        /**
         * @brief retrieves the entities for a given tag
         *
         * @param tag to filter by
         * @return std::vector<std::shared_ptr<IEntity>>&
         */
        virtual std::vector<std::shared_ptr<IEntity>> &operator[](IEntity::Tags tag) = 0;
    };
}

#endif /* ISCENE_HPP */