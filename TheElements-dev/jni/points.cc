/*
 * points.c
 * ----------------------------
 * Defines functions for manipulating data
 * for individual points, such as adding and
 * deleting points.
 */

#include "points.h"
#include "portals.h"

#ifndef NDEBUG // Debug
#define LOGGING 1
#else // Release
#define LOGGING 0
#endif

static int dx;
static int dy;

int createPoint(int xCoord, int yCoord, struct Element* element)
{
    int created = -1;
    //If there are points available
    if (loq > 0)
    {
        //Get the pointer to the particle, decrement loq
        int i = created = avail[--loq];
        //Indicate that the particle is in use
        a_set[i] = TRUE;
        
        //Set x and y
        a_x[i] = xCoord+0.5;
        a_y[i] = yCoord+0.5;

        //Put it in the allcoords array
        allCoords[getIndex(a_x[i], a_y[i])] = i;

        // Set the element of the point
        a_element[i] = element;

        //Set the velocities
        a_xVel[i] = 0;
        a_yVel[i] = 0;

        //Set the frozen state
        a_frozen[i] = FALSE;

        //Clear all particle special vals
        clearSpecialValsToElementDefault(i);

        //Set the initial heat
        if(element->startingTemp == 0)
        {
            a_heat[i] = cAtmosphere->heat; //To be a variable later on
        }
        else
        {
            a_heat[i] = element->startingTemp;
        }

        //Set the point in the pixels array
        setBitmapColor(xCoord, yCoord, element);

        //Unfreeze particles around it
        //unFreezeParticles(xCoord, yCoord);
    }

    return created;
}
void deletePoint(int particle)
{
    //Store x and y in temporary variables for faster use
    int tempX = a_x[particle];
    int tempY = a_y[particle];

    //Unfreeze the particles around it
    //unFreezeParticles(tempX, tempY);
    //Clear it in the pixels array
    clearBitmapColor(tempX, tempY);
    //Clear it in the points array
    allCoords[getIndex(tempX, tempY)] = -1;

    //Unset the particle
    a_set[particle] = FALSE;
    //Add it to the avail array
    avail[loq] = particle;
    ++loq;
}


void erasePoint(int particle) {
    if (particle != -1)
    {
        if (hasSpecial(particle, SPECIAL_PORTAL)) {
            deletePortal(getParticleSpecialVal(particle, SPECIAL_PORTAL));
        } else {
            deletePoint(particle);
        }
    }
}


void unSetPoint(int particle)
{
    //Unset the particle
    a_set[particle] = FALSE;
    //Add it to the avail array
    avail[loq] = particle;
    ++loq;
}

void setElement(int particle, struct Element* newElement)
{
    int i;
    a_element[particle] = newElement;
    clearSpecialValsToElementDefault(particle);
    setBitmapColor(a_x[particle], a_y[particle], newElement);
}

void setBitmapColor(int xCoord, int yCoord, struct Element* element)
{
    unsigned char* pixel = colors + (getColorIndex(xCoord,yCoord) *3);
    pixel[0] = element->red;
    pixel[1] = element->green;
    pixel[2] = element->blue;
}
void clearBitmapColor(int xCoord, int yCoord)
{
    unsigned char* pixel = colors +  (getColorIndex(xCoord, yCoord) * 3);
    pixel[0] = cAtmosphere->backgroundRed;
    pixel[1] = cAtmosphere->backgroundGreen;
    pixel[2] = cAtmosphere->backgroundBlue;
}
void unFreezeParticles(int xCoord, int yCoord)
{
    /*
      int tempX, tempY;
      for (dx = -1; dx <= 1; dx++)
      {
      for (dy = -1; dy <= 1; dy++)
      {
      tempX = xCoord + dx;
      tempY = yCoord + dy;
      if (tempX < workWidth && tempX >= 0 && tempY < workHeight && tempY >= 0)
      {
      struct Particle* tempParticle = allCoords[getIndex(tempX, tempY)];

      if (tempParticle != -1)
      {
      //Unfreeze the particle
      tempParticle->frozen = FALSE;
      }
      }
      }
      }
    */
}

//Change a particle's heat and fix it
void changeHeat(unsigned char *heat, int heatChange)
{
    if (heatChange < 0)
    {
        if (*heat + heatChange < 0)
        {
            *heat = 0;
        }
        else
        {
            *heat += heatChange;
        }
    }
    else
    {
        if (*heat + heatChange > 255)
        {
            *heat = 255;
        }
        else
        {
            *heat += heatChange;
        }
    }
}

//Function to check if a particle has a given special
int hasSpecial(int tempParticle, int special)
{
    int* specials;
    int* start = specials = a_element[tempParticle]->specials;
    int* end = start + MAX_SPECIALS;
    for(; specials < end; ++specials)
    {
        if (*specials == special) return TRUE;
    }
    return FALSE;
}

//Gets a particle's special value, falling back to element special val if needed
//WARNING: Just gets the first special of that type it finds
int getParticleSpecialVal(int tempParticle, int special)
{
    int* specials;
    int* start = specials = a_element[tempParticle]->specials;
    int* end = start + MAX_SPECIALS;
    for (; specials < end; ++specials)
    {
        if (*specials == special)
        {
            return a_specialVals[tempParticle][specials-start];
        }
    }

    // Didn't find it in the particle, fall back to element
    return getElementSpecialVal(a_element[tempParticle], special);
}
//Sets a particle's special value
//WARNING: Just sets the first special of that type it finds
void setParticleSpecialVal(int tempParticle, int special, int val)
{
    int* specials;
    int* start = specials = a_element[tempParticle]->specials;
    int* end = start + MAX_SPECIALS;
    for (; specials < end; ++specials)
    {
        if (*specials == special)
        {
            a_specialVals[tempParticle][specials-start] = val;
        }
    }
}
//Gets an element's special value
//WARNING: Just gets the first special of that type it finds
int getElementSpecialVal(struct Element* tempElement, int special)
{
    int* specials;
    int* start = specials = tempElement->specials;
    int* end = start + MAX_SPECIALS;
    for (; specials < end; ++specials)
    {
        if (*specials == special)
        {
            return tempElement->specialVals[specials-start];
        }
    }

    return SPECIAL_VAL_UNSET;
}

// Clears all special vals to the element special val if any
void clearSpecialValsToElementDefault(int tempParticle)
{
    for (int i = 0 ; i < MAX_SPECIALS; ++i)
    {
        if (a_set[tempParticle] &&
        		// Burn and tunnel are treated specially, because they
                // use the element and particle special values simultaneously.
        		a_element[tempParticle]->specials[i] != SPECIAL_BURN &&
        		a_element[tempParticle]->specials[i] != SPECIAL_TUNNEL) {
            a_specialVals[tempParticle][i] = a_element[tempParticle]->specialVals[i];
        }
        else {
            a_specialVals[tempParticle][i] = SPECIAL_VAL_UNSET;
        }
    }
}
