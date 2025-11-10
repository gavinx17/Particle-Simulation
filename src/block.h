#ifndef BLOCK_H
#define BLOCK_H

#include "particle.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>

class Block {
    public:
    float height, width, mass, x, y,halfWidth,halfHeight,left,right,top,bottom;
    unsigned int VAO, VBO, shaderProgram;
    Block() : x(0), y(0), height(0.0f), width(0.0f) {}


    Block(float startX, float startY, float startHeight, float startWidth)
        : x(startX), y(startY), height(startHeight), width(startWidth) {}
    bool CheckCollision(Particle& p);
    void InitBlock();
    void DrawBlock();
};
#endif