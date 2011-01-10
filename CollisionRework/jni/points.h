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
//Include macros
#include "macros.h"

void CreatePoint(int xCoord, int yCoord, struct Element element);
void DeletePoint(struct Particle particle);
void setElement(struct Particle particle, struct Element newElement);
void setBitmapColor(int xCoord, int yCoord, struct Element element);
void createBitmapFromPoints(void);
void unFreezeParticles(int xCoord, int yCoord);

#endif //!POINTS_H_INCLUDED
