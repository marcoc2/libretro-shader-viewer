#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "basic_shader_viewer.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <nfd.h>
#include <iostream>
#include <algorithm>

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D imageTexture;
uniform float pixelSize;

void main() {
    vec2 texSize = textureSize(imageTexture, 0);
    vec2 pixelated = floor(TexCoord * texSize / pixelSize) * pixelSize / texSize;
    FragColor = texture(imageTexture, pixelated);
}
)";

bool ShaderViewer::loadShaders() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // Verificar erros de compilação do vertex shader
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Erro na compilação do vertex shader:\n" << infoLog << std::endl;
        return false;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // Verificar erros de compilação do fragment shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Erro na compilação do fragment shader:\n" << infoLog << std::endl;
        return false;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Verificar erros de linking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Erro no linking do shader program:\n" << infoLog << std::endl;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}

void ShaderViewer::setupQuad() {
    float vertices[] = {
        // positions        // texture coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,   // top left
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom left
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom right
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f    // top right
    };
    unsigned int indices[] = {
        0, 1, 2,   // first triangle
        0, 2, 3    // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

bool ShaderViewer::init() {
    if (!loadShaders()) {
        return false;
    }
    setupQuad();
    return true;
}

bool ShaderViewer::loadImage(const char* path) {
    if (texture) {
        glDeleteTextures(1, &texture);
    }

    int channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &imageWidth, &imageHeight, &channels, 0);
    
    if (!data) {
        std::cerr << "Failed to load image: " << path << std::endl;
        return false;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLenum format = channels == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
    return true;
}

void ShaderViewer::calculateAspectRatio() {
    if (!texture || viewportWidth == 0 || viewportHeight == 0) return;

    float windowAspect = (float)viewportWidth / viewportHeight;
    float imageAspect = (float)imageWidth / imageHeight;
    
    float scaleX = 1.0f;
    float scaleY = 1.0f;

    if (windowAspect > imageAspect) {
        // Janela mais larga que a imagem
        scaleX = imageAspect / windowAspect;
    } else {
        // Janela mais alta que a imagem
        scaleY = windowAspect / imageAspect;
    }

    // Matriz de projeção ortográfica com escala correta
    float projection[16] = {
        scaleX, 0.0f,   0.0f, 0.0f,
        0.0f,   scaleY, 0.0f, 0.0f,
        0.0f,   0.0f,   1.0f, 0.0f,
        0.0f,   0.0f,   0.0f, 1.0f
    };

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projection);
}

void ShaderViewer::setViewportSize(int width, int height) {
    viewportWidth = width;
    viewportHeight = height;
    glViewport(0, 0, width, height);
    calculateAspectRatio();
}

void ShaderViewer::render() {
    if (!texture) return;

    glUseProgram(shaderProgram);
    glUniform1f(glGetUniformLocation(shaderProgram, "pixelSize"), pixelSize);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shaderProgram, "imageTexture"), 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ShaderViewer::renderGui() {
    ImGui::Begin("Controles do Shader");
    
    if (ImGui::Button("Carregar Imagem")) {
        nfdchar_t *outPath = NULL;
        nfdresult_t result = NFD_OpenDialog("png,jpg,jpeg", NULL, &outPath);
        
        if (result == NFD_OKAY) {
            loadImage(outPath);
            calculateAspectRatio();  // Recalcula aspect ratio quando carrega nova imagem
            free(outPath);
        }
    }

    if (texture) {
        ImGui::Text("Dimensões da imagem: %dx%d", imageWidth, imageHeight);
        ImGui::Text("Dimensões da viewport: %dx%d", viewportWidth, viewportHeight);
        if (ImGui::SliderFloat("Tamanho do Pixel", &pixelSize, 1.0f, 32.0f)) {
            // Alterado para usar GL_NEAREST para melhor efeito de pixelização
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
    }

    ImGui::End();
}

ShaderViewer::~ShaderViewer() {
    if (shaderProgram) glDeleteProgram(shaderProgram);
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
    if (texture) glDeleteTextures(1, &texture);
}