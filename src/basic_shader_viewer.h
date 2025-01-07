#ifndef BASIC_SHADER_VIEWER_H
#define BASIC_SHADER_VIEWER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class ShaderViewer {
private:
    GLuint shaderProgram = 0;
    GLuint VAO = 0, VBO = 0, EBO = 0;
    GLuint texture = 0;
    int imageWidth = 0, imageHeight = 0;
    int viewportWidth = 0, viewportHeight = 0;
    float pixelSize = 1.0f;

    bool loadShaders();
    void setupQuad();
    void calculateAspectRatio();

public:
    bool init();
    bool loadImage(const char* path);
    void render();
    void renderGui();
    void setViewportSize(int width, int height);
    ~ShaderViewer();
};

#endif // BASIC_SHADER_VIEWER_H