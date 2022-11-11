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
#include "Camera2DComponent.hpp"
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
#include "Animation2D.hpp"
#include "SpaceModule.hpp"

namespace ecs
{
    std::vector<Position> GameSystem::playerSpawns;
    std::vector<std::pair<Enemy::EnemyType, Position>> GameSystem::enemies;

    // Purge of out of bounds entities frequency in ms
    #define PURGE_FREQUENCY 200
    // Size of valib area around camera in px
    #define VALID_BORDER_SIZE 100

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

    std::map<Missile::MissileType, std::string> GameSystem::_missilesSprites = {

        {Missile::MissileType::P_SIMPLE, "assets/Player/BasicMissile.png"},
        {Missile::MissileType::P_CONDENSED, "assets/Player/ChargedMissile.png"},
        {Missile::MissileType::E_CLASSIC, "assets/Enemies/RedEnemy4/RedEnemy4 - Missile.png"},
        {Missile::MissileType::E_SINUSOIDAL, "assets/Enemies/RedEnemy3/RedEnemy3 - Missile.png"},
        {Missile::MissileType::E_HOMING_MISSILE, "assets/Enemies/RedEnemy2/RedEnemy2 - Missile.png"}};
    std::map<std::string, int> GameSystem::_spriteFrameCounts =
        {
            {"assets/Player/ChargedMissile.png", 5},
            {"assets/Player/BasicMissile.png", 5},
            {"assets/Player/MainShipSSP1.png", 4},
            {"assets/Player/MainShipSSP2.png", 4},
            {"assets/Player/MainShipSSP3.png", 4},
            {"assets/Player/MainShipSSP4.png", 4},
            {"assets/Enemies/RedEnemy2/RedEnemy2 - Missile.png", 4},
            {"assets/Enemies/RedEnemy3/RedEnemy3 - Missile.png", 4},
            {"assets/Enemies/RedEnemy4/RedEnemy4 - Missile.png", 4}};

    std::map<std::string, float> GameSystem::_spriteRotations =
        {
            {"assets/Player/ChargedMissile.png", 0.0F},
            {"assets/Player/BasicMissile.png", 0.0F},
            {"assets/Player/MainShipSSP1.png", 0.0F},
            {"assets/Enemies/RedEnemy2/RedEnemy2 - Missile.png", 180.0F},
            {"assets/Enemies/RedEnemy3/RedEnemy3 - Missile.png", 180.0F},
            {"assets/Enemies/RedEnemy4/RedEnemy4 - Missile.png", 180.0F}};

    std::map<std::string, Animation2D::AnimationType> GameSystem::_spriteAnimType = {
            {"assets/Player/ChargedMissile.png", Animation2D::AnimationType::ONCE},
            {"assets/Player/BasicMissile.png", Animation2D::AnimationType::ONCE},
            {"assets/Player/MainShipSSP1.png", Animation2D::AnimationType::ONCE},
            {"assets/Enemies/RedEnemy2/RedEnemy2 - Missile.png", Animation2D::AnimationType::LOOP},
            {"assets/Enemies/RedEnemy3/RedEnemy3 - Missile.png", Animation2D::AnimationType::LOOP},
            {"assets/Enemies/RedEnemy4/RedEnemy4 - Missile.png", Animation2D::AnimationType::LOOP}
    };

    std::map<Missile::MissileType, std::pair<std::function<float(float)>, std::function<float(float)>>> GameSystem::_missilesTrajectories = {
        {Missile::MissileType::P_SIMPLE, {[](float dt) { return 4 * dt; }, [](float) { return 0; }}},
        {Missile::MissileType::P_CONDENSED, {[](float dt) { return 4 * dt; }, [](float) { return 0; }}},
        {Missile::MissileType::E_CLASSIC, {[](float dt) { return -4 * dt; }, [](float) { return 0; }}},
        {Missile::MissileType::E_SINUSOIDAL, {[](float dt) { return -dt; }, [](float a) { return std::sin(a / 10) * 50; }}}};

    std::vector<std::string> GameSystem::_playersSprite =  {
        "assets/Player/MainShipSSP1.png",
        "assets/Player/MainShipSSP2.png",
        "assets/Player/MainShipSSP3.png",
        "assets/Player/MainShipSSP4.png"
    };

    std::vector<std::string> GameSystem::_modulesSprite = {
        "assets/Player/Module1.png",
        "assets/Player/Module2.png",
        "assets/Player/Module3.png",
        "assets/Player/Module4.png"
    };

    void GameSystem::init(ecs::SceneManager &sceneManager)
    {
        std::cerr << "GameSystem::init" << std::endl;
        sceneManager.addScene(createSplashScreenScene(), SceneType::SPLASH);
        sceneManager.addScene(createConnectionScene(), SceneType::CONNECTION);
        sceneManager.addScene(createLobbyScene(), SceneType::LOBBY);
        sceneManager.addScene(createSettingMenu(), SceneType::SOUND);
        sceneManager.addScene(createHelpMenu(), SceneType::HELP);
        sceneManager.addScene(createGameScene(), SceneType::GAME);
        sceneManager.addScene(createEndMenu(), SceneType::END);
        if (Core::networkRole == NetworkRole::CLIENT) {
            createMusic(sceneManager.getScene(SceneType::GAME), "assets/Music/Game 2.ogg");
            sceneManager.setCurrentScene(SceneType::SPLASH);
        }
        else if (Core::networkRole == NetworkRole::SERVER)
            sceneManager.setCurrentScene(SceneType::LOBBY);
        _collideSystem.init(sceneManager);
        _aiSystem.init(sceneManager);
        setAddNRmEntityCallbacks();
    }

