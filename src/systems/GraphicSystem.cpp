/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** GraphicSystem.cpp
*/

#include <iostream>

#include "GraphicSystem.hpp"
#include "Position.hpp"
#include "Rect.hpp"
#include "Sprite.hpp"
#include "String.hpp"
#include "Texture2D.hpp"
#include "HitboxComponent.hpp"
#include "ParticleCloud.hpp"

#include "SFML/Graphics.h"

namespace ecs
{
    Window GraphicSystem::_window = Window(800, 600, "R-Type");

    GraphicSystem::GraphicSystem()
    {
    }
    void GraphicSystem::init(SceneManager &sceneManager)
    {
        std::cerr << "GraphicSystem::init" << std::endl;

        for (auto &scene : sceneManager.getScenes()) {
            for (auto &entity : (*scene.second)[IEntity::Tags::SPRITE_2D])
                loadSprite(entity);
            for (auto &e : (*scene.second)[IEntity::Tags::RENDERABLE_3D])
                loadModel(e);
            for (auto &e : (*scene.second)[IEntity::Tags::TEXT])
                loadText(e);
        }
    }

    Window &GraphicSystem::getWindow()
    {
        return _window;
    }

    void GraphicSystem::update(SceneManager &sceneManager, uint64_t)
    {
        for (auto &scene : sceneManager.getScenes())
            for (auto &e : (*scene.second)[IEntity::Tags::TEXT])
                loadText(e);
        if (_window.shouldClose()) {
            sceneManager.setShouldClose(true);
            return;
        }
        _window.clear();
        for (auto &e : sceneManager.getCurrentScene()[IEntity::Tags::SPRITE_2D])
            displaySprite(e);
        for (auto &e : sceneManager.getCurrentScene()[IEntity::Tags::TEXT])
            displayText(e);
        _window.display();
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
    }

    void GraphicSystem::unloadEntity(std::shared_ptr<IEntity> entity)
    {
        if (entity->hasTag(IEntity::Tags::SPRITE_2D))
            unloadSprite(entity);
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

            _textures.at(sprite->getValue()).first->setRect(r->left, r->top, r->width, r->height);
            _textures.at(sprite->getValue()).first->drawRec({pos->x, pos->y});
        } catch (std::runtime_error &) {
            _textures.at(sprite->getValue()).first->draw(pos->x, pos->y);
        }
    }

    void GraphicSystem::displayParticles(std::shared_ptr<IEntity> &entity) const
    {
        // static auto sphere = Sphere(1, sf::Color(87, 12, 13));
        // auto particlesCloudEntity = (*entity)[IComponent::Type::PARTICLES];
        // std::shared_ptr<ecs::ParticleCloud> particlesCloud = nullptr;

        // if (particlesCloudEntity == nullptr)
        //     return;
        // particlesCloud = Component::castComponent<ParticleCloud>(particlesCloudEntity);
        // for (auto &position : particlesCloud->getPos())
        //     Shape3D::drawSphere(position, sphere.getRadius(), sphere.getColor());
    }

    void GraphicSystem::displayCollidable(std::shared_ptr<IEntity> &entity) const
    {
        // auto components = entity->getFilteredComponents({IComponent::Type::HITBOX});
        // auto hitbox = Component::castComponent<Hitbox>(components[0]);

        // if (hitbox->is3D())
        //     ::DrawBoundingBox(hitbox->getBBox(), RED);
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
}