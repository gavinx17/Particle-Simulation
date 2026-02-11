#ifndef BUTTON_H
#define BUTTON_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

class Button  {
    public:
        float height, width, mass, x, y,halfWidth,halfHeight,left,right,top,bottom;
        unsigned int VAO, VBO, shaderProgram;
        Button() : x(0), y(0), height(0.0f), width(0.0f) {}

        Button(float x, float y, float height, float width)
            : x(x), y(y), height(height), width(width) {}
        void InitButton();
        void DrawButton();
        void Update();
};
#endif