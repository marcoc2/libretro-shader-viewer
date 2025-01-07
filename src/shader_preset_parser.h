#ifndef SHADER_PRESET_PARSER_H
#define SHADER_PRESET_PARSER_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>

struct ShaderPass {
    std::string shader_path;
    bool filter_linear = false;
    bool srgb_framebuffer = false;
    std::string scale_type_x = "viewport";
    std::string scale_type_y = "viewport";
    float scale_x = 1.0f;
    float scale_y = 1.0f;
    std::map<std::string, float> parameters;
};

struct ShaderPreset {
    std::vector<ShaderPass> passes;
    int shader_count = 0;
    std::string name;
};

class PresetParser {
private:
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t");
        if (first == std::string::npos)
            return "";
        size_t last = str.find_last_not_of(" \t");
        return str.substr(first, (last - first + 1));
    }

    static std::pair<std::string, std::string> parseLine(const std::string& line) {
        size_t equalPos = line.find('=');
        if (equalPos == std::string::npos)
            return {"", ""};

        std::string key = trim(line.substr(0, equalPos));
        std::string value = trim(line.substr(equalPos + 1));
        
        // Remove quotes if present
        if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
            value = value.substr(1, value.size() - 2);
        }
        
        return {key, value};
    }

    static bool parseBoolean(const std::string& value) {
        return value == "true" || value == "1";
    }

    static float parseFloat(const std::string& value, float defaultValue = 0.0f) {
        try {
            return std::stof(value);
        } catch (...) {
            return defaultValue;
        }
    }

public:
    static ShaderPreset parse(const std::string& filename) {
        ShaderPreset preset;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Failed to open preset file: " << filename << std::endl;
            return preset;
        }

        std::string line;
        while (std::getline(file, line)) {
            // Skip comments and empty lines
            if (line.empty() || line[0] == '#')
                continue;

            auto [key, value] = parseLine(line);
            if (key.empty())
                continue;

            // Parse shader count
            if (key == "shaders") {
                preset.shader_count = parseFloat(value, 0);
                preset.passes.resize(preset.shader_count);
            }
            // Parse individual shader properties
            else {
                size_t passIndex;
                // Check if this is a numbered property (e.g., shader0, scale_type_x0)
                if (key.length() > 1 && isdigit(key.back())) {
                    passIndex = key.back() - '0';
                    key = key.substr(0, key.length() - 1);

                    if (passIndex >= preset.passes.size())
                        continue;

                    // Parse pass-specific properties
                    if (key == "shader")
                        preset.passes[passIndex].shader_path = value;
                    else if (key == "filter_linear")
                        preset.passes[passIndex].filter_linear = parseBoolean(value);
                    else if (key == "srgb_framebuffer")
                        preset.passes[passIndex].srgb_framebuffer = parseBoolean(value);
                    else if (key == "scale_type_x")
                        preset.passes[passIndex].scale_type_x = value;
                    else if (key == "scale_type_y")
                        preset.passes[passIndex].scale_type_y = value;
                    else if (key == "scale_x")
                        preset.passes[passIndex].scale_x = parseFloat(value, 1.0f);
                    else if (key == "scale_y")
                        preset.passes[passIndex].scale_y = parseFloat(value, 1.0f);
                }
            }
        }

        // Set preset name from filename
        size_t lastSlash = filename.find_last_of("/\\");
        size_t lastDot = filename.find_last_of('.');
        if (lastSlash == std::string::npos)
            lastSlash = 0;
        else
            lastSlash++;
        if (lastDot == std::string::npos)
            lastDot = filename.length();
        preset.name = filename.substr(lastSlash, lastDot - lastSlash);

        return preset;
    }

    static void print(const ShaderPreset& preset) {
        std::cout << "Shader Preset: " << preset.name << std::endl;
        std::cout << "Number of passes: " << preset.shader_count << std::endl;
        
        for (size_t i = 0; i < preset.passes.size(); i++) {
            const auto& pass = preset.passes[i];
            std::cout << "\nPass " << i << ":" << std::endl;
            std::cout << "  Shader: " << pass.shader_path << std::endl;
            std::cout << "  Filter Linear: " << (pass.filter_linear ? "true" : "false") << std::endl;
            std::cout << "  sRGB Framebuffer: " << (pass.srgb_framebuffer ? "true" : "false") << std::endl;
            std::cout << "  Scale Type X: " << pass.scale_type_x << std::endl;
            std::cout << "  Scale Type Y: " << pass.scale_type_y << std::endl;
            std::cout << "  Scale X: " << pass.scale_x << std::endl;
            std::cout << "  Scale Y: " << pass.scale_y << std::endl;
        }
    }
};

#endif // SHADER_PRESET_PARSER_H