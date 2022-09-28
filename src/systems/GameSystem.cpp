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

#include "GameSystem.hpp"
#include "EventSystem.hpp"
#include "CollideSystem.hpp"
#include "UIComponent.hpp"
#include "CameraComponent.hpp"
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
#include "CameraComponent.hpp"
#include "SoundComponent.hpp"
#include "MusicComponent.hpp"
#include "ParticleCloud.hpp"
#include "ModelAnim.hpp"
#include "Window.hpp"

namespace indie
{
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

    void GameSystem::init(indie::SceneManager &sceneManager)
    {
        std::cerr << "GameSystem::init" << std::endl;

        sceneManager.addScene(createScene(), SceneManager::SceneType::GAME);
        sceneManager.addScene(createSplashScreen(), SceneManager::SceneType::SPLASH);
        sceneManager.addScene(createMainMenu(), SceneManager::SceneType::MAIN_MENU);
        sceneManager.addScene(createSoundMenu(), SceneManager::SceneType::SOUND);
        sceneManager.addScene(createHelpMenu(), SceneManager::SceneType::HELP);
        sceneManager.addScene(createControllerMenu(), SceneManager::SceneType::CONTROLLER);
        sceneManager.addScene(createPreGameMenu(), SceneManager::SceneType::PREGAME);
        sceneManager.addScene(createPauseMenu(), SceneManager::SceneType::PAUSE);
        sceneManager.addScene(createEndMenu(), SceneManager::SceneType::END);
        sceneManager.setCurrentScene(SceneManager::SceneType::SPLASH);
        _collideSystem.init(sceneManager);
        AudioDevice::getMasterVolume() = 0.5;
        _aiSystem.init(sceneManager);
    }

    void GameSystem::replaceTextBindings(indie::SceneManager &sceneManager, std::shared_ptr<Player> players, int firstText)
    {
        if (SceneManager::getCurrentSceneType() == SceneManager::SceneType::CONTROLLER) {
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

    void GameSystem::updateTextBindings(indie::SceneManager &sceneManager, std::shared_ptr<Player> players, int firstText)
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

    void GameSystem::updatePlayerUI(SceneManager &sceneManager, std::vector<std::shared_ptr<IEntity>> &ui_elems)
    {
        std::vector<std::shared_ptr<IEntity>> to_destroy;

        if (SceneManager::getCurrentSceneType() == SceneManager::SceneType::GAME) {
            for (auto &ui_elem : ui_elems) {
                auto component = Component::castComponent<UIComponent>((*ui_elem)[IComponent::Type::UI]);
                component->update();
                if (component->shouldBeDestroyed() && component->getTextToDestroy()) {
                    to_destroy.push_back(component->getTextToDestroy());
                    to_destroy.push_back(ui_elem);
                }
            }
        }
        for (auto &e : to_destroy) {
            sceneManager.getCurrentScene().removeEntity(e);
        }
    }

    void GameSystem::update(indie::SceneManager &sceneManager, uint64_t dt)
    {
        int firstText = 9;

        if (SceneManager::getCurrentSceneType() == SceneManager::SceneType::CONTROLLER) {
            for (auto &e : sceneManager.getScene(SceneManager::SceneType::GAME)[IEntity::Tags::PLAYER]) {
                auto players = Component::castComponent<Player>((*e)[IComponent::Type::PLAYER]);
                updateTextBindings(sceneManager, players, firstText);
                replaceTextBindings(sceneManager, players, firstText);
                firstText += 5;
            }
        }
        if (sceneManager.getCurrentSceneType() == SceneManager::SceneType::SPLASH) {
            timeElasped += dt;
            if (timeElasped > 3000) {
                sceneManager.setCurrentScene(SceneManager::SceneType::MAIN_MENU);
            }
        }
        updatePlayers(sceneManager, dt);
        updatePlayerUI(sceneManager, sceneManager.getCurrentScene()[IEntity::Tags::UI]);
        _aiSystem.update(sceneManager, dt);
        updateAIs(sceneManager, dt);
        updateBombs(sceneManager, dt);
        _collideSystem.update(sceneManager, dt);

        auto renderables = sceneManager.getCurrentScene()[IEntity::Tags::RENDERABLE_3D];

        for (auto &renderable : renderables) {
            if (renderable->hasComponent(IComponent::Type::ANIMATION)) {
                auto component = Component::castComponent<ModelAnim>((*renderable)[IComponent::Type::ANIMATION]);
                if (component->getNbFrames() == -1)
                    continue;
                component->triggerPlay(renderable);
                if (!component->shouldPlay())
                    continue;
                component->getCurrentFrame()++;
                if (component->getCurrentFrame() >= component->getNbFrames())
                    component->getCurrentFrame() = 0;
            }
        }
        if (SceneManager::getCurrentSceneType() == SceneManager::SceneType::GAME) {
            std::string result = "Player ";
            Position pos = {280, 100};
            if (nbr_player == 0) {
                result = "AI win!";
                pos.x += 50;
            } else if (nbr_player == 1 && nbr_ai == 0) {
                auto entity = sceneManager.getCurrentScene()[IEntity::Tags::PLAYER][0];
                auto comp = Component::castComponent<Player>((*entity)[IComponent::Type::PLAYER]);
                result.append(std::to_string(comp->getId()));
                result.append(" won !");
            } else
                return;
            std::shared_ptr<Entity> entity = createText(result, pos, 40);

            sceneManager.getScene(SceneManager::SceneType::END)[IEntity::Tags::TEXT].push_back(entity);
            sceneManager.setCurrentScene(SceneManager::SceneType::END);
        }
    }

    std::unique_ptr<IScene> GameSystem::createSplashScreen()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createSplashScreen, this));
        std::shared_ptr<Entity> entity = std::make_shared<Entity>();
        std::shared_ptr<Position> pos = std::make_shared<Position>(550, 350);
        std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>("test_pictures/raylib_logo.png");
        std::shared_ptr<Entity> entity2 = createText("Super Bomberman", Position(200, 50), 50);
        std::shared_ptr<Entity> entity3 = createText("Made by Indie Studio", Position(250, 100), 30);
        std::shared_ptr<Entity> entity4 = createText("Iona Dommel-Prioux\nAntoine Penot\nCamille Maux\nIzaac Carcenac-Sautron\nLéo Maman\nMaxence Folio\nRoxanne Baert", Position(10, 450), 15);

