/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Entity.cpp
*/

#include <algorithm>
#include <iostream>

#include "Entity.hpp"

namespace indie
{

    const std::map<Entity::Tags, std::vector<std::vector<IComponent::Type>>> Entity::entityTags = {
        {IEntity::Tags::SPRITE_2D,
         {{IComponent::Type::SPRITE, IComponent::Type::POSITION}}},
        {Entity::Tags::TEXT,
         {{IComponent::Type::TEXT, IComponent::Type::POSITION}}},
        {Entity::Tags::RENDERABLE_3D,
         {{IComponent::Type::POSITION, IComponent::Type::MODEL}}},
        {Entity::Tags::GRID,
         {{IComponent::Type::GRID}}},
        {Entity::Tags::CUBE,
         {{IComponent::Type::POSITION, IComponent::Type::CUBE}}},
        {Entity::Tags::SPHERE,
         {{IComponent::Type::POSITION, IComponent::Type::SPHERE}}},
        {Entity::Tags::AUDIBLE,
         {{IComponent::Type::MUSIC},
          {IComponent::Type::SOUND}}},
        {Entity::Tags::COLLIDABLE,
         {{IComponent::Type::HITBOX}}},
        {Entity::Tags::CAMERA,
         {{IComponent::Type::CAMERA}}},
        {Entity::Tags::CALLABLE,
         {{IComponent::Type::EVT_LISTENER}}},
        {Entity::Tags::BONUS,
         {{IComponent::Type::BONUS, IComponent::Type::POSITION}}},
        {Entity::Tags::PLAYER,
         {{IComponent::Type::PLAYER}}},
        {Entity::Tags::AESTHETIC,
         {{IComponent::Type::PARTICLES}}},
        {Entity::Tags::BOMB,
         {{IComponent::Type::BOMB}}},
        {Entity::Tags::TIMED,
         {{IComponent::Type::TIMER}}},
        {Entity::Tags::DESTRUCTIBLE,
         {{IComponent::Type::DESTRUCTIBLE}}},
        {Entity::Tags::UI,
         {{IComponent::Type::UI}}},
        {Entity::Tags::RADAR,
         {{IComponent::Type::RADAR}}},
        {Entity::Tags::AI,
         {{IComponent::Type::VELOCITY, IComponent::Type::AI, IComponent::Type::POSITION}}}};

    IEntity &Entity::addComponent(std::shared_ptr<IComponent> component)
    {
        bool notFound = false;

        IComponent::Type type = component->getType();
        _componentsType.push_back(type);
        _components[type] = component;
        for (auto &tag : entityTags) {
            if (this->hasTag(tag.first))
                continue;
            for (auto &vec : tag.second) {
                notFound = false;
                for (auto &ctag : vec) {
                    if (std::find(_componentsType.begin(), _componentsType.end(), ctag) == _componentsType.end()) {
                        notFound = true;
                        break;
                    }
                }
                if (notFound)
                    continue;
                _tags.push_back(tag.first);
                break;
            }
        }
        return *this;
    }

    IEntity &Entity::addComponents(std::vector<std::shared_ptr<IComponent>> components)
    {
        for (auto &component : components)
            this->addComponent(component);
        return *this;
    }

    bool Entity::hasTag(Tags tag) const
    {
        return (std::find(_tags.begin(), _tags.end(), tag) != _tags.end());
    }

    bool Entity::hasComponent(IComponent::Type type) const
    {
        return (std::find(_componentsType.begin(), _componentsType.end(), type) != _componentsType.end());
    }

    std::map<IComponent::Type, std::shared_ptr<IComponent>> &Entity::getComponents()
    {
        return _components;
    }

    std::vector<std::shared_ptr<IComponent>> Entity::getFilteredComponents(std::vector<IComponent::Type> components)
    {
        std::vector<std::shared_ptr<IComponent>> res;

        for (auto &c : components) {
            if (_components.find(c) == _components.end())
                throw std::invalid_argument("Entity: Component type not found");
            res.push_back(_components[c]);
        }
        return res;
    }

    std::shared_ptr<IComponent> &Entity::operator[](IComponent::Type type)
    {
        static std::shared_ptr<IComponent> null = nullptr;

        if (type >= IComponent::Type::TYPE_NB)
            throw std::invalid_argument("Entity: Component type not found");
        if (_components.find(type) != _components.end())
            return _components.at(type);
        return null;
    }
}