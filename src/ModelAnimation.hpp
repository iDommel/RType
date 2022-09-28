/*
** EPITECH PROJECT, 2022
** indie
** File description:
** animation
*/

#ifndef MODELANIMATIONH
    #define MODELANIMATIONH

    #include <iostream>
    #include <memory>

    #include "Model.hpp"

    struct ModelAnimation;
    struct Model;

    namespace indie {
        /**
         * @brief ModelAnimation object
         */
        class ModelAnimation {
            public:
                /**
                 * @brief Construct / load ModelAnimation object
                 * @param fileName Path to the animation file
                 */
                ModelAnimation(const std::string &fileName);
                /**
                 * @brief Destroy ModelAnimation object
                 */
                ~ModelAnimation();

                /**
                 * @brief Update ModelAnimation object
                 * @param model Model to update
                 * @param frame Frame counter of the animation
                 */
                void updateModelAnimation(Model &model, int frame);
                /**
                 * @brief Unload ModelAnimation object
                 */
                void unloadModelAnimation();
                /**
                 * @brief Check validity of ModelAnimation object
                 * @param model Model to check
                 */
                bool isValid(::Model model);
                /**
                 * @brief Get the max frame of ModelAnimation object
                 */
                int getFrameCount();
            private:
                /**
                 * @brief List of animation object
                 */
                ::ModelAnimation *anims;
                unsigned int _animsCount;
                /// @brief Boolean to see if the animation is loaded or not
                bool _isLoaded;
        };
    }

#endif /* !MODELANIMATIONH */
