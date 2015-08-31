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
#include <stdlib.h>
#include <sys/time.h>

bool collisionSpecials(int firstParticle, int secondParticle, float oldXFirst, float oldYFirst);
void specialSpawn(int particle);
void specialBreak(int particle);
void specialGrow(int particle);
void specialHeat(int particle);
void specialExplode(int particle);
void specialLife(int particle);
void specialWander(int particle);
void specialJump(int particle);
void specialTunnel(int particle);
int specialBurn(int particle);
void specialConductive(int particle);
void specialTrail(int particle);
bool specialFlow(int particle, int sp, float oldX, float oldY);

#endif
