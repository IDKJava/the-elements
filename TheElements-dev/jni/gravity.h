#ifndef GRAVITY_H
#define GRAVITY_H

#include "app.h"

// Update gravity field in response to added object
void updateGravityField(SpaceObj *obj, bool invert);
// Get the unit vector pointing in the direction of gravity at the given location
// Takes into account which world we're currently on. Optionally returns magnitude.
void getFallField(float x, float y, float *fx, float *fy, float *fmag);

// Helper functions to create gravity objects
bool makeBlackHole(int x, int y);
bool makeWhiteHole(int x, int y);
bool makeCurlHole(int x, int y);
bool makeNullGravity(int x, int y, int ex, int ey);

#endif // GRAVITY_H