/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** SceneManager.hpp
*/

#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include <functional>
#include <iostream>
#include <map>
#include <memory>

#include "IScene.hpp"

namespace indie
{
    class SceneManager
    {
    public:
        enum class SceneType {
            NONE = -1,
            GAME,
            MAIN_MENU,
            CONTROLLER,
            HELP,
            PAUSE,
            OPTION,
            SPLASH,
            SOUND,
            NB_SCENES,
            PREGAME,
            PREVIOUS,
            END
        };

        /**
         * @brief Add a scene to the scene manager
         * @param scene Scene to add
         * @param sceneType Type of the scene, needs to be unique for each scene
         */
        void addScene(std::unique_ptr<IScene> scene, SceneType sceneType);

        /**
         * @brief Returns the current scene
         * @return Reference of the current scene
         */
        IScene &getCurrentScene();

        /**
         * @brief Set the current scene to the scene corresponding to the given sceneType
         * @param sceneType Type of the scene to set as the current scene
         * @param initScene Boolean to reinitialize the scene, set too false by default
         */
        void setCurrentScene(SceneType scene, bool initScene = false);

        /**
         * @brief Get all scenes contained in sceneManager
         * @return Returns a reference to the scenes's map
         */
        std::map<SceneType, std::unique_ptr<IScene>> &getScenes();

        /**
         * @brief Set callback as addEntityCallback for all scene
         * @param callback Callback to set as addEntityCallback
         */
        void setAddEntityCallback(std::function<void(std::shared_ptr<IEntity>)> callback);
        /// @brief this will be read by the core to stop the main loop and close the program
        void setShouldClose(bool shouldClose);
        /// @brief Tells you if you should close the window
        bool getShouldClose();

        /**
         * @brief Set callback as removeEntityCallback for all scene
         * @param callback Callback to set as removeEntityCallback
         */
        void setRemoveEntityCallback(std::function<void(std::shared_ptr<IEntity>)> callback);

        /**
         * @brief Get the Current Scene Type object
         *
         * @return SceneType
         */
        static SceneType &getCurrentSceneType();

        /**
         * @brief Get the Previous Scene Type object
         *
         * @return SceneType
         */
        static SceneType &getPreviousSceneType();

        std::vector<SceneType> getSceneTypeList();
        /**
         * @brief Get the Scene object
         *
         * @param sceneType the scene type you want to access
         * @return std::shared_ptr<IScene>
         */
        IScene &getScene(SceneType sceneType);

    private:
        std::map<SceneType, std::unique_ptr<IScene>> _scenes;
        SceneType _currentScene = SceneType::NONE;
        bool _shouldClose = false;
    };
}

#endif /* SCENE_MANAGER_HPP */