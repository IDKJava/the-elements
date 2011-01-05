/*
 * points.c
 * ----------------------------
 * Defines functions for manipulating data
 * for individual points, such as adding and
 * deleting points.
 */

#include "points.h"

void CreatePoint(int xCoord, int yCoord, int elementVal)
{
	//Stores the index of the new point
	int i;
	if (loq > 0)
	{
		//Get the next available index off the stack
		loq--;
		i = avail[loq];
		avail[loq] = -1;

		//Set x and y
		x[i] = xCoord;
		y[i] = yCoord;

		//Put it in the allcoords array
		allcoords[xCoord][yCoord] = i;

		// Set the element of the point
		element[i] = e;

		//Set the velocities
		xvel[i] = 0;
		yvel[i] = 0;

		//Set the frozen state
		frozen[i] = FALSE;

		//Set the point in the pixels array
		setBitmapColor(xCoord, yCoord, elementVal);

		//Indicate that the index is in use
		set[i] = TRUE;

		//Unfreeze particles around it
		unFreezeParticles(xCoord, yCoord);
	}
}
void DeletePoint(int index)
{
	//Store x and y in temporary variables for faster use
	int tempX = x[index];
	int tempY = y[index];

	//Unfreeze the particles around it
	unFreezeParticles(tempX, tempY);
	//Clear it in the pixels array
	setBitmapColor((int) tempX, (int) tempY, 3);
	//Clear it in the points array
	allcoords[(int) tempX]][(int) tempY] = -1;
	//Unset it and add the index on the stack
	set[index] = FALSE;
	avail[loq] = index;
	loq++;
}

void setElement(int particle, int newElement)
{
	element[particle] = newElement;
	setBitmapColor(x[particle], y[particle], newElement);
}

void setBitmapColor(int xCoord, int yCoord, int newElement)
{
	colors[3 * (yCoord * workHeight + xCoord)] = red[newElement];
	colors[3 * (yCoord * workHeight + xCoord) + 1] = green[newElement];
	colors[3 * (yCoord * workHeight + xCoord) + 2] = blue[newElement];
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
				int tempIndex = allcoords[tempX][tempY];

				if (tempIndex != EMPTY)
				{
					//Unfreeze the particle
					frozen[atemp] = 0;
				}
			}
		}
	}
}
