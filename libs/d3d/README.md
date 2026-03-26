# D3D Wrapper (Yohanes Oktanio)

> Inspired by GameMaker 8.1's 3D functions.

This library provides a set of wrappers for drawing 3D primitives in OpenGL, designed to be easy to use for those familiar with GameMaker's D3D functions.

## Available Functions

### `d3d_draw_floor`
Draws a floor from `(x1, y1, z1)` to `(x2, y1, z2)`.
```cpp
void d3d_draw_floor(float x1, float y1, float z1, float x2, float y2, float z2, GLuint tex, float hrepeat, float vrepeat);
```

### `d3d_draw_wall`
Draws a vertical wall from `(x1, y1, z1)` to `(x2, y2, z2)`.
```cpp
void d3d_draw_wall(float x1, float y1, float z1, float x2, float y2, float z2, GLuint tex, float hrepeat, float vrepeat);
```

### `d3d_draw_block`
Draws a 3D block with texture applied to all 6 faces.
```cpp
void d3d_draw_block(float x1, float y1, float z1, float x2, float y2, float z2, GLuint tex, float hrepeat, float vrepeat);
```

### `d3d_draw_cylinder`
Draws a cylinder between two points.
- `closed`: If true, draws the top and bottom caps.
- `steps`: Number of segments for the circular cross-section.
```cpp
void d3d_draw_cylinder(float x1, float y1, float z1, float x2, float y2, float z2, GLuint tex, float hrepeat, float vrepeat, bool closed, int steps);
```

### `d3d_draw_ellipsoid`
Draws an ellipsoid or sphere within the specified bounding box.
- `steps`: Number of segments for both latitude and longitude.
```cpp
void d3d_draw_ellipsoid(float x1, float y1, float z1, float x2, float y2, float z2, GLuint tex, float hrepeat, float vrepeat, int steps);
```

## How to use
Include `d3d.h` in your project and call these functions within your rendering loop. Make sure your `CMakeLists.txt` includes `libs/d3d` and compiles `d3d.cpp`.

---

Copyright (c) 2026 Yohanes Oktanio. All rights reserved.

