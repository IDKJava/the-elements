/*
 * points.h
 * ----------------------------
 * Declares functions for manipulating data
 * for individual points, such as adding and
 * deleting points.
 */

#ifndef POINTS_H_INCLUDED
#define POINTS_H_INCLUDED

//Include global variables
#include "app.h"

void CreatePoint(int k, int l, int e);
void DeletePoint(int partnum);
void setElement(int particle, int element);
void setBitmapColor(int xpos, int ypos, int element);
void createBitmapFromPoints(void);
void unFreezeParticles(int xcentre, int ycentre);

#endif
