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

// Create a point and return the index of the created point, or -1
int createPoint(int xCoord, int yCoord, struct Element* element);
void deletePoint(int particle);
void unSetPoint(int particle);
void setElement(int particle, struct Element* newElement);
void setBitmapColor(int xCoord, int yCoord, struct Element* element);
void clearBitmapColor(int xCoord, int yCoord);
void createBitmapFromPoints(void);
void unFreezeParticles(int xCoord, int yCoord);
void changeHeat(unsigned char *heat, int heatChange);
int hasSpecial(int tempParticle, int special);
int getParticleSpecialVal(int tempParticle, int special);
void setParticleSpecialVal(int tempParticle, int special, int val);
int getElementSpecialVal(struct Element* tempElement, int special);
void clearSpecialValsToElementDefault(int tempParticle);


// Utility function to check if a point is in bounds
inline bool coordInBounds(int x, int y)
{
    return x >= 0 && x < workWidth && y >= 0 && y < workHeight;
}

#endif //!POINTS_H_INCLUDED
