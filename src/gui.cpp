#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

#define max_size 1
#define min_size -1
#define height 1200
#define width 1200

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

float sigmoid(float n) {
    return 1.0f / (1.0f + exp(-n));
}

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    

    GLFWwindow* window = glfwCreateWindow(width, height, "IMGui Test", nullptr, nullptr);
    glfwMakeContextCurrent(window);   // MUST come first

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
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
    // --- Set up OpenGL state ---
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) {
        glViewport(0, 0, w, h);
    });

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    float my_color[4] = {1,1,1,1};
    bool my_tool_active = true;
    bool show_graph = false;
    // --- Main loop ---
    while (!glfwWindowShouldClose(window) && my_tool_active)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
                if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
                if (ImGui::MenuItem("Close", "Ctrl+W"))  { my_tool_active = false; }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if(ImGui::Checkbox("Graph", &show_graph));
        if(show_graph) {
            float samples[100];
            for (int n = 0; n < 100; n++)
                samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
            ImGui::PlotLines("Samples", samples, 100, 0, nullptr, -1.0f, 1.0f, ImVec2(0, 80));
        }
        // Edit a color stored as 4 floats

        ImGui::ColorEdit4("Color", my_color);

        ImGui::TextColored(
            ImVec4(my_color[0], my_color[1], my_color[2], my_color[3]),
            "Important Stuff"
        );

        // Display contents in a scrolling region
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
