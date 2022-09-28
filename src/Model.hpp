/*
** EPITECH PROJECT, 2022
** indie
** File description:
** models
*/

#ifndef MODELH
    #define MODELH

    struct Model;
    struct Vector3;
    struct Color;
    struct BoundingBox;

    #include <memory>
    #include <string>

namespace indie {

    ///@brief Model 3D object
    class Model {
        public:
            /**
             * @brief Construct a new model object
             * @param filename path to the model file
             * @param textureFile path to the texture file
             */
            Model(const std::string &modelFile, const std::string &textureFile);
            ///@brief destroy the model object
            ~Model();
            /**
             * @brief Draw the model
             * @param position vector 3d to draw the model
             * @param tint Color of the model (WHITE = using texture loaded)
             * @param scale float of scaling (1.0f = default scaling)
             */
            void draw(Vector3 position, Color tint, float scale = 1.0f);
            /// @brief unload the model object
            void unload();
            /**
             * @brief Draw (with rotation) the model
             * @param position vector 3d to draw the model
             * @param rotationAxis position of the center of rotation
             * @param rotationAngle rotation of the model
             * @param scale vector of scaling
             * @param tint Color of the model (WHITE = using texture loaded)
             */
            void drawRotate(Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint);
            /**
             * @brief Get the boudning box of the model (should be updated with a pos to make an hitbox)
             * @return The bounding box of the model
             */
            BoundingBox getBoundingBox(void);

            ::Model &getModel();

        private:
            ///@brief Pointer to the model struct of raylib
            std::unique_ptr <::Model> ptr;
    };
}

#endif /* !MODELH */