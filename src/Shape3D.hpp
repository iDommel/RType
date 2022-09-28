/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Shape3D
*/

#ifndef SHAPE3DH
#define SHAPE3DH

struct Vector3;
struct Color;

namespace indie
{
    /**struct Shape3D;
     * @brief Shape3D object
     */
    class Shape3D
    {
    public:
        /**
         * @brief Construct a new Shape3D object
         */
        Shape3D();
        /**
         * @brief Destroy Shape3D object
         */
        ~Shape3D();

        /**
         * @brief Draw sphere
         * @param centerPos position of the center of the sphere
         * @param radius raduis of the sphere
         * @param color Color of the sphere
         */
        static void drawSphere(Vector3 centerPos, float radius, Color color);
        /**
         * @brief Draw cube
         * @param position position of the center of the sphere
         * @param size size {x, y, z} of the cube
         * @param color Color of the cube
         */
        static void drawCube(Vector3 position, Vector3 size, Color color);
        /**
         * @brief Draw grid
         * @param slices size of the grid
         * @param spacing size of one slice
         */
        static void drawGrid(int slices, float spacing);
    };
}

#endif /* !MODELH */