/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** ParticlesSystem
*/

#ifndef PARTICLESSYSTEM_HPP_
#define PARTICLESSYSTEM_HPP_

#include "ISystem.hpp"
#include "ParticleCloud.hpp"

namespace ecs
{
    class ParticlesSystem : public ISystem
    {
    public:
        ParticlesSystem();
        void init(SceneManager &manager) final;
        void update(SceneManager &manager, uint64_t deltaTime) final;
        void destroy() final;

        /**
         * @brief The callback to be called when an entity is added to a scene
         * @param entity The Entity that was added
         * @param scene Scene to add entity into
         */
        void onEntityAdded(std::shared_ptr<IEntity> entity, IScene &scene) final;
        /**
         * @brief The callback to be called when an entity is removed from a scene
         * @param entity The Entity that was removed
         */
        void onEntityRemoved(std::shared_ptr<IEntity> entity, IScene &scene) final;
        ~ParticlesSystem();

    protected:
    private:
    };
}

#endif /* !PARTICLESSYSTEM_HPP_ */
