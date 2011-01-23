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

	//Unset all the particles
	for(i = 0; i < MAX_POINTS; i++)
	{
		avail[i] = particles[i];
		particles[i]->set = FALSE;
	}

	//Clear allCoords and our pixels array
	for(i = 0; i < workWidth; i++)
	{
		for(j = 0; j < workHeight; j++)
		{
			allCoords[getIndex(j, i)] = NULL;
			colors[3 * (j * workHeight + i)] =blankRed;
			colors[3 * (j * workHeight + i) + 1] = blankGreen;
			colors[3 * (j * workHeight + i) + 2] = blankBlue;
		}
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
	free(allCoords);
	free(elements);
	free(particles);
	free(avail);

	//Allocate memory
	colors = malloc(3 * points * sizeof(char));

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
