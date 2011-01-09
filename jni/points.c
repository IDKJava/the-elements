/*
 * points.c
 * ----------------------------
 * Defines functions for manipulating data
 * for individual points, such as adding and
 * deleting points.
 */

#include "points.h"

void CreatePoint(int xCoord, int yCoord, Element element)
{
	//Stores the index of the new point
	Particle* i;
	if (usedPoints < MAX_POINTS)
	{
		//Set x and y
		i.x = xCoord;
		i.y = yCoord;

		//Put it in the allcoords array
		allcoords[xCoord][yCoord] = i;

		// Set the element of the point
		i.element = element;

		//Set the velocities
		i.xVel = 0;
		i.yVel = 0;

		//Set the frozen state
		i.frozen = FALSE;

		//Set the point in the pixels array
		setBitmapColor(xCoord, yCoord, element);

		//Unfreeze particles around it
		unFreezeParticles(xCoord, yCoord);

		//Incrememnt usedPoints
		usedPoints++;
	}
}
void DeletePoint(Particle particle)
{
	//Store x and y in temporary variables for faster use
	int tempX = particle.x;
	int tempY = particle.y;

	//Unfreeze the particles around it
	unFreezeParticles(tempX, tempY);
	//Clear it in the pixels array
	setBitmapColor((int) tempX, (int) tempY, 3);
	//Clear it in the points array
	allcoords[(int) tempX]][(int) tempY] = NULL;

	//Unallocate the memory
	free(part);

	//Decrement usedPoints
	usedPoints--;
}

void setElement(Particle particle, Element newElement)
{
	particle.element = newElement;
	setBitmapColor(particle.x, particle.y, newElement);
}

void setBitmapColor(int xCoord, int yCoord, Element element)
{
	colors[3 * (yCoord * workHeight + xCoord)] = element.red;
	colors[3 * (yCoord * workHeight + xCoord) + 1] = element.green;
	colors[3 * (yCoord * workHeight + xCoord) + 2] = element.blue;
}
void createBitmapFromPoints(void)
{
	//Not implemented yet
}
void unFreezeParticles(int xCoord, int yCoord)
{
	int dx;
	int dy;
	for (dx = -1; dx <= 1; dx++)
	{
		for (dy = -1; dy <= 1; dy++)
		{
			int tempX = xCoord + dx;
			int tempY = yCoord + dy;
			if (tempX < workWidth && tempX > 0 && tempY < workHeight && tempY > 0)
			{
				Particle* tempParticle = allcoords[tempX][tempY];

				if (tempParticle != NULL)
				{
					//Unfreeze the particle
					tempParticle.frozen = FALSE;
				}
			}
		}
	}
}
