/*
 * collide.h
 * ----------------------------
 * Declares the collide function, which is the
 * heart of our app. It processes all the collisions by
 * type number. It alone is several hundred lines
 * long, thus the need for a separate file.
 */

#ifndef COLLIDE_H_INCLUDED
#define COLLIDE_H_INCLUDED

//Include stdlib for random
#include <stdlib.h>
//Include stdio for itoa
#include <stdio.h>
//Include the global variables
#include "app.h"
//Include the points functions
#include "points.h"
//Include logging
//#include <android/log.h>
//Include collision data
//#include "collisions.h"

// Resolves a collision between the first particle and the particle located in
// *target. `old` points to the allCoords location the first particle is coming
// from. After this function, target and old should point to valid particles or
// be empty if some particles were deleted.
// Returns whether the first particle was bounced to its old location (used to
// determine look-around freezing).
bool collide(int firstParticle, int* target, int* old, float oldXFirst, float oldYFirst);

#endif //!COLLIDE_H_INCLUDED
