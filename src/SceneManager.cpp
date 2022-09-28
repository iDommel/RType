/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** SceneManager.cpp
*/

#include "raylib.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include "GameSystem.hpp"
#include "SceneManager.hpp"

namespace indie
{
    void SceneManager::addScene(std::unique_ptr<IScene> scene, SceneType sceneType)
    {
        if (_scenes.find(sceneType) != _scenes.end())
            throw std::invalid_argument("SceneManager: Scene already exists");
        _scenes[sceneType] = std::move(scene);
    }

    IScene &SceneManager::getCurrentScene()
    {
        if (_scenes.find(_currentScene) == _scenes.end())
            throw std::runtime_error("SceneManager: Invalid current scene");
        return *_scenes[_currentScene];
    }

    void SceneManager::setCurrentScene(SceneType sceneType, bool initScene)
    {
        if (_scenes.find(sceneType) == _scenes.end())
            throw std::invalid_argument("SceneManager: Invalid scene type: " + std::to_string((int)sceneType));
        getPreviousSceneType() = _currentScene;
        _currentScene = sceneType;
        getCurrentSceneType() = sceneType;
        if (getPreviousSceneType() == SceneType::GAME && _currentScene != SceneType::PAUSE) {
            GameSystem::setNbrPlayer(4);
            auto addEntityCallback = _scenes[SceneType::GAME]->getAddEntityCallback();
            auto removeEntityCallback = _scenes[SceneType::GAME]->getRemoveEntityCallback();
            _scenes[SceneType::GAME] = _scenes[SceneType::GAME]->initScene();
            _scenes[SceneType::GAME]->setAddEntityCallback(addEntityCallback);
            _scenes[SceneType::GAME]->setRemoveEntityCallback(removeEntityCallback);
        } else if (getPreviousSceneType() == SceneType::END) {
            auto addEntityCallback = _scenes[SceneType::END]->getAddEntityCallback();
            auto removeEntityCallback = _scenes[SceneType::END]->getRemoveEntityCallback();
            _scenes[SceneType::END] = _scenes[SceneType::END]->initScene();
            _scenes[SceneType::END]->setAddEntityCallback(addEntityCallback);
            _scenes[SceneType::END]->setRemoveEntityCallback(removeEntityCallback);
        } else if (getPreviousSceneType() == SceneType::CONTROLLER) {
            auto addEntityCallback = _scenes[SceneType::CONTROLLER]->getAddEntityCallback();
            auto removeEntityCallback = _scenes[SceneType::CONTROLLER]->getRemoveEntityCallback();
            _scenes[SceneType::CONTROLLER] = _scenes[SceneType::CONTROLLER]->initScene();
            _scenes[SceneType::CONTROLLER]->setAddEntityCallback(addEntityCallback);
            _scenes[SceneType::CONTROLLER]->setRemoveEntityCallback(removeEntityCallback);
        } else if (getPreviousSceneType() == SceneType::PREGAME) {
            auto addEntityCallback = _scenes[SceneType::PREGAME]->getAddEntityCallback();
            auto removeEntityCallback = _scenes[SceneType::PREGAME]->getRemoveEntityCallback();
            _scenes[SceneType::PREGAME] = _scenes[SceneType::PREGAME]->initScene();
            _scenes[SceneType::PREGAME]->setAddEntityCallback(addEntityCallback);
            _scenes[SceneType::PREGAME]->setRemoveEntityCallback(removeEntityCallback);
        } 

        auto addEntityCallback = _scenes[_currentScene]->getAddEntityCallback();
        auto removeEntityCallback = _scenes[_currentScene]->getRemoveEntityCallback();
        if (initScene) {
            _scenes[_currentScene] = _scenes[_currentScene]->initScene();
            _scenes[_currentScene]->setAddEntityCallback(addEntityCallback);
            _scenes[_currentScene]->setRemoveEntityCallback(removeEntityCallback);
        }
    }

    std::map<SceneManager::SceneType, std::unique_ptr<IScene>> &SceneManager::getScenes()
    {
        return _scenes;
    }

    void SceneManager::setAddEntityCallback(std::function<void(std::shared_ptr<IEntity>)> callback)
    {
        for (auto &scene : _scenes) {
            scene.second->setAddEntityCallback(callback);
        }
    }

    void SceneManager::setShouldClose(bool shouldClose)
    {
        _shouldClose = shouldClose;
    }

    bool SceneManager::getShouldClose()
    {
        return _shouldClose;
    }

    void SceneManager::setRemoveEntityCallback(std::function<void(std::shared_ptr<IEntity>)> callback)
    {
        for (auto &scene : _scenes) {
            scene.second->setRemoveEntityCallback(callback);
        }
    }

    SceneManager::SceneType &SceneManager::getCurrentSceneType()
    {
        static SceneType currentSceneType = SceneType::NONE;

        return currentSceneType;
    }

    SceneManager::SceneType &SceneManager::getPreviousSceneType()
    {
        static SceneType previousSceneType = SceneType::NONE;

        return previousSceneType;
    }


    IScene &SceneManager::getScene(SceneType sceneType)
    {
        if (_scenes.find(sceneType) == _scenes.end())
            throw std::invalid_argument("SceneManager: Invalid scene type: " + std::to_string((int)sceneType));
        return *_scenes[sceneType];
    }

    std::vector<SceneManager::SceneType> SceneManager::getSceneTypeList()
    {
        std::vector<SceneType> sceneTypeList;

        for (auto &scene : _scenes) {
            sceneTypeList.push_back(scene.first);
        }
        return sceneTypeList;
    }
}