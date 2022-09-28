/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** ParticlesSystem
*/

#ifndef PARTICLESSYSTEM_HPP_
#define PARTICLESSYSTEM_HPP_

#include "ISystem.hpp"
#include "ParticleCloud.hpp"

namespace indie {
    class ParticlesSystem  : public ISystem {
        public:
            ParticlesSystem();
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
            ~ParticlesSystem();

        protected:
        private:
    };
}

#endif /* !PARTICLESSYSTEM_HPP_ */
