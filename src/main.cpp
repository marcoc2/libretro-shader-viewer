#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <nfd.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <filesystem>

#include "shader_manager.h"
#include "shader_viewer.h"
#include "shader_params.h"
#include "shader_preset_parser.h"
#include "shader_loader.h"

class PixelArtViewer {
private:
    ShaderViewer shaderViewer;
    GLuint inputTexture = 0;
    int imageWidth = 0;
    int imageHeight = 0;
    int viewportWidth = 0;
    int viewportHeight = 0;
    bool needsRefresh = false;
    std::string currentImagePath;
    std::string currentPresetPath;
    ShaderParams params;
    ShaderPreset currentPreset;
    ShaderLoader shaderLoader;
    bool shadersLoaded = false;

    bool loadImage(const std::string& path) {
        if (inputTexture) {
            glDeleteTextures(1, &inputTexture);
        }

        int channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &imageWidth, &imageHeight, &channels, 4);
        
        if (!data) {
            std::cerr << "Failed to load image: " << path << std::endl;
            return false;
        }

        glGenTextures(1, &inputTexture);
        glBindTexture(GL_TEXTURE_2D, inputTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        stbi_image_free(data);
        currentImagePath = path;
        needsRefresh = true;
        std::cout << "Imagem carregada: " << path << " (" << imageWidth << "x" << imageHeight << ")" << std::endl;
        return true;
    }

    bool loadPreset(const std::string& path) {
        std::cout << "Iniciando carregamento do preset: " << path << std::endl;
        
        currentPresetPath = path;
        currentPreset = PresetParser::parse(path);
        shaderLoader.setBasePath(std::filesystem::path(path).parent_path().string());
        
        if (currentPreset.shader_count == 0) {
            std::cerr << "Failed to load preset or no shaders defined" << std::endl;
            return false;
        }

        std::cout << "Preset carregado com sucesso. Número de shaders: " << currentPreset.shader_count << std::endl;

        // Carrega os shaders
        std::vector<std::string> shaderSources = shaderLoader.loadShadersFromPreset(currentPreset);
        
        if (shaderSources.empty()) {
            std::cerr << "Failed to load shader sources" << std::endl;
            return false;
        }

        std::cout << "Shader sources carregadas: " << shaderSources.size() << " arquivos" << std::endl;
        for (size_t i = 0; i < shaderSources.size(); i++) {
            std::cout << "Shader " << i << " tamanho: " << shaderSources[i].length() << " bytes" << std::endl;
        }

        // Atualiza o ShaderViewer com os novos shaders
        if (!shaderViewer.loadShaders(shaderSources)) {
            std::cerr << "Failed to compile shaders" << std::endl;
            return false;
        }

        std::cout << "Shaders compilados e carregados com sucesso" << std::endl;
        shadersLoaded = true;
        needsRefresh = true;
        return true;
    }

