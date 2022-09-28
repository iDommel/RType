/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** GraphicSystem.cpp
*/

#include "raylib.h"

#include <iostream>

#include "GraphicSystem.hpp"
#include "CameraComponent.hpp"
#include "Cube.hpp"
#include "Grid.hpp"
#include "Model3D.hpp"
#include "Position.hpp"
#include "Rect.hpp"
#include "Shape3D.hpp"
#include "Sphere.hpp"
#include "Sprite.hpp"
#include "String.hpp"
#include "Texture2D.hpp"
#include "HitboxComponent.hpp"
#include "ModelAnim.hpp"
#include "ModelAnimation.hpp"
#include "ParticleCloud.hpp"

namespace indie
{

    GraphicSystem::GraphicSystem()
    {
    }
    void GraphicSystem::init(SceneManager &sceneManager)
    {
        std::cerr << "GraphicSystem::init" << std::endl;
        _window = std::make_unique<Window>(800, 600, FLAG_WINDOW_RESIZABLE, "Indie Studio");

        for (auto &scene : sceneManager.getScenes()) {
            for (auto &entity : (*scene.second)[IEntity::Tags::SPRITE_2D])
                loadSprite(entity);
            for (auto &e : (*scene.second)[IEntity::Tags::RENDERABLE_3D])
                loadModel(e);
            for (auto &e : (*scene.second)[IEntity::Tags::TEXT])
                loadText(e);
        }
    }

    void GraphicSystem::update(SceneManager &sceneManager, uint64_t)
    {
        for (auto &scene : sceneManager.getScenes())
            for (auto &e : (*scene.second)[IEntity::Tags::TEXT])
                loadText(e);
        if (_window->shouldClose()) {
            sceneManager.setShouldClose(true);
            return;
        }
        _window->beginDraw();
        _window->clearBackground(RAYWHITE);
        for (auto &e : sceneManager.getCurrentScene()[IEntity::Tags::CAMERA]) {
            auto camComponent = (*e)[IComponent::Type::CAMERA];
            auto cam = Component::castComponent<CameraComponent>(camComponent);

            cam->getCamera().beginDrawScope();
            for (auto &e : sceneManager.getCurrentScene()[IEntity::Tags::RENDERABLE_3D])
                displayModel(e);
            for (auto &e : sceneManager.getCurrentScene()[IEntity::Tags::GRID])
                displayGrid(e);
            for (auto &e : sceneManager.getCurrentScene()[IEntity::Tags::SPHERE])
                displaySphere(e);
            for (auto &e : sceneManager.getCurrentScene()[IEntity::Tags::CUBE])
                displayCube(e);
            for (auto &e : sceneManager.getCurrentScene()[IEntity::Tags::AESTHETIC])
                displayParticles(e);
            /* for (auto &e : sceneManager.getCurrentScene()[IEntity::Tags::COLLIDABLE])
                displayCollidable(e); */
            cam->getCamera().endDrawScope();
        }
        for (auto &e : sceneManager.getCurrentScene()[IEntity::Tags::SPRITE_2D])
            displaySprite(e);
        for (auto &e : sceneManager.getCurrentScene()[IEntity::Tags::TEXT])
            displayText(e);
        _window->endDraw();
    }

    void GraphicSystem::destroy()
    {
        std::cerr << "GraphicSystem::destroy" << std::endl;
    }

    void GraphicSystem::loadEntity(std::shared_ptr<IEntity> entity)
    {
        if (entity->hasTag(IEntity::Tags::SPRITE_2D)) {
            std::cerr << "loadSprite" << std::endl;
            loadSprite(entity);
        }
        if (entity->hasTag(IEntity::Tags::RENDERABLE_3D)) {
            std::cerr << "loadModel" << std::endl;
            loadModel(entity);
        }
    }

    void GraphicSystem::unloadEntity(std::shared_ptr<IEntity> entity)
    {
        if (entity->hasTag(IEntity::Tags::SPRITE_2D))
            unloadSprite(entity);
        if (entity->hasTag(IEntity::Tags::RENDERABLE_3D)) {
            unloadModel(entity);
        }
    }

    void GraphicSystem::loadSprite(std::shared_ptr<IEntity> &entity)
    {
        auto sprite = Component::castComponent<Sprite>((*entity)[IComponent::Type::SPRITE]);

        if (_textures.find(sprite->getValue()) != _textures.end())
            _textures[sprite->getValue()].second++;
        else
            _textures[sprite->getValue()] = std::make_pair(std::make_unique<Texture>(sprite->getValue()), 1);
        if (sprite->getNbFrame() == 0)
            return;

        auto spriteRect = Component::castComponent<Rect>((*entity)[IComponent::Type::RECT]);

        spriteRect->width = _textures[sprite->getValue()].first->getWidth() / sprite->getNbFrame();
        spriteRect->height = _textures[sprite->getValue()].first->getHeight();
    }

