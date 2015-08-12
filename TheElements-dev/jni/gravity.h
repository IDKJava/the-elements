#ifndef GRAVITY_H
#define GRAVITY_H

#include "app.h"

// Update gravity field in response to added object
void updateGravityField(SpaceHole *hole);
// Get the unit vector pointing in the direction of gravity at the given location
// Takes into account which world we're currently on. Optionally returns magnitude.
void getFallField(float x, float y, float *fx, float *fy, float *fmag);

#endif // GRAVITY_H