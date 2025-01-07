#include <iostream>
#include "shader_preset_parser.h"
#include "shader_loader.h"

int main() {
    std::string presetPath = "crt-hyllian-multipass.glslp";
    
    // Parse o preset
    ShaderPreset preset = PresetParser::parse(presetPath);
    PresetParser::print(preset);

    // Carrega os shaders
    ShaderLoader loader;
    loader.setBasePath(presetPath);
    std::vector<std::string> shaderSources = loader.loadShadersFromPreset(preset);

    // Imprime informações sobre os shaders carregados
    std::cout << "\nShader Contents:" << std::endl;
    for (size_t i = 0; i < shaderSources.size(); i++) {
        std::cout << "\nPass " << i << " shader (" 
                  << preset.passes[i].shader_path << "):" << std::endl;
        // Imprime apenas as primeiras linhas para visualização
        size_t pos = shaderSources[i].find('\n', 200);
        if (pos == std::string::npos) pos = shaderSources[i].length();
        std::cout << shaderSources[i].substr(0, pos) << "...\n";
    }

    return 0;
}