    void renderGui() {
        ImGui::Begin("Shader Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        
        // Seção de carregamento
        if (ImGui::CollapsingHeader("Load Files", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Button("Load Image")) {
                nfdchar_t *outPath = NULL;
                nfdresult_t result = NFD_OpenDialog("png,jpg,jpeg,bmp", NULL, &outPath);
                
                if (result == NFD_OKAY) {
                    bool success = loadImage(outPath);
                    std::cout << "Carregamento de imagem: " << (success ? "sucesso" : "falha") << std::endl;
                    free(outPath);
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Load Shader Preset")) {
                nfdchar_t *outPath = NULL;
                nfdresult_t result = NFD_OpenDialog("glslp", NULL, &outPath);
                
                if (result == NFD_OKAY) {
                    bool success = loadPreset(outPath);
                    std::cout << "Carregamento de preset: " << (success ? "sucesso" : "falha") << std::endl;
                    free(outPath);
                }
            }

            if (!currentImagePath.empty()) {
                ImGui::Text("Image: %s", std::filesystem::path(currentImagePath).filename().string().c_str());
                ImGui::Text("Resolution: %dx%d", imageWidth, imageHeight);
            }

            if (!currentPresetPath.empty()) {
                ImGui::Text("Preset: %s", std::filesystem::path(currentPresetPath).filename().string().c_str());
                ImGui::Text("Shader Count: %d", currentPreset.shader_count);
            }
        }

        if (ImGui::Button("Refresh Shader")) {
            needsRefresh = true;
        }

        ImGui::Separator();
        
        // Parâmetros dos shaders
        if (shadersLoaded) {
            // Pass 0 Parameters
            if (ImGui::CollapsingHeader("Pass 0 Parameters", ImGuiTreeNodeFlags_DefaultOpen)) {
                needsRefresh |= ImGui::SliderFloat("Sharpness", &params.sharpness, 1.0f, 5.0f);
                needsRefresh |= ImGui::SliderFloat("Anti-Ringing", &params.antiRinging, 0.0f, 1.0f);
                needsRefresh |= ImGui::SliderFloat("Input Gamma", &params.inputGamma, 0.0f, 5.0f);
            }

            // Pass 1 Parameters
            if (ImGui::CollapsingHeader("Pass 1 Parameters", ImGuiTreeNodeFlags_DefaultOpen)) {
                needsRefresh |= ImGui::SliderFloat("Output Gamma", &params.outputGamma, 0.0f, 5.0f);
                needsRefresh |= ImGui::SliderFloat("Phosphor", &params.phosphor, 0.0f, 1.0f);
                needsRefresh |= ImGui::SliderFloat("Color Boost", &params.colorBoost, 1.0f, 2.0f);
                
                if (ImGui::TreeNode("Color Channel Boost")) {
                    needsRefresh |= ImGui::SliderFloat("Red Boost", &params.redBoost, 1.0f, 2.0f);
                    needsRefresh |= ImGui::SliderFloat("Green Boost", &params.greenBoost, 1.0f, 2.0f);
                    needsRefresh |= ImGui::SliderFloat("Blue Boost", &params.blueBoost, 1.0f, 2.0f);
                    ImGui::TreePop();
                }

                needsRefresh |= ImGui::SliderFloat("Scanlines Strength", &params.scanlinesStrength, 0.0f, 1.0f);
                needsRefresh |= ImGui::SliderFloat("Beam Min Width", &params.beamMinWidth, 0.0f, 1.0f);
                needsRefresh |= ImGui::SliderFloat("Beam Max Width", &params.beamMaxWidth, 0.0f, 1.0f);
            }
        } else {
            ImGui::TextColored(ImVec4(1,1,0,1), "No shaders loaded. Please load a shader preset.");
        }

        // Debug info
        if (ImGui::CollapsingHeader("Debug Info")) {
            ImGui::Text("Shaders loaded: %s", shadersLoaded ? "Yes" : "No");
            ImGui::Text("Needs refresh: %s", needsRefresh ? "Yes" : "No");
            if (shadersLoaded) {
                ImGui::Text("Pass 0 program: %u", shaderViewer.getPass0Program());
                ImGui::Text("Pass 1 program: %u", shaderViewer.getPass1Program());
            }
        }

        ImGui::End();
    }

public:
    void onFramebufferSizeChanged(int width, int height) {
        viewportWidth = width;
        viewportHeight = height;
        shaderViewer.setViewportSize(width, height);
        needsRefresh = true;
        std::cout << "Viewport redimensionada para: " << width << "x" << height << std::endl;
    }

    bool init(GLFWwindow* window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        
        // Configura o estilo do ImGui para melhor visibilidade
        ImGui::StyleColorsDark();
        ImGui::GetStyle().Alpha = 1.0f;
        ImGui::GetStyle().WindowRounding = 4.0f;
        ImGui::GetStyle().FrameRounding = 4.0f;
        ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
        
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");

        // Obtém o tamanho inicial da janela
        glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
        
        return shaderViewer.init(viewportWidth, viewportHeight);
    }

    void render() {
        // Limpa o buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Primeiro renderiza o shader se houver imagem
        if (inputTexture) {
            if (needsRefresh) {
                shaderViewer.setShaderParams(params);
            }
            shaderViewer.render(inputTexture, imageWidth, imageHeight);
            if (needsRefresh) {
                needsRefresh = false;
            }
        }

        // Reseta o estado do OpenGL para o ImGui
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);
        glDisable(GL_SCISSOR_TEST);
        glViewport(0, 0, viewportWidth, viewportHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Renderiza a interface
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderGui();

        ImGui::Render();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void cleanup() {
        if (inputTexture) {
            glDeleteTextures(1, &inputTexture);
        }
        
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    auto* viewer = static_cast<PixelArtViewer*>(glfwGetWindowUserPointer(window));
    if (viewer) {
        viewer->onFramebufferSizeChanged(width, height);
    }
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Pixel Art Shader Viewer", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    PixelArtViewer viewer;
    if (!viewer.init(window)) {
        return -1;
    }

    // Configura o callback de redimensionamento
    glfwSetWindowUserPointer(window, &viewer);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Configura o tamanho inicial
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    viewer.onFramebufferSizeChanged(width, height);

    while (!glfwWindowShouldClose(window)) {
        viewer.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    viewer.cleanup();
    glfwTerminate();
    return 0;
}