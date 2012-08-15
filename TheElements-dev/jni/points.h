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
void deletePoint(struct Particle* particle);
void unSetPoint(struct Particle* particle);
void setElement(struct Particle* particle, struct Element* newElement);
void setBitmapColor(int xCoord, int yCoord, struct Element* element);
void clearBitmapColor(int xCoord, int yCoord);
void createBitmapFromPoints(void);
void unFreezeParticles(int xCoord, int yCoord);
char changeHeat(struct Particle* tempParticle, int heatChange);
char fixHeat(int heat);
char hasSpecial(struct Particle* tempParticle, int special);
char getParticleSpecialVal(struct Particle* tempParticle, int special);
void setParticleSpecialVal(struct Particle* tempParticle, int special, char val);
char getElementSpecialVal(struct Element* tempElement, int special);
void clearSpecialVals(struct Particle* tempParticle);

#endif //!POINTS_H_INCLUDED
