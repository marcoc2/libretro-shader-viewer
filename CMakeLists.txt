cmake_minimum_required(VERSION 3.10)
project(PixelArtShaderViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontra pacotes necessários
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(PkgConfig REQUIRED)

# Adiciona diretórios de dependências
add_subdirectory(external/glad)
add_subdirectory(external/imgui)
add_subdirectory(external/nfd)

# Configuração do executável principal
add_executable(pixel_shader_viewer 
    src/main.cpp
)

# Configuração do executável de teste do parser
add_executable(test_parser 
    src/test_parser.cpp
)

# Configuração do visualizador básico
add_executable(basic_shader_viewer
    src/basic_shader_viewer.cpp
    src/basic_shader_viewer_main.cpp
)

# Inclui diretórios de cabeçalho para o projeto principal
target_include_directories(pixel_shader_viewer PRIVATE
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/imgui
    ${CMAKE_SOURCE_DIR}/external/nfd/src/include
    ${CMAKE_SOURCE_DIR}/external/stb
    ${CMAKE_SOURCE_DIR}/src
)

# Inclui diretórios de cabeçalho para o teste do parser
target_include_directories(test_parser PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Inclui diretórios de cabeçalho para o visualizador básico
target_include_directories(basic_shader_viewer PRIVATE
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/imgui
    ${CMAKE_SOURCE_DIR}/external/nfd/src/include
    ${CMAKE_SOURCE_DIR}/external/stb
    ${CMAKE_SOURCE_DIR}/src
)

# Linka as bibliotecas para o projeto principal
target_link_libraries(pixel_shader_viewer PRIVATE
    glad
    imgui
    nfd
    glfw
    OpenGL::GL
    dl
)

# Linka as bibliotecas para o visualizador básico
target_link_libraries(basic_shader_viewer PRIVATE
    glad
    imgui
    nfd
    glfw
    OpenGL::GL
    dl
)
