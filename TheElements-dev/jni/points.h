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

void createPoint(int xCoord, int yCoord, struct Element* element);
void deletePoint(int particle);
void unSetPoint(int particle);
void setElement(int particle, struct Element* newElement);
void setBitmapColor(int xCoord, int yCoord, struct Element* element);
void clearBitmapColor(int xCoord, int yCoord);
void createBitmapFromPoints(void);
void unFreezeParticles(int xCoord, int yCoord);
void changeHeat(char *heat, int heatChange);
int hasSpecial(int tempParticle, int special);
int getParticleSpecialVal(int tempParticle, int special);
void setParticleSpecialVal(int tempParticle, int special, int val);
int getElementSpecialVal(struct Element* tempElement, int special);
void clearSpecialVals(int tempParticle);

#endif //!POINTS_H_INCLUDED
