#ifndef PORTALS_H
#define PORTALS_H

#include "app.h"

// advances the nextPortal variable to the next available portal index
bool advanceNextPortal();

// Create a new portal going from the given x,y to ex,ey
bool makePortal(int x, int y, int ex, int ey);

// Deletes a portal at a given index as well as its pair if present
bool deletePortal(int idx);

// Deletes the points associated with the given portal
void deletePortalPoints(int portalIndex);

// Handles the collision for portals
bool specialPortal(int firstParticle, int secondParticle);


#endif // PORTALS_H