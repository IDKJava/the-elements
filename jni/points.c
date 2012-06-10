/*
 * points.c
 * ----------------------------
 * Defines functions for manipulating data
 * for individual points, such as adding and
 * deleting points.
 */

#include "points.h"

static int dx;
static int dy;

void createPoint(int xCoord, int yCoord, struct Element* element)
{
	//If there are points available
	if (loq > 0)
	{
		int index;
		//Decrement number of points available
		loq--;
		//Get the pointer to the particle
		struct Particle* i = avail[loq];
		//Indicate that the particle is in use
		i->set = TRUE;
	
		//Set x and y
		i->x = xCoord;
		i->y = yCoord;

		//Put it in the allcoords array
		allCoords[getIndex(xCoord, yCoord)] = i;

		// Set the element of the point
		i->element = element;

		//Set the velocities
		i->xVel = 0;
		i->yVel = 0;

		//Set the frozen state
		i->frozen = FALSE;

		//Copy special vals from element if needed
		if (!element->useElementSpecialVals)
		{
			initSpecialVals(i, element);
		}
		for(index = 0; index < MAX_SPECIALS; index++)
		{
			i->specialVals[index] = i->element->specialVals[index];
		}

		//Set the initial heat
		if(element->startingTemp == 0)
		{
			//__android_log_write(ANDROID_LOG_INFO, "TheElements", "Atmosphere temp");
			i->heat = cAtmosphere->heat; //To be a variable later on
		}
		else
		{
			//__android_log_write(ANDROID_LOG_ERROR, "TheElements", "Element temp");
			i->heat = element->startingTemp;
		}

		//Set the point in the pixels array
		setBitmapColor(xCoord, yCoord, element);

		//Unfreeze particles around it
		//unFreezeParticles(xCoord, yCoord);
	}
}
void deletePoint(struct Particle* particle)
{
	//Store x and y in temporary variables for faster use
	int tempX = particle->x;
	int tempY = particle->y;

	//Unfreeze the particles around it
	//unFreezeParticles(tempX, tempY);
	//Clear it in the pixels array
	clearBitmapColor(tempX, tempY);
	//Clear it in the points array
	allCoords[getIndex(tempX, tempY)] = NULL;

	//Unset the particle
	particle->set = FALSE;
	//Add it to the avail array
	avail[loq] = particle;
	loq++;
}
void unSetPoint(struct Particle* particle)
{
	//Unset the particle
	particle->set = FALSE;
	//Add it to the avail array
	avail[loq] = particle;
	loq++;
}

void setElement(struct Particle* particle, struct Element* newElement)
{
	int i;
	particle->element = newElement;
	if (!newElement->useElementSpecialVals)
	{
		initSpecialVals(particle, newElement);
	}
	setBitmapColor(particle->x, particle->y, newElement);
}

void setBitmapColor(int xCoord, int yCoord, struct Element* element)
{
	colors[3 * getColorIndex(xCoord, yCoord)] = element->red;
	colors[3 * getColorIndex(xCoord, yCoord) + 1] = element->green;
	colors[3 * getColorIndex(xCoord, yCoord) + 2] = element->blue;
}
void clearBitmapColor(int xCoord, int yCoord)
{
	colors[3 * getColorIndex(xCoord, yCoord)] = cAtmosphere->backgroundRed;
	colors[3 * getColorIndex(xCoord, yCoord) + 1] = cAtmosphere->backgroundGreen;
	colors[3 * getColorIndex(xCoord, yCoord) + 2] = cAtmosphere->backgroundBlue;
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

				if (tempParticle != NULL)
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
char changeHeat(struct Particle* tempParticle, int heatChange)
{
	tempParticle->heat = fixHeat(tempParticle->heat + heatChange);
}

//Function to fix heat to [0,255]
char fixHeat(int heat)
{
	if (heat < 0)
	{
		heat = 0;
	}
	else if(heat > 255)
	{
		heat = 255;
	}

	return heat;
}

//Function to check if a particle has a given special
char hasSpecial(struct Particle* tempParticle, int special)
{
	int i;
	for(i = 0; i < MAX_SPECIALS; i++)
	{
		if (tempParticle->element->specials[i] == special) return TRUE;
	}
	return FALSE;
}

//Gets a particle's special value
char getSpecialVal(struct Particle* tempParticle, int special)
{
	struct Element* tempElement;
	tempElement = tempParticle->element;

	int i;
	for (i = 0; i < MAX_SPECIALS; i++)
	{
		if (tempElement->specials[i] == special)
		{
			if (tempElement->useElementSpecialVals)
			{
				return tempElement->specialVals[i];
			}
			else
			{
				return tempParticle->specialVals[i];
			}
		}
	}

	return FALSE;
}
//Sets a particle's special value
void setSpecialVal(struct Particle* tempParticle, int special, char val)
{
	int i;
	for (i = 0; i < MAX_SPECIALS; i++)
	{
		if (tempParticle->element->specials[i] == special)
		{
			tempParticle->specialVals[i] = val;
			return;
		}
	}
}

//Initialize a particle's special vals (only if not staying with the default forever)
void initSpecialVals(struct Particle* tempParticle, struct Element* tempElement)
{
	int i;
	for (i = 0; i < MAX_SPECIALS; i++)
	{
		tempParticle->specialVals[i] = tempElement->specialVals[i];
	}
}
