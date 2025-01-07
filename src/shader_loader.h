#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

class ShaderLoader {
private:
    std::string basePath;

    static std::string readFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Failed to open shader file: " << filepath << std::endl;
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

public:
    ShaderLoader() = default;

    void setBasePath(const std::string& presetPath) {
        basePath = presetPath;
        std::cout << "Base path set to: " << basePath << std::endl;
    }

    std::string loadShader(const std::string& shaderPath) {
        std::filesystem::path fullPath;
        
        // Se o caminho é relativo, concatena com o basePath
        if (std::filesystem::path(shaderPath).is_relative()) {
            fullPath = std::filesystem::path(basePath) / shaderPath;
        } else {
            fullPath = shaderPath;
        }

        std::cout << "Tentando carregar shader de: " << fullPath << std::endl;
        
        if (!std::filesystem::exists(fullPath)) {
            std::cerr << "Shader file not found: " << fullPath << std::endl;
            return "";
        }

        return readFile(fullPath.string());
    }

    std::vector<std::string> loadShadersFromPreset(const ShaderPreset& preset) {
        std::vector<std::string> shaderSources;
        
        for (const auto& pass : preset.passes) {
            std::cout << "Carregando shader: " << pass.shader_path << std::endl;
            std::string source = loadShader(pass.shader_path);
            if (source.empty()) {
                std::cerr << "Failed to load shader: " << pass.shader_path << std::endl;
                continue;
            }
            shaderSources.push_back(source);
        }

        return shaderSources;
    }
};

#endif // SHADER_LOADER_H