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
#include "Trajectory.hpp"
#include "Animation2D.hpp"
#include "Enemy.hpp"

namespace ecs
{
    std::vector<Position> GameSystem::playerSpawns;

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
        {Missile::MissileType::EN, "assets/Enemies/RedEnemy4/RedEnemy4 - Missile.png"}};
    std::map<std::string, int> GameSystem::_spriteFrameCounts =
        {
            {"assets/Player/ChargedMissile.png", 5},
            {"assets/Player/BasicMissile.png", 5},
            {"assets/Player/MainShipSSP1.png", 4},
            {"assets/Enemies/RedEnemy2/RedEnemy2 - Missile.png", 4},
            {"assets/Enemies/RedEnemy4/RedEnemy4 - Missile.png", 4}};

    std::map<Missile::MissileType, std::pair<std::function<float(float)>, std::function<float(float)>>> GameSystem::_missilesTrajectories = {
        {Missile::MissileType::P_SIMPLE, {[](float dt) { return 4 * dt; }, [](float) { return 0; }}},
        {Missile::MissileType::P_CONDENSED, {[](float dt) { return 4 * dt; }, [](float) { return 0; }}},
        {Missile::MissileType::EN, {[](float dt) { return -4 * dt; }, [](float) { return 0; }}}};

    void GameSystem::init(ecs::SceneManager &sceneManager)
    {
        std::cerr << "GameSystem::init" << std::endl;
        sceneManager.addScene(createSplashScreenScene(), SceneType::SPLASH);
        // sceneManager.addScene(createMainMenuScene(), SceneType::MAIN_MENU);
        sceneManager.addScene(createConnectionScene(), SceneType::CONNECTION);
        sceneManager.addScene(createLobbyScene(), SceneType::LOBBY);
        sceneManager.addScene(createSettingMenu(), SceneType::SOUND);
        sceneManager.addScene(createHelpMenu(), SceneType::HELP);
        sceneManager.addScene(createGameScene(), SceneType::GAME);
        if (Core::networkRole == NetworkRole::CLIENT) {
            createMusic("assets/Music/music.ogg");
            sceneManager.setCurrentScene(SceneType::SPLASH);
        }
        else if (Core::networkRole == NetworkRole::SERVER)
            sceneManager.setCurrentScene(SceneType::LOBBY);
        sceneManager.addScene(createEndMenu(), SceneType::END);
        _collideSystem.init(sceneManager);
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
            for (auto &entity : sceneManager.getCurrentScene()[IEntity::Tags::TRAJECTORY]) {
                auto trajectory = Component::castComponent<Trajectory>((*entity)[IComponent::Type::TRAJECTORY]);
                auto position = Component::castComponent<Position>((*entity)[IComponent::Type::POSITION]);
                trajectory->update(position);
            }
            updateEnemies(sceneManager.getCurrentScene(), dt);
        }
        for (auto &camera : sceneManager.getCurrentScene()[IEntity::Tags::CAMERA_2D]) {
            auto cameraComp = Component::castComponent<Camera2DComponent>((*camera)[IComponent::Type::CAMERA_2D]);
            auto pos = Component::castComponent<Position>((*camera)[IComponent::Type::POSITION]);
            auto vel = Component::castComponent<Velocity>((*camera)[IComponent::Type::VELOCITY]);
            *pos = (*pos) + (*vel) * (float)(dt / 1000.0f);
            cameraComp->getCamera().update();
        }

        for (auto &animation : sceneManager.getCurrentScene()[IEntity::Tags::ANIMATED_2D]) {
            auto animationComp = Component::castComponent<Animation2D>((*animation)[IComponent::Type::ANIMATION_2D]);
            if (animationComp->getAnimationType() != Animation2D::AnimationType::FIXED) {
                animationComp->increment();
            }
        }
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

    void GameSystem::createBindingsEvent(std::shared_ptr<Entity> &entity, int id_player, int button)
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
                auto component = sceneManager.getScene(SceneType::GAME)[IEntity::Tags::PLAYER][id_player];
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

    void GameSystem::updatePlayers(SceneManager &sceneManager, uint64_t dt)
    {
        auto players = sceneManager.getCurrentScene()[IEntity::Tags::PLAYER];

        for (auto &player : players) {
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
                // TODO: The collision should probably lead to player's death
                std::cout << "Hitboxes collide !" << std::endl;
            }

            splitVel.y = (*vel).y;
            splitVel.x = 0;
            (*pos) = (*pos) + (splitVel * (float)(dt / 1000.0f));
            (*hitbox) += splitVel * (float)(dt / 1000.0f);
        }
    }

    void GameSystem::updateEnemies(IScene &scene, uint64_t dt)
    {
        auto enemies = scene[IEntity::Tags::ENEMY];

        for (auto &enemy : enemies) {
            auto enComp = Component::castComponent<Enemy>((*enemy)[IComponent::Type::ENEMY]);
            auto enPos = Component::castComponent<Position>((*enemy)[IComponent::Type::POSITION]);
            Position pos(enPos->x, enPos->y + (SCALE / 2));
            if (enComp->isShootTime() && !enComp->isShooting()) {
                // Shoot
                GameSystem::createMissile(scene, Entity::idCounter, pos, Missile::MissileType::EN);
                Message msg(EntityAction::CREATE, Entity::idCounter++, EntityType::MISSILE, pos.getVector2(), quint8(Missile::MissileType::EN));
                emit writeMsg(msg);
                if (enComp->getNbMissile() > 1) {
                    enComp->setShooting(true);
                    enComp->startSalvoTimer();
                    enComp->getSalvo()++;
                } else
                    enComp->startShootTimer();
            } else if (enComp->salvoTime() && enComp->isShooting()) {
                // Shoot a salvo
                GameSystem::createMissile(scene, Entity::idCounter, pos, Missile::MissileType::EN);
                Message msg(EntityAction::CREATE, Entity::idCounter++, EntityType::MISSILE, pos.getVector2(), quint8(Missile::MissileType::EN));
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
    }

    std::unique_ptr<IScene> GameSystem::createSplashScreenScene()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createSplashScreenScene, this), SceneType::SPLASH);
        std::shared_ptr<Entity> entity = std::make_shared<Entity>();
        std::shared_ptr<Position> pos = std::make_shared<Position>(550, 350);
        std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("assets/enemy/sprites/enemy1.png");
        std::shared_ptr<Entity> entity2 = createText("R-Type", Position(900, 50), 50);
        std::shared_ptr<Entity> entity3 = createText("Clearly made by us", Position(850, 100), 30);
        std::shared_ptr<Entity> entity4 = createText("Iona Dommel-Prioux\nAntoine Penot\nCamille Maux\nIzaac Carcenac-Sautron\nCyril Dehaese\nRoxane Baert", Position(10, 450), 15);

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
        std::shared_ptr<Sprite> bg = std::make_shared<Sprite>("assets/Background/Background.png");
        std::shared_ptr<Position> bgPos = std::make_shared<Position>(800 / 2 - 400, 600 / 2 - 300);
        std::shared_ptr<Entity> playButtonEntity = createImage("assets/MainMenu/Play/Button Normal.png", Position(865, 407), 114, 38);
        std::shared_ptr<Entity> optionButtonEntity = createImage("assets/MainMenu/Icon/option.png", Position(0, 0), 32, 31);
        std::shared_ptr<Entity> manetteButtonEntity = createImage("assets/MainMenu/Icon/info.png", Position(15, 930), 5, 31);
        std::shared_ptr<Entity> quitButtonEntity = createImage("assets/MainMenu/Quit/Button Normal.png", Position(865, 537), 114, 38);

        backgroundEntity->addComponent(bg)
            .addComponent(bgPos);
        createMsgEvent(playButtonEntity, NetworkMessageType::READY);
        createSceneEvent(optionButtonEntity, SceneType::SOUND);
        createSceneEvent(manetteButtonEntity, SceneType::HELP);
        createMsgEvent(quitButtonEntity, NetworkMessageType::DISCONNECTED);
        createSceneEvent(quitButtonEntity, SceneType::NONE);
        scene->addEntities({createMusic("assets/Music/music.ogg"), backgroundEntity, playButtonEntity, optionButtonEntity, manetteButtonEntity, quitButtonEntity});
        return scene;
    }

    std::unique_ptr<IScene> GameSystem::createSettingMenu()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createSettingMenu, this), SceneType::OPTION);
        std::shared_ptr<Entity> entity1 = createImage("assets/Background/Option_Background.png", Position(0, 0), 1920, 1080);
        std::shared_ptr<Entity> entity2 = createImage("assets/MainMenu/Icon/back.png", Position(0, 0), 25, 21);
        std::shared_ptr<Entity> entity3 = createImage("assets/MainMenu/Icon/minus.png", Position(820, 450), 24, 9);
        std::shared_ptr<Entity> entity4 = createImage("assets/MainMenu/Icon/plus.png", Position(1040, 450), 24, 24);
        std::shared_ptr<Entity> entity5 = createImage("assets/MainMenu/Icon/sound off.png", Position(820, 550), 35, 24);
        std::shared_ptr<Entity> entity6 = createImage("assets/MainMenu/Icon/sound on.png", Position(1040, 550), 35, 24);
        std::shared_ptr<Entity> entity7 = createText("Option Menu", Position(800, 100), 50);
        std::shared_ptr<Entity> entity8 = createText("Volume", Position(870, 200), 50);
        std::shared_ptr<Entity> entity9 = createText("50", Position(925, 450), 80);

        createSceneEvent(entity2, SceneType::PREVIOUS);
        createSoundEvent(entity3, "-");
        createSoundEvent(entity4, "+");
        createSoundEvent(entity5, "mute");
        createSoundEvent(entity6, "unmute");
        scene->addEntities({createMusic("assets/Music/music.ogg"), entity1, entity2, entity3, entity4, entity5, entity6, entity7, entity8, entity9});
        return scene;
    }

    std::unique_ptr<IScene> GameSystem::createHelpMenu()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createHelpMenu, this), SceneType::HELP);
        std::shared_ptr<Entity> entity1 = createImage("assets/Background/Option_Background.png", Position(0, 0), 1920, 1080);
        std::shared_ptr<Entity> entity2 = createImage("assets/MainMenu/Icon/back.png", Position(0, 0), 25, 21);
        std::shared_ptr<Entity> entity3 = createText("Welcome in our game: RType.", Position(650, 100), 50);
        std::shared_ptr<Entity> entity4 = createText("Commande", Position(850, 200), 40);
        std::shared_ptr<Entity> entity5 = createText("Left: Left", Position(850, 300), 40);
        std::shared_ptr<Entity> entity6 = createText("Right: Right", Position(850, 350), 40);
        std::shared_ptr<Entity> entity7 = createText("Up: Up", Position(850, 400), 40);
        std::shared_ptr<Entity> entity8 = createText("Down: Down", Position(850, 450), 40);
        std::shared_ptr<Entity> entity9 = createText("Shoot: Right CTRL", Position(850, 500), 40);

        createSceneEvent(entity2, SceneType::PREVIOUS);

        scene->addEntities({createMusic("assets/Music/music.ogg") ,entity1, entity2, entity3, entity4, entity5, entity6, entity7, entity8, entity9});
        return scene;
    }

    std::unique_ptr<IScene> GameSystem::createEndMenu()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createEndMenu, this), SceneType::END);
        std::shared_ptr<Entity> entity1 = createImage("assets/MainMenu/pause.png", Position(0, 0), 800, 600);
        std::shared_ptr<Entity> entity2 = createText("End", Position(350, 25), 50);
        std::shared_ptr<Entity> quitButtonEntity = createImage("assets/MainMenu/Quit/Button Normal.png", Position(800 / 2 - 60, 800 / 2 - 18), 120, 28);

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

    std::shared_ptr<IEntity> GameSystem::createMusic(std::string path)
    {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>();
        std::shared_ptr<MusicComponent> component = std::make_shared<MusicComponent>(path);

        entity->addComponent(component);
        return entity;
    }

    void GameSystem::activateNetwork()
    {
        _networkActivated = true;
    }

    bool GameSystem::isNetworkActivated()
    {
        return _networkActivated;
    }

    void GameSystem::createPlayer(IScene &scene, int keyRight, int keyLeft, int keyUp, int keyDown, int keyMissile, long unsigned int id, Position pos, bool isMe)
    {
        std::shared_ptr<Entity> playerEntity = std::make_shared<Entity>(id);
        std::shared_ptr<Position> playerPos = std::make_shared<Position>(pos);
        std::shared_ptr<Velocity> playerVel = std::make_shared<Velocity>(0, 0);
        Rectangle rect = {playerPos->x + SCALE / 4, playerPos->y + SCALE / 4, SCALE, SCALE};
        std::shared_ptr<Hitbox> playerHitbox = std::make_shared<Hitbox>(rect);
        std::shared_ptr<Player> player = std::make_shared<Player>(id, keyUp, keyDown, keyLeft, keyRight, keyMissile);
        std::shared_ptr<EventListener> playerListener = std::make_shared<EventListener>();
        std::shared_ptr<Sprite> playerSprite = std::make_shared<Sprite>("assets/Player/MainShipSSP1.png", 0.0f, 2.0f);
        std::shared_ptr<Animation2D> anim = std::make_shared<Animation2D>(_spriteFrameCounts["assets/Player/MainShipSSP1.png"], 30, Animation2D::AnimationType::LOOP);
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
            playerListener->addGamepadEvent(id - 1, (GamepadButton)GAMEPAD_BUTTON_LEFT_FACE_UP, moveUpCallbacks);
            playerListener->addGamepadEvent(id - 1, (GamepadButton)GAMEPAD_BUTTON_LEFT_FACE_RIGHT, moveRightCallbacks);
            playerListener->addGamepadEvent(id - 1, (GamepadButton)GAMEPAD_BUTTON_LEFT_FACE_DOWN, moveDownCallbacks);
            playerListener->addGamepadEvent(id - 1, (GamepadButton)GAMEPAD_BUTTON_LEFT_FACE_LEFT, moveLeftCallbacks);
            playerListener->addGamepadStickEvent(id - 1, GAMEPAD_AXIS_LEFT_X, moveHorizontalStickCallback);
            playerListener->addGamepadStickEvent(id - 1, GAMEPAD_AXIS_LEFT_Y, moveVerticalStickCallback);
            playerEntity->addComponent(playerListener);
        }
        playerEntity->addComponents({player, playerPos, playerSprite, anim, playerVel, playerHitbox, destruct});
        scene.addEntity(playerEntity);
    }

    void GameSystem::createMissile(IScene &scene, long unsigned int id, Position playerPos, Missile::MissileType type)
    {
        if (quint8(type) >= quint8(Missile::MissileType::NB))
            throw std::invalid_argument("Missile type: " + quint8(type));
        std::shared_ptr<Entity> entity = std::make_shared<Entity>(id);
        std::shared_ptr<Missile> missile = std::make_shared<Missile>(type);
        std::shared_ptr<Position> pos = std::make_shared<Position>(playerPos);
        int nbFrames = _spriteFrameCounts.find(_missilesSprites[type]) != _spriteFrameCounts.end() ? _spriteFrameCounts[_missilesSprites[type]] : 0;
        std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(_missilesSprites[type], type == Missile::MissileType::EN ? 180.0f : 0.0f, 1.0f);
        std::shared_ptr<Animation2D> anim = std::make_shared<Animation2D>(nbFrames, 24, type == Missile::MissileType::EN ? Animation2D::AnimationType::LOOP : Animation2D::AnimationType::ONCE);
        std::shared_ptr<Trajectory> trajectory = std::make_shared<Trajectory>(
            _missilesTrajectories[type].first,
            _missilesTrajectories[type].second, std::make_shared<Position>(*pos));

        entity->addComponent(missile)
            .addComponent(sprite)
            .addComponent(anim)
            .addComponent(pos)
            .addComponent(trajectory);
        scene.addEntity(entity);
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