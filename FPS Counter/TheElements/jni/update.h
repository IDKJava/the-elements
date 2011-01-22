/*
 * update.c
 * -----------------------------------
 * Declares the function UpdateView(), which
 * is called every frame to update all the
 * particles' positions. Also provides all
 * the necessary includes and defines.
 */

#ifndef UPDATE_H_INCLUDED
#define UPDATE_H_INCLUDED

#include <stdlib.h>

//Include the global variables
#include "app.h"
//Include the points functions
#include "points.h"
//Include the collision function
#include "collide.h"
//Include the clear funtion (rsetup)
#include "setup.h"

void UpdateView(void);

#endif
