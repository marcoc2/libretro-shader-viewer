#include "basic_shader_viewer.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    ShaderViewer* viewer = static_cast<ShaderViewer*>(glfwGetWindowUserPointer(window));
    if (viewer) {
        viewer->setViewportSize(width, height);
    }
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Visualizador de Shader", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();

    ShaderViewer viewer;
    if (!viewer.init()) {
        return -1;
    }

    // Configura o callback de redimensionamento
    glfwSetWindowUserPointer(window, &viewer);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Configura o tamanho inicial da viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    viewer.setViewportSize(width, height);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        viewer.render();
        viewer.renderGui();

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