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

void collide (int firstParticle, int secondParticle);

#endif //!COLLIDE_H_INCLUDED
