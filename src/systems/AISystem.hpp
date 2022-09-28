/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** AISystem.hpp
*/

#ifndef AISystem_HPP_
#define AISystem_HPP_

#include <array>

#include "ISystem.hpp"
#include "CollideSystem.hpp"

namespace indie
{
    class Position;
    class AIPlayer;

    class AISystem : public ISystem
    {
    public:

        enum class DIRECTION
        {
            UP,
            DOWN,
            LEFT,
            RIGHT
        };

        AISystem(CollideSystem &collideSystem);

        void init(SceneManager &manager) final;
        void update(SceneManager &manager, uint64_t deltaTime) final;
        void destroy() final;

        void loadEntity(std::shared_ptr<IEntity> entity) final;
        void unloadEntity(std::shared_ptr<IEntity> entity) final;

    private:
        void loadMap(AIPlayer &ai, std::array<std::array<char, 5>, 5> &map, Position &pos, std::shared_ptr<IEntity> me);
        void loadExplosionInMap(std::array<std::array<char, 5>, 5> &map, std::shared_ptr<IEntity> &explosion, int px, int pz);
        void moveAI(std::array<std::array<char, 5>, 5> &map, AIPlayer &ai, std::shared_ptr<IEntity> entity, SceneManager &sceneManager);
        void changeDir(std::array<std::array<char, 5>, 5> &map, AIPlayer &ai, std::shared_ptr<IEntity> entity);
        void poseBomb(AIPlayer &ai, SceneManager &sceneManager, std::array<std::array<char, 5>, 5> &map);
        bool escape(AIPlayer &ai, std::array<std::array<char, 5>, 5> &map, std::shared_ptr<IEntity> entity);

        CollideSystem &_collideSystem;
    };
}

#endif /* AISystem_HPP_ */