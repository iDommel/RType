/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Scene.hpp
*/

#ifndef SCENE_HPP
#define SCENE_HPP

#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "IEntity.hpp"
#include "IScene.hpp"
#include "SceneManager.hpp"

namespace ecs
{

    class Core;

    class Scene : public IScene
    {
    public:
        /**
         * @brief Scene constructor
         * @param init Scene init function
         * @param type Scene type
         */
        Scene(std::function<std::unique_ptr<IScene>()> init, SceneType type);

        /// @brief Add entity to scene
        IScene &addEntity(std::shared_ptr<IEntity> entity);
        /// @brief add several entities at once to a scene
        IScene &addEntities(std::vector<std::shared_ptr<IEntity>> entity);

        /// @brief Removes the given entity from scene
        void removeEntity(std::shared_ptr<IEntity> entity);
        /**
         * @brief Inits the scene by calling its init function
         * @return Returns the new scene's ptr
         */
        std::unique_ptr<IScene> initScene();

        /**
         * @brief Get entities with all correcponding tags
         * @param tags Tags to search for
         * @return Returns a vector of entities
         */
        std::map<IEntity::Tags, std::vector<std::shared_ptr<IEntity>>> getTaggedEntities(std::vector<IEntity::Tags> tags);

        /**
         * @brief Get every entity in the scene
         * @return Returns a vector of entities
         */
        std::vector<std::shared_ptr<IEntity>> getAllEntities();


        /**
         * @brief Set the callback function to call when an entity is added
         * @param callback Callback function
         */
        void setAddEntityCallback(std::function<void(std::shared_ptr<IEntity>, IScene &)> callback);

        /**
         * @brief Set the callback function to call when an entity is removed
         * @param callback Callback function
         */
        void setRemoveEntityCallback(std::function<void(std::shared_ptr<IEntity>, IScene &)> callback);

        std::function<void(std::shared_ptr<IEntity>, IScene &)> getAddEntityCallback() { return _addEntityCallback; };

        std::function<void(std::shared_ptr<IEntity>, IScene &)> getRemoveEntityCallback() { return _removeEntityCallback; };

        /**
         * @brief retrieves the entities for a given tag
         *
         * @param tag to filter by
         * @return std::vector<std::shared_ptr<IEntity>>&
         */
        std::vector<std::shared_ptr<IEntity>> &operator[](IEntity::Tags tag);

        SceneType getSceneType() const { return _type; }

        std::shared_ptr<IEntity> getEntityById(QUuid id);

    protected:
        SceneType _type;
        /// @brief Entities sorted by tags
        std::map<IEntity::Tags, std::vector<std::shared_ptr<IEntity>>> _taggedEntities;
        /// @brief Scene's init function; called by GameSystem::init & Scene::reloadScene
        std::function<std::unique_ptr<IScene>()> _initFunc;
        /// @brief Callback when an entity is added to the scene
        std::function<void(std::shared_ptr<IEntity>, IScene &)> _addEntityCallback;
        /// @brief Callback when an entity is removed from the scene
        std::function<void(std::shared_ptr<IEntity>, IScene &)> _removeEntityCallback;
        /// @brief Entities that are not sorted by tags
        std::vector<std::shared_ptr<IEntity>> _entities;
    };
}

#endif /* SCENE_HPP */