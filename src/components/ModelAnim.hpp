/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** ModelAnim.hpp
*/

#ifndef MODEL_ANIM_HPP
#define MODEL_ANIM_HPP

#include "Component.hpp"

namespace indie
{
    class ModelAnim : public Component
    {
    public:
        /**
         * @brief Construct a new ModelAnim component
         * @param filepath Path to the file containing the animation
         * @param currentFrame Current frame of the animation
         */
        ModelAnim(std::string const &filepath, int currentFrame = 0):
        Component(IComponent::Type::ANIMATION), _filepath(filepath), _currFrame(currentFrame){};

        /// @brief Get the animation filepath
        std::string getAnimPath() { return _filepath; };
        /// @brief Get the reference of the current frame
        int &getCurrentFrame() { return _currFrame; };
        /// @brief Get the reference of the animation number of frames
        int &getNbFrames() { return _nbFrames; };
        void setShouldPlay(bool shouldPlay) { _play = shouldPlay; };
        bool shouldPlay() const { return _play; };
        void triggerPlay(std::shared_ptr<IEntity> e) {if (_trigger) _trigger(e);};
        void setTrigger(std::function<void(std::shared_ptr<IEntity>)> trigger) { _trigger = trigger; };
    private:
        std::string _filepath;
        int _currFrame;
        /// @brief Set at -1 for initialization by the graphic system
        int _nbFrames = -1;
        bool _isInitialized = true;
        bool _play = false;
        std::function<void(std::shared_ptr<IEntity>)> _trigger;
    };
}

#endif /* !MODEL_ANIM_HPP */