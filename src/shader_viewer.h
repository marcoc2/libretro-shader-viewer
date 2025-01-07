#ifndef SHADER_VIEWER_H
#define SHADER_VIEWER_H

#include "shader_manager.h"
#include "shader_params.h"
#include <string>
#include <iostream>

class ShaderViewer {
private:
    GLuint pass0Program = 0;
    GLuint pass1Program = 0;
    GLuint intermediateFramebuffer = 0;
    GLuint intermediateTexture = 0;
    GLuint vao = 0;
    GLuint vbo = 0;
    ShaderParams params;
    int viewportWidth = 0;
    int viewportHeight = 0;

    void setupQuad() {
        float vertices[] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void setupFramebuffer(int width, int height) {
        if (intermediateFramebuffer) {
            glDeleteFramebuffers(1, &intermediateFramebuffer);
            glDeleteTextures(1, &intermediateTexture);
        }

        glGenFramebuffers(1, &intermediateFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, intermediateFramebuffer);

        glGenTextures(1, &intermediateTexture);
        glBindTexture(GL_TEXTURE_2D, intermediateTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, intermediateTexture, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Framebuffer is not complete!" << std::endl;
        }

        viewportWidth = width;
        viewportHeight = height;
    }

    void updateUniforms(GLuint program, GLuint texture, int width, int height) {
        glUseProgram(program);

        // Define matriz MVP (identidade por enquanto)
        float mvp[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        
        GLint mvpLocation = glGetUniformLocation(program, "MVPMatrix");
        if (mvpLocation != -1) {
            glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, mvp);
        }

        // Texturas
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        GLint location = glGetUniformLocation(program, "s_p");
        if (location != -1) {
            glUniform1i(location, 0);
        }

        // Dimensões
        location = glGetUniformLocation(program, "TextureSize");
        if (location != -1) {
            glUniform2f(location, (float)width, (float)height);
        }

        location = glGetUniformLocation(program, "InputSize");
        if (location != -1) {
            glUniform2f(location, (float)width, (float)height);
        }

        location = glGetUniformLocation(program, "OutputSize");
        if (location != -1) {
            glUniform2f(location, (float)viewportWidth, (float)viewportHeight);
        }

        // Parâmetros do Pass 0
        if (program == pass0Program) {
            location = glGetUniformLocation(program, "SHARPNESS");
            if (location != -1) glUniform1f(location, params.sharpness);
            
            location = glGetUniformLocation(program, "CRT_ANTI_RINGING");
            if (location != -1) glUniform1f(location, params.antiRinging);
            
            location = glGetUniformLocation(program, "InputGamma");
            if (location != -1) glUniform1f(location, params.inputGamma);
        }
        // Parâmetros do Pass 1
        else if (program == pass1Program) {
            location = glGetUniformLocation(program, "OutputGamma");
            if (location != -1) glUniform1f(location, params.outputGamma);
            
            location = glGetUniformLocation(program, "PHOSPHOR");
            if (location != -1) glUniform1f(location, params.phosphor);
            
            location = glGetUniformLocation(program, "COLOR_BOOST");
            if (location != -1) glUniform1f(location, params.colorBoost);
            
            location = glGetUniformLocation(program, "RED_BOOST");
            if (location != -1) glUniform1f(location, params.redBoost);
            
            location = glGetUniformLocation(program, "GREEN_BOOST");
            if (location != -1) glUniform1f(location, params.greenBoost);
            
            location = glGetUniformLocation(program, "BLUE_BOOST");
            if (location != -1) glUniform1f(location, params.blueBoost);
            
            location = glGetUniformLocation(program, "SCANLINES_STRENGTH");
            if (location != -1) glUniform1f(location, params.scanlinesStrength);
            
            location = glGetUniformLocation(program, "BEAM_MIN_WIDTH");
            if (location != -1) glUniform1f(location, params.beamMinWidth);
            
            location = glGetUniformLocation(program, "BEAM_MAX_WIDTH");
            if (location != -1) glUniform1f(location, params.beamMaxWidth);
        }
    }

    void cleanupPrograms() {
        if (pass0Program) {
            glDeleteProgram(pass0Program);
            pass0Program = 0;
        }
        if (pass1Program) {
            glDeleteProgram(pass1Program);
            pass1Program = 0;
        }
    }

public:
    bool init(int width, int height) {
        setupQuad();
        setupFramebuffer(width, height);
        return true;
    }

    bool loadShaders(const std::vector<std::string>& shaderSources) {
        // Limpa programas existentes
        cleanupPrograms();

        if (shaderSources.size() < 2) {
            std::cerr << "Need at least two shaders (pass0 and pass1)" << std::endl;
            return false;
        }

        std::cout << "Criando programa para pass0..." << std::endl;
        pass0Program = ShaderManager::createProgram(shaderSources[0]);
        if (!pass0Program) {
            std::cerr << "Failed to create pass0 program" << std::endl;
            return false;
        }

        std::cout << "Criando programa para pass1..." << std::endl;
        pass1Program = ShaderManager::createProgram(shaderSources[1]);
        if (!pass1Program) {
            std::cerr << "Failed to create pass1 program" << std::endl;
            cleanupPrograms();
            return false;
        }

        std::cout << "Programas criados com sucesso: " << pass0Program << ", " << pass1Program << std::endl;
        return true;
    }

    bool hasShaders() const {
        return pass0Program != 0 && pass1Program != 0;
    }

    GLuint getPass0Program() const { return pass0Program; }
    GLuint getPass1Program() const { return pass1Program; }

    void setShaderParams(const ShaderParams& newParams) {
        params = newParams;
    }

    void setViewportSize(int width, int height) {
        setupFramebuffer(width, height);
    }

    void render(GLuint inputTexture, int width, int height) {
        if (!hasShaders()) {
            // Remove o log que está spammando
            // std::cerr << "Shaders não carregados ainda" << std::endl;
            return;
        }

        // Remove este log também
        // std::cout << "Renderizando com programas: " << pass0Program << ", " << pass1Program << std::endl;

        // Primeiro pass
        // std::cout << "Iniciando Pass 0..." << std::endl;  // Remove
        glBindFramebuffer(GL_FRAMEBUFFER, intermediateFramebuffer);
        glViewport(0, 0, viewportWidth, viewportHeight);
        updateUniforms(pass0Program, inputTexture, width, height);
        
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Segundo pass
        // std::cout << "Iniciando Pass 1..." << std::endl;  // Remove
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, viewportWidth, viewportHeight);
        updateUniforms(pass1Program, intermediateTexture, viewportWidth, viewportHeight);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Verificar erros OpenGL - Mantenha este
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cout << "Erro OpenGL: " << error << std::endl;
        }
    }

    ~ShaderViewer() {
        cleanupPrograms();
        if (intermediateFramebuffer) glDeleteFramebuffers(1, &intermediateFramebuffer);
        if (intermediateTexture) glDeleteTextures(1, &intermediateTexture);
        if (vao) glDeleteVertexArrays(1, &vao);
        if (vbo) glDeleteBuffers(1, &vbo);
    }
};

#endif // SHADER_VIEWER_H