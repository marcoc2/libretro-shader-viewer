#ifndef SHADER_PROCESSOR_H
#define SHADER_PROCESSOR_H

#include <string>
#include <sstream>
#include <vector>
#include <iostream>

struct ProcessedShader {
    std::string vertexSource;
    std::string fragmentSource;
};

class ShaderProcessor {
private:
    static std::string getShaderHeader() {
        std::stringstream header;
        header << "#version 330 core\n\n";
        
        // Parâmetros constantes
        header << "const float SHARPNESS = 1.0;\n";
        header << "const float CRT_ANTI_RINGING = 0.8;\n";
        header << "const float InputGamma = 2.5;\n";
        header << "const float B = 0.0;\n";  // Adicionado aqui
        header << "const float C = 0.5;\n";  // Adicionado aqui
        header << "\n";

        // Matriz invX
        header << "const mat4 invX = mat4(\n";
        header << "    vec4((-B - 6.0*C)/6.0,   (12.0 - 9.0*B - 6.0*C)/6.0,  -(12.0 - 9.0*B - 6.0*C)/6.0,   (B + 6.0*C)/6.0),\n";
        header << "    vec4((3.0*B + 12.0*C)/6.0, (-18.0 + 12.0*B + 6.0*C)/6.0, (18.0 - 15.0*B - 12.0*C)/6.0, -C),\n";
        header << "    vec4((-3.0*B - 6.0*C)/6.0, 0.0, (3.0*B + 6.0*C)/6.0, 0.0),\n";
        header << "    vec4(B/6.0, (6.0 - 2.0*B)/6.0, B/6.0, 0.0)\n";
        header << ");\n\n";

        // Macros
        header << "#define GAMMA_IN(color) pow(color, vec4(InputGamma))\n";
        header << "#define tex2D texture\n\n";

        return header.str();
    }

    static std::string createVertexShader() {
        std::stringstream vs;
        vs << "layout (location = 0) in vec4 VertexCoord;\n";
        vs << "layout (location = 1) in vec2 TexCoord;\n";
        vs << "out vec2 texCoord;\n";
        vs << "out vec4 color;\n\n";
        
        vs << "uniform mat4 MVPMatrix;\n";
        vs << "uniform vec2 OutputSize;\n";
        vs << "uniform vec2 TextureSize;\n";
        vs << "uniform vec2 InputSize;\n\n";
        
        vs << "void main() {\n";
        vs << "    gl_Position = MVPMatrix * VertexCoord;\n";
        vs << "    color = vec4(1.0);\n";
        vs << "    texCoord = TexCoord;\n";
        vs << "}\n";
        
        return vs.str();
    }

    static std::string createFragmentShader() {
        std::stringstream fs;
        fs << "in vec2 texCoord;\n";
        fs << "in vec4 color;\n";
        fs << "out vec4 FragColor;\n\n";
        
        fs << "uniform sampler2D s_p;\n";
        fs << "uniform vec2 OutputSize;\n";
        fs << "uniform vec2 TextureSize;\n";
        fs << "uniform vec2 InputSize;\n\n";
        
        fs << "void main() {\n";
        fs << "    vec2 texture_size = vec2(SHARPNESS*TextureSize.x, TextureSize.y);\n";
        fs << "    vec2 dx = vec2(1.0/texture_size.x, 0.0);\n";
        fs << "    vec2 dy = vec2(0.0, 1.0/texture_size.y);\n";
        fs << "    vec2 pix_coord = texCoord*texture_size+vec2(-0.5,0.0);\n";
        fs << "    vec2 tc = (floor(pix_coord)+vec2(0.5,0.0))/texture_size;\n";
        fs << "    vec2 fp = fract(pix_coord);\n";
        fs << "    vec4 c10 = GAMMA_IN(texture(s_p, tc     - dx));\n";
        fs << "    vec4 c11 = GAMMA_IN(texture(s_p, tc        ));\n";
        fs << "    vec4 c12 = GAMMA_IN(texture(s_p, tc     + dx));\n";
        fs << "    vec4 c13 = GAMMA_IN(texture(s_p, tc + 2.0*dx));\n";
        fs << "    vec4 min_sample = min(c11,c12);\n";
        fs << "    vec4 max_sample = max(c11,c12);\n";
        fs << "    mat4 color_matrix = mat4(c10, c11, c12, c13);\n";
        fs << "    vec4 lobes = vec4(fp.x*fp.x*fp.x, fp.x*fp.x, fp.x, 1.0);\n";
        fs << "    vec4 invX_Px  = invX * lobes;\n";
        fs << "    vec4 color = color_matrix * invX_Px;\n";
        fs << "    vec4 aux = color;\n";
        fs << "    color = clamp(color, min_sample, max_sample);\n";
        fs << "    color = mix(aux, color, CRT_ANTI_RINGING);\n";
        fs << "    FragColor = color;\n";
        fs << "}\n";
        
        return fs.str();
    }

public:
    static ProcessedShader process(const std::string& source, const std::string& section = "") {
        ProcessedShader result;
        std::cout << "Processando shader... ";
        
        std::string header = getShaderHeader();
        result.vertexSource = header + createVertexShader();
        result.fragmentSource = header + createFragmentShader();

        std::cout << "OK" << std::endl;
        return result;
    }
};

#endif // SHADER_PROCESSOR_H