/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** GameSystem.hpp
*/

#ifndef GAME_SYSTEM_HPP
#define GAME_SYSTEM_HPP

#include <chrono>

#include "Message.hpp"
#include "ISystem.hpp"
#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Position.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "CollideSystem.hpp"
#include "AISystem.hpp"
#include "ANetworkSystem.hpp"
#include <QtCore>  // for networked event handling

#define GAME_MAP_WIDTH 15
#define GAME_MAP_HEIGHT 15
#define GAME_TILE_SIZE 12
#define GAME_NB_INDESTRUCTIBLE_WALL 0  //(GAME_MAP_WIDTH * GAME_MAP_HEIGHT) / 7
#define GAME_NB_DESTRUCTIBLE_WALL (GAME_MAP_WIDTH * GAME_MAP_HEIGHT) / 3

#define SPLASH_TIMEOUT 3000       // value in milliseconds
#define CONNECTION_TIMEOUT 30000  // value in milliseconds

struct Vector3;

namespace ecs
{
    class IEntity;
    class Scene;
    class Position;

    class GameSystem : public QObject, public ISystem
    {
        Q_OBJECT
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
         * @param scene Scene to add entity into
         */
        void onEntityAdded(std::shared_ptr<IEntity> entity, SceneType scene) final;
        /**
         * @brief The callback to be called when an entity is removed from a scene
         * @param entity The Entity that was removed
         */
        void onEntityRemoved(std::shared_ptr<IEntity> entity) final;

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

        /// @brief Toggles if the network functionalities are enabled
        void activateNetwork();
        /// @brief Checks if network is enabled
        /// @return if network is enabled
        bool isNetworkActivated();

        static std::vector<Position> playerSpawns;

    signals:
        void writeMsg(const Message &message);

    public slots:
        void createPlayer(IScene &scene, int keyRight, int keyLeft, int keyUp, int keyDown, int keyBomb, long unsigned int id, Position pos, bool isMe);
        void createSimpleMissile(IScene &scene, long unsigned int id, Position pos);

    private:
        /// @brief Read map file and generate all the game scene entities
        /// @return Return the scene with all the map entities
        std::unique_ptr<IScene> ReadMap();
        /// @brief Choose what sprite choose for the entity
        /// @return Return the entity with the good sprite
        std::shared_ptr<Entity> whichWall(std::string mapAround, int x, int y);
        /// @brief Choose what enemy generate
        /// @return Return the good entity or Nullptr if no entity match
        std::shared_ptr<Entity> whichEnemy(int mobId, int x, int y);

        /// @brief Adds a entity with a music component to a scene, the AudioSystem then loads it
        /// @param scene The scene to add the entity to
        static void createMusic(Scene &scene);
        /// @brief Adds a entity with a sound component to a scene, the AudioSystem then loads it
        /// @param scene The scene to add the entity to
        static void createSound(Scene &scene);
        /// @brief Create an image entity
        /// @param path Path to the image to load
        /// @param position Position of the Image
        /// @param heigh Height of the Image
        /// @param width Width of the Image
        /// @return Returns a pointer to an entity with an Image Component with information on its position and size
        std::shared_ptr<Entity> createImage(std::string path, Position position, int heigh, int width);
        /// @brief Create an Text entity
        /// @param text Text to display
        /// @param position Initial position of the text
        /// @param fontSize font size of the text
        /// @return Returns a pointer to an entity with an Text Component with information on its position and size
        std::shared_ptr<Entity> createText(std::string text, Position position, float fontSize);
        /// @brief Create a 3DCamera entity
        /// @param camPos Initial position of the camera
        /// @param camTarget Initial target of the camera
        /// @return Returns a pointer to an entity with an Camera Component with information on its position and target
        std::shared_ptr<IEntity> create3DCamera(Vector3 camPos, Vector3 camTarget);
        void createSoundEvent(std::shared_ptr<Entity> &sound, std::string value);
        void createNumberEvent(std::shared_ptr<Entity> &entity, int nbr_player);
        void createSceneEvent(std::shared_ptr<Entity> &scene, SceneType sceneType);
        void createBindingsEvent(std::shared_ptr<Entity> &entity, int id_player, int button);

        /// @brief Create a MouseEvent that writes a msg through the NetworkSystem
        /// @param entity Entity to add the mouse event to
        /// @param msg Message to send when left mouse button is pressed
        void createMsgEvent(std::shared_ptr<Entity> &entity, const std::string &msg);
        /// @brief Create a MouseEvent that writes a msg through the NetworkSystem
        /// @param entity Entity to add the mouse event to
        /// @param msg Message to send when left mouse button is pressed
        void createMsgEvent(std::shared_ptr<Entity> &entity, const NetworkMessageType &msg);

        std::unique_ptr<IScene> createGameScene();
        std::unique_ptr<IScene> createConnectionScene();
        std::unique_ptr<IScene> createSplashScreenScene();
        std::unique_ptr<IScene> createMainMenuScene();
        std::unique_ptr<IScene> createLobbyScene();

        void changeBindings(SceneManager &SceneManager, int id_player, int button);
        void replaceTextBindings(ecs::SceneManager &sceneManager, std::shared_ptr<Player> players, int firstText);

        void updateTextBindings(ecs::SceneManager &sceneManager, std::shared_ptr<Player> players, int firstText);
        void updatePlayers(SceneManager &scene, uint64_t dt);

        int timeElasped = 0;
        static unsigned int nbr_player;
        static unsigned int nbr_ai;
        static const Position _uiPos[4];
        static const std::map<int, std::string> _bindings;

        CollideSystem _collideSystem;
        AISystem _aiSystem;

        bool _networkActivated = false;
    };
}

#endif /* GAME_SYSTEM_HPP */