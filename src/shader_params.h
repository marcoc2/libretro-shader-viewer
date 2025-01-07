#ifndef SHADER_PARAMS_H
#define SHADER_PARAMS_H

// Estrutura compartilhada de parâmetros do shader
struct ShaderParams {
    // Pass 0 params
    float sharpness = 1.0f;
    float antiRinging = 0.8f;
    float inputGamma = 2.5f;
    
    // Pass 1 params
    float outputGamma = 2.2f;
    float phosphor = 1.0f;
    float colorBoost = 1.5f;
    float redBoost = 1.0f;
    float greenBoost = 1.0f;
    float blueBoost = 1.0f;
    float scanlinesStrength = 0.72f;
    float beamMinWidth = 0.86f;
    float beamMaxWidth = 1.0f;
};

#endif // SHADER_PARAMS_H