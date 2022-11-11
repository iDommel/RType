/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Scene.cpp
*/

#include "Scene.hpp"

#include <algorithm>

#include "Core.hpp"

namespace ecs
{

    Scene::Scene(std::function<std::unique_ptr<IScene>()> init, SceneType scene) : _initFunc(init), _type(scene)
    {
    }

    IScene &Scene::addEntity(std::shared_ptr<IEntity> entity)
    {
        for (auto &tag : entity->getTags()) {
            _taggedEntities[tag].push_back(entity);
        }
        _entities.push_back(entity);
        if (_addEntityCallback)
            _addEntityCallback(entity, *this);
        return *this;
    }

    IScene &Scene::addEntities(std::vector<std::shared_ptr<IEntity>> entities)
    {
        for (auto &entity : entities) {
            for (auto &tag : entity->getTags()) {
                _taggedEntities[tag].push_back(entity);
            }
            _entities.push_back(entity);
            if (_addEntityCallback)
                _addEntityCallback(entity, *this);
        }
        return *this;
    }

    void Scene::removeEntity(std::shared_ptr<IEntity> entity)
    {
        if (!entity)
            return;
        for (auto &tag : entity->getTags()) {
            auto it = std::find(_taggedEntities[tag].begin(), _taggedEntities[tag].end(), entity);
            if (it != _taggedEntities[tag].end())
                _taggedEntities[tag].erase(it);
        }
        _entities.erase(std::find(_entities.begin(), _entities.end(), entity));
        if (_removeEntityCallback)
            _removeEntityCallback(entity, *this);
    }

    std::unique_ptr<IScene> Scene::initScene()
    {
        return _initFunc();
    }

    std::map<IEntity::Tags, std::vector<std::shared_ptr<IEntity>>> Scene::getTaggedEntities(std::vector<IEntity::Tags> tags)
    {
        std::map<IEntity::Tags, std::vector<std::shared_ptr<IEntity>>> taggedEntities;

        for (auto &tag : tags) {
            taggedEntities[tag] = _taggedEntities[tag];
        }
        return taggedEntities;
    }

    void Scene::setAddEntityCallback(std::function<void(std::shared_ptr<IEntity>, IScene &)> callback)
    {
        _addEntityCallback = callback;
    }

    void Scene::setRemoveEntityCallback(std::function<void(std::shared_ptr<IEntity>, IScene &)> callback)
    {
        _removeEntityCallback = callback;
    }

    std::vector<std::shared_ptr<IEntity>> &Scene::operator[](IEntity::Tags tag)
    {
        return _taggedEntities[tag];
    }

    std::vector<std::shared_ptr<IEntity>> Scene::getAllEntities()
    {
        return _entities;
    }
    std::shared_ptr<IEntity> Scene::getEntityById(QUuid id)
    {
        for (auto &entity : _entities) {
            if (entity->getId() == id)
                return entity;
        }
        return nullptr;
    }
}