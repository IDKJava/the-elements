/*
 * specials.h
 * --------------------------
 * Header file for specials.c. Contains functions and
 * declarations related to special values and functions
 * on the various elements.
 */

#ifndef SPECIALS_H_INCLUDED
#define SPECIALS_H_INCLUDED

#include "app.h"
#include "stdbool.h"

bool collisionSpecials(struct Particle* firstParticle, struct Particle* secondParticle);

#endif
