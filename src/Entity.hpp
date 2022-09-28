/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Entity.hpp
*/

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "IEntity.hpp"
#include "components/IComponent.hpp"

namespace indie
{
    class Entity : public IEntity
    {
    public:
        ///@brief Map between Entity tags and combinaisons of Components tags
        static const std::map<Tags, std::vector<std::vector<IComponent::Type>>> entityTags;

        ///@brief Add component to entity and update entity's tags accordingly, mustn't be called after moving entity to scene
        IEntity &addComponent(std::shared_ptr<IComponent> component);
        ///@brief Add component to entity and update entity's tags accordingly
        IEntity &addComponents(std::vector<std::shared_ptr<IComponent>> components);

        /**
         * @brief Get entity's components
         * @return Returns a reference of the entity's components vector
         */
        std::map<IComponent::Type, std::shared_ptr<IComponent>> &getComponents();
        /**
         * @brief Get entity's tags
         * @return Returns a reference of the entity's tags vector
         */
        const std::vector<Tags> &getTags() const { return _tags; };

        /**
         * @brief Search if entity has a tag
         * @param tag Tag to search for
         * @return Returns true if entity has the given tag, false otherwise
         */
        bool hasTag(Tags tag) const;

        /**
         * @brief checks if an entity has a given component
         *
         * @param type the type to check
         * @return true if it has it, false if it doesn't
         */
        bool hasComponent(IComponent::Type type) const final;
        /**
         * @brief Get components of given types
         * @param components Vector of components types to search for
         * @return Returns a vector of components of the given types in the same order
         */
        std::vector<std::shared_ptr<IComponent>> getFilteredComponents(std::vector<IComponent::Type> components);
        /**
         * @brief Allows the access to an entity's component via a Tag. Throws a std::runtime_error if the component is not found.
         *
         * @param tag The tag to acces
         * @return A pointer to the component
         */
        std::shared_ptr<IComponent> &operator[](IComponent::Type type);

    private:
        std::vector<Tags> _tags;
        std::map<IComponent::Type, std::shared_ptr<IComponent>> _components;
        std::vector<IComponent::Type> _componentsType;
    };
}

#endif  // ENTITY_HPP