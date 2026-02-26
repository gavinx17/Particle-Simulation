#include "block.h"
#include "particle.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstdio>
#include <random>
#include <ctime>
#include <iostream>

using namespace std;

char* Block::getShaderCode(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        cerr << "Failed to open shader file: " << filename << endl;
        return nullptr;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* code = new char[length + 1];
    fread(code, 1, length, file);
    code[length] = '\0';
    fclose(file);
    return code;
}

bool Block::CheckCollision(Particle& p) {
    float closestX = max(left, min(p.x, right));
    float closestY = max(bottom, min(p.y, top));

    float dx = p.x - closestX;
    float dy = p.y - closestY;
    
    return ((dx * dx) + (dy * dy)) < (p.radius * p.radius);
}
void Block::InitBlock(float centerX, float centerY, float blockWidth, float blockHeight) {
    x = centerX;
    y = centerY;
    width = blockWidth;
    height = blockHeight;

    left   = x - width / 2.0f;
    right  = x + width / 2.0f;
    top    = y + height / 2.0f;
    bottom = y - height / 2.0f;

    float vertices[18] = {
        left,  top,    0.0f,
        right, top,    0.0f,
        right, bottom, 0.0f,

        right, bottom, 0.0f,
        left,  bottom, 0.0f,
        left,  top,    0.0f
    };

    // OpenGL VAO/VBO setup (same as before)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Block::DrawBlock() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void Block::Update() {

}