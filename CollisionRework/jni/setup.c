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
	//Calculate the number of pixels
	int points = workWidth * workHeight;

	//TODO: Load custom elements
	//Calculate numElements
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
	free(drawSolid);
	free(allCoords);

	//Allocate memory
	colors = malloc(3 * points * sizeof(char));
	red = malloc(points * sizeof(char));
	green = malloc(points * sizeof(char));
	blue = malloc(points * sizeof(char));

	fallVel = malloc(numElements * sizeof(int));
	density = malloc(numElements * sizeof(int));
	state = malloc(numElements * sizeof(int));
	special = malloc(numElements * sizeof(int));
	drawSolid = malloc(numElements * sizeof(int));

	allCoords = malloc(workWidth * workHeight * zoom * sizeof(int)); //Two dimensional array, so when calling use allcoords[getIndex(x, y)];
}
