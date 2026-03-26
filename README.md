<h1 align="center">FPP Retro Engine</h1>

> Drawing 3D Perspective & Texture Drawing Function with D3D Wrapper

<p align="center">
<img src="./static/d1.png">
</p>

## Features
- **3D Perspective Rendering**: Using GLUT and OpenGL.
- **Texture Loading**: Powered by `stb_image`.
- **D3D Wrapper**: GameMaker 8.1 style functions for easy 3D primitive drawing.

## D3D Functions
This project includes a wrapper in `libs/d3d` that mimics GameMaker 8.1's 3D drawing functions:
- `d3d_draw_floor`: Draws a horizontal floor.
- `d3d_draw_wall`: Draws a vertical wall.
- `d3d_draw_block`: Draws a 3D textured block (6 faces).
- `d3d_draw_cylinder`: Draws a textured cylinder with optional caps.
- `d3d_draw_ellipsoid`: Draws a textured ellipsoid/sphere.

## Build
```bash
mkdir build
cd build
cmake ..
make
./main
```