        entity->addComponent(pos)
            .addComponent(sprite);
        scene->addEntities({entity, entity2, entity3, entity4});
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
                    if (AudioDevice::getMasterVolume() < 1 && value == "+") {
                        AudioDevice::getMasterVolume() += 0.1;
                        AudioDevice::setVolume(AudioDevice::getMasterVolume());
                        value2->getValue() = std::to_string(int(AudioDevice::getMasterVolume() * 100));
                    } else if (AudioDevice::getMasterVolume() >= 0.1 && value == "-") {
                        AudioDevice::getMasterVolume() -= 0.1;
                        AudioDevice::setVolume(AudioDevice::getMasterVolume());
                        value2->getValue() = std::to_string(int(AudioDevice::getMasterVolume() * 100));
                    }
                }
            },
            [](SceneManager &, Vector2 /*mousePosition*/) {},
            [](SceneManager &, Vector2 /*mousePosition*/) {},
            [](SceneManager &, Vector2 /*mousePosition*/) {});

        std::shared_ptr<EventListener> eventListener = std::make_shared<EventListener>();

        eventListener->addMouseEvent(MOUSE_BUTTON_LEFT, mouseCallbacks);
        entity->addComponent(eventListener);
    }

    void GameSystem::createSceneEvent(std::shared_ptr<Entity> &entity, SceneManager::SceneType scenetype)
    {
        MouseCallbacks mouseCallbacks(
            [scenetype, entity, this](SceneManager &sceneManager, Vector2 mousePosition) {
                auto comp = entity->getFilteredComponents({IComponent::Type::SPRITE, IComponent::Type::POSITION, IComponent::Type::RECT});
                auto pos = Component::castComponent<Position>(comp[1]);
                auto sprite = Component::castComponent<Sprite>(comp[0]);
                auto rect = Component::castComponent<Rect>(comp[2]);

                if (mousePosition.x > pos->x && mousePosition.x < pos->x + rect->width &&
                    mousePosition.y > pos->y && mousePosition.y < pos->y + rect->height) {
                    if (scenetype == SceneManager::SceneType::PREVIOUS)
                        sceneManager.setCurrentScene(SceneManager::getPreviousSceneType());
                    else if (scenetype == SceneManager::SceneType::NONE) {
                        exit(0);
                    } else if (scenetype == SceneManager::SceneType::GAME && sceneManager.getCurrentSceneType() != SceneManager::SceneType::PAUSE) {
                        sceneManager.setCurrentScene(SceneManager::SceneType::GAME, true);
                        _collideSystem.reloadCollidables3D(sceneManager);
                        EventSystem::reloadScene(sceneManager, SceneManager::SceneType::GAME);
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
                auto component = sceneManager.getScene(SceneManager::SceneType::GAME)[IEntity::Tags::PLAYER][id_player];
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

    bool GameSystem::_playSupporters = false;
    int GameSystem::_nbFrame = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> GameSystem::_startTime;

    void GameSystem::setPlaySupporters(bool play)
    {
        _playSupporters = play;
    }

    bool GameSystem::getPlaySupporters()
    {
        return _playSupporters;
    }

    void GameSystem::setStartTime(std::chrono::time_point<std::chrono::high_resolution_clock> startTime)
    {
        _startTime = startTime;
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> GameSystem::getStartTime()
    {
        return _startTime;
    }

    void GameSystem::setNbFrame(int nbFrame)
    {
        _nbFrame = nbFrame;
    }

    int GameSystem::getNbFrame()
    {
        return _nbFrame;
    }

    void GameSystem::createSupporter(IScene &scene, Position pos)
    {
        std::shared_ptr<Entity> supporterEntity = std::make_shared<Entity>();
        std::shared_ptr<Position> posComp = std::make_shared<Position>(pos);
        std::shared_ptr<Model3D> modelCom = std::make_shared<Model3D>("assets_test/guy.iqm", "assets_test/guytex.png");
        std::shared_ptr<ModelAnim> modelAnimComp = std::make_shared<ModelAnim>("assets_test/guyanim.iqm");

        modelAnimComp->getNbFrames() = 61;
        modelAnimComp->setTrigger([](std::shared_ptr<IEntity> e) {
            auto animComp = Component::castComponent<ModelAnim>((*e)[IComponent::Type::ANIMATION]);
            auto now = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - GameSystem::getStartTime()).count();
            GameSystem::setNbFrame(animComp->getCurrentFrame());

            if (GameSystem::getPlaySupporters() && delta < 2050) {
                animComp->setShouldPlay(true);
            } else if (GameSystem::getPlaySupporters() && GameSystem::getNbFrame() == 1) {
                GameSystem::setPlaySupporters(false);
                animComp->setShouldPlay(false);
            } else {
                animComp->setShouldPlay(false);
            }
        });
        supporterEntity->addComponents({posComp, modelCom, modelAnimComp});
        scene.addEntity(supporterEntity);
    }

    void GameSystem::createSupporters(IScene &scene)
    {
        createSupporter(scene, Position(0, GAME_TILE_SIZE, -10));
        createSupporter(scene, Position(15, GAME_TILE_SIZE, -20));
        createSupporter(scene, Position(11, GAME_TILE_SIZE, -5));
        createSupporter(scene, Position(23, GAME_TILE_SIZE, -15));
        createSupporter(scene, Position(5, GAME_TILE_SIZE, -3));
        createSupporter(scene, Position(20, GAME_TILE_SIZE, -10));
        createSupporter(scene, Position(35, GAME_TILE_SIZE, -20));
        createSupporter(scene, Position(50, GAME_TILE_SIZE, -5));
        createSupporter(scene, Position(55, GAME_TILE_SIZE, -15));
        createSupporter(scene, Position(60, GAME_TILE_SIZE, -3));
        createSupporter(scene, Position(70, GAME_TILE_SIZE, -10));
        createSupporter(scene, Position(65, GAME_TILE_SIZE, -20));
        createSupporter(scene, Position(80, GAME_TILE_SIZE, -5));
        createSupporter(scene, Position(85, GAME_TILE_SIZE, -15));
        createSupporter(scene, Position(69, GAME_TILE_SIZE, -3));
        createSupporter(scene, Position(55, GAME_TILE_SIZE, -15));
        createSupporter(scene, Position(60, GAME_TILE_SIZE, -3));
        createSupporter(scene, Position(130, GAME_TILE_SIZE, -10));
        createSupporter(scene, Position(125, GAME_TILE_SIZE, -20));
        createSupporter(scene, Position(110, GAME_TILE_SIZE, -5));
        createSupporter(scene, Position(115, GAME_TILE_SIZE, -15));
        createSupporter(scene, Position(119, GAME_TILE_SIZE, -3));
        createSupporter(scene, Position(50, GAME_TILE_SIZE, -10));
        createSupporter(scene, Position(515, GAME_TILE_SIZE, -20));
        createSupporter(scene, Position(511, GAME_TILE_SIZE, -5));
        createSupporter(scene, Position(523, GAME_TILE_SIZE, -15));
        createSupporter(scene, Position(55, GAME_TILE_SIZE, -3));
        createSupporter(scene, Position(520, GAME_TILE_SIZE, -10));
        createSupporter(scene, Position(535, GAME_TILE_SIZE, -20));
        createSupporter(scene, Position(550, GAME_TILE_SIZE, -5));
        createSupporter(scene, Position(555, GAME_TILE_SIZE, -15));
        createSupporter(scene, Position(560, GAME_TILE_SIZE, -3));
        createSupporter(scene, Position(570, GAME_TILE_SIZE, -10));
        createSupporter(scene, Position(565, GAME_TILE_SIZE, -20));
        createSupporter(scene, Position(580, GAME_TILE_SIZE, -5));
        createSupporter(scene, Position(585, GAME_TILE_SIZE, -15));
        createSupporter(scene, Position(569, GAME_TILE_SIZE, -3));
        createSupporter(scene, Position(555, GAME_TILE_SIZE, -15));
        createSupporter(scene, Position(560, GAME_TILE_SIZE, -3));
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

    std::shared_ptr<IEntity> GameSystem::createCamera(Vector3 camPos, Vector3 camTarget)
    {
        std::shared_ptr<Entity> cam = std::make_shared<Entity>();
        std::shared_ptr<CameraComponent> camera = std::make_shared<CameraComponent>(camTarget, camPos);

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
            
            splitVel.z = 0;
            (*pos) = (*pos) + (splitVel * (float)(dt / 1000.0f));
            (*hitbox) += splitVel * (float)(dt / 1000.0f);
            for (auto &collider : _collideSystem.getColliders(player)) {
                if (collider->hasTag(IEntity::Tags::BONUS)) {
                    auto bonusComp = Component::castComponent<Bonus>((*collider)[IComponent::Type::BONUS]);
                    (*playerComp).handleBonus(*bonusComp);
                    sceneManager.getCurrentScene().removeEntity(collider);
                } else if (!collider->hasTag(IEntity::Tags::TIMED) && !collider->hasTag(IEntity::Tags::BOMB) && !collider->hasTag(IEntity::Tags::RADAR)) {
                    (*pos).x = lastPos.x;
                    (*hitbox) -= splitVel * (float)(dt / 1000.0f);
                    break;
                }
            }

            splitVel.z = (*vel).z;
            splitVel.x = 0;
            (*pos) = (*pos) + (splitVel * (float)(dt / 1000.0f));
            (*hitbox) += splitVel * (float)(dt / 1000.0f);
            for (auto &collider : _collideSystem.getColliders(player)) {
                if (collider->hasTag(IEntity::Tags::BONUS)) {
                    auto bonusComp = Component::castComponent<Bonus>((*collider)[IComponent::Type::BONUS]);
                    (*playerComp).handleBonus(*bonusComp);
                    sceneManager.getCurrentScene().removeEntity(collider);
                } else if (!collider->hasTag(IEntity::Tags::TIMED) && !collider->hasTag(IEntity::Tags::BOMB) && !collider->hasTag(IEntity::Tags::RADAR)) {
                    (*pos).z = lastPos.z;
                    (*hitbox) -= splitVel * (float)(dt / 1000.0f);
                    break;
                }
            }
            playerComp->updateBombsVec();
        }
    }

    void GameSystem::updateAIs(SceneManager &sceneManager, uint64_t dt)
    {
        auto players = sceneManager.getCurrentScene()[IEntity::Tags::AI];

        for (auto &player : players) {
            auto pos = Component::castComponent<Position>((*player)[IComponent::Type::POSITION]);
            auto lastPos = *pos;
            auto vel = Component::castComponent<Velocity>((*player)[IComponent::Type::VELOCITY]);
            auto ai = Component::castComponent<AIPlayer>((*player)[IComponent::Type::AI]);
            auto hitbox = Component::castComponent<Hitbox>((*player)[IComponent::Type::HITBOX]);
            auto radarHitbox = Component::castComponent<Hitbox>((*ai->getRadar())[IComponent::Type::HITBOX]);

            (*pos) = *pos + (*vel * (float)(dt / 1000.0f));
            (*hitbox) += *vel * (float)(dt / 1000.0f);
            (*radarHitbox) += *vel * (float)(dt / 1000.0f);
            for (auto &collider : _collideSystem.getColliders(player)) {
                if (collider->hasTag(IEntity::Tags::BONUS)) {
                    auto bonusComp = Component::castComponent<Bonus>((*collider)[IComponent::Type::BONUS]);
                    ai->handleBonus(*bonusComp);
                    sceneManager.getCurrentScene().removeEntity(collider);
                } else if (!collider->hasTag(IEntity::Tags::TIMED) && !collider->hasTag(IEntity::Tags::BOMB) && !collider->hasTag(IEntity::Tags::RADAR)) {
                    (*pos) = lastPos;
                    (*hitbox) -= *vel * (float)(dt / 1000.0f);
                    (*radarHitbox) -= *vel * (float)(dt / 1000.0f);
                    break;
                }
            }
            ai->updateBombsVec();
        }
    }

    void GameSystem::updateBombs(SceneManager &sceneManager, uint64_t dt)
    {
        auto bombs = sceneManager.getCurrentScene()[IEntity::Tags::BOMB];
        auto explosions = sceneManager.getCurrentScene()[IEntity::Tags::TIMED];

        for (auto &bomb : bombs) {
            auto comp = Component::castComponent<Bomb>((*bomb)[IComponent::Type::BOMB]);
            auto pos = Component::castComponent<Position>((*bomb)[IComponent::Type::POSITION]);
            Vector3 vec = {pos->x, pos->y, pos->z};

            comp->setTimer(comp->getTimer() - dt);
            if (comp->getTimer() <= 0) {
                comp->explode(sceneManager, vec);
                sceneManager.getCurrentScene().removeEntity(bomb);
            }
        }

        for (auto &explosion : explosions) {
            auto comp = Component::castComponent<Timer>((*explosion)[IComponent::Type::TIMER]);

            comp->getTime() -= dt;
            if (comp->getTime() <= 0) {
                sceneManager.getCurrentScene().removeEntity(explosion);
                continue;
            }

            for (auto &collider : _collideSystem.getColliders(explosion)) {
                if (collider->hasTag(IEntity::Tags::DESTRUCTIBLE)) {
                    if (collider->hasTag(IEntity::Tags::PLAYER)) {
                        auto player = Component::castComponent<Player>((*collider)[IComponent::Type::PLAYER]);

                        player->kill();
                        GameSystem::setPlaySupporters(true);
                        GameSystem::setStartTime(std::chrono::high_resolution_clock::now());
                    } else if (collider->hasTag(IEntity::Tags::AI)) {
                        auto ai = Component::castComponent<AIPlayer>((*collider)[IComponent::Type::AI]);

                        sceneManager.getCurrentScene().removeEntity(ai->getRadar());
                        GameSystem::setPlaySupporters(true);
                        GameSystem::setStartTime(std::chrono::high_resolution_clock::now());
                    } else if (!collider->hasTag(IEntity::Tags::PLAYER)) {
                        auto tempPos = Component::castComponent<Position>((*collider)[IComponent::Type::POSITION]);
                        int chance = std::rand() % 4;

                        if (chance == 3)
                            createBonus(sceneManager.getCurrentScene(), *tempPos);
                    }
                    sceneManager.getCurrentScene().removeEntity(collider);
                } else if (collider->hasTag(IEntity::Tags::BOMB)) {
                    auto bombComp = Component::castComponent<Bomb>((*collider)[IComponent::Type::BOMB]);
                    auto pos = Component::castComponent<Position>((*collider)[IComponent::Type::POSITION]);
                    Vector3 vec = {pos->x, pos->y, pos->z};

                    bombComp->setTimer(0);
                    bombComp->explode(sceneManager, vec);
                    sceneManager.getCurrentScene().removeEntity(collider);
                }
            }
        }
    }

    std::unique_ptr<indie::IScene> GameSystem::createMainMenu()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createMainMenu, this));
        std::shared_ptr<Entity> entity1 = std::make_shared<Entity>();
        std::shared_ptr<Sprite> component = std::make_shared<Sprite>("assets/MainMenu/menu.png");
        std::shared_ptr<Position> component2 = std::make_shared<Position>(800 / 2 - 400, 600 / 2 - 300);
        std::shared_ptr<Entity> entity2 = createImage("assets/MainMenu/play_unpressed.png", Position(800 / 2 - 60, 500 / 2 - 18), 120, 28);
        std::shared_ptr<Entity> entity3 = createImage("assets/MainMenu/sound.png", Position(800 - 80, 600 - 80), 80, 80);
        std::shared_ptr<Entity> entity4 = createImage("assets/MainMenu/controller.png", Position(0, 600 - 80), 80, 80);
        std::shared_ptr<Entity> entity5 = createImage("assets/MainMenu/help.png", Position(0, 0), 80, 80);
        std::shared_ptr<Entity> entity6 = createImage("assets/MainMenu/quit_unpressed.png", Position(800 / 2 - 60, 700 / 2 - 18), 120, 28);


        entity1->addComponent(component2)
            .addComponent(component);
        scene->addEntity(entity1);
        createSceneEvent(entity2, SceneManager::SceneType::PREGAME);
        createSceneEvent(entity3, SceneManager::SceneType::SOUND);
        createSceneEvent(entity4, SceneManager::SceneType::CONTROLLER);
        createSceneEvent(entity5, SceneManager::SceneType::HELP);
        createSceneEvent(entity6, SceneManager::SceneType::NONE);
        scene->addEntities({entity2, entity3, entity4, entity5, entity6});
        return scene;
    }

    std::unique_ptr<indie::IScene> GameSystem::createSoundMenu()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createSoundMenu, this));
        std::shared_ptr<Entity> entity1 = createImage("assets/MainMenu/other_menu.png", Position(0, 0), 800, 600);
        std::shared_ptr<Entity> entity2 = createImage("assets/MainMenu/fleche.png", Position(0, 0), 80, 80);
        std::shared_ptr<Entity> entity3 = createImage("assets/MainMenu/minus.png", Position(220, 250), 80, 80);
        std::shared_ptr<Entity> entity4 = createImage("assets/MainMenu/plus.png", Position(500, 250), 80, 80);
        std::shared_ptr<Entity> entity5 = createText("Sound Menu", Position(250, 50), 50);
        std::shared_ptr<Entity> entity6 = createText("Master Volume", Position(300, 200), 25);
        std::shared_ptr<Entity> entity7 = createText("50", Position(370, 250), 80);

        createSceneEvent(entity2, SceneManager::SceneType::PREVIOUS);
        createSoundEvent(entity3, "-");
        createSoundEvent(entity4, "+");
        scene->addEntities({entity1, entity2, entity3, entity4, entity5, entity6, entity7});
        return scene;
    }

    std::unique_ptr<indie::IScene> GameSystem::createHelpMenu()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createHelpMenu, this));
        std::shared_ptr<Entity> entity1 = createImage("assets/MainMenu/other_menu.png", Position(0, 0), 800, 600);
        std::shared_ptr<Entity> entity2 = createImage("assets/MainMenu/fleche.png", Position(0, 0), 80, 80);
        std::shared_ptr<Entity> entity3 = createText("How to play", Position(250, 50), 50);
        std::shared_ptr<Entity> entity4 = createText("Welcome in our game: Boomberman made by Indie Studio.", Position(10, 150), 25);
        std::shared_ptr<Entity> entity5 = createText("You will be able to plant water bombs to destroy destructible\nblocks and maybe get some boosts.", Position(10, 250), 25);
        std::shared_ptr<Entity> entity6 = createText("If your bombs hit an opponent you will kill him.\nThe goal is to be the last man standing.", Position(10, 350), 25);

        createSceneEvent(entity2, SceneManager::SceneType::PREVIOUS);
        scene->addEntities({entity1, entity2, entity3, entity4, entity5, entity6});
        return scene;
    }

    std::unique_ptr<indie::IScene> GameSystem::createControllerMenu()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createControllerMenu, this));
        std::shared_ptr<Entity> entity1 = createImage("assets/MainMenu/other_menu.png", Position(0, 0), 800, 600);
        std::shared_ptr<Entity> entity2 = createImage("assets/MainMenu/fleche.png", Position(0, 0), 80, 80);
        std::shared_ptr<Entity> entity3 = createText("Controller Menu", Position(200, 50), 50);
        std::shared_ptr<Entity> entity4 = createText("Player 1", Position(50, 150), 25);
        std::shared_ptr<Entity> entity5 = createText("Player 2", Position(500, 150), 25);
        std::shared_ptr<Entity> entity6 = createText("Player 3", Position(50, 400), 25);
        std::shared_ptr<Entity> entity7 = createText("Player 4", Position(500, 400), 25);
        std::shared_ptr<Entity> entity8 = createText("UP:\nLEFT:\nRIGHT:\nDOWN:\nBOMB:", Position(10, 200), 20);
        std::shared_ptr<Entity> entity9 = createText("UP:\nLEFT:\nRIGHT:\nDOWN:\nBOMB:", Position(10, 450), 20);
        std::shared_ptr<Entity> entity10 = createText("UP:\nLEFT:\nRIGHT:\nDOWN:\nBOMB:", Position(500, 200), 20);
        std::shared_ptr<Entity> entity11 = createText("UP:\nLEFT:\nRIGHT:\nDOWN:\nBOMB:", Position(500, 450), 20);
        std::shared_ptr<Entity> entity12 = createText("", Position(100, 200), 20);
        std::shared_ptr<Entity> entity13 = createText("", Position(100, 230), 20);
        std::shared_ptr<Entity> entity14 = createText("", Position(100, 260), 20);
        std::shared_ptr<Entity> entity15 = createText("", Position(100, 290), 20);
        std::shared_ptr<Entity> entity16 = createText("", Position(100, 320), 20);
        std::shared_ptr<Entity> entity18 = createText("", Position(600, 200), 20);
        std::shared_ptr<Entity> entity19 = createText("", Position(600, 230), 20);
        std::shared_ptr<Entity> entity20 = createText("", Position(600, 260), 20);
        std::shared_ptr<Entity> entity21 = createText("", Position(600, 290), 20);
        std::shared_ptr<Entity> entity22 = createText("", Position(600, 320), 20);
        std::shared_ptr<Entity> entity23 = createText("", Position(100, 450), 20);
        std::shared_ptr<Entity> entity24 = createText("", Position(100, 480), 20);
        std::shared_ptr<Entity> entity25 = createText("", Position(100, 510), 20);
        std::shared_ptr<Entity> entity26 = createText("", Position(100, 540), 20);
        std::shared_ptr<Entity> entity27 = createText("", Position(100, 570), 20);
        std::shared_ptr<Entity> entity28 = createText("", Position(600, 450), 20);
        std::shared_ptr<Entity> entity29 = createText("", Position(600, 480), 20);
        std::shared_ptr<Entity> entity30 = createText("", Position(600, 510), 20);
        std::shared_ptr<Entity> entity31 = createText("", Position(600, 540), 20);
        std::shared_ptr<Entity> entity32 = createText("", Position(600, 570), 20);

        createSceneEvent(entity2, SceneManager::SceneType::PREVIOUS);
        createBindingsEvent(entity12, 0, 0);
        createBindingsEvent(entity13, 0, 1);
        createBindingsEvent(entity14, 0, 2);
        createBindingsEvent(entity15, 0, 3);
        createBindingsEvent(entity16, 0, 4);
        createBindingsEvent(entity18, 1, 0);
        createBindingsEvent(entity19, 1, 1);
        createBindingsEvent(entity20, 1, 2);
        createBindingsEvent(entity21, 1, 3);
        createBindingsEvent(entity22, 1, 4);
        scene->addEntities({entity1, entity2, entity3, entity4, entity5, entity6, entity7});
        scene->addEntities({entity8, entity9, entity10, entity11});
        scene->addEntities({entity12, entity13, entity14, entity15, entity16, entity18, entity19, entity20, entity21, entity22});
        scene->addEntities({entity23, entity24, entity25, entity26, entity27, entity28, entity29, entity30, entity31, entity32});
        return scene;
    }

    std::unique_ptr<indie::IScene> GameSystem::createPreGameMenu()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createPreGameMenu, this));
        std::shared_ptr<Entity> entity1 = createImage("assets/MainMenu/other_menu.png", Position(0, 0), 800, 600);
        std::shared_ptr<Entity> entity2 = createImage("assets/MainMenu/fleche.png", Position(0, 0), 80, 80);
        std::shared_ptr<Entity> entity3 = createText("Number of player", Position(250, 150), 35);
        std::shared_ptr<Entity> entity5 = createText("2", Position(300, 250), 50);
        std::shared_ptr<Entity> entity6 = createText("3", Position(400, 250), 50);
        std::shared_ptr<Entity> entity7 = createText("4", Position(500, 250), 50);
        std::shared_ptr<Entity> entity8 = createImage("assets/MainMenu/play_unpressed.png", Position(800 / 2 - 60, 700 / 2 - 18), 120, 28);
        std::shared_ptr<Entity> entity9 = createImage("assets/MainMenu/circle.png", Position(470, 230), 80, 80);

        createSceneEvent(entity2, SceneManager::SceneType::MAIN_MENU);
        createNumberEvent(entity5, 2);
        createNumberEvent(entity6, 3);
        createNumberEvent(entity7, 4);
        createSceneEvent(entity8, SceneManager::SceneType::GAME);
        scene->addEntities({entity1, entity2, entity3, entity5, entity6, entity7, entity8, entity9});
        return scene;
    }

    std::unique_ptr<IScene> GameSystem::createScene()
    {
        ButtonCallbacks pauseCallbacks(
            [](SceneManager &) {},
            [](SceneManager &scenemanager) {
                scenemanager.setCurrentScene(SceneManager::SceneType::PAUSE);
            },
            [](SceneManager &) {},
            [](SceneManager &) {});
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createScene, this));
        std::shared_ptr<Entity> entity2 = std::make_shared<Entity>();
        std::shared_ptr<EventListener> listener = std::make_shared<EventListener>();
        Vector3 camPos = {GAME_MAP_WIDTH * GAME_TILE_SIZE / 2 /* / 8 * 5 */, 250.0f, GAME_MAP_HEIGHT * GAME_TILE_SIZE};
        Vector3 camTarget = {GAME_MAP_WIDTH * GAME_TILE_SIZE / 2, 0.0f, GAME_MAP_HEIGHT * GAME_TILE_SIZE / 2};

        listener->addKeyboardEvent(KEY_P, pauseCallbacks);
        entity2->addComponent(listener);
        createSupporters(*scene);
        createMusic(*scene);
        generateMap("assets/maps/map2.txt", *scene);
        scene->addEntities({createCamera(camPos, camTarget), entity2});
        return scene;
    }

    std::unique_ptr<IScene> GameSystem::createPauseMenu()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createPauseMenu, this));
        std::shared_ptr<Entity> entity = createText("Pause", Position(325, 50), 50);
        std::shared_ptr<Entity> entity2 = createImage("assets/MainMenu/resume_unpressed.png", Position(800 / 2 - 60, 400 / 2 - 18), 120, 28);
        std::shared_ptr<Entity> entity3 = createImage("assets/MainMenu/sound.png", Position(800 - 80, 600 - 80), 80, 80);
        std::shared_ptr<Entity> entity4 = createImage("assets/MainMenu/controller.png", Position(0, 600 - 80), 80, 80);
        std::shared_ptr<Entity> entity5 = createImage("assets/MainMenu/help.png", Position(0, 0), 80, 80);
        std::shared_ptr<Entity> entity6 = createImage("assets/MainMenu/quit_unpressed.png", Position(800 / 2 - 60, 800 / 2 - 18), 120, 28);
        std::shared_ptr<Entity> entity7 = createImage("assets/MainMenu/mainmenu_unpressed.png", Position(800 / 2 - 60, 600 / 2 - 18), 120, 28);
        std::shared_ptr<Entity> entity8 = createImage("assets/MainMenu/pause.png", Position(0, 0), 800, 600);

        createSceneEvent(entity2, SceneManager::SceneType::GAME);
        createSceneEvent(entity3, SceneManager::SceneType::SOUND);
        createSceneEvent(entity4, SceneManager::SceneType::CONTROLLER);
        createSceneEvent(entity5, SceneManager::SceneType::HELP);
        createSceneEvent(entity6, SceneManager::SceneType::NONE);
        createSceneEvent(entity7, SceneManager::SceneType::MAIN_MENU);
        scene->addEntities({entity8, entity, entity2, entity3, entity4, entity5, entity6, entity7});
        return (scene);
    }

    std::unique_ptr<IScene> GameSystem::createEndMenu()
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::bind(&GameSystem::createEndMenu, this));
        std::shared_ptr<Entity> entity1 = createImage("assets/MainMenu/pause.png", Position(0, 0), 800, 600);
        std::shared_ptr<Entity> entity2 = createText("End", Position(350, 25), 50);
        std::shared_ptr<Entity> entity3 = createImage("assets/MainMenu/play_unpressed.png", Position(800 / 2 - 60, 400 / 2 - 18), 120, 28);
        std::shared_ptr<Entity> entity7 = createImage("assets/MainMenu/quit_unpressed.png", Position(800 / 2 - 60, 800 / 2 - 18), 120, 28);
        std::shared_ptr<Entity> entity8 = createImage("assets/MainMenu/mainmenu_unpressed.png", Position(800 / 2 - 60, 600 / 2 - 18), 120, 28);

        createSceneEvent(entity3, SceneManager::SceneType::PREGAME);
        createSceneEvent(entity7, SceneManager::SceneType::NONE);
        createSceneEvent(entity8, SceneManager::SceneType::MAIN_MENU);
        scene->addEntities({entity1, entity2, entity3, entity7, entity8});
        return (scene);
    }

    void GameSystem::createBonus(IScene &scene, const Position &pos)
    {
        std::shared_ptr<Entity> bonus = std::make_shared<Entity>();
        std::shared_ptr<Bonus> bonusComp = nullptr;
        std::shared_ptr<Position> bonusPos = std::make_shared<Position>(pos);
        std::shared_ptr<Sphere> bonusSphere = nullptr;
        Vector3 size = {GAME_TILE_SIZE, GAME_TILE_SIZE, GAME_TILE_SIZE};
        Vector3 spherePos = {std::roundf(pos.x / GAME_TILE_SIZE) * GAME_TILE_SIZE - GAME_TILE_SIZE / 2, pos.y, std::roundf(pos.z / GAME_TILE_SIZE) * GAME_TILE_SIZE - GAME_TILE_SIZE / 2};
        std::shared_ptr<Hitbox> bonusHitbox = std::make_shared<Hitbox>(CollideSystem::makeBBoxFromSizePos(size, spherePos));

        switch (std::rand() % 3) {
        case 0:
            bonusComp = std::make_shared<Bonus>(Bonus::Type::BOMB);
            bonusSphere = std::make_shared<Sphere>(GAME_TILE_SIZE / 3, BLACK);
            break;
        case 1:
            bonusComp = std::make_shared<Bonus>(Bonus::Type::SPEED);
            bonusSphere = std::make_shared<Sphere>(GAME_TILE_SIZE / 3, GREEN);
            break;
        case 2:
            bonusComp = std::make_shared<Bonus>(Bonus::Type::POWER);
            bonusSphere = std::make_shared<Sphere>(GAME_TILE_SIZE / 3, RED);
            break;
        }
        bonus->addComponent(bonusComp)
            .addComponent(bonusSphere)
            .addComponent(bonusPos)
            .addComponent(bonusHitbox);
        scene.addEntity(bonus);
    }

    void GameSystem::createMusic(Scene &scene)
    {
        std::shared_ptr<Entity> musicEntity = std::make_shared<Entity>();
        std::shared_ptr<MusicComponent> musicComponent = std::make_shared<MusicComponent>("music.ogg");

        musicEntity->addComponent(musicComponent);
        scene.addEntities({musicEntity});
    }

    void GameSystem::createAIPlayer(IScene &scene, int id, Position pos)
    {
        std::shared_ptr<Entity> player = std::make_shared<Entity>();
        std::shared_ptr<Position> aiPos = std::make_shared<Position>(pos);
        std::shared_ptr<Velocity> vel = std::make_shared<Velocity>(0, 0);
        std::shared_ptr<Hitbox> hitbox = std::make_shared<Hitbox>(true);
        std::shared_ptr<Model3D> model = std::make_shared<Model3D>("assets_test/playerobj.obj", "assets_test/playerobj.png", 3.0f);
        std::shared_ptr<AIPlayer> aiComponent = std::make_shared<AIPlayer>(id);
        std::shared_ptr<Destructible> destruct = std::make_shared<Destructible>();
        Vector3 radarSize = {GAME_TILE_SIZE * 5, GAME_TILE_SIZE, GAME_TILE_SIZE * 5};
        Vector3 radarPos = {pos.x - radarSize.x / 2, pos.y, pos.z - radarSize.z / 2};
        std::shared_ptr<Hitbox> radarBox = std::make_shared<Hitbox>(CollideSystem::makeBBoxFromSizePos(radarSize, radarPos));
        std::shared_ptr<Entity> radar = std::make_shared<Entity>();
        std::shared_ptr<Radar> radarR = std::make_shared<Radar>();

        player->addComponent(aiPos)
            .addComponent(vel)
            .addComponent(hitbox)
            .addComponent(model)
            .addComponent(aiComponent)
            .addComponent(destruct);
        radar->addComponent(radarBox)
            .addComponent(radarR);
        scene.addEntities({player, radar});
        aiComponent->setRadar(radar);
    }

    static const int offset = 20;
    static const int textWidth = 200;
    const Position GameSystem::_uiPos[4] = {
        Position(offset, 0),
        Position(800 - (textWidth + offset), 0),
        Position(0, 600 - (textWidth + offset)),
        Position(800 - (textWidth + offset), 600 - (textWidth + offset))
    };

    void GameSystem::createPlayerUI(IScene &scene, std::shared_ptr<IEntity> player)
    {
        std::shared_ptr<Player> playerComp = Component::castComponent<Player>((*player)[IComponent::Type::PLAYER]);
        int id = playerComp->getId();
        std::shared_ptr<Entity> textEntity = std::make_shared<Entity>();
        std::shared_ptr<Position> uiPos = std::make_shared<Position>(_uiPos[id - 1]);
        std::shared_ptr<String> uiText = std::make_shared<String>("", "", 16.0f);
        std::shared_ptr<Entity> uiEntity = std::make_shared<Entity>();

        textEntity->addComponents({uiPos, uiText});
        std::shared_ptr<UIComponent> uiComponent = std::make_shared<UIComponent>(player, textEntity);
        uiEntity->addComponent(uiComponent);
        scene.addEntities({textEntity, uiEntity});
    }

    void GameSystem::createPlayer(IScene &scene, int keyRight, int keyLeft, int keyUp, int keyDown, int keyBomb, int id, Position pos)
    {
        std::shared_ptr<Entity> playerEntity = std::make_shared<Entity>();
        std::shared_ptr<Position> playerPos = std::make_shared<Position>(pos);
        std::shared_ptr<Velocity> playerVel = std::make_shared<Velocity>(0, 0);
        BoundingBox towerBoundingBox = {{pos.x - 4.2f, pos.y + 0.0f, pos.z - 4.0f}, {pos.x + 4.2f, pos.y + 23.0f, pos.z + 4.0f}};
        std::shared_ptr<Hitbox> playerHitbox = std::make_shared<Hitbox>(towerBoundingBox);
        std::shared_ptr<Model3D> model = std::make_shared<Model3D>("assets_test/playerobj.obj", "assets_test/playerobj.png", 3.0f);
        std::shared_ptr<Player> player = std::make_shared<Player>(id, keyUp, keyDown, keyLeft, keyRight, keyBomb);
        std::shared_ptr<EventListener> playerListener = std::make_shared<EventListener>();
        std::shared_ptr<Destructible> destruct = std::make_shared<Destructible>();
        ButtonCallbacks moveRightCallbacks(
            [player, playerEntity](SceneManager &manager) {
                player->moveRight(manager, playerEntity, 1);
            },
            [player, playerEntity](SceneManager &manager) {
                player->stopRight(manager, playerEntity, 1);
            },
            [player, playerEntity](SceneManager &manager) {
                player->moveRight(manager, playerEntity, 1);
            },
            [player, playerEntity](SceneManager &manager) {
                player->stopRight(manager, playerEntity, 1);
            });
        ButtonCallbacks moveLeftCallbacks(
            [player, playerEntity](SceneManager &manager) {
                player->moveLeft(manager, playerEntity, 1);
            },
            [player, playerEntity](SceneManager &manager) {
                player->stopLeft(manager, playerEntity, 17);
            },
            [player, playerEntity](SceneManager &manager) {
                player->moveLeft(manager, playerEntity, 1);
            },
            [player, playerEntity](SceneManager &manager) {
                player->stopLeft(manager, playerEntity, 17);
            });
        ButtonCallbacks moveUpCallbacks(
            [player, playerEntity](SceneManager &manager) {
                player->moveUp(manager, playerEntity, 1);
            },
            [player, playerEntity](SceneManager &manager) {
                player->stopUp(manager, playerEntity, 1);
            },
            [player, playerEntity](SceneManager &manager) {
                player->moveUp(manager, playerEntity, 1);
            },
            [player, playerEntity](SceneManager &manager) {
                player->stopUp(manager, playerEntity, 1);
            });
        ButtonCallbacks moveDownCallbacks(
            [player, playerEntity](SceneManager &manager) {
                player->moveDown(manager, playerEntity, 1);
            },
            [player, playerEntity](SceneManager &manager) {
                player->stopDown(manager, playerEntity, 1);
            },
            [player, playerEntity](SceneManager &manager) {
                player->moveDown(manager, playerEntity, 1);
            },
            [player, playerEntity](SceneManager &manager) {
                player->stopDown(manager, playerEntity, 1);
            });
        ButtonCallbacks bombCallbacks(
            [player, playerEntity](SceneManager &manager) {
                player->generateBomb(manager, playerEntity);
            },
            [player, playerEntity](SceneManager &) {},
            [player, playerEntity](SceneManager &) {},
            [player, playerEntity](SceneManager &) {});
        std::function<void(SceneManager &, float)> moveHorizontalStickCallback = [player, playerEntity](SceneManager &manager, float value) {
            player->moveHorizontal(manager, playerEntity, value);
        };
        std::function<void(SceneManager &, float)> moveVerticalStickCallback = [player, playerEntity](SceneManager &manager, float value) {
            player->moveVertical(manager, playerEntity, value);
        };
    
        playerListener->addKeyboardEvent((KeyboardKey)player->getTagUp(), moveUpCallbacks);
        playerListener->addKeyboardEvent((KeyboardKey)player->getTagLeft(), moveLeftCallbacks);
        playerListener->addKeyboardEvent((KeyboardKey)player->getTagRight(), moveRightCallbacks);
        playerListener->addKeyboardEvent((KeyboardKey)player->getTagDown(), moveDownCallbacks);
        playerListener->addKeyboardEvent((KeyboardKey)player->getTagBomb(), bombCallbacks);
        playerListener->addGamepadEvent(id - 1, (GamepadButton)GAMEPAD_BUTTON_LEFT_FACE_UP, moveUpCallbacks);
        playerListener->addGamepadEvent(id - 1, (GamepadButton)GAMEPAD_BUTTON_LEFT_FACE_RIGHT, moveRightCallbacks);
        playerListener->addGamepadEvent(id - 1, (GamepadButton)GAMEPAD_BUTTON_LEFT_FACE_DOWN, moveDownCallbacks);
        playerListener->addGamepadEvent(id - 1, (GamepadButton)GAMEPAD_BUTTON_LEFT_FACE_LEFT, moveLeftCallbacks);
        playerListener->addGamepadEvent(id - 1, (GamepadButton)GAMEPAD_BUTTON_RIGHT_FACE_LEFT, bombCallbacks);
        playerListener->addGamepadStickEvent(id - 1, GAMEPAD_AXIS_LEFT_X, moveHorizontalStickCallback);
        playerListener->addGamepadStickEvent(id - 1, GAMEPAD_AXIS_LEFT_Y, moveVerticalStickCallback);
        playerEntity->addComponents({player, playerPos, playerVel, playerHitbox, model, playerListener, destruct});
        createPlayerUI(scene, playerEntity);
        scene.addEntity(playerEntity);
    }

    void GameSystem::loadEntity(std::shared_ptr<IEntity> entity)
    {
        _collideSystem.loadEntity(entity);
    }

    void GameSystem::unloadEntity(std::shared_ptr<IEntity> entity)
    {
        _collideSystem.unloadEntity(entity);
        if (entity->hasComponent(IComponent::Type::PLAYER))
            nbr_player -= 1;
        else if (entity->hasComponent(IComponent::Type::AI))
            nbr_ai -= 1;
    }

    void GameSystem::changeBindings(SceneManager &sceneManager, int id_player, int button)
    {
        auto entity = sceneManager.getScene(SceneManager::SceneType::GAME)[IEntity::Tags::PLAYER][id_player];
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