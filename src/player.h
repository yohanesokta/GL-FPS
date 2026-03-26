#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"

bool checkAllCollisions(float x, float y, float z);
void updatePlayer();
void handleMouse(int button, int state, int x, int y);

#endif