    void GraphicSystem::unloadSprite(std::shared_ptr<IEntity> &entity)
    {
        auto sprite = Component::castComponent<Sprite>((*entity)[IComponent::Type::SPRITE]);

        if (_textures[sprite->getValue()].second != 1)
            _textures[sprite->getValue()].second--;
        else
            _textures.erase(sprite->getValue());
    }

    void GraphicSystem::displaySprite(std::shared_ptr<IEntity> &entity) const
    {
        auto components = entity->getFilteredComponents({IComponent::Type::SPRITE, IComponent::Type::POSITION});
        auto sprite = Component::castComponent<Sprite>(components[0]);
        auto pos = Component::castComponent<Position>(components[1]);

        try {
            auto rect = (*entity)[IComponent::Type::RECT];
            auto r = Component::castComponent<Rect>(rect);
            Vector2 p = {pos->x, pos->y};

            _textures.at(sprite->getValue()).first->setRect(r->left, r->top, r->width, r->height);
            _textures.at(sprite->getValue()).first->drawRec(p);
        } catch (std::runtime_error &) {
            _textures.at(sprite->getValue()).first->draw(pos->x, pos->y);
        }
    }

    void GraphicSystem::displayParticles(std::shared_ptr<IEntity> &entity) const
    {
        static auto sphere = Sphere(1, BLUE);
        auto particlesCloudEntity = (*entity)[IComponent::Type::PARTICLES];
        std::shared_ptr<indie::ParticleCloud> particlesCloud = nullptr;

        if (particlesCloudEntity == nullptr)
            return;
        particlesCloud = Component::castComponent<ParticleCloud>(particlesCloudEntity);
        for (auto &position : particlesCloud->getPos())
            Shape3D::drawSphere(position, sphere.getRadius(), sphere.getColor());
    }

    void GraphicSystem::displayModel(std::shared_ptr<IEntity> &entity)
    {
        auto components = entity->getFilteredComponents({IComponent::Type::MODEL, IComponent::Type::POSITION});
        auto model = Component::castComponent<Model3D>(components[0]);
        auto pos = Component::castComponent<Position>(components[1]);
        Vector3 position = {pos->x, pos->y, pos->z};

        if ((*entity)[IComponent::Type::ANIMATION] != nullptr) {
            auto anim = Component::castComponent<ModelAnim>((*entity)[IComponent::Type::ANIMATION]);
            Vector3 size = {model->getSize(), model->getSize(), model->getSize()};
            _animations[anim->getAnimPath()].first->updateModelAnimation(*_models[model->getModelPath()].first, anim->getCurrentFrame());
            Vector3 x = {1.0f, 0.0f, 0.0f};
            _models[model->getModelPath()].first->drawRotate(position, x, -90.0f, size, WHITE);
        } else {
            Vector3 x = {0.0f, 1.0f, 0.0f};
            Vector3 size = {model->getSize(), model->getSize(), model->getSize()};
            _models.at(model->getModelPath()).first->drawRotate(position, x, model->getRotation(), size, WHITE);
        }
    }

    void GraphicSystem::displayCollidable(std::shared_ptr<IEntity> &entity) const
    {
        auto components = entity->getFilteredComponents({IComponent::Type::HITBOX});
        auto hitbox = Component::castComponent<Hitbox>(components[0]);

        if (hitbox->is3D())
            ::DrawBoundingBox(hitbox->getBBox(), RED);
    }

    void GraphicSystem::loadModel(std::shared_ptr<IEntity> &entity)
    {
        auto model = Component::castComponent<Model3D>((*entity)[IComponent::Type::MODEL]);
        auto boxComponent = (*entity)[IComponent::Type::HITBOX];
        std::shared_ptr<indie::Hitbox> hitbox = nullptr;

        if (_models.find(model->getModelPath()) != _models.end())
            _models[model->getModelPath()].second++;
        else
            _models[model->getModelPath()] = std::make_pair(std::make_unique<Model>(model->getModelPath(), model->getTexturePath()), 1);
        if ((*entity)[IComponent::Type::ANIMATION] != nullptr)
            loadModelAnimation(entity);
        if (boxComponent == nullptr)
            return;
        hitbox = Component::castComponent<Hitbox>(boxComponent);
        if (hitbox->is3D() && !hitbox->isInitialized()) {
            auto box = _models[model->getModelPath()].first->getBoundingBox();
            auto pos = hitbox->getBBox().max;
            box.max.x += pos.x;
            box.max.y += pos.y;
            box.max.z += pos.z;
            box.min.x += pos.x;
            box.min.y += pos.y;
            box.min.z += pos.z;
            hitbox->setBBox(box);
        }
    }

