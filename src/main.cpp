#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "particle.h"
#include "block.h"

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

    const int num = 1;
    int particleNumber = 0;
    int height = 720, width = 1280;

    Particle p[num];
    Block b;

    // Initialize particle positions
    for (int i = 0; i < num; i++) {
        float startX = min_size + ((float)rand() / RAND_MAX) * (max_size - min_size);
        float startY = min_size + ((float)rand() / RAND_MAX) * (max_size - min_size);
        p[i] = Particle(startX, startY, 0.03f); // Last argument is radius for every particle.
    }

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

    b.InitBlock();
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
        for (int i = 0; i < num; i++) {
            p[i].DrawParticle();
            p[i].Update(0.001f);
            if (p[i].CheckCollision(p[i], b)) {
                    p[i].vx += 0.006f;
                    p[i].vy += 0.006f;
                }
            for (int j = i + 1; j < num; j++) {
                if (p[i].CheckCollision(p[i], p[j])) {
                    p[i].vx += 0.006f;
                    p[j].vx -= 0.006f;
                    p[i].vy += 0.006f;
                    p[j].vy -= 0.006f;
                }
            }
            if (p[i].y <= -1.0f && p[i].vy < 0.0f) {
                // Reverse direction at bottom and start growing
                p[i].vy *= -1.0f;
                p[i].growing = true;
            }
            else if (p[i].y >= 1.0f && p[i].vy > 0.0f) {
                // Reverse direction at top and start shrinking
                p[i].vy *= -1.0f;
                p[i].growing = false;
            }

            float growthRate = 0.00001f * (1.0f - fabs(p[i].y - 0.5f) * 2.0f);
            p[i].radius += p[i].growing ? growthRate : -growthRate;
        }

        // --- Handle input ---
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            p[particleNumber].vx += 0.006f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            p[particleNumber].vx -= 0.006f;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            p[particleNumber].vy += 0.005f;
            p[particleNumber].y += 0.005f;
        }

        static bool tabPressed = false;
        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
            if (!tabPressed) {
                particleNumber = (particleNumber + 1) % num;
                tabPressed = true;
            }
        } else {
            tabPressed = false;
        }
        b.DrawBlock();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
