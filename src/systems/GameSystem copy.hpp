/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** GameSystem.hpp
*/

#ifndef GAME_SYSTEM_HPP
#define GAME_SYSTEM_HPP

#include <chrono>

#include "ISystem.hpp"
#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Position.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "CollideSystem.hpp"
#include "AISystem.hpp"

#define GAME_MAP_WIDTH 15
#define GAME_MAP_HEIGHT 15
#define GAME_TILE_SIZE 12
#define GAME_NB_INDESTRUCTIBLE_WALL 0  //(GAME_MAP_WIDTH * GAME_MAP_HEIGHT) / 7
#define GAME_NB_DESTRUCTIBLE_WALL (GAME_MAP_WIDTH * GAME_MAP_HEIGHT) / 3

struct Vector3;

namespace indie
{
    class IEntity;
    class Scene;
    class Position;

    class GameSystem : public ISystem
    {
    public:
        GameSystem() : _aiSystem(_collideSystem)
        {
            nbr_player = 4;
            nbr_ai = 0;
        };

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

        static const std::string getBinding(int keyboard);

        static int getTag(std::string key);

        static unsigned int getNbrPlayer() { return nbr_player; };

        static void setNbrPlayer(unsigned int nbr) { nbr_player = nbr; };

        static void setPlaySupporters(bool play);
        static bool getPlaySupporters();

        /// @brief setter for the supporter animations
        static void setStartTime(std::chrono::time_point<std::chrono::high_resolution_clock>);
        /// @brief getter for the supporter animations
        static std::chrono::time_point<std::chrono::high_resolution_clock> getStartTime();

        /// @brief setter for the frames of the supporter animations
        static void setNbFrame(int);
        /// @brief getter for the frames of the supporter animations
        static int getNbFrame();
        static unsigned int getNbrAi() { return nbr_ai; };

        static void setNbrAi(unsigned int nbr) { nbr_ai = nbr; };

    private:
        std::unique_ptr<IScene> createScene();
        std::unique_ptr<IScene> createSplashScreen();
        std::unique_ptr<IScene> createMainMenu();
        std::unique_ptr<IScene> createSoundMenu();
        std::unique_ptr<IScene> createHelpMenu();
        std::unique_ptr<IScene> createControllerMenu();
        std::unique_ptr<IScene> createPreGameMenu();
        std::unique_ptr<IScene> createPauseMenu();
        static void createPlayerUI(IScene &, std::shared_ptr<IEntity>);
        void createSupporters(IScene &);
        void createSupporter(IScene &, Position pos);
        void createNumberEvent(std::shared_ptr<Entity> &entity, int _nbr_player);
        std::unique_ptr<IScene> createEndMenu();
        void changeBindings(SceneManager &SceneManager, int id_player, int button);
        void createSceneEvent(std::shared_ptr<Entity> &scene, SceneManager::SceneType sceneType);
        void createSoundEvent(std::shared_ptr<Entity> &sound, std::string value);
        std::shared_ptr<Entity> createButton(std::string path, Position position, int heigh, int width);
        std::shared_ptr<Entity> createText(std::string text, Position position, float fontSize);
        void createBindingsEvent(std::shared_ptr<Entity> &entity, int id_player, int button);
        void replaceTextBindings(indie::SceneManager &sceneManager, std::shared_ptr<Player> players, int firstText);
        void updateTextBindings(indie::SceneManager &sceneManager, std::shared_ptr<Player> players, int firstText);
        static void createPlayer(IScene &scene, int keyRight, int keyLeft, int keyUp, int keyDown, int keyBomb, int id, Position pos);
        void updatePlayerUI(SceneManager &, std::vector<std::shared_ptr<IEntity>> &);
        static void createAIPlayer(IScene &scene, int id, Position pos);
        void updatePlayers(SceneManager &scene, uint64_t dt);
        void updateAIs(SceneManager &scene, uint64_t dt);
        void updateBombs(SceneManager &scene, uint64_t dt);
        std::shared_ptr<IEntity> createCamera(Vector3 camPos, Vector3 camTarget);
        void createBonus(IScene &scene, const Position &pos);
        void generateMap(const std::string &filename, IScene &scene);
        void createSpawn(int x, int y, IScene &scene);
        static void createMusic(Scene &scene);
        static void createSound(Scene &scene);

        int timeElasped = 0;
        static unsigned int nbr_player;
        static unsigned int nbr_ai;
        static const Position _uiPos[4];
        static const std::map<int, std::string> _bindings;
        static bool _playSupporters;
        static std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
        static int _nbFrame;
        CollideSystem _collideSystem;
        AISystem _aiSystem;
    };
}

#endif /* GAME_SYSTEM_HPP */