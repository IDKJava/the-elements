/*
 * collisions.h
 * -------------------------------
 * Contains all the collision data in a table. At some
 * point hopefully we can make some code that will load
 * this collision data from a file. When adding an element,
 * it must be added here.
 */

#ifndef COLLISIONS_H_INCLUDED
#define COLLISIONS_H_INCLUDED

extern char reciprocals[NUM_COLLISIONS];
extern char collision[NUM_BASE_ELEMENTS][NUM_BASE_ELEMENTS];

#endif //!COLLISIONS_H_INCLUDED
