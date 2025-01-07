# Libretro Shader Viewer

An OpenGL-based viewer for Libretro (RetroArch) GLSL shaders.

## Description

This program allows you to view and test Libretro (RetroArch) GLSL shader presets (.glslp) files, applying them to images in real-time. It supports:
- Loading and displaying images
- Loading Libretro shader presets (.glslp)
- Real-time shader parameter adjustment
- Multiple shader passes
- Live preview window

## Dependencies

- OpenGL 3.3+
- GLFW3
- GLAD
- ImGui
- Native File Dialog (NFD)
- CMake (for building)

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

1. Run the program:
```bash
./pixel_shader_viewer
```

2. Use the interface to:
   - Load an image ("Load Image" button)
   - Load a shader preset ("Load Shader Preset" button)
   - Adjust shader parameters using the sliders
   - Preview changes in real-time

## Project Structure

- `src/`: Source code
  - `shaders/`: Example shader files
  - `main.cpp`: Main program
  - `shader_viewer.h`: Core viewer implementation
  - `shader_processor.h`: GLSL shader processor
  - `shader_loader.h`: Shader file loader
  - `shader_preset_parser.h`: GLSLP preset parser
  - `shader_params.h`: Shader parameters

## License

Copyright (C) 2024 Marco

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

## Credits

- The example CRT shader (crt-hyllian) is authored by Hyllian and is used under its own license terms.
- ImGui library by Omar Cornut and contributors
- GLFW library by Camilla Löwy and contributors
- Native File Dialog by Michael Labbe
- stb_image by Sean Barrett

## Contributing

Contributions are welcome. Please feel free to submit a Pull Request.