    void GraphicSystem::unloadModel(std::shared_ptr<IEntity> &entity)
    {
        auto model = Component::castComponent<Model3D>((*entity)[IComponent::Type::MODEL]);

        if (_models[model->getModelPath()].second != 1)
            _models[model->getModelPath()].second--;
        else
            _models.erase(model->getModelPath());
    }

    void GraphicSystem::displaySphere(std::shared_ptr<IEntity> &entity) const
    {
        auto components = entity->getFilteredComponents({IComponent::Type::SPHERE, IComponent::Type::POSITION});

        if (components.size() != 2)
            throw std::runtime_error("GraphicSystem::displaySphere could not get component Sphere & Vector from entity");

        auto sphere = Component::castComponent<Sphere>(components[0]);
        auto pos = Component::castComponent<Position>(components[1]);
        Vector3 position = {pos->x, pos->y, pos->z};

        Shape3D::drawSphere(position, sphere->getRadius(), sphere->getColor());
    }

    void GraphicSystem::displayGrid(std::shared_ptr<IEntity> &entity) const
    {
        auto grid = Component::castComponent<Grid>((*entity)[IComponent::Type::GRID]);

        Shape3D::drawGrid(grid->getSlices(), grid->getSpacing());
    }

    void GraphicSystem::displayCube(std::shared_ptr<IEntity> &entity) const
    {
        auto components = entity->getFilteredComponents({IComponent::Type::CUBE, IComponent::Type::POSITION});
        auto cube = Component::castComponent<Cube>(components[0]);
        auto pos = Component::castComponent<Position>(components[1]);
        Vector3 position = {pos->x, pos->y, pos->z};

        Shape3D::drawCube(position, cube->getSize(), cube->getColor());
    }

    void GraphicSystem::displayText(std::shared_ptr<IEntity> &entity) const
    {
        auto components = entity->getFilteredComponents({IComponent::Type::TEXT, IComponent::Type::POSITION});
        auto text = Component::castComponent<String>(components[0]);
        auto pos = Component::castComponent<Position>(components[1]);

        _texts.at(text->getValue()).first->draw(pos->x, pos->y, text->getFontSize(), BLACK);
    }

    void GraphicSystem::loadText(std::shared_ptr<IEntity> &entity)
    {
        auto text = Component::castComponent<String>((*entity)[IComponent::Type::TEXT]);

        if (_texts.find(text->getValue()) != _texts.end())
            _texts[text->getValue()].second++;
        else
            _texts[text->getValue()] = std::make_pair(std::make_unique<Text>(text->getValue(), text->getFontFile()), 1);
    }

    void GraphicSystem::unloadText(std::shared_ptr<IEntity> &entity)
    {
        auto text = Component::castComponent<String>((*entity)[IComponent::Type::TEXT]);

        if (_texts[text->getValue()].second != 1)
            _texts[text->getValue()].second--;
        else
            _texts.erase(text->getValue());
    }

    void GraphicSystem::loadModelAnimation(std::shared_ptr<IEntity> &entity)
    {
        auto anim = Component::castComponent<ModelAnim>((*entity)[IComponent::Type::ANIMATION]);

        if (_animations.find(anim->getAnimPath()) != _animations.end())
            _animations[anim->getAnimPath()].second++;
        else
            _animations[anim->getAnimPath()] = std::make_pair(std::make_unique<ModelAnimation>(anim->getAnimPath()), 1);
        if (anim->getNbFrames() < 0) {
            anim->getNbFrames() = _animations[anim->getAnimPath()].first->getFrameCount();
        }
    }

    void GraphicSystem::unloadModelAnimation(std::shared_ptr<IEntity> &entity)
    {
        auto anim = Component::castComponent<ModelAnim>((*entity)[IComponent::Type::ANIMATION]);

        if (_animations[anim->getAnimPath()].second < 1)
            _animations[anim->getAnimPath()].second--;
        else
            _animations.erase(anim->getAnimPath());
    }
}