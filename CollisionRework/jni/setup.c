/*
 * setup.c
 * -----------------------------
 * Defines the setup function, which initializes
 * all of the arrays and variables necessary.
 */

#include "setup.h"

void setup()
{
	int i, j;
	loq = MAX_POINTS;
	unsigned char blankRed = red[3];
	unsigned char blankGreen = green[3];
	unsigned char blankBlue = blue[3];

	for (i = 0; i < MAX_POINTS; i++)
	{
		set[i] = 0;
		avail[i] = i;
	}
	/* Network stuff not needed
	for (j = 0; j < 8; j++)
	{
		username[j] = 0;
		password[j] = 0;
	}
	userlength = 0;
	passlength = 0;
	*/

	for (i = 0; i < workHeight; i++)
	{
		for (j = 0; j < workWidth; j++)
		{
			//Clear the points array
			allcoords[getIndex(j, i)] = -1;
			//Clear the pixels array
			colors[3 * (i * workHeight + j)] = blankRed;
			colors[3 * (i * workHeight + j) + 1] = blankGreen;
			colors[3 * (i * workHeight + j) + 2] = blankBlue;
		}
	}
}

//Set up all the variable sized arrays
void arraySetup()
{
	//Variables for special size and special value size, because these are variable-sized multidimensional arrays
	
	//Calculate the number of pixels
	int points = workWidth * workHeight;

	//TODO: Load custom elements
	//Calculate numElements
	//Calculate special size
	//Calculate special value size
	numElements = NUM_BASE_ELEMENTS; //Changed later

	//Make sure everything is deallocated
	free(colors);
	free(red);
	free(green);
	free(blue);
	free(fallVel);
	free(density);
	free(state);
	free(special);
	free(specialVal);
	free(drawSolid);
	free(inertia);
	free(allCoords);

	//Allocate memory
	colors = malloc(3 * points * sizeof(char));
	red = malloc(points * sizeof(char));
	green = malloc(points * sizeof(char));
	blue = malloc(points * sizeof(char));

	elements = malloc(numElements * sizeof(struct Element*));
	particles = malloc(points * sizeof(struct Particle*));
	avail = malloc(points * sizeof(struct Particle*));

	allCoords = malloc(workWidth * workHeight * zoom * sizeof(struct Particle*)); //Two dimensional array, so when calling use allcoords[getIndex(x, y)];
	int i;
	for(i = 0; i < numElements; i++)
	{
		if(i < NUM_BASE_ELEMENTS)
		{
			elements[i] = (struct Element*) malloc(sizeof(struct Element));
			elements[i]->red = baseRed[i];
			elements[i]->green = baseGreen[i];
			elements[i]->blue = baseBlue[i];
			elements[i]->fallVel = baseFallVel[i];
			elements[i]->density = baseDensity[i];
			elements[i]->state = baseState[i];
			elements[i]->specials = baseSpecial[i];
			elements[i]->specialVals = (char*) malloc(2 * sizeof(char));
				elements[i]->specialVals[0] = baseSpecialValue[i][0];
				elements[i]->specialVals[1] = baseSpecialValue[i][1];
			elements[i]->inertia = baseInertia[i];
			elements[i]->highestTemp = baseHighestTemp[i];
			elements[i]->lowestTemp = baseLowestTemp[i];
		}
		else
		{
			//TODO: Gotta load the thing from the array, allocate the memory for it, and store the pointer here
		}
	}

	//TODO: Higher/lower element pointers
}
