/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** GraphicSystem.hpp
*/

#ifndef GRAPHIC_SYSTEM_HPP
#define GRAPHIC_SYSTEM_HPP

#include "ISystem.hpp"
#include "SceneManager.hpp"
#include "Window.hpp"
#include "Texture2D.hpp"
#include "Model.hpp"
#include "Text.hpp"
#include "ModelAnimation.hpp"

namespace indie
{

    class GraphicSystem : public ISystem
    {
    public:
        GraphicSystem();
        void init(SceneManager &manager) final;
        void update(SceneManager &manager, uint64_t deltaTime) final;
        void destroy() final;

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

    private:
        void loadSprite(std::shared_ptr<IEntity> &entity);
        void unloadSprite(std::shared_ptr<IEntity> &entity);
        void displaySprite(std::shared_ptr<IEntity> &entity) const;

        void loadModel(std::shared_ptr<IEntity> &entity);
        void unloadModel(std::shared_ptr<IEntity> &entity);
        void displayModel(std::shared_ptr<IEntity> &entity);

        void displayGrid(std::shared_ptr<IEntity> &entity) const;
        void displaySphere(std::shared_ptr<IEntity> &entity) const;
        void displayCube(std::shared_ptr<IEntity> &entity) const;
        void displayParticles(std::shared_ptr<IEntity> &entity) const;
        void displayCollidable(std::shared_ptr<IEntity> &entity) const;
        void displayRadar(std::shared_ptr<IEntity> &entity) const;

        void loadText(std::shared_ptr<IEntity> &entity);
        void unloadText(std::shared_ptr<IEntity> &entity);
        void displayText(std::shared_ptr<IEntity> &entity) const;

        void loadModelAnimation(std::shared_ptr<IEntity> &entity);
        void unloadModelAnimation(std::shared_ptr<IEntity> &entity);

        std::unique_ptr<Window> _window;
        std::map<std::string, std::pair<std::unique_ptr<Texture>, int>> _textures;
        std::map<std::string, std::pair<std::unique_ptr<Model>, int>> _models;
        std::map<std::string, std::pair<std::unique_ptr<Text>, int>> _texts;
        std::map<std::string, std::pair<std::unique_ptr<ModelAnimation>, int>> _animations;
    };

}

#endif /* GRAPHIC_SYSTEM_HPP */