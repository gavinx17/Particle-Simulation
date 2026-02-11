#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "particle.h"
#include "block.h"
#include "button.h"

using namespace std;

#define max_size 1
#define min_size -1

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(void)
{
    srand(static_cast<unsigned>(time(0)));
    int height = 400, width = 400;
    Particle p;
    std::vector<Block> blocks;
    blocks.emplace_back();
    blocks.emplace_back();
    //Button btn;
    bool buttonClicked = false;

    float startX = min_size + ((float)rand() / RAND_MAX) * (max_size - min_size);
    float startY = min_size + ((float)rand() / RAND_MAX) * (max_size - min_size);
    p = Particle(startX, startY, 0.03f); // Last argument is radius for every particle.

    // --- Initialize GLFW ---
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Particle Simulation", nullptr, nullptr);
    if (!window) {
        cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // --- Initialize GLAD ---
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    blocks[0].InitBlock(0.5f, 0.245f);
    blocks[1].InitBlock(-0.5f, 0.245f);
    //btn.InitButton();
    // --- Set up OpenGL state ---
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) {
        glViewport(0, 0, w, h);
    });

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // --- Main loop ---
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(0.078f, 0.75f, 0.078f);
        //btn.Update();
        p.DrawParticle();
        p.Update(0.0008f);
        for(Block& block: blocks) {
            block.DrawBlock();
            if (p.CheckCollision(p, block)) {
                p.vx *= -1;
                p.vy *= -1;
            }
        }

        // --- Handle input ---
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            p.vx += 0.0006f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            p.vx -= 0.0006f;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            p.vy += 0.004f;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
