/*
 * points.c
 * ----------------------------
 * Defines functions for manipulating data
 * for individual points, such as adding and
 * deleting points.
 */

#include "points.h"
#include <android/log.h>
#include <stdio.h>

static int dx;
static int dy;

void createPoint(int xCoord, int yCoord, struct Element* element)
{
	//If there are point available
	if (loq > 0)
	{
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

		//Set the initial heat
		if(element->startingTemp == 0)
		{
			//__android_log_write(ANDROID_LOG_INFO, "TheElements", "Atmosphere temp");
			i->heat = ATMOSPHERE_TEMP; //To be a variable later on
		}
		else
		{
			//__android_log_write(ANDROID_LOG_ERROR, "TheElements", "Element temp");
			i->heat = element->startingTemp;
		}

		//Set the point in the pixels array
		setBitmapColor(xCoord, yCoord, element);

		//Unfreeze particles around it
		unFreezeParticles(xCoord, yCoord);
	}
}
void deletePoint(struct Particle* particle)
{
	//Store x and y in temporary variables for faster use
	int tempX = particle->x;
	int tempY = particle->y;

	//Unfreeze the particles around it
	unFreezeParticles(tempX, tempY);
	//Clear it in the pixels array
	setBitmapColor(tempX, tempY, elements[ERASER_ELEMENT]);
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
	particle->element = newElement;
	setBitmapColor(particle->x, particle->y, newElement);
}

void setBitmapColor(int xCoord, int yCoord, struct Element* element)
{
	colors[3 * getIndex(xCoord, yCoord)] = element->red;
	colors[3 * getIndex(xCoord, yCoord) + 1] = element->green;
	colors[3 * getIndex(xCoord, yCoord) + 2] = element->blue;
}
void createBitmapFromPoints(void)
{
	//Not implemented yet
}
void unFreezeParticles(int xCoord, int yCoord)
{
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
}
