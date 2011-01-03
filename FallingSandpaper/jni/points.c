/*
 * points.c
 * ----------------------------
 * Defines functions for manipulating data
 * for individual points, such as adding and
 * deleting points.
 */

#include "points.h"

void CreatePoint(int k, int l, int e)
{
	int i;
	if (loq > 0)
	{
		loq--;
		i = avail[loq];
		avail[loq] = -1;
		x[i] = k;
		y[i] = l;

		allcoords[k][l] = i;

		// Set the element of the point
		element[i] = e;

		//velocity setting part - all start at 0

		xvel[i] = 0;
		yvel[i] = 0;
		frozen[i] = 0;

		setBitmapColor(k, l, e);

		set[i] = 1;

		unFreezeParticles(k, l);
	}
}
void DeletePoint(int partnum)
{
	int tempx = x[partnum];
	int tempy = y[partnum];
	unFreezeParticles(tempx, tempy);
	setBitmapColor((int) x[partnum], (int) y[partnum], 3);
	allcoords[(int) x[partnum]][(int) y[partnum]] = -1;
	//cleaning up
	x[partnum] = 0;
	y[partnum] = 0;
	frozen[partnum] = 0;
	element[partnum] = 0;
	xvel[partnum] = 0;
	yvel[partnum] = 0;
	set[partnum] = 0;
	avail[loq] = partnum;
	loq++;

}

void setBitmapColor(int xpos, int ypos, int element)
{
	colors[3 * (xpos + ypos * 512)] = red[element];
	colors[3 * (xpos + ypos * 512) + 1] = green[element];
	colors[3 * (xpos + ypos * 512) + 2] = blue[element];
}

//this function unfreezes particles around a point
void unFreezeParticles(int xcentre, int ycentre)
{
	int ix;
	int jy;
	for (ix = -1; ix <= 1; ix++)
	{
		for (jy = -1; jy <= 1; jy++)
		{

			int tempx = xcentre + ix;
			int tempy = ycentre + jy;
			if (tempx < maxx && tempx > 0 && tempy < maxy && tempy > 0)
			{

				int atemp = allcoords[tempx][tempy];

				if (atemp != -1)
				{
					frozen[atemp] = 0; //reset the freeze counter
				}
			}

		}
	}
}
