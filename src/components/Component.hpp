/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** AComponent.hpp
*/

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <memory>
#include <stdexcept>

#include "IComponent.hpp"

namespace indie
{

    class Component : public IComponent
    {
    public:
        Component(const Type &type);

        const Type &getType() const override;

        bool isInitialized() const override { return _isInitialized; };

        /**
         * @brief Try casting the component to the given type
         * @tparam T type expected
         * @param component IComponent to try cast
         * @return std::shared_ptr<T> casted component or null shared_ptr if cast failed
         */
        template <typename T>
        static std::shared_ptr<T> castComponent(std::shared_ptr<IComponent> &component)
        {
            std::shared_ptr<T> res = std::dynamic_pointer_cast<T>(component);

            if (res == nullptr)
                throw std::runtime_error("Component: dynamic_pointer_cast failed");
            return res;
        }

    protected:
        Type _type = Type::TYPE_NB;
        bool _isInitialized = false;
    };

    template <typename T>
    std::shared_ptr<T> castComponent(std::shared_ptr<IComponent> component)
    {
        std::shared_ptr<T> res = std::dynamic_pointer_cast<T>(component);

        if (res == nullptr)
            throw std::runtime_error("Component: dynamic_pointer_cast failed");
        return res;
    }

}

#endif /* COMPONENT_HPP */