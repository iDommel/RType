/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** EventSystem.cpp
*/

#include "raylib.h"

#include <algorithm>

#include "Component.hpp"
#include "EventListener.hpp"
#include "Window.hpp"
#include "EventSystem.hpp"

namespace indie
{
    std::map<int, std::vector<std::shared_ptr<EventListener>>> EventSystem::_listeners;

    void EventSystem::init(SceneManager &sceneManager)
    {
        for (auto &index : sceneManager.getSceneTypeList()) {
            for (auto &entity : sceneManager.getScene(index)[IEntity::Tags::CALLABLE]) {
                auto listener = Component::castComponent<EventListener>((*entity)[IComponent::Type::EVT_LISTENER]);
                if (listener)
                    _listeners[(int)index].push_back(listener);
            }
        }
    }

    void EventSystem::update(SceneManager &sceneManager, uint64_t)
    {
        for (auto &listener : _listeners[(int)sceneManager.getCurrentSceneType()]) {
            handleKeyboard(sceneManager, listener);
            handleMouse(sceneManager, listener);
            for (int i = 0; i < _maxGamepads; i++) {
                if (Window::isGamepadAvailable(i)) {
                    handleGamepadSticks(sceneManager, listener, i);
                    handleGamepad(sceneManager, listener, i);
                }
            }
        }
    }

    void EventSystem::handleKeyboard(SceneManager &manager, std::shared_ptr<EventListener> listener)
    {
        for (auto &it : listener->getKeyboardMappings()) {
            bool wasPressed = false;
            if (it.second.pressed && Window::isKeyPressed(it.first)) {
                it.second.pressed(manager);
                wasPressed = true;
            }
            if (it.second.down && Window::isKeyDown(it.first)) {
                it.second.down(manager);
                wasPressed = true;
            }
            if (it.second.released && Window::isKeyReleased(it.first)) {
                it.second.released(manager);
                wasPressed = false;
            }
            if (it.second.up && wasPressed && Window::isKeyUp(it.first)) {
                it.second.up(manager);
                wasPressed = false;
            }
        }
    }

    void EventSystem::handleMouse(SceneManager &manager, std::shared_ptr<EventListener> listener)
    {
        for (auto &it : listener->getMouseMappings()) {
            Vector2 pos = Window::getMousePosition();
            if (it.second._pressed && Window::isMouseButtonPressed(it.first)) {
                it.second._pressed(manager, pos);
                break;
            }
            if (it.second._down && Window::isMouseButtonDown(it.first)) {
                it.second._down(manager, pos);
                break;
            }
            if (it.second._released && Window::isMouseButtonReleased(it.first)) {
                it.second._released(manager, pos);
                break;
            }
            if (Window::isMouseButtonUp(it.first)) {
                it.second._up(manager, pos);
                break;
            }
        }
    }

    void EventSystem::handleGamepad(SceneManager &manager, std::shared_ptr<EventListener> listener, int nb)
    {
        for (auto &it : listener->getGamepadMappings(nb)) {
            bool wasPressed = false;
            if (it.second.pressed && Window::isGamepadButtonPressed(nb, it.first)) {
                it.second.pressed(manager);
                wasPressed = true;
            }
            if (it.second.released && Window::isGamepadButtonReleased(nb, it.first)) {
                it.second.released(manager);
                wasPressed = true;
            }
            if (it.second.down && Window::isGamepadButtonDown(nb, it.first)) {
                it.second.down(manager);
                wasPressed = false;
            }
            if (it.second.up && wasPressed && Window::isGamepadButtonUp(nb, it.first)) {
                it.second.up(manager);
            }
        }
    }

    void EventSystem::handleGamepadSticks(SceneManager &manager, std::shared_ptr<EventListener> listener, int nb)
    {
        for (auto &it : listener->getGamepadStickMappings(nb)) {
            float value = Window::getGamepadAxisMovement(nb, it.first);
            it.second(manager, value);
        }
    }

    void EventSystem::destroy()
    {
        std::cerr << "EventSystem destroy" << std::endl;
    }

    void EventSystem::loadEntity(std::shared_ptr<IEntity> entity)
    {
        if (entity->hasTag(IEntity::Tags::CALLABLE)) {
            std::shared_ptr<EventListener> listener = Component::castComponent<EventListener>((*entity)[Component::Type::EVT_LISTENER]);
            _listeners[(int)SceneManager::getCurrentSceneType()].push_back(listener);
        }
    }

    void EventSystem::unloadEntity(std::shared_ptr<IEntity> entity)
    {
        if (entity->hasTag(IEntity::Tags::CALLABLE)) {
            auto currentListeners = _listeners[(int)SceneManager::getCurrentSceneType()];
            std::shared_ptr<EventListener> listener = Component::castComponent<EventListener>((*entity)[Component::Type::EVT_LISTENER]);
            auto it = std::find(currentListeners.begin(), currentListeners.end(), listener);
            if (it != currentListeners.end()) {
                currentListeners.erase(it);
            }
        }
    }

    void EventSystem::reloadScene(SceneManager &manager, SceneManager::SceneType sceneType)
    {
        auto newEntities = manager.getScene(sceneType)[IEntity::Tags::CALLABLE];
        std::vector<std::shared_ptr<EventListener>> newListeners;

        for (auto &e: newEntities) {
            auto listener = Component::castComponent<EventListener>((*e)[Component::Type::EVT_LISTENER]);
            if (listener)
                newListeners.push_back(listener);
        }

        _listeners[(int)sceneType] = newListeners;
    }
}