/*
 * setup.h
 * -----------------------------
 * Contains the rsetup function, which initializes
 * all of the arrays and variables necessary.
 */

//Include the global macros
#include "macros.h"

//All the variables that need to get set
int loq;
int cpoint;

float x[TPoints];
float y[TPoints];
int xvel[TPoints];
int yvel[TPoints];
int element[TPoints];
int oldx[TPoints];
int oldy[TPoints];
int set[TPoints];
int avail[TPoints];
int spawn[TPoints];
char frozen[TPoints];

char username[8];
char password[8];
char userlength;
char passlength;

int allcoords[900][900];

unsigned char colors[TPixels * 3];
unsigned char red[TElements];
unsigned char green[TElements];
unsigned char blue[TElements];

void rsetup()
{
	int j, o, k;
	loq = TPoints;
	cpoint = 0;
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

	for (o = 0; o < 1024; o++)
	{
		for (k = 0; k < 512; k++)
		{
			allcoords[k][o] = -1; // -1 is empty
			colors[3 * (k + 512 * o)] = blankred; //0
			colors[3 * (k + 512 * o) + 1] = blankblue; //0
			colors[3 * (k + 512 * o) + 2] = blankgreen; //0
		}
	}
}
