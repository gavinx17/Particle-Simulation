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

bool Block::CheckCollision(Particle& p) {
    float closestX = max(left, min(p.x, right));
    float closestY = max(bottom, min(p.y, top));

    float dx = p.x - closestX;
    float dy = p.y - closestY;
    
    return ((dx * dx) + (dy * dy)) < (p.radius * p.radius);
}
void Block::InitBlock() {
    random_device rd;
    unsigned int seed = rd() ? rd() : static_cast<unsigned int>(std::time(nullptr));
    mt19937 gen(seed);
    uniform_real_distribution<float> distrib(-.7f, .7f);

    float random_num = distrib(gen);
    float vertices[18] = {
        -random_num, -0.2f, 0.0f,  // top left
         random_num,  -0.2f, 0.0f,  // top right
         random_num, -1.0f, 0.0f,  // bottom right

         random_num, -1.0f, 0.0f,  // bottom right
        -random_num, -1.0f, 0.0f,  // bottom left
        -random_num,  -0.2f, 0.0f   // top left
    };
    // Compute min/max bounds from vertices
    float minX = vertices[0], maxX = vertices[0];
    float minY = vertices[1], maxY = vertices[1];
    for (int i = 0; i < 18; i += 3) {
        if (vertices[i] < minX) minX = vertices[i];
        if (vertices[i] > maxX) maxX = vertices[i];
        if (vertices[i + 1] < minY) minY = vertices[i + 1];
        if (vertices[i + 1] > maxY) maxY = vertices[i + 1];
    }
    // Set block properties
    x = (minX + maxX) / 2.0f;
    y = (minY + maxY) / 2.0f;
    width  = maxX - minX;
    height = maxY - minY;
    left   = minX;
    right  = maxX;
    top    = maxY;
    bottom = minY;
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.5, 0.2, 1.0);
        }
    )";

    // Loop through vertex array to find min/max
    for (int i = 0; i < 18; i += 3) {
        if (vertices[i] < minX) minX = vertices[i];
        if (vertices[i] > maxX) maxX = vertices[i];
        if (vertices[i + 1] < minY) minY = vertices[i + 1];
        if (vertices[i + 1] > maxY) maxY = vertices[i + 1];
    }

    // Set block properties
    x = (minX + maxX) / 2.0f;          // center X
    y = (minY + maxY) / 2.0f;          // center Y
    width = maxX - minX;               // total width
    height = maxY - minY;              // total height
    // Create VAO and VBO once
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Build shaders once
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Delete temporary shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set up attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
void Block::DrawBlock() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}