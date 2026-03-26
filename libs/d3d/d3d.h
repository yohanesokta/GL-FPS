#ifndef D3D_H
#define D3D_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string.h>
#include <math.h>

void d3d_draw_floor(float x1, float y1, float z1, float x2, float y2, float z2, GLuint tex, float hrepeat, float vrepeat);
void d3d_draw_wall(float x1, float y1, float z1, float x2, float y2, float z2, GLuint tex, float hrepeat, float vrepeat);
void d3d_draw_block(float x1, float y1, float z1, float x2, float y2, float z2, GLuint tex, float hrepeat, float vrepeat);
void d3d_draw_cylinder(float x1, float y1, float z1, float x2, float y2, float z2, GLuint tex, float hrepeat, float vrepeat, bool closed, int steps);
void d3d_draw_ellipsoid(float x1, float y1, float z1, float x2, float y2, float z2, GLuint tex, float hrepeat, float vrepeat, int steps);

// Collision Detection
bool d3d_collision_block(float px, float py, float pz, float pr, float x1, float y1, float z1, float x2, float y2, float z2);
bool d3d_collision_cylinder(float px, float py, float pz, float pr, float x1, float y1, float z1, float x2, float y2, float z2);
bool d3d_collision_ellipsoid(float px, float py, float pz, float pr, float x1, float y1, float z1, float x2, float y2, float z2);


#endif
