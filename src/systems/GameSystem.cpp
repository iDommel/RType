/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** GameSystem.cpp
*/

#include "raylib.h"

#include <functional>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>

#include "GameSystem.hpp"
#include "EventSystem.hpp"
#include "CollideSystem.hpp"
#include "UIComponent.hpp"
#include "Camera3DComponent.hpp"
#include "GamepadStickCallbacks.hpp"
#include "Core.hpp"
#include "Entity.hpp"
#include "EventListener.hpp"
#include "HitboxComponent.hpp"
#include "AudioDevice.hpp"
#include "Player.hpp"
#include "AIPlayer.hpp"
#include "Radar.hpp"
#include "Position.hpp"
#include "Rect.hpp"
#include "Scene.hpp"
#include "Sprite.hpp"
#include "String.hpp"
#include "Bonus.hpp"
#include "Model3D.hpp"
#include "Sphere.hpp"
#include "Grid.hpp"
#include "Velocity.hpp"
#include "Bomb.hpp"
#include "Timer.hpp"
#include "Destructible.hpp"
#include "SoundComponent.hpp"
#include "MusicComponent.hpp"
#include "ParticleCloud.hpp"
#include "ModelAnim.hpp"
#include "Window.hpp"

namespace ecs
{
    std::vector<Position> GameSystem::_playerSpawns;

    const std::string GameSystem::getBinding(int keyboard)
    {
        return (_bindings.find(keyboard)->second);
    }

    int GameSystem::getTag(std::string key)
    {
        for (auto e = _bindings.begin(); e != _bindings.end(); e++)
            if (e->second == key)
                return (e->first);
        return (0);
    }

    const std::map<int, std::string> GameSystem::_bindings = {
        {KEY_DOWN, "down"},
        {KEY_UP, "up"},
        {KEY_LEFT, "left"},
        {KEY_RIGHT, "right"},
        {KEY_SPACE, "space"},
        {KEY_ENTER, "enter"},
        {KEY_BACKSPACE, "backspace"},
        {KEY_ESCAPE, "escape"},
        {KEY_END, "end"},
        {KEY_RIGHT_CONTROL, "right_ctrl"},
        {KEY_A, "A"},
        {KEY_B, "B"},
        {KEY_C, "C"},
        {KEY_D, "D"},
        {KEY_E, "E"},
        {KEY_F, "F"},
        {KEY_G, "G"},
        {KEY_H, "H"},
        {KEY_I, "I"},
        {KEY_J, "J"},
        {KEY_K, "K"},
        {KEY_L, "L"},
        {KEY_M, "M"},
        {KEY_N, "N"},
        {KEY_O, "O"},
        {KEY_P, "P"},
        {KEY_Q, "Q"},
        {KEY_R, "R"},
        {KEY_S, "S"},
        {KEY_T, "T"},
        {KEY_U, "U"},
        {KEY_V, "V"},
        {KEY_W, "W"},
        {KEY_X, "X"},
        {KEY_Y, "Y"},
        {KEY_Z, "Z"}};

    unsigned int GameSystem::nbr_player;

    unsigned int GameSystem::nbr_ai;

    void GameSystem::init(ecs::SceneManager &sceneManager)
    {
        std::cerr << "GameSystem::init" << std::endl;
        sceneManager.addScene(createSplashScreenScene(), SceneType::SPLASH);
        // sceneManager.addScene(createMainMenuScene(), SceneType::MAIN_MENU);
        sceneManager.addScene(createConnectionScene(), SceneType::CONNECTION);
        sceneManager.addScene(createLobbyScene(), SceneType::LOBBY);
        sceneManager.addScene(createGameScene(), SceneType::GAME);
        if (Core::networkRole == NetworkRole::CLIENT)
            sceneManager.setCurrentScene(SceneType::SPLASH);
        else if (Core::networkRole == NetworkRole::SERVER)
            sceneManager.setCurrentScene(SceneType::LOBBY);
        _collideSystem.init(sceneManager);
        AudioDevice::getMasterVolume() = 0.5;
        _aiSystem.init(sceneManager);
    }

