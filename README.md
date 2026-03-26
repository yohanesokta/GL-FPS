

<h1 align="center">FPP Retro Engine</h1>

> **Drawing 3D Perspective & Texture Drawing Function with D3D Wrapper**

<p align="center">
<img src="./static/d1.png" alt="FPP Retro Engine Screenshot">
</p>

---

# Overview

**FPP Retro Engine** adalah engine **buatan saya sendiri**. Konsep 3D sederhana yang meniru gaya **GameMaker 8.1** untuk menggambar dunia 3D dengan perspektif, termasuk lantai, dinding, blok, dan objek 3D lain dengan tekstur. Engine ini menggunakan kombinasi **GLUT + OpenGL** untuk rendering dan **stb_image** untuk memuat tekstur.

Cocok untuk membuat demo FPS retro, prototyping game 3D sederhana, atau belajar konsep rendering 3D.

---

# Example: Shooting Engine

<p align="center">
<img src="./static/out.gif" alt="Shooting Engine Demo">
</p>

---

# Features

* **3D Perspective Rendering**
  Rendering dunia 3D berbasis perspektif menggunakan OpenGL/GLUT.

* **Texture Loading**
  Memuat tekstur dari file PNG, JPG, atau BMP dengan library `stb_image`.

* **D3D Wrapper**
  Wrapper yang meniru fungsi-fungsi **GameMaker 8.1 D3D** untuk menggambar lantai, dinding, blok, dan bentuk 3D lainnya.

* **Lightweight & Simple**
  Cocok untuk belajar 3D rendering tanpa engine besar seperti Unity atau Unreal.

---

# D3D Functions

Semua fungsi tersedia di folder `libs/d3d/`:

| Function             | Description                                        | Parameters                                                        |
| -------------------- | -------------------------------------------------- | ----------------------------------------------------------------- |
| `d3d_draw_floor`     | Menggambar lantai horizontal.                      | `x1, y1, z1, x2, y2, z2, texture, hrepeat, vrepeat`               |
| `d3d_draw_wall`      | Menggambar dinding vertikal.                       | `x1, y1, z1, x2, y2, z2, texture, hrepeat, vrepeat`               |
| `d3d_draw_block`     | Menggambar blok 3D (6 sisi) dengan tekstur.        | `x, y, z, width, height, depth, texture`                          |
| `d3d_draw_cylinder`  | Menggambar silinder dengan tekstur dan opsi tutup. | `x, y, z, radius, height, segments, texture, top_cap, bottom_cap` |
| `d3d_draw_ellipsoid` | Menggambar ellipsoid atau bola dengan tekstur.     | `x, y, z, radius_x, radius_y, radius_z, segments, rings, texture` |

> Semua fungsi mengikuti pola **D3D GameMaker style**, jadi mudah digunakan jika kamu familiar dengan GM8.1.

---

# Installation / Build

1. Clone repo:

```bash
git clone https://github.com/yohanesokta/GL-FPS.git
cd GL-FPS
```

2. Buat folder build dan compile:

```bash
mkdir build
cd build
cmake ..
make
```

3. Jalankan demo:

```bash
./main
```

---

# Dependencies

* **CMake** ≥ 3.20
* **OpenGL**
* **GLUT / FreeGLUT**
* **stb_image** (included in `libs/`)
* Compiler: GCC/Clang/Visual Studio

> Windows users: Pastikan `freeglut` sudah di-install dan path `GL`/`GLUT` tersedia di compiler.

---

# Usage

Contoh menggambar lantai + dinding + blok di `main.cpp`:

```cpp
// Lantai
d3d_draw_floor(0, 0, 0, 100, 0, 100, background_get_texture(bk_floor1), 10, 10);

// Dinding
d3d_draw_wall(0, 0, 0, 0, 50, 100, background_get_texture(bk_wall1), 5, 10);

// Blok
d3d_draw_block(20, 0, 20, 10, 10, 10, background_get_texture(bk_block1));
```

> Fungsi `background_get_texture()` memuat OpenGL texture dari file PNG/JPG yang sudah di-load.

---

# Notes

* Engine ini **tidak menggunakan shading modern** (hanya fixed-function OpenGL).
* Bisa dikembangkan untuk FPS, engine retro, atau eksperimen 3D kecil.
* Semua koordinat menggunakan **unit dunia** (misal 1 unit = 1 meter di dunia game).