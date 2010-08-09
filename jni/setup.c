/*
 * setup.c
 * -----------------------------
 * Defines the rsetup function, which initializes
 * all of the arrays and variables necessary.
 */

#include "setup.h"

void rsetup()
{
	int j, o, k;
	loq = TPoints;
	cpoint = 0;
	play = 1;
	size = 4;
	unsigned char blankred = red[3];
	unsigned char blankgreen = green[3];
	unsigned char blankblue = blue[3];

	for (j = 0; j < TPoints; j++)
	{
		x[j] = 0;
		y[j] = 0;
		xvel[j] = 0;
		yvel[j] = 0;
		element[j] = 0;
		oldx[j] = 0;
		oldy[j] = 0;
		set[j] = 0;
		avail[j] = j;
		spawn[j] = -1;
		frozen[j] = 0;
	}
	for (j = 0; j < 8; j++)
	{
		username[j] = 0;
		password[j] = 0;
	}
	userlength = 0;
	passlength = 0;

	for (o = 0; o < HEIGHT; o++)
	{
		for (k = 0; k < WIDTH; k++)
		{
			allcoords[k][o] = -1; // -1 is empty
			colors[3 * (k + WIDTH * o)] = blankred; //0
			colors[3 * (k + WIDTH * o) + 1] = blankblue; //0
			colors[3 * (k + WIDTH * o) + 2] = blankgreen; //0
		}
	}
}