    void GameSystem::setAddNRmEntityCallbacks()
    {
        _onEntityAddedCallbacks[IEntity::Tags::MISSILE] = std::bind(&GameSystem::createSound, std::placeholders::_1, "assets/Sounds/jump.wav", QUuid::createUuid());
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
            if (players->changeLeft == 2 || players->changeLeft == 0) {
                auto components = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText + 1];
                auto text = components->getFilteredComponents({IComponent::Type::TEXT});
                auto value = Component::castComponent<String>(text[0]);
                value->getValue() = players->getLeft();
                players->changeLeft = 0;
            }
            if (players->changeRight == 2 || players->changeRight == 0) {
                auto components = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText + 2];
                auto text = (*components)[IComponent::Type::TEXT];
                auto value = Component::castComponent<String>(text);
                value->getValue() = players->getRight();
                players->changeRight = 0;
            }
            if (players->changeDown == 2 || players->changeDown == 0) {
                auto components = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText + 3];
                auto text = (*components)[IComponent::Type::TEXT];
                auto value = Component::castComponent<String>(text);
                value->getValue() = players->getDown();
                players->changeDown = 0;
            }
            if (players->changeBomb == 2 || players->changeBomb == 0) {
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
        if (players->changeUp == 1) {
            auto entity = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText];
            auto text = (*entity)[IComponent::Type::TEXT];
            auto value = Component::castComponent<String>(text);
            value->getValue() = "|";
        } else if (players->changeLeft == 1) {
            auto components = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText + 1];
            auto text = (*components)[IComponent::Type::TEXT];
            auto value = Component::castComponent<String>(text);
            value->getValue() = "|";
        } else if (players->changeRight == 1) {
            auto components = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText + 2];
            auto text = (*components)[IComponent::Type::TEXT];
            auto value = Component::castComponent<String>(text);
            value->getValue() = "|";
        } else if (players->changeDown == 1) {
            auto components = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText + 3];
            auto text = (*components)[IComponent::Type::TEXT];
            auto value = Component::castComponent<String>(text);
            value->getValue() = "|";
        } else if (players->changeBomb == 1) {
            auto components = sceneManager.getCurrentScene()[IEntity::Tags::TEXT][firstText + 4];
            auto text = (*components)[IComponent::Type::TEXT];
            auto value = Component::castComponent<String>(text);
            value->getValue() = "|";
        }
    }

    void GameSystem::purgeAroundCameraEntities(ecs::SceneManager &sceneManager, uint64_t dt, std::shared_ptr<ecs::Position> camPos)
    {
        const int validBoundingZone = VALID_BORDER_SIZE;
        static uint64_t lastPurge = 0;
        if ((lastPurge += dt) < PURGE_FREQUENCY)
            return;
        else
            lastPurge = 0;
        auto rect = Rect(camPos->x - validBoundingZone,
            camPos->y - validBoundingZone,
            camPos->x + 1920 + validBoundingZone,
            camPos->y + 1080 + validBoundingZone);

        for (auto &entity : sceneManager.getCurrentScene().getAllEntities()) {
            auto component = (*entity)[IComponent::Type::POSITION];
            if (component == nullptr)
                continue;
            auto pos = Component::castComponent<Position>(component);
            if (pos && !(rect.contains(pos->x, pos->y)))
                sceneManager.getCurrentScene().removeEntity(entity);
        }
    }

    void GameSystem::update(ecs::SceneManager &sceneManager, uint64_t dt)
    {
        if (sceneManager.getCurrentSceneType() == SceneType::SPLASH) {
            timeElasped += dt;
            if (timeElasped > SPLASH_TIMEOUT) {
                if (Core::networkRole == NetworkRole::CLIENT)
                    sceneManager.setCurrentScene(SceneType::CONNECTION);
                if (Core::networkRole == NetworkRole::SERVER)
                    sceneManager.setCurrentScene(SceneType::GAME);
                timeElasped = 0;
            }
        } else if (sceneManager.getCurrentSceneType() == SceneType::CONNECTION) {
            timeElasped += dt;
            if (timeElasped > CONNECTION_TIMEOUT && Core::networkRole == NetworkRole::CLIENT) {
                std::cerr << "Connection failed" << std::endl;
                sceneManager.setShouldClose(true);
            }
        }
        if (sceneManager.getCurrentSceneType() != SceneType::GAME)
            return;
        if (Core::networkRole == NetworkRole::SERVER) {
            updatePlayers(sceneManager, dt);
            updateModules(sceneManager, dt);
            updateProjectiles(sceneManager, dt);
            updateEnemies(sceneManager, dt);
        } else if (Core::networkRole == NetworkRole::CLIENT) {
            for (auto &animation : sceneManager.getCurrentScene()[IEntity::Tags::ANIMATED_2D]) {
                auto animationComp = Component::castComponent<Animation2D>((*animation)[IComponent::Type::ANIMATION_2D]);
                if (animationComp->getAnimationType() != Animation2D::AnimationType::FIXED) {
                    animationComp->increment();
                }
            }
            _collideSystem.update(sceneManager, dt);
        }
        for (auto &camera : sceneManager.getCurrentScene()[IEntity::Tags::CAMERA_2D]) {
            auto cameraComp = Component::castComponent<Camera2DComponent>((*camera)[IComponent::Type::CAMERA_2D]);
            auto pos = Component::castComponent<Position>((*camera)[IComponent::Type::POSITION]);
            auto vel = Component::castComponent<Velocity>((*camera)[IComponent::Type::VELOCITY]);
            *pos = (*pos) + (*vel) * (float)(dt / 1000.0f);
            cameraComp->getCamera().update();
            if (Core::networkRole == NetworkRole::SERVER)
                purgeAroundCameraEntities(sceneManager, dt, pos);
        }
    }

    std::unique_ptr<IScene> GameSystem::createConnectionScene()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createConnectionScene, this), SceneType::CONNECTION);
        std::shared_ptr<Entity> entity = createText("Waiting for connection...", Position(200, 150), 30, "assets/Font/techno_hideo.ttf");

        scene->addEntity(entity);
        return scene;
    }

    void GameSystem::destroy()
    {
        std::cerr << "GameSystem::destroy" << std::endl;
        _collideSystem.destroy();
    }

    std::shared_ptr<Entity> GameSystem::createImage(std::string path, Position position, int heigh, int width, float rotation = 0.0f, float scale = 1.0f)
    {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>();
        std::shared_ptr<Sprite> component = std::make_shared<Sprite>(path, rotation, scale);
        std::shared_ptr<Position> component2 = std::make_shared<Position>(position);
        std::shared_ptr<Rect> component3 = std::make_shared<Rect>(0, 0, heigh, width);

        entity->addComponent(component2)
            .addComponent(component)
            .addComponent(component3);
        return (entity);
    }

    std::shared_ptr<Entity> GameSystem::createText(std::string text, Position position, float fontSize, std::string path)
    {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>();
        std::shared_ptr<String> component = std::make_shared<String>(text, path, fontSize);
        std::shared_ptr<Position> component2 = std::make_shared<Position>(position);

        entity->addComponent(component2)
            .addComponent(component);
        return (entity);
    }

    void GameSystem::createSoundEvent(std::shared_ptr<Entity> &entity, std::string value)
    {
        MouseCallbacks mouseCallbacks([value, entity](SceneManager &sceneManger, Vector2 mousePosition) {
                auto comp = entity->getFilteredComponents({IComponent::Type::SPRITE, IComponent::Type::POSITION, IComponent::Type::RECT});
                auto pos = Component::castComponent<Position>(comp[1]);
                auto sprite = Component::castComponent<Sprite>(comp[0]);
                auto rect = Component::castComponent<Rect>(comp[2]);

                if (mousePosition.x > pos->x && mousePosition.x < pos->x + rect->width &&
                    mousePosition.y > pos->y && mousePosition.y < pos->y + rect->height) {
                    auto comp2 = sceneManger.getCurrentScene()[IEntity::Tags::TEXT][2];
                    auto text = (*comp2)[IComponent::Type::TEXT];
                    auto value2 = Component::castComponent<String>(text);
                    if ((value == "+" || value == "-") && AudioDevice::isMute) {
                        AudioDevice::isMute = false;
                        AudioDevice::setVolume(AudioDevice::oldVolume);
                    }
                    if (AudioDevice::masterVolume <= 1 && value == "+") {
                        AudioDevice::setVolume(AudioDevice::masterVolume + 0.1);
                        value2->getValue() = std::to_string(int(AudioDevice::masterVolume * 100));
                    } else if (AudioDevice::masterVolume >= 0.1 && value == "-") {
                        AudioDevice::setVolume(AudioDevice::masterVolume - 0.1);
                        value2->getValue() = std::to_string(int(AudioDevice::masterVolume * 100));
                    }
                    if (value == "unmute" && AudioDevice::isMute == true) {
                        std::cerr << "unmute: " << AudioDevice::oldVolume << std::endl;
                        AudioDevice::isMute = false;
                        AudioDevice::setVolume(AudioDevice::oldVolume > 1 ? 1 : AudioDevice::oldVolume);
                    } else if (value == "mute" && AudioDevice::isMute == false) {
                        AudioDevice::isMute = true;
                        AudioDevice::setVolume(0);
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
            [scenetype, entity, this](SceneManager &sceneManager, Vector2 mousePosition) {
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
            [entity, this, msg](SceneManager &sceneManager, Vector2 mousePosition) {
                auto comp = entity->getFilteredComponents({IComponent::Type::SPRITE, IComponent::Type::POSITION, IComponent::Type::RECT});
                auto pos = Component::castComponent<Position>(comp[1]);
                auto sprite = Component::castComponent<Sprite>(comp[0]);
                auto rect = Component::castComponent<Rect>(comp[2]);

                if (mousePosition.x > pos->x && mousePosition.x < pos->x + rect->width &&
                    mousePosition.y > pos->y && mousePosition.y < pos->y + rect->height) {
                    emit writeMsg(Message(msg));
                }
            },
            [](SceneManager &, Vector2 /*mousePosition*/) {},
            [](SceneManager &, Vector2 /*mousePosition*/) {},
            [](SceneManager &, Vector2 /*mousePosition*/) {});

        eventListener->addMouseEvent(MOUSE_BUTTON_LEFT, mouseCallbacks);
        entity->addComponent(eventListener);
    }

    void GameSystem::createMsgEvent(std::shared_ptr<Entity> &entity, const NetworkMessageType &msg)
    {
        std::shared_ptr<EventListener> eventListener = std::make_shared<EventListener>();
        MouseCallbacks mouseCallbacks(
            [entity, this, msg](SceneManager &sceneManager, Vector2 mousePosition) {
                auto comp = entity->getFilteredComponents({IComponent::Type::SPRITE, IComponent::Type::POSITION, IComponent::Type::RECT});
                auto pos = Component::castComponent<Position>(comp[1]);
                auto sprite = Component::castComponent<Sprite>(comp[0]);
                auto rect = Component::castComponent<Rect>(comp[2]);

                if (mousePosition.x > pos->x && mousePosition.x < pos->x + rect->width &&
                    mousePosition.y > pos->y && mousePosition.y < pos->y + rect->height) {
                    emit writeMsg(Message(msg));
                }
            },
            [](SceneManager &, Vector2 /*mousePosition*/) {},
            [](SceneManager &, Vector2 /*mousePosition*/) {},
            [](SceneManager &, Vector2 /*mousePosition*/) {});

        eventListener->addMouseEvent(MOUSE_BUTTON_LEFT, mouseCallbacks);
        entity->addComponent(eventListener);
    }

    void GameSystem::createBindingsEvent(std::shared_ptr<Entity> &entity, QUuid id_player, int button)
    {
        MouseCallbacks mouseCallbacks(
            [entity, button, id_player, this](SceneManager &sceneManager, Vector2 mousePosition) {
                auto comp = (*entity)[IComponent::Type::POSITION];
                auto pos = Component::castComponent<Position>(comp);

                if (mousePosition.x > pos->x && mousePosition.x < pos->x + 50 &&
                    mousePosition.y > pos->y && mousePosition.y < pos->y + 20) {
                    changeBindings(sceneManager, id_player, button);
                }
            },
            [](SceneManager &, Vector2 /*mousePosition*/) {},
            [](SceneManager &, Vector2 /*mousePosition*/) {},
            [entity, button, id_player](SceneManager &sceneManager, Vector2 /*mousePosition*/) {
                std::shared_ptr<IEntity> component = nullptr;
                for (auto &entity : sceneManager.getScene(SceneType::GAME)[IEntity::Tags::PLAYER]) {
                    if (entity->getId() == id_player) {
                        component = entity;
                        break;
                    }
                }
                if (component == nullptr)
                    return;
                auto comp = component->getFilteredComponents({IComponent::Type::PLAYER, IComponent::Type::EVT_LISTENER});
                auto player = Component::castComponent<Player>(comp[0]);
                auto event = Component::castComponent<EventListener>(comp[1]);
                std::string get = "";
                char input = 0;

                if (player->changeUp == 1) {
                    input = Window::getKeyPressed();
                    if (input != 0) {
                        get.assign(1, input);
                        event->replaceKeyboardEvent((KeyboardKey)player->getTagUp(), (KeyboardKey)GameSystem::getTag(get));
                        player->setUP(get);
                        player->changeUp = 2;
                    }
                } else if (player->changeLeft == 1) {
                    input = Window::getKeyPressed();
                    if (input != 0) {
                        get.assign(1, input);
                        event->replaceKeyboardEvent((KeyboardKey)player->getTagLeft(), (KeyboardKey)GameSystem::getTag(get));
                        player->setLEFT(get);
                        player->changeLeft = 2;
                    }
                } else if (player->changeRight == 1) {
                    input = Window::getKeyPressed();
                    if (input != 0) {
                        get.assign(1, input);
                        event->replaceKeyboardEvent((KeyboardKey)player->getTagRight(), (KeyboardKey)GameSystem::getTag(get));
                        player->setRIGHT(get);
                        player->changeRight = 2;
                    }
                } else if (player->changeDown == 1) {
                    input = Window::getKeyPressed();
                    if (input != 0) {
                        get.assign(1, input);
                        event->replaceKeyboardEvent((KeyboardKey)player->getTagDown(), (KeyboardKey)GameSystem::getTag(get));
                        player->setDOWN(get);
                        player->changeDown = 2;
                    }
                } else if (player->changeBomb == true) {
                    input = Window::getKeyPressed();
                    if (input != 0) {
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
            [entity, _nbr_player, this](SceneManager &sceneManager, Vector2 mousePosition) {
                auto comp = (*entity)[IComponent::Type::POSITION];
                auto pos = Component::castComponent<Position>(comp);

                if (mousePosition.x > pos->x && mousePosition.x < pos->x + 50 &&
                    mousePosition.y > pos->y && mousePosition.y < pos->y + 50) {
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
        std::shared_ptr<Camera3DComponent> camera = std::make_shared<Camera3DComponent>(camTarget, camPos);

        cam->addComponent(camera);
        return cam;
    }

    void GameSystem::updateProjectiles(SceneManager &sceneManager, uint64_t dt)
    {
        for (auto &entity : sceneManager.getCurrentScene()[IEntity::Tags::TRAJECTORY]) {
            auto trajectory = Component::castComponent<Trajectory>((*entity)[IComponent::Type::TRAJECTORY]);
            auto position = Component::castComponent<Position>((*entity)[IComponent::Type::POSITION]);
            auto hitbox = Component::castComponent<Hitbox>((*entity)[IComponent::Type::HITBOX]);

            trajectory->update(position);
            if (hitbox) {
                Rectangle newRect = {position->x, position->y, hitbox->getRect().width, hitbox->getRect().height};
                hitbox->setRect(newRect);
            }
        }
    }

    void GameSystem::updatePlayers(SceneManager &sceneManager, uint64_t dt)
    {
        std::vector<std::shared_ptr<IEntity>> playersToDestroy;

        for (auto &player : sceneManager.getCurrentScene()[IEntity::Tags::PLAYER]) {
            auto pos = Component::castComponent<Position>((*player)[IComponent::Type::POSITION]);
            auto lastPos = *pos;
            auto vel = Component::castComponent<Velocity>((*player)[IComponent::Type::VELOCITY]);
            auto playerComp = Component::castComponent<Player>((*player)[IComponent::Type::PLAYER]);
            auto hitbox = Component::castComponent<Hitbox>((*player)[IComponent::Type::HITBOX]);
            auto splitVel = *vel;

            splitVel.y = 0;
            (*pos) = (*pos) + (splitVel * (float)(dt / 1000.0f));
            (*hitbox) += splitVel * (float)(dt / 1000.0f);
            for (auto &collider : _collideSystem.getColliders(player)) {
                if (collider->hasTag(IEntity::Tags::WALL) || collider->hasTag(IEntity::Tags::ENEMY)) {
                    if (playerComp->getSpaceModule() != nullptr) {
                        writeMsg(Message(EntityAction::DELETE, playerComp->getSpaceModule()->getId()));
                        sceneManager.getCurrentScene().removeEntity(playerComp->getSpaceModule());
                    }
                    sceneManager.getCurrentScene().removeEntity(player);
                    Message msg(EntityAction::DELETE, player->getId());
                    writeMsg(msg);
                } else if (collider->hasTag(IEntity::Tags::MISSILE)) {
                    auto missile = Component::castComponent<Missile>((*collider)[IComponent::Type::MISSILE]);
                    auto sprite = Component::castComponent<Sprite>((*collider)[IComponent::Type::SPRITE]);
                    if (missile->getMissileType() == Missile::MissileType::E_SINUSOIDAL || missile->getMissileType() == Missile::MissileType::E_CLASSIC || missile->getMissileType() == Missile::MissileType::E_HOMING_MISSILE ) {
                        if (playerComp->getSpaceModule() != nullptr) {
                            writeMsg(Message(EntityAction::DELETE, playerComp->getSpaceModule()->getId()));
                            sceneManager.getCurrentScene().removeEntity(playerComp->getSpaceModule());
                        }
                        sceneManager.getCurrentScene().removeEntity(collider);
                        playersToDestroy.push_back(player);
                        Message playerMsg(EntityAction::DELETE, player->getId());
                        Message missileMsg(EntityAction::DELETE, collider->getId());
                        writeMsg(missileMsg);
                        writeMsg(playerMsg);
                    }
                } else if (collider->hasTag(IEntity::Tags::BONUS)) {
                    auto bonus = Component::castComponent<Bonus>((*collider)[IComponent::Type::BONUS]);
                    if (bonus->getBonusType() == Bonus::Type::MODULE && playerComp->getSpaceModule() == nullptr) {
                        QUuid modId = QUuid::createUuid();
                        auto modPos = Component::castComponent<Position>((*collider)[IComponent::Type::POSITION]);
                        playerComp->setSpaceModule(GameSystem::createSpaceModule(sceneManager, modId, *modPos, playerComp->getId(), player));
                        writeMsg(Message(EntityAction::CREATE, modId, EntityType::MODULE, modPos->getVector2(), quint8(playerComp->getId())));
                    }
                    writeMsg(Message(EntityAction::DELETE, collider->getId()));
                    sceneManager.getCurrentScene().removeEntity(collider);
                }
            }

            splitVel.y = (*vel).y;
            splitVel.x = 0;
            (*pos) = (*pos) + (splitVel * (float)(dt / 1000.0f));
            (*hitbox) += splitVel * (float)(dt / 1000.0f);
        }
        for (auto &player : playersToDestroy)
            sceneManager.getCurrentScene().removeEntity(player);
        if ((sceneManager.getCurrentScene()[IEntity::Tags::PLAYER]).size() == 0) {
            sceneManager.setCurrentScene(SceneType::END);
            std::cerr << "No more players left" << std::endl;
        }
    }

    void GameSystem::updateModules(SceneManager &sceneManager, uint64_t dt)
    {
        auto modules = sceneManager.getCurrentScene()[IEntity::Tags::SPACE_MODULE];
        std::vector<std::shared_ptr<IEntity>> modulesToDestroy;

        for (auto &module : modules) {
            auto pos = Component::castComponent<Position>((*module)[IComponent::Type::POSITION]);
            auto vel = Component::castComponent<Velocity>((*module)[IComponent::Type::VELOCITY]);
            auto hitbox = Component::castComponent<Hitbox>((*module)[IComponent::Type::HITBOX]);
            auto modComp = Component::castComponent<SpaceModule>((*module)[IComponent::Type::SPACE_MODULE]);

            if (modComp->getBoundMode() == SpaceModule::BoundMode::NONE) {
                auto splitVel = *vel;
                splitVel.y = 0;
                (*pos) = (*pos) + (splitVel * (float)(dt / 1000.0f));
                (*hitbox) += splitVel * (float)(dt / 1000.0f);
                splitVel.y = (*vel).y;
                splitVel.x = 0;
                (*pos) = (*pos) + (splitVel * (float)(dt / 1000.0f));
                (*hitbox) += splitVel * (float)(dt / 1000.0f);
            } else {
                auto playerPos = Component::castComponent<Position>((*modComp->getPlayer())[IComponent::Type::POSITION]);
                *pos = *playerPos;
                if (modComp->getBoundMode() == SpaceModule::BoundMode::FRONT)
                    pos->x += SCALE;
                else
                    pos->x -= SCALE / 2;
                Rectangle rect = hitbox->getRect();
                rect.x = pos->x;
                rect.y = pos->y;
                hitbox->setRect(rect);
            }

            for (auto &collider : _collideSystem.getColliders(module)) {
                if (collider->hasTag(IEntity::Tags::WALL) || collider->hasTag(IEntity::Tags::ENEMY)) {
                    auto player = Component::castComponent<Player>((*modComp->getPlayer())[IComponent::Type::PLAYER]);
                    player->setSpaceModule(nullptr);
                    modulesToDestroy.push_back(module);
                } else if (collider->hasTag(IEntity::Tags::MISSILE)) {
                    auto missile = Component::castComponent<Missile>((*collider)[IComponent::Type::MISSILE]);
                    auto sprite = Component::castComponent<Sprite>((*collider)[IComponent::Type::SPRITE]);
                    if (missile->getMissileType() == Missile::MissileType::E_SINUSOIDAL || missile->getMissileType() == Missile::MissileType::E_CLASSIC || missile->getMissileType() == Missile::MissileType::E_HOMING_MISSILE ) {
                        auto player = Component::castComponent<Player>((*modComp->getPlayer())[IComponent::Type::PLAYER]);
                        player->setSpaceModule(nullptr);
                        sceneManager.getCurrentScene().removeEntity(collider);
                        modulesToDestroy.push_back(module);
                        writeMsg(Message(EntityAction::DELETE, collider->getId()));
                    }
                }
            }
        }
        for (auto &module : modulesToDestroy) {
            sceneManager.getCurrentScene().removeEntity(module);
            writeMsg(Message(EntityAction::DELETE, module->getId()));
        }
    }

    void GameSystem::updateEnemies(SceneManager &sceneManager, uint64_t dt)
    {
        auto enemies = sceneManager.getCurrentScene()[IEntity::Tags::ENEMY];
        std::vector<std::shared_ptr<IEntity>> enemiesToDestroy;

        for (auto &enemy : enemies) {
            auto enComp = Component::castComponent<Enemy>((*enemy)[IComponent::Type::ENEMY]);
            auto enPos = Component::castComponent<Position>((*enemy)[IComponent::Type::POSITION]);
            auto hitbox = Component::castComponent<Hitbox>((*enemy)[IComponent::Type::HITBOX]);

            Rectangle newRect = {enPos->x, enPos->y, hitbox->getRect().width, hitbox->getRect().height};
            hitbox->setRect(newRect);
            Position pos(enPos->x - SCALE, enPos->y + (SCALE / 2));
            for (auto &collider : _collideSystem.getColliders(enemy)) {
                if (collider->hasTag(IEntity::Tags::WALL)) {
                    enemiesToDestroy.push_back(enemy);
                    Message msg(EntityAction::DELETE, enemy->getId());
                    writeMsg(msg);
                } else if (collider->hasTag(IEntity::Tags::MISSILE)) {
                    auto missile = Component::castComponent<Missile>((*collider)[IComponent::Type::MISSILE]);
                    if (missile->getMissileType() == Missile::MissileType::P_SIMPLE ||
                        missile->getMissileType() == Missile::MissileType::P_CONDENSED) {
                        auto bonus = enComp->lootBonus(*enPos);
                        if (bonus != nullptr) {
                            sceneManager.getCurrentScene().addEntity(bonus);
                            writeMsg(Message(EntityAction::CREATE, bonus->getId(), EntityType::BONUS, enPos->getVector2(), 0));
                        }
                        enemiesToDestroy.push_back(enemy);
                        sceneManager.getCurrentScene().removeEntity(collider);
                        Message enemyMsg(EntityAction::DELETE, enemy->getId());
                        Message missileMsg(EntityAction::DELETE, collider->getId());
                        writeMsg(enemyMsg);
                        writeMsg(missileMsg);
                    }
                }
            }
            if (enComp->isShootTime() && !enComp->isShooting()) {
                // Shoot
                QUuid id = QUuid::createUuid();
                GameSystem::createMissile(sceneManager, id, pos, enComp->getMissileType(), IEntity::Tags::PLAYER);
                Message msg(EntityAction::CREATE, id, EntityType::MISSILE, pos.getVector2(), quint8(enComp->getMissileType()));
                emit writeMsg(msg);
                if (enComp->getNbMissile() > 1) {
                    enComp->setShooting(true);
                    enComp->startSalvoTimer();
                    enComp->getSalvo()++;
                } else
                    enComp->startShootTimer();
            } else if (enComp->salvoTime() && enComp->isShooting()) {
                // Shoot a salvo
                QUuid id = QUuid::createUuid();
                GameSystem::createMissile(sceneManager, id, pos, enComp->getMissileType(), IEntity::Tags::PLAYER);
                Message msg(EntityAction::CREATE, id, EntityType::MISSILE, pos.getVector2(), quint8(enComp->getMissileType()));
                emit writeMsg(msg);
                enComp->getSalvo()++;
                if (enComp->getSalvo() == enComp->getNbMissile()) {
                    enComp->getSalvo() = 0;
                    enComp->startShootTimer();
                    enComp->setShooting(false);
                } else
                    enComp->startSalvoTimer();
            }
        }
        for (auto &enemy : enemiesToDestroy) {
            sceneManager.getCurrentScene().removeEntity(enemy);
        }
    }

    std::unique_ptr<IScene> GameSystem::createSplashScreenScene()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createSplashScreenScene, this), SceneType::SPLASH);
        std::shared_ptr<Entity> entity = std::make_shared<Entity>();
        std::shared_ptr<Position> pos = std::make_shared<Position>(550, 350);
        std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/enemy/sprites/enemy1.png");
        std::shared_ptr<Entity> entity2 = createText("R-Type", Position(800, 50), 50, "assets/Font/techno_hideo.ttf");
        std::shared_ptr<Entity> entity3 = createText("Clearly made by us", Position(700, 100), 25, "assets/Font/techno_hideo.ttf");
        std::shared_ptr<Entity> entity4 = createText("Iona Dommel-Prioux\nAntoine Penot\nCamille Maux\nIzaac Carcenac-Sautron\nCyril Dehaese\nRoxane Baert", Position(10, 450), 15, "assets/Font/techno_hideo.ttf");

        scene->addEntities({entity, entity2, entity3, entity4});
        return scene;
    }

    std::unique_ptr<IScene> GameSystem::createLobbyScene()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createLobbyScene, this), SceneType::LOBBY);
        std::shared_ptr<Entity> backgroundEntity = std::make_shared<Entity>();
        std::shared_ptr<Sprite> bg = std::make_shared<Sprite>("assets/Background/Background.png");
        std::shared_ptr<Position> bgPos = std::make_shared<Position>(960, 540);
        std::shared_ptr<Entity> playButtonEntity = createImage("assets/MainMenu/Play/Button Normal.png", Position(843, 400), 274, 91, 0.0f, 2.4f);
        std::shared_ptr<Entity> optionButtonEntity = createImage("assets/MainMenu/Icon/option.png", Position(45, 45), 75, 75, 0.0f, 2.4f);
        std::shared_ptr<Entity> manetteButtonEntity = createImage("assets/MainMenu/Icon/info.png", Position(15, 950), 20, 75, 0.0f, 2.4f);
        std::shared_ptr<Entity> quitButtonEntity = createImage("assets/MainMenu/Quit/Button Normal.png", Position(843, 550), 274, 91, 0.0f, 2.4f);

        backgroundEntity->addComponent(bg)
            .addComponent(bgPos);
        createMusic(*scene, "assets/Music/Menu.ogg");
        createMsgEvent(playButtonEntity, NetworkMessageType::READY);
        createSceneEvent(optionButtonEntity, SceneType::SOUND);
        createSceneEvent(manetteButtonEntity, SceneType::HELP);
        createMsgEvent(quitButtonEntity, NetworkMessageType::DISCONNECTED);
        createSceneEvent(quitButtonEntity, SceneType::NONE);
        scene->addEntities({backgroundEntity, playButtonEntity, optionButtonEntity, manetteButtonEntity, quitButtonEntity});
        return scene;
    }

    std::unique_ptr<IScene> GameSystem::createSettingMenu()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createSettingMenu, this), SceneType::OPTION);
        std::shared_ptr<Entity> entity1 = createImage("assets/Background/Option_Background.png", Position(960, 540), 0, 0);
        std::shared_ptr<Entity> entity2 = createImage("assets/MainMenu/Icon/back.png", Position(35, 30), 60, 50, 0.0f, 2.4f);
        std::shared_ptr<Entity> entity3 = createImage("assets/MainMenu/Icon/minus.png", Position(750, 490), 60, 24, 0.0f, 2.4f);
        std::shared_ptr<Entity> entity4 = createImage("assets/MainMenu/Icon/plus.png", Position(1030, 480), 60, 60, 0.0f, 2.4f);
        std::shared_ptr<Entity> entity5 = createImage("assets/MainMenu/Icon/sound off.png", Position(760, 580), 84, 60, 0.0f, 2.4f);
        std::shared_ptr<Entity> entity6 = createImage("assets/MainMenu/Icon/sound on.png", Position(1035, 580), 84, 60, 0.0f, 2.4f);
        std::shared_ptr<Entity> entity7 = createText("Option Menu", Position(600, 100), 50, "assets/Font/techno_hideo.ttf");
        std::shared_ptr<Entity> entity8 = createText("Volume", Position(700, 200), 50, "assets/Font/techno_hideo.ttf");
        std::shared_ptr<Entity> entity9 = createText("50", Position(820, 450), 80, "assets/Font/techno_hideo.ttf");

        createMusic(*scene, "assets/Music/Menu.ogg");
        createSceneEvent(entity2, SceneType::PREVIOUS);
        createSoundEvent(entity3, "-");
        createSoundEvent(entity4, "+");
        createSoundEvent(entity5, "mute");
        createSoundEvent(entity6, "unmute");
        scene->addEntities({entity1, entity2, entity3, entity4, entity5, entity6, entity7, entity8, entity9});
        return scene;
    }

    std::unique_ptr<IScene> GameSystem::createHelpMenu()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createHelpMenu, this), SceneType::HELP);
        std::shared_ptr<Entity> entity1 = createImage("assets/Background/Option_Background.png", Position(960, 540), 0, 0);
        std::shared_ptr<Entity> entity2 = createImage("assets/MainMenu/Icon/back.png", Position(35, 30), 60, 50, 0.0f, 2.4f);
        std::shared_ptr<Entity> entity3 = createText("Welcome in our game: RType.", Position(100, 100), 50, "assets/Font/techno_hideo.ttf");
        std::shared_ptr<Entity> entity4 = createText("Commande", Position(150, 250), 40, "assets/Font/techno_hideo.ttf");
        std::shared_ptr<Entity> entity5 = createText("Left: Left", Position(150, 350), 40, "assets/Font/techno_hideo.ttf");
        std::shared_ptr<Entity> entity6 = createText("Right: Right", Position(150, 450), 40, "assets/Font/techno_hideo.ttf");
        std::shared_ptr<Entity> entity7 = createText("Up: Up", Position(150, 550), 40, "assets/Font/techno_hideo.ttf");
        std::shared_ptr<Entity> entity8 = createText("Down: Down", Position(150, 650), 40, "assets/Font/techno_hideo.ttf");
        std::shared_ptr<Entity> entity9 = createText("Shoot: Right CTRL", Position(150, 750), 40, "assets/Font/techno_hideo.ttf");

        createMusic(*scene, "assets/Music/Menu.ogg");
        createSceneEvent(entity2, SceneType::PREVIOUS);

        scene->addEntities({entity1, entity2, entity3, entity4, entity5, entity6, entity7, entity8, entity9});
        return scene;
    }

    std::unique_ptr<IScene> GameSystem::createEndMenu()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createEndMenu, this), SceneType::END);
        std::shared_ptr<Entity> entity1 = createImage("assets/Background/Background.png", Position(0, 0), 800, 600);
        std::shared_ptr<Entity> entity2 = createText("End", Position(350, 25), 50, "assets/Font/techno_hideo.ttf");
        std::shared_ptr<Entity> quitButtonEntity = createImage("assets/MainMenu/Quit/Button Normal.png", Position(800 / 2 - 60, 800 / 2 - 18), 120, 28);

        createMusic(*scene, "assets/Music/Menu.ogg");
        createMsgEvent(quitButtonEntity, NetworkMessageType::DISCONNECTED);
        createSceneEvent(quitButtonEntity, SceneType::NONE);
        scene->addEntities({entity1, entity2, quitButtonEntity});
        return (scene);
    }

    std::unique_ptr<IScene> GameSystem::createGameScene()
    {
        auto scene = ReadMap();
        scene->addEntity(create2DCamera(0, 0));
        return scene;
    }

    std::shared_ptr<IEntity> GameSystem::create2DCamera(int x, int y)
    {
        std::shared_ptr<Entity> cam = std::make_shared<Entity>();
        std::shared_ptr<Position> pos = std::make_shared<Position>(x, y);
        std::shared_ptr<Velocity> vel = std::make_shared<Velocity>(Player::_defaultSpeed * 0.1f, 0);
        std::shared_ptr<Camera2DComponent> camera = std::make_shared<Camera2DComponent>(pos);

        cam->addComponent(camera)
            .addComponent(pos)
            .addComponent(vel);
        return cam;
    }

    void GameSystem::createMusic(IScene &scene, std::string path)
    {
        std::shared_ptr<Entity> musicEntity = std::make_shared<Entity>();
        std::shared_ptr<MusicComponent> musicComponent = std::make_shared<MusicComponent>(path);

        musicEntity->addComponent(musicComponent);
        scene.addEntities({musicEntity});
    }

    void GameSystem::createSound(IScene &scene, const std::string &file, QUuid id)
    {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>(id);
        std::shared_ptr<SoundComponent> sound = std::make_shared<SoundComponent>(file);

        entity->addComponent(sound);
        scene.addEntity(entity);
    }

    void GameSystem::activateNetwork()
    {
        _networkActivated = true;
    }

    bool GameSystem::isNetworkActivated()
    {
        return _networkActivated;
    }

    void GameSystem::createPlayer(IScene &scene, int keyRight, int keyLeft, int keyUp, int keyDown, int keyMissile, int keyModule, QUuid id, Position pos, bool isMe)
    {
        static int idCounter = 0;
        std::shared_ptr<Entity> playerEntity = std::make_shared<Entity>(id);
        std::shared_ptr<Position> playerPos = std::make_shared<Position>(pos);
        std::shared_ptr<Velocity> playerVel = std::make_shared<Velocity>(0, 0);
        Rectangle rect = {playerPos->x, playerPos->y, SCALE, SCALE};
        std::shared_ptr<Hitbox> playerHitbox = std::make_shared<Hitbox>(rect);
        std::shared_ptr<Player> player = std::make_shared<Player>(idCounter, keyUp, keyDown, keyLeft, keyRight, keyMissile, keyModule);
        std::shared_ptr<EventListener> playerListener = std::make_shared<EventListener>();
        std::string spriteFile = (idCounter >= GameSystem::_playersSprite.size() ? GameSystem::_playersSprite[0] : GameSystem::_playersSprite[idCounter]);
        std::shared_ptr<Sprite> playerSprite = std::make_shared<Sprite>(spriteFile, 0.0f, 2.0f);
        std::shared_ptr<Animation2D> anim = std::make_shared<Animation2D>(_spriteFrameCounts[spriteFile], 30, Animation2D::AnimationType::LOOP);
        std::shared_ptr<Destructible> destruct = std::make_shared<Destructible>();
        ButtonCallbacks missileCallbacks(
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::PRESSED, KeyboardKey::KEY_RIGHT_CONTROL));
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::RELEASED, KeyboardKey::KEY_RIGHT_CONTROL));
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::DOWN, KeyboardKey::KEY_RIGHT_CONTROL));
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::UP, KeyboardKey::KEY_RIGHT_CONTROL));
            });
        ButtonCallbacks moveRightCallbacks(
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::PRESSED, KeyboardKey::KEY_RIGHT));
                else
                    player->moveRight(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::RELEASED, KeyboardKey::KEY_RIGHT));
                else
                    player->stopRight(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::DOWN, KeyboardKey::KEY_RIGHT));
                else
                    player->moveRight(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::UP, KeyboardKey::KEY_RIGHT));
                else
                    player->stopRight(manager, playerEntity, 1);
            });
        ButtonCallbacks moveLeftCallbacks(
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::PRESSED, KeyboardKey::KEY_LEFT));
                else
                    player->moveLeft(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::RELEASED, KeyboardKey::KEY_LEFT));
                else
                    player->stopLeft(manager, playerEntity, 17);
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::DOWN, KeyboardKey::KEY_LEFT));
                else
                    player->moveLeft(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::UP, KeyboardKey::KEY_LEFT));
                else
                    player->stopLeft(manager, playerEntity, 17);
            });
        ButtonCallbacks moveUpCallbacks(
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::PRESSED, KeyboardKey::KEY_UP));
                else
                    player->moveUp(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::RELEASED, KeyboardKey::KEY_UP));
                else
                    player->stopUp(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::DOWN, KeyboardKey::KEY_UP));
                else
                    player->moveUp(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::UP, KeyboardKey::KEY_UP));
                else
                    player->stopUp(manager, playerEntity, 1);
            });
        ButtonCallbacks moveDownCallbacks(
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::PRESSED, KeyboardKey::KEY_DOWN));
                else
                    player->moveDown(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::RELEASED, KeyboardKey::KEY_DOWN));
                else
                    player->stopDown(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::DOWN, KeyboardKey::KEY_DOWN));
                else
                    player->moveDown(manager, playerEntity, 1);
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::UP, KeyboardKey::KEY_DOWN));
                else
                    player->stopDown(manager, playerEntity, 1);
            });
            ButtonCallbacks moduleCallbacks(
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::PRESSED, KeyboardKey::KEY_SPACE));
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::RELEASED, KeyboardKey::KEY_SPACE));
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::DOWN, KeyboardKey::KEY_SPACE));
            },
            [&, this, player, playerEntity](SceneManager &manager) {
                if (this->isNetworkActivated())
                    emit writeMsg(Message(EventType::KEYBOARD, KeyState::UP, KeyboardKey::KEY_SPACE));
            });

        std::function<void(SceneManager &, float)> moveHorizontalStickCallback = [&, this, player, playerEntity](SceneManager &manager, float value) {
            if (this->isNetworkActivated())
                emit writeMsg(Message("PLAYER STICK MOVED HORIZONTALLY"));
            else
                player->moveHorizontal(manager, playerEntity, value);
        };
        std::function<void(SceneManager &, float)> moveVerticalStickCallback = [&, this, player, playerEntity](SceneManager &manager, float value) {
            if (this->isNetworkActivated())
                emit writeMsg(Message("PLAYER STICK MOVED VERTICALLY"));
            else
                player->moveVertical(manager, playerEntity, value);
        };

        if (isMe) {
            playerListener->addKeyboardEvent((KeyboardKey)player->getTagUp(), moveUpCallbacks);
            playerListener->addKeyboardEvent((KeyboardKey)player->getTagLeft(), moveLeftCallbacks);
            playerListener->addKeyboardEvent((KeyboardKey)player->getTagRight(), moveRightCallbacks);
            playerListener->addKeyboardEvent((KeyboardKey)player->getTagDown(), moveDownCallbacks);
            playerListener->addKeyboardEvent((KeyboardKey)player->getTagBomb(), missileCallbacks);
            playerListener->addKeyboardEvent((KeyboardKey)player->getTagModule(), moduleCallbacks);
            playerListener->addGamepadEvent(idCounter, (GamepadButton)GAMEPAD_BUTTON_LEFT_FACE_UP, moveUpCallbacks);
            playerListener->addGamepadEvent(idCounter, (GamepadButton)GAMEPAD_BUTTON_LEFT_FACE_RIGHT, moveRightCallbacks);
            playerListener->addGamepadEvent(idCounter, (GamepadButton)GAMEPAD_BUTTON_LEFT_FACE_DOWN, moveDownCallbacks);
            playerListener->addGamepadEvent(idCounter, (GamepadButton)GAMEPAD_BUTTON_LEFT_FACE_LEFT, moveLeftCallbacks);
            playerListener->addGamepadStickEvent(idCounter, GAMEPAD_AXIS_LEFT_X, moveHorizontalStickCallback);
            playerListener->addGamepadStickEvent(idCounter, GAMEPAD_AXIS_LEFT_Y, moveVerticalStickCallback);
            playerEntity->addComponent(playerListener);
        }
        idCounter++;
        playerEntity->addComponents({player, playerPos, playerSprite, anim, playerVel, playerHitbox, destruct});
        scene.addEntity(playerEntity);
    }

    Message GameSystem::shootModuleMissile(SceneManager &manager, std::shared_ptr<IEntity> module, Missile::MissileType type)
    {
        if (module == nullptr)
            throw std::invalid_argument("Shoot Module Missile: module entity null");
        QUuid idMissile = QUuid::createUuid();
        auto pos = Component::castComponent<Position>((*module)[IComponent::Type::POSITION]);
        auto modComp = Component::castComponent<SpaceModule>((*module)[IComponent::Type::SPACE_MODULE]);
        Vector2 missilePos = {pos->x, pos->y + (SCALE / 2)};

        if (modComp->getBoundMode() == SpaceModule::BoundMode::FRONT)
            missilePos.x += SCALE;
        else if (modComp->getBoundMode() == SpaceModule::BoundMode::BACK)
            missilePos.x -= SCALE / 2;
        GameSystem::createMissile(manager, idMissile, Position(missilePos), type);
        Message msg(EntityAction::CREATE, idMissile, EntityType::MISSILE, missilePos, quint8(type));
        return msg;
    }

    void GameSystem::createMissile(SceneManager &sceneManager, QUuid id, Position position, Missile::MissileType type, IEntity::Tags targetType)
    {
        if (quint8(type) >= quint8(Missile::MissileType::NB_MISSILE) || quint8(type) == quint8(Missile::MissileType::HOMING_MISSILE))
            throw std::invalid_argument("Missile type invalid: " + quint8(type));
        std::shared_ptr<Entity> entity = std::make_shared<Entity>(id);
        std::shared_ptr<Missile> missile = std::make_shared<Missile>(type);
        Rectangle missileHitbox = {position.x, position.y, SCALE / 2, SCALE / 2};
        std::shared_ptr<Hitbox> hitbox = std::make_shared<Hitbox>(missileHitbox);
        std::shared_ptr<Position> pos = std::make_shared<Position>(position);
        int nbFrames = _spriteFrameCounts.find(_missilesSprites[type]) != _spriteFrameCounts.end() ? _spriteFrameCounts[_missilesSprites[type]] : 0;
        float rotation = _spriteRotations.find(_missilesSprites[type]) != _spriteRotations.end() ? _spriteRotations[_missilesSprites[type]] : 0.0F;
        Animation2D::AnimationType animType = _spriteAnimType.find(_missilesSprites[type]) != _spriteAnimType.end() ? _spriteAnimType[_missilesSprites[type]] : Animation2D::AnimationType::ONCE;
        std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(_missilesSprites[type], rotation, 1.0f);
        std::shared_ptr<Animation2D> anim = std::make_shared<Animation2D>(nbFrames, 24, animType);
        std::shared_ptr<Trajectory> trajectory = nullptr;

        if (Core::networkRole == NetworkRole::SERVER) {
            if (quint8(type) < quint8(Missile::MissileType::HOMING_MISSILE))
                trajectory = std::make_shared<Trajectory>(_missilesTrajectories[type].first, _missilesTrajectories[type].second, pos);
            else
                trajectory = generateMissileTrajectory(sceneManager, pos, targetType);
            if (trajectory != nullptr)
                entity->addComponent(trajectory);
        }
        entity->addComponent(missile)
            .addComponent(sprite)
            .addComponent(anim)
            .addComponent(hitbox)
            .addComponent(pos);
        sceneManager.getCurrentScene().addEntity(entity);
    }

    std::shared_ptr<Trajectory> GameSystem::generateMissileTrajectory(SceneManager& sceneManager, std::shared_ptr<Position> missilePos, IEntity::Tags targetType)
    {
        std::shared_ptr<Trajectory> trajectory = nullptr;
        std::shared_ptr<Position> target = nullptr;
        float distRef = -1.0f, coeffDirX = .0f, coeffDirY = .0f;

        if (targetType != IEntity::Tags::PLAYER && targetType != IEntity::Tags::ENEMY)
            throw std::invalid_argument("Generate missile trajectory: invalid target type");
        for (auto &entity : sceneManager.getCurrentScene()[targetType]) {
            auto pos = Component::castComponent<Position>((*entity)[IComponent::Type::POSITION]);
            float dist = AVector::getDistance2D(*pos, *missilePos);
            if (distRef < 0 || dist < distRef) {
                distRef = dist;
                target = pos;
            }
        }
        if (!target)
            return nullptr;
        coeffDirX = (target->x + SCALE / 2 - missilePos->x) / distRef;
        coeffDirY = (target->y + SCALE / 2 - missilePos->y) / distRef;
        trajectory = std::make_shared<Trajectory>(
            [ coeffDirX ](float t) { return t * 4 * coeffDirX; },
            [ coeffDirY ](float t) { return t * 4 * coeffDirY; },
            missilePos
        );
        return trajectory;
    }

    std::shared_ptr<IEntity> GameSystem::createSpaceModule(SceneManager &manager, QUuid id, Position position, uint8_t playerNb, std::shared_ptr<IEntity> player)
    {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>(id);
        std::shared_ptr<Position> pos = std::make_shared<Position>(position);
        std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(GameSystem::_modulesSprite[playerNb], .0f, 2.0f);
        std::shared_ptr<Animation2D> anim = std::make_shared<Animation2D>(4, 24);
        std::shared_ptr<SpaceModule> mod = std::make_shared<SpaceModule>(player);

        entity->addComponent(pos)
            .addComponent(anim)
            .addComponent(mod)
            .addComponent(sprite);
        if (Core::networkRole == NetworkRole::SERVER) {
            Rectangle rect = {pos->x, pos->y, SCALE / 2, SCALE / 2};
            std::shared_ptr<Hitbox> hitbox = std::make_shared<Hitbox>(rect);
            std::shared_ptr<Velocity> velocity = std::make_shared<Velocity>(Player::_defaultSpeed * 0.1f, 0);

            entity->addComponent(hitbox)
                .addComponent(velocity);
        }
        manager.getCurrentScene().addEntity(entity);
        return entity;
    }

    std::shared_ptr<IEntity> GameSystem::createBonus(QUuid id, Position pos)
    {
        std::shared_ptr<Entity> bonus = std::make_shared<Entity>(id);
        std::shared_ptr<Bonus> bonusComp = std::make_shared<Bonus>(Bonus::Type::MODULE);
        std::shared_ptr<Position> bonusPos = std::make_shared<Position>(pos);
        std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/Bonus/Bonus.png", 0.0f, 2.0f);
        std::shared_ptr<Animation2D> anim = std::make_shared<Animation2D>(6, 24);
        Rectangle rect = {pos.x, pos.y, SCALE, SCALE};
        std::shared_ptr<Hitbox> hitbox = std::make_shared<Hitbox>(rect);

        bonus->addComponent(bonusComp)
            .addComponent(anim)
            .addComponent(sprite)
            .addComponent(hitbox)
            .addComponent(bonusPos);
        return bonus;
    }

    void GameSystem::onEntityAdded(std::shared_ptr<IEntity> entity, IScene &scene)
    {
        for (auto tag : entity->getTags()) {
            if (_onEntityAddedCallbacks.find(tag) != _onEntityAddedCallbacks.end()) {
                _onEntityAddedCallbacks[tag](scene);
            }
        }
        _collideSystem.onEntityAdded(entity, scene);
    }

    void GameSystem::onEntityRemoved(std::shared_ptr<IEntity> entity, IScene &scene)
    {
        for (auto tag : entity->getTags()) {
            if (_onEntityRemovedCallbacks.find(tag) != _onEntityRemovedCallbacks.end()) {
                _onEntityRemovedCallbacks[tag](scene);
            }
        }
        _collideSystem.onEntityRemoved(entity, scene);
        if (entity->hasComponent(IComponent::Type::PLAYER))
            nbr_player -= 1;
        else if (entity->hasComponent(IComponent::Type::AI))
            nbr_ai -= 1;
    }

    void GameSystem::changeBindings(SceneManager &sceneManager, QUuid id_player, int button)
    {
        auto entities = sceneManager.getScene(SceneType::GAME)[IEntity::Tags::PLAYER];
        std::shared_ptr<IEntity> playerEntity = nullptr;
        for (auto &entity : entities) {
            if (entity->getId() == id_player) {
                playerEntity = entity;
                break;
            }
        }
        if (playerEntity == nullptr)
            return;
        auto component = (*playerEntity)[IComponent::Type::PLAYER];
        auto player = Component::castComponent<Player>(component);

        switch (button) {
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