    void GameSystem::replaceTextBindings(ecs::SceneManager &sceneManager, std::shared_ptr<Player> players, int firstText)
    {
        if (SceneManager::getCurrentSceneType() == SceneType::CONTROLLER) {
            if (players->changeUp == 2 || players->changeUp == 0) {
                auto components = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText];
                auto text = (*components)[IComponent::Type::TEXT];
                auto value = Component::castComponent<String>(text);
                value->getValue() = players->getUp();
                players->changeUp = 0;
            }
            if (players->changeLeft == 2 || players->changeLeft == 0)
            {
                auto components = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText + 1];
                auto text = components->getFilteredComponents({IComponent::Type::TEXT});
                auto value = Component::castComponent<String>(text[0]);
                value->getValue() = players->getLeft();
                players->changeLeft = 0;
            }
            if (players->changeRight == 2 || players->changeRight == 0)
            {
                auto components = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText + 2];
                auto text = (*components)[IComponent::Type::TEXT];
                auto value = Component::castComponent<String>(text);
                value->getValue() = players->getRight();
                players->changeRight = 0;
            }
            if (players->changeDown == 2 || players->changeDown == 0)
            {
                auto components = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText + 3];
                auto text = (*components)[IComponent::Type::TEXT];
                auto value = Component::castComponent<String>(text);
                value->getValue() = players->getDown();
                players->changeDown = 0;
            }
            if (players->changeBomb == 2 || players->changeBomb == 0)
            {
                auto components = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText + 4];
                auto text = (*components)[IComponent::Type::TEXT];
                auto value = Component::castComponent<String>(text);
                value->getValue() = players->getBomb();
                players->changeBomb = 0;
            }
        }
    }

    void GameSystem::updateTextBindings(ecs::SceneManager &sceneManager, std::shared_ptr<Player> players, int firstText)
    {
        if (players->changeUp == 1)
        {
            auto entity = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText];
            auto text = (*entity)[IComponent::Type::TEXT];
            auto value = Component::castComponent<String>(text);
            value->getValue() = "|";
        }
        else if (players->changeLeft == 1)
        {
            auto components = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText + 1];
            auto text = (*components)[IComponent::Type::TEXT];
            auto value = Component::castComponent<String>(text);
            value->getValue() = "|";
        }
        else if (players->changeRight == 1)
        {
            auto components = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText + 2];
            auto text = (*components)[IComponent::Type::TEXT];
            auto value = Component::castComponent<String>(text);
            value->getValue() = "|";
        }
        else if (players->changeDown == 1)
        {
            auto components = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText + 3];
            auto text = (*components)[IComponent::Type::TEXT];
            auto value = Component::castComponent<String>(text);
            value->getValue() = "|";
        }
        else if (players->changeBomb == 1)
        {
            auto components = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText + 4];
            auto text = (*components)[IComponent::Type::TEXT];
            auto value = Component::castComponent<String>(text);
            value->getValue() = "|";
        }
    }

    void GameSystem::update(ecs::SceneManager &sceneManager, uint64_t dt)
    {
        // int firstText = 9;

        // if (SceneManager::getCurrentSceneType() == SceneType::CONTROLLER) {
        //     for (auto &e : sceneManager.getScene(SceneType::GAME)[IEntity::Tags::PLAYER]) {
        //         auto players = Component::castComponent<Player>((*e)[IComponent::Type::PLAYER]);
        //         updateTextBindings(sceneManager, players, firstText);
        //         replaceTextBindings(sceneManager, players, firstText);
        //         firstText += 5;
        //     }
        // }
        if (sceneManager.getCurrentSceneType() == SceneType::SPLASH) {
            timeElasped += dt;
            if (timeElasped > SPLASH_TIMEOUT)
            {
                if (Core::networkRole == NetworkRole::CLIENT)
                    sceneManager.setCurrentScene(SceneType::CONNECTION);
                if (Core::networkRole == NetworkRole::SERVER)
                    sceneManager.setCurrentScene(SceneType::GAME);
                timeElasped = 0;
            }
        } else if (sceneManager.getCurrentSceneType() == SceneType::CONNECTION) {
            timeElasped += dt;
            if (timeElasped > CONNECTION_TIMEOUT && Core::networkRole == NetworkRole::CLIENT)
            {
                std::cerr << "Connection failed" << std::endl;
                sceneManager.setShouldClose(true);
            }
        }
        updatePlayers(sceneManager, dt);
        // _aiSystem.update(sceneManager, dt);
        // _collideSystem.update(sceneManager, dt);

        // auto renderables = sceneManager.getCurrentScene()[IEntity::Tags::RENDERABLE_3D];

        // for (auto &renderable : renderables) {
        //     if (renderable->hasComponent(IComponent::Type::ANIMATION)) {
        //         auto component = Component::castComponent<ModelAnim>((*renderable)[IComponent::Type::ANIMATION]);
        //         if (component->getNbFrames() == -1)
        //             continue;
        //         component->triggerPlay(renderable);
        //         if (!component->shouldPlay())
        //             continue;
        //         component->getCurrentFrame()++;
        //         if (component->getCurrentFrame() >= component->getNbFrames())
        //             component->getCurrentFrame() = 0;
        //     }
        // }
    }

    std::unique_ptr<IScene> GameSystem::createConnectionScene()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createConnectionScene, this), SceneType::CONNECTION);
        std::shared_ptr<Entity> entity = createText("Waiting for connection...", Position(200, 150), 30);

        scene->addEntity(entity);
        return scene;
    }

    void GameSystem::destroy()
    {
        std::cerr << "GameSystem::destroy" << std::endl;
        _collideSystem.destroy();
    }

    std::shared_ptr<Entity> GameSystem::createImage(std::string path, Position position, int heigh, int width)
    {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>();
        std::shared_ptr<Sprite> component = std::make_shared<Sprite>(path);
        std::shared_ptr<Position> component2 = std::make_shared<Position>(position);
        std::shared_ptr<Rect> component3 = std::make_shared<Rect>(0, 0, heigh, width);

        entity->addComponent(component2)
            .addComponent(component)
            .addComponent(component3);
        return (entity);
    }

    std::shared_ptr<Entity> GameSystem::createText(std::string text, Position position, float fontSize)
    {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>();
        std::shared_ptr<String> component = std::make_shared<String>(text, "", fontSize);
        std::shared_ptr<Position> component2 = std::make_shared<Position>(position);

        entity->addComponent(component2)
            .addComponent(component);
        return (entity);
    }

    void GameSystem::createSoundEvent(std::shared_ptr<Entity> &entity, std::string value)
    {
        MouseCallbacks mouseCallbacks([value, entity](SceneManager &sceneManger, Vector2 mousePosition)
                                      {
                auto comp = entity->getFilteredComponents({IComponent::Type::SPRITE, IComponent::Type::POSITION, IComponent::Type::RECT});
                auto pos = Component::castComponent<Position>(comp[1]);
                auto sprite = Component::castComponent<Sprite>(comp[0]);
                auto rect = Component::castComponent<Rect>(comp[2]);

                if (mousePosition.x > pos->x && mousePosition.x < pos->x + rect->width &&
                    mousePosition.y > pos->y && mousePosition.y < pos->y + rect->height) {
                    auto comp2 = sceneManger.getCurrentScene()[IEntity::Tags::TEXT][2];
                    auto text = (*comp2)[IComponent::Type::TEXT];
                    auto value2 = Component::castComponent<String>(text);
                    if (AudioDevice::getMasterVolume() < 1 && value == "+") {
                        AudioDevice::getMasterVolume() += 0.1;
                        AudioDevice::setVolume(AudioDevice::getMasterVolume());
                        value2->getValue() = std::to_string(int(AudioDevice::getMasterVolume() * 100));
                    } else if (AudioDevice::getMasterVolume() >= 0.1 && value == "-") {
                        AudioDevice::getMasterVolume() -= 0.1;
                        AudioDevice::setVolume(AudioDevice::getMasterVolume());
                        value2->getValue() = std::to_string(int(AudioDevice::getMasterVolume() * 100));
                    }
                } },
                                      [](SceneManager &, Vector2 /*mousePosition*/) {},
                                      [](SceneManager &, Vector2 /*mousePosition*/) {},
                                      [](SceneManager &, Vector2 /*mousePosition*/) {});

        std::shared_ptr<EventListener> eventListener = std::make_shared<EventListener>();

        eventListener->addMouseEvent(MOUSE_BUTTON_LEFT, mouseCallbacks);
        entity->addComponent(eventListener);
    }

    void GameSystem::createSceneEvent(std::shared_ptr<Entity> &entity, SceneType scenetype)
    {
        MouseCallbacks mouseCallbacks(
            [scenetype, entity, this](SceneManager &sceneManager, Vector2 mousePosition)
            {
                auto comp = entity->getFilteredComponents({IComponent::Type::SPRITE, IComponent::Type::POSITION, IComponent::Type::RECT});
                auto pos = Component::castComponent<Position>(comp[1]);
                auto sprite = Component::castComponent<Sprite>(comp[0]);
                auto rect = Component::castComponent<Rect>(comp[2]);

                if (mousePosition.x > pos->x && mousePosition.x < pos->x + rect->width &&
                    mousePosition.y > pos->y && mousePosition.y < pos->y + rect->height) {
                    if (scenetype == SceneType::PREVIOUS)
                        sceneManager.setCurrentScene(SceneManager::getPreviousSceneType());
                    else if (scenetype == SceneType::NONE) {
                        exit(0);
                    } else if (scenetype == SceneType::GAME && sceneManager.getCurrentSceneType() != SceneType::PAUSE) {
                        sceneManager.setCurrentScene(SceneType::GAME, true);
                        _collideSystem.reloadCollidables3D(sceneManager);
                        EventSystem::reloadScene(sceneManager, SceneType::GAME);
                    } else
                        sceneManager.setCurrentScene(scenetype);
                }
            },
            [](SceneManager &, Vector2 /*mousePosition*/) {},
            [](SceneManager &, Vector2 /*mousePosition*/) {},
            [](SceneManager &, Vector2 /*mousePosition*/) {});

        std::shared_ptr<EventListener> eventListener = std::make_shared<EventListener>();

        eventListener->addMouseEvent(MOUSE_BUTTON_LEFT, mouseCallbacks);
        entity->addComponent(eventListener);
    }

    void GameSystem::createMsgEvent(std::shared_ptr<Entity> &entity, const std::string &msg)
    {
        std::shared_ptr<EventListener> eventListener = std::make_shared<EventListener>();
        MouseCallbacks mouseCallbacks(
            [entity, this, msg](SceneManager &sceneManager, Vector2 mousePosition)
            {
                auto comp = entity->getFilteredComponents({IComponent::Type::SPRITE, IComponent::Type::POSITION, IComponent::Type::RECT});
                auto pos = Component::castComponent<Position>(comp[1]);
                auto sprite = Component::castComponent<Sprite>(comp[0]);
                auto rect = Component::castComponent<Rect>(comp[2]);

                if (mousePosition.x > pos->x && mousePosition.x < pos->x + rect->width &&
                    mousePosition.y > pos->y && mousePosition.y < pos->y + rect->height)
                {
                    emit writeMsg(msg);
                }
            },
            [](SceneManager &, Vector2 /*mousePosition*/) {},
            [](SceneManager &, Vector2 /*mousePosition*/) {},
            [](SceneManager &, Vector2 /*mousePosition*/) {});

        eventListener->addMouseEvent(MOUSE_BUTTON_LEFT, mouseCallbacks);
        entity->addComponent(eventListener);
    }

    void GameSystem::createBindingsEvent(std::shared_ptr<Entity> &entity, int id_player, int button)
    {
        MouseCallbacks mouseCallbacks(
            [entity, button, id_player, this](SceneManager &sceneManager, Vector2 mousePosition)
            {
                auto comp = (*entity)[IComponent::Type::POSITION];
                auto pos = Component::castComponent<Position>(comp);

                if (mousePosition.x > pos->x && mousePosition.x < pos->x + 50 &&
                    mousePosition.y > pos->y && mousePosition.y < pos->y + 20)
                {
                    changeBindings(sceneManager, id_player, button);
                }
            },
            [](SceneManager &, Vector2 /*mousePosition*/) {},
            [](SceneManager &, Vector2 /*mousePosition*/) {},
            [entity, button, id_player](SceneManager &sceneManager, Vector2 /*mousePosition*/) {
                auto component = sceneManager.getScene(SceneType::GAME)[IEntity::Tags::PLAYER][id_player];
                auto comp = component->getFilteredComponents({IComponent::Type::PLAYER, IComponent::Type::EVT_LISTENER});
                auto player = Component::castComponent<Player>(comp[0]);
                auto event = Component::castComponent<EventListener>(comp[1]);
                std::string get = "";
                char input = 0;

                if (player->changeUp == 1)
                {
                    input = Window::getKeyPressed();
                    if (input != 0)
                    {
                        get.assign(1, input);
                        event->replaceKeyboardEvent((KeyboardKey)player->getTagUp(), (KeyboardKey)GameSystem::getTag(get));
                        player->setUP(get);
                        player->changeUp = 2;
                    }
                }
                else if (player->changeLeft == 1)
                {
                    input = Window::getKeyPressed();
                    if (input != 0)
                    {
                        get.assign(1, input);
                        event->replaceKeyboardEvent((KeyboardKey)player->getTagLeft(), (KeyboardKey)GameSystem::getTag(get));
                        player->setLEFT(get);
                        player->changeLeft = 2;
                    }
                }
                else if (player->changeRight == 1)
                {
                    input = Window::getKeyPressed();
                    if (input != 0)
                    {
                        get.assign(1, input);
                        event->replaceKeyboardEvent((KeyboardKey)player->getTagRight(), (KeyboardKey)GameSystem::getTag(get));
                        player->setRIGHT(get);
                        player->changeRight = 2;
                    }
                }
                else if (player->changeDown == 1)
                {
                    input = Window::getKeyPressed();
                    if (input != 0)
                    {
                        get.assign(1, input);
                        event->replaceKeyboardEvent((KeyboardKey)player->getTagDown(), (KeyboardKey)GameSystem::getTag(get));
                        player->setDOWN(get);
                        player->changeDown = 2;
                    }
                }
                else if (player->changeBomb == true)
                {
                    input = Window::getKeyPressed();
                    if (input != 0)
                    {
                        get.assign(1, input);
                        event->replaceKeyboardEvent((KeyboardKey)player->getTagBomb(), (KeyboardKey)GameSystem::getTag(get));
                        player->setBOMB(get);
                        player->changeBomb = 2;
                    }
                }
            });

        std::shared_ptr<EventListener> eventListener = std::make_shared<EventListener>();

        eventListener->addMouseEvent(MOUSE_BUTTON_LEFT, mouseCallbacks);
        entity->addComponent(eventListener);
    }

    void GameSystem::createNumberEvent(std::shared_ptr<Entity> &entity, int _nbr_player)
    {
        MouseCallbacks selector(
            [entity, _nbr_player, this](SceneManager &sceneManager, Vector2 mousePosition)
            {
                auto comp = (*entity)[IComponent::Type::POSITION];
                auto pos = Component::castComponent<Position>(comp);

                if (mousePosition.x > pos->x && mousePosition.x < pos->x + 50 &&
                    mousePosition.y > pos->y && mousePosition.y < pos->y + 50)
                {
                    auto entity = sceneManager.getCurrentScene()[IEntity::Tags::SPRITE_2D][3];
                    auto component = (*entity)[IComponent::Type::POSITION];
                    auto pos1 = Component::castComponent<Position>(component);

                    pos1->x = pos->x - 30;
                    pos1->y = pos->y - 20;
                    nbr_player = _nbr_player;
                    nbr_ai = 4 - _nbr_player;
                }
            },
            [](SceneManager &, Vector2) {},
            [](SceneManager &, Vector2) {},
            [](SceneManager &, Vector2) {});

        std::shared_ptr<EventListener> eventListener = std::make_shared<EventListener>();

        eventListener->addMouseEvent(MOUSE_BUTTON_LEFT, selector);
        entity->addComponent(eventListener);
    }

    std::shared_ptr<IEntity> GameSystem::create3DCamera(Vector3 camPos, Vector3 camTarget)
    {
        std::shared_ptr<Entity> cam = std::make_shared<Entity>();
        std::shared_ptr<CameraComponent> camera = std::make_shared<CameraComponent>(camTarget, camPos);

        cam->addComponent(camera);
        return cam;
    }

    void GameSystem::updatePlayers(SceneManager &sceneManager, uint64_t dt)
    {
        auto players = sceneManager.getCurrentScene()[IEntity::Tags::PLAYER];

        for (auto &player : players)
        {
            auto pos = Component::castComponent<Position>((*player)[IComponent::Type::POSITION]);
            auto lastPos = *pos;
            auto vel = Component::castComponent<Velocity>((*player)[IComponent::Type::VELOCITY]);
            auto playerComp = Component::castComponent<Player>((*player)[IComponent::Type::PLAYER]);
            auto hitbox = Component::castComponent<Hitbox>((*player)[IComponent::Type::HITBOX]);
            auto splitVel = *vel;

            splitVel.y = 0;
            (*pos) = (*pos) + (splitVel * (float)(dt / 1000.0f));
            (*hitbox) += splitVel * (float)(dt / 1000.0f);
            for (auto &collider : _collideSystem.getColliders(player))
            {
                std::cout << "PERDU !" << std::endl;
                //sceneManager.setCurrentScene(SceneType::END);
            }

            splitVel.y = (*vel).y;
            splitVel.x = 0;
            (*pos) = (*pos) + (splitVel * (float)(dt / 1000.0f));
            (*hitbox) += splitVel * (float)(dt / 1000.0f);
        }
    }

    std::unique_ptr<IScene> GameSystem::createSplashScreenScene()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createSplashScreenScene, this), SceneType::SPLASH);
        std::shared_ptr<Entity> entity = std::make_shared<Entity>();
        std::shared_ptr<Position> pos = std::make_shared<Position>(550, 350);
        std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/enemy/sprites/enemy1.png");
        std::shared_ptr<Entity> entity2 = createText("R-Type", Position(200, 50), 50);
        std::shared_ptr<Entity> entity3 = createText("Clearly made by us", Position(250, 100), 30);
        std::shared_ptr<Entity> entity4 = createText("Iona Dommel-Prioux\nAntoine Penot\nCamille Maux\nIzaac Carcenac-Sautron\nLéo Maman\nCyril Dehaese\nRoxanne Baert", Position(10, 450), 15);

        scene->addEntities({entity, entity2, entity3, entity4});
        return scene;
    }

    std::unique_ptr<ecs::IScene> GameSystem::createMainMenuScene()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createMainMenuScene, this), SceneType::MAIN_MENU);
        std::shared_ptr<Entity> backgroundEntity = std::make_shared<Entity>();
        std::shared_ptr<Entity> playButtonEntity = createImage("assets/MainMenu/play_unpressed.png", Position(800 / 2 - 60, 500 / 2 - 18), 120, 28);
        std::shared_ptr<Sprite> component = std::make_shared<Sprite>("assets/Background/Background1.png");
        std::shared_ptr<Position> component2 = std::make_shared<Position>(800 / 2 - 400, 600 / 2 - 300);

        backgroundEntity->addComponent(component2)
            .addComponent(component);
        createSceneEvent(playButtonEntity, SceneType::GAME);
        scene->addEntities({backgroundEntity, playButtonEntity});
        return scene;
    }

    std::unique_ptr<IScene> GameSystem::createLobbyScene()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createLobbyScene, this), SceneType::LOBBY);
        std::shared_ptr<Entity> backgroundEntity = std::make_shared<Entity>();
        std::shared_ptr<Sprite> bg = std::make_shared<Sprite>("assets/Background/Background1.png");
        std::shared_ptr<Position> bgPos = std::make_shared<Position>(800 / 2 - 400, 600 / 2 - 300);
        std::shared_ptr<Entity> playButtonEntity = createImage("assets/MainMenu/play_unpressed.png", Position(800 / 2 - 60, 500 / 2 - 18), 120, 28);

        backgroundEntity->addComponent(bg)
            .addComponent(bgPos);
        createMsgEvent(playButtonEntity, READY);
        scene->addEntities({backgroundEntity, playButtonEntity});
        return scene;
    }

    std::unique_ptr<IScene> GameSystem::createGameScene()
    {
        ButtonCallbacks pauseCallbacks(
            [](SceneManager &) {},
            [](SceneManager &scenemanager)
            {
                scenemanager.setCurrentScene(SceneType::PAUSE);
            },
            [](SceneManager &) {},
            [](SceneManager &) {});
        return ReadMap();
    }

    void GameSystem::createMusic(Scene &scene)
    {
        std::shared_ptr<Entity> musicEntity = std::make_shared<Entity>();
        std::shared_ptr<MusicComponent> musicComponent = std::make_shared<MusicComponent>("assets/music/exports/space-asteroids.ogg");

        musicEntity->addComponent(musicComponent);
        scene.addEntities({musicEntity});
    }

    void GameSystem::activateNetwork()
    {
        _networkActivated = true;
    }

    bool GameSystem::isNetworkActivated()
    {
        return _networkActivated;
    }

    void GameSystem::createPlayer(IScene &scene, int keyRight, int keyLeft, int keyUp, int keyDown, int keyBomb, int id, Position pos, bool isMe)
    {
        std::shared_ptr<Entity> playerEntity = std::make_shared<Entity>();
        std::shared_ptr<Position> playerPos = std::make_shared<Position>(pos);
        std::shared_ptr<Velocity> playerVel = std::make_shared<Velocity>(0, 0);
        Rectangle rect = {playerPos->x + SCALE / 4, playerPos->y + SCALE / 4, SCALE, SCALE};
        std::shared_ptr<Hitbox> playerHitbox = std::make_shared<Hitbox>(rect);
        std::shared_ptr<Player> player = std::make_shared<Player>(id, keyUp, keyDown, keyLeft, keyRight, keyBomb);
        std::shared_ptr<EventListener> playerListener = std::make_shared<EventListener>();
        std::shared_ptr<Sprite> playerSprite = std::make_shared<Sprite>("assets/Player/MainShip.png", 0.0f, 2.0f);
        std::shared_ptr<Destructible> destruct = std::make_shared<Destructible>();
        ButtonCallbacks moveRightCallbacks(
            [&, this, player, playerEntity, id](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg("PLAYER "+ std::to_string(id) +" RIGHT PRESSED");
                else
                    player->moveRight(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity, id](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg("PLAYER "+ std::to_string(id) +" RIGHT RELEASED");
                else
                    player->stopRight(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity, id](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg("PLAYER "+ std::to_string(id) +" RIGHT DOWN");
                else
                    player->moveRight(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity, id](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg("PLAYER "+ std::to_string(id) +" RIGHT UP");
                else
                    player->stopRight(manager, playerEntity, 1);
            });
        ButtonCallbacks moveLeftCallbacks(
            [&, this, player, playerEntity, id](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg("PLAYER "+ std::to_string(id) +" LEFT PRESSED");
                else
                    player->moveLeft(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity, id](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg("PLAYER "+ std::to_string(id) +" LEFT RELEASED");
                else
                    player->stopLeft(manager, playerEntity, 17);
            },
            [&, this, player, playerEntity, id](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg("PLAYER "+ std::to_string(id) +" LEFT DOWN");
                else
                    player->moveLeft(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity, id](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg("PLAYER "+ std::to_string(id) +" LEFT UP");
                else
                    player->stopLeft(manager, playerEntity, 17);
            });
        ButtonCallbacks moveUpCallbacks(
            [&, this, player, playerEntity, id](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg("PLAYER "+ std::to_string(id) +" UP PRESSED");
                else
                    player->moveUp(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity, id](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg("PLAYER "+ std::to_string(id) +" UP RELEASED");
                else
                    player->stopUp(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity, id](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg("PLAYER "+ std::to_string(id) +" UP DOWN");
                else
                    player->moveUp(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity, id](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg("PLAYER "+ std::to_string(id) +" UP UP");
                else
                    player->stopUp(manager, playerEntity, 1);
            });
        ButtonCallbacks moveDownCallbacks(
            [&, this, player, playerEntity, id](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg("PLAYER "+ std::to_string(id) +" DOWN PRESSED");
                else
                    player->moveDown(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity, id](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg("PLAYER "+ std::to_string(id) +" DOWN RELEASED");
                else
                    player->stopDown(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity, id](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg("PLAYER "+ std::to_string(id) +" DOWN DOWN");
                else
                    player->moveDown(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity, id](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg("PLAYER "+ std::to_string(id) +" DOWN UP");
                else
                    player->stopDown(manager, playerEntity, 1);
            });

        std::function<void(SceneManager &, float)> moveHorizontalStickCallback = [&, this, player, playerEntity](SceneManager &manager, float value)
        {
            if (this->isNetworkActivated())
                emit writeMsg("PLAYER STICK MOVED HORIZONTALLY");
            else
                player->moveHorizontal(manager, playerEntity, value);
        };
        std::function<void(SceneManager &, float)> moveVerticalStickCallback = [&, this, player, playerEntity](SceneManager &manager, float value)
        {
            if (this->isNetworkActivated())
                emit writeMsg("PLAYER STICK MOVED VERTICALLY");
            else
                player->moveVertical(manager, playerEntity, value);
        };

        if (isMe) {
            playerListener->addKeyboardEvent((KeyboardKey)player->getTagUp(), moveUpCallbacks);
            playerListener->addKeyboardEvent((KeyboardKey)player->getTagLeft(), moveLeftCallbacks);
            playerListener->addKeyboardEvent((KeyboardKey)player->getTagRight(), moveRightCallbacks);
            playerListener->addKeyboardEvent((KeyboardKey)player->getTagDown(), moveDownCallbacks);
            playerListener->addGamepadEvent(id - 1, (GamepadButton)GAMEPAD_BUTTON_LEFT_FACE_UP, moveUpCallbacks);
            playerListener->addGamepadEvent(id - 1, (GamepadButton)GAMEPAD_BUTTON_LEFT_FACE_RIGHT, moveRightCallbacks);
            playerListener->addGamepadEvent(id - 1, (GamepadButton)GAMEPAD_BUTTON_LEFT_FACE_DOWN, moveDownCallbacks);
            playerListener->addGamepadEvent(id - 1, (GamepadButton)GAMEPAD_BUTTON_LEFT_FACE_LEFT, moveLeftCallbacks);
            playerListener->addGamepadStickEvent(id - 1, GAMEPAD_AXIS_LEFT_X, moveHorizontalStickCallback);
            playerListener->addGamepadStickEvent(id - 1, GAMEPAD_AXIS_LEFT_Y, moveVerticalStickCallback);
            playerEntity->addComponent(playerListener);
        }
        playerEntity->addComponents({player, playerPos, playerSprite, playerVel, playerHitbox, destruct});
        scene.addEntity(playerEntity);
    }

    void GameSystem::onEntityAdded(std::shared_ptr<IEntity> entity, SceneType scene)
    {
        _collideSystem.onEntityAdded(entity, scene);
    }

    void GameSystem::onEntityRemoved(std::shared_ptr<IEntity> entity)
    {
        _collideSystem.onEntityRemoved(entity);
        if (entity->hasComponent(IComponent::Type::PLAYER))
            nbr_player -= 1;
        else if (entity->hasComponent(IComponent::Type::AI))
            nbr_ai -= 1;
    }

    void GameSystem::changeBindings(SceneManager &sceneManager, int id_player, int button)
    {
        auto entity = sceneManager.getScene(SceneType::GAME)[IEntity::Tags::PLAYER][id_player];
        auto component = (*entity)[IComponent::Type::PLAYER];
        auto player = Component::castComponent<Player>(component);

        switch (button)
        {
        case 0:
            player->changeUp = 1;
            player->changeDown = 0;
            player->changeLeft = 0;
            player->changeRight = 0;
            break;
        case 1:
            player->changeLeft = 1;
            player->changeDown = 0;
            player->changeRight = 0;
            player->changeUp = 0;
            break;
        case 2:
            player->changeRight = 1;
            player->changeDown = 0;
            player->changeLeft = 0;
            player->changeUp = 0;
            break;
        case 3:
            player->changeDown = 1;
            player->changeLeft = 0;
            player->changeRight = 0;
            player->changeUp = 0;
            break;
        case 4:
            player->changeBomb = 1;
        }
    }
}