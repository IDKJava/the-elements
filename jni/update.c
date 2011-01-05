/*
 * update.c
 * -----------------------------------
 * Defines the function UpdateView(), which
 * is called every frame to update all the
 * particles' positions.
 */

#include "update.h"

void UpdateView(void)
{
	if (shouldClear)
	{
		setup();
		shouldClear = FALSE;
	}
	//Draw points
	if (fingerState == FINGER_DOWN)
	{
		if (ym != 0)
		{
			int yc;
			int xc;
			for (yc = size; yc >= -size; yc--)
			{
				for (xc = -size; xc <= size; xc++)
				{
					if ((xc * xc) + (yc * yc) <= (size * size))
					{
						//Draw randomized for anything other than non-moving, Eraser, and Drag
						if (!solid[celement] && celement != 16 && celement!= 3)
						{
							if (rand() % 3 == 1 && xc + xm < maxx && xc + xm > 0 && yc + ym < maxy && yc + ym > 0 && allcoords[(int) (xc + xm)][(int) (yc + ym)] == -1)
							{
								CreatePoint(xm + xc, ym + yc, celement);
							}
						}
						//Draw fully for anything non-moving
						else if (solid[celement])
						{
							if (xc + xm < maxx && xc + xm > 0 && yc + ym < maxy && yc + ym > 0 && allcoords[(int) (xc + xm)][(int) (yc + ym)] == -1)
							{
								CreatePoint(xm + xc, ym + yc, celement);
							}
						}
						//Special Drag case
						else if (celement == 16)
						{
							if (allcoords[lmx + xc][lmy + yc] != -1 && fallvel[element[allcoords[lmx + xc][lmy + yc]]] != 0 && xc + lmx < maxx && xc + lmx > 0 && yc + lmy < maxy && yc + lmy > 0)
							{
								xvel[allcoords[lmx + xc][lmy + yc]] += (xm - lmx);
								yvel[allcoords[lmx + xc][lmy + yc]] += (ym - lmy);
							}
						}
						//Special Eraser case
						else
						{
							if (allcoords[(int) (xc + xm)][(int) (yc + ym)] != -1 && xc + xm < maxx && xc + xm > 0 && yc + ym < maxy && yc + ym > 0)
							{
								DeletePoint(allcoords[xm + xc][ym + yc]);
							}
						}
					}
				}
			}
		}

	}

	if (play == TRUE)
	{
		int counter;
		int rtop; //used to prevent bugs when fire reaches the top

		int tempx, tempy, ox, oy; //For speed we're going to create temp variables to store stuff
		int oelement; //these are also used to check to see if the element has changed to do stuff about freezing particles

		// Move the particles and do collisions
		for (counter = 0; counter < TPoints; counter++)
		{
			//If the particle is set and unfrozen
			if (set[counter] && frozen[counter] < 4)
			{
				//Random delete of fire
				int random = rand();
				if (element[counter] == 5 && ((random % 7) == 0))
				{
					DeletePoint(counter);
					continue;
				}

				//Set the temp and old variables
				oy = (int) y[counter];
				ox = (int) x[counter];
				oldy[counter] = oy;
				oldx[counter] = ox;
				oelement = element[counter];

				//If accel control, do yvel based on that
				if ((int) gravy != 0 && accelcon)
				{
					y[counter] += ((gravy / 9.8) * fallvel[oelement] + yvel[counter]);
				}
				//Otherwise, just do the fallvelocity
				else if (accelcon == 0)
				{
					y[counter] += fallvel[oelement] + yvel[counter];
				}
				//Accelerometer control, but no gravity (held horizontal)
				else
				{
					y[counter] += yvel[counter];
				}

				//If accel control is on, calculate new x using the gravity set
				if ((int) gravx != 0 && accelcon == 1)
				{
					x[counter] += ((gravx / 9.8) * fallvel[oelement] + xvel[counter]);
				}
				//Otherwise, just add xvel
				else
				{
					x[counter] += xvel[counter];
				}


				if (xvel[counter] > 0)
				{
					//Check for breaking
					if ((oelement == 15 || oelement == 21) && xvel[counter] > 5)
					{
						element[counter] = 0; //change particle to sand if the velocity on the wall is great enough
						setBitmapColor((int) x[counter], (int) y[counter], 0); //Set the color also
					}
					else if (oelement == 15 || oelement == 21)
					{
						x[counter] = ox;
						y[counter] = oy;
						xvel[counter] = 0;
					}
					else
					{
						//Default case: reduce velocity
						xvel[counter] -= 1;
					}
				}
				else if (xvel[counter] < 0)
				{
					//Check for breaking
					if ((oelement == 15 || oelement == 21) && xvel[counter] < -5)
					{
						element[counter] = 0; //change particle to sand if the velocity on the wall is great enough
						setBitmapColor((int) x[counter], (int) y[counter] ,0); //Set the color also
					}
					else if (oelement == 15 || oelement == 21)
					{
						x[counter] = ox;
						y[counter] = oy;
						xvel[counter] = 0;
					}
					else
					{
						//Default case: decrease speed by one
						xvel[counter] += 1;
					}
				}

				if (yvel[counter] > 0)
				{
					//Check for breaking
					if ((oelement == 15 || oelement  == 21) && yvel[counter] > 5)
					{
						element[counter] = 0; //change particle to sand if the velocity on the wall is great enough
						setBitmapColor((int) x[counter], (int) y[counter], 0); //Set the color also
					}
					else if (oelement == 15 || oelement == 21)
					{
						x[counter] = ox;
						y[counter] = oy;
						yvel[counter] = 0;
					}
					else
					{
						//Default case: decrease speed by 1
						yvel[counter] -= 1;
					}
				}
				else if (yvel[counter] < 0)
				{
					//Check for breaking
					if ((oelement == 15 || oelement == 21) && yvel[counter] < -5)
					{
						element[counter] = 0; //change particle to sand if the velocity on the wall is great enough
						setBitmapColor((int) x[counter], (int) y[counter], 0); //Set the color also
					}
					else if (oelement == 15 || oelement == 21)
					{
						x[counter] = ox;
						y[counter] = oy;
						yvel[counter] = 0;
					}
					else
					{
						//Default case: decrease speed by 1
						yvel[counter] += 1;
					}
				}

				//Boundary checking
				if ((int) y[counter] >= maxy)
				{
					y[counter] = oy;
					yvel[counter] = 0;
					xvel[counter] = 0;
				}
				if ((int) x[counter] >= maxx || (int) x[counter] <= 0)
				{
					x[counter] = ox;
					xvel[counter] = 0;
					yvel[counter] = 0;
				}
				if ((int) y[counter] <= 0) //If the particle is above the top of the screen
				{
					if (element[counter] == 5) //If it's fire
					{
						//Move it back
						y[counter] = oy;
						x[counter] = ox;
						//Delete it
						DeletePoint(counter);
						continue;
					}
					else //Not fire
					{
						//Just bounce it back and kill velocity
						y[counter] = oy;
						yvel[counter] = 0;
						xvel[counter] = 0;
						if (element[counter] == 18) //If it's steam
						{
							xvel[counter] = rand() % 3 - 1; //Add a random velocity
						}
					}
				}

				//Set other temp variables for next section
				tempx = (int) x[counter];
				tempy = (int) y[counter];

				//Special cycles
				if (fireburn[element[counter]] == 1) //Fire cycle
				{
					frozen[counter] = 0;
					random = rand();
					if (collision[element[allcoords[tempx + 1][tempy]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allcoords[tempx + 1][tempy], 5);
					}
					random = rand();
					if (collision[element[allcoords[tempx][tempy - 1]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allcoords[tempx][tempy - 1], 5);
					}
					random = rand();
					if (collision[element[allcoords[tempx - 1][tempy]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allcoords[tempx - 1][tempy], 5);
					}
					random = rand();
					if (collision[element[allcoords[tempx][tempy + 1]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allcoords[tempx][tempy + 1], 5);
						yvel[allcoords[tempx][tempy + 1]] = 2;
					}
					random = rand();
					if (collision[element[allcoords[tempx + 1][tempy + 1]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allcoords[tempx + 1][tempy + 1], 5);
						yvel[allcoords[tempx][tempy + 1]] = 2;
					}
					random = rand();
					if (collision[element[allcoords[tempx - 1][tempy + 1]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allcoords[tempx - 1][tempy + 1], 5);
						yvel[allcoords[tempx][tempy + 1]] = 2;
					}
					random = rand();
					if (collision[element[allcoords[tempx + 1][tempy - 1]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allcoords[tempx + 1][tempy - 1], 5);
					}
					random = rand();
					if (collision[element[allcoords[tempx - 1][tempy - 1]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allcoords[tempx - 1][tempy - 1], 5);
					}
				}
				if (element[counter] == 8) //Spawn cycle
				{
					frozen[counter] = 0;
					int check1, check2, temp;
					for (check1 = -2; check1 <= 2; check1++)
					{
						for (check2 = -2; check2 <= 2; check2++)
						{
							if (tempx + check1 > 1 && tempx + check1 < maxx && tempy + check2 >= 0 && tempy + check2 < maxy)
							{
								temp = allcoords[tempx + check1][tempy + check2];
								if (temp != -1 && element[temp] == 7) //There's a generator adjacent
								{
									element[temp] = 8; //Change the generator to a spawn
									spawn[temp] = spawn[counter]; //Make the spawn element the same as this one
								}
								else if (temp == -1 && rand() % 200 == 0 && loq < TPoints - 1) //There's an empty spot
								{
									CreatePoint(tempx + check1, tempy + check2, spawn[counter]); //1/200 chance of spawning
								}
							}
						}
					}
				}
				if (growing[element[counter]] == 1) //Ice cycle
				{
					frozen[counter] = 0;
					int check1, check2, temp;
					for (check1 = -1; check1 <= 1; check1++)
					{
						for (check2 = -1; check2 <= 1; check2++)
						{
							temp = allcoords[tempx + check1][tempy + check2];
							if (temp != -1 && element[temp] == 1 && rand() % 10 == 0)
							{
								//Change the water to ice
								setElement(temp, element[counter]);
								frozen[temp] = 0;
							}
						}
					}
				}
				if (condensing[element[counter]] != -1) //Steam cycle
				{
					frozen[counter] = 0;
					if (rand() % 200 == 0) //1/200 chance
					{
						setElement(counter, condensing[element[counter]]); //"Condense" the steam
					}
				}

				//Updating allcoords and collision resolution
				int atemporary = allcoords[tempx][tempy];

				//If the space the particle is trying to move to is taken
				if (atemporary != -1 && atemporary != counter)
				{
					int secondElementTemp = element[atemporary];

					collide(counter, atemporary); //Call collision() on the two particles

					//If nothing has changed
					if (x[counter] == ox
							&& y[counter] == oy
							&& element[counter] == oelement
							&& x[atemporary] == tempx
							&& y[atemporary] == tempy
							&& element[atemporary] == secondElementTemp
							&& xvel[counter] == 0
							&& yvel[counter] == 0)
					{
						frozen[counter]++; //Increment the frozen count
					}
					else
					{
						if (x[counter] != ox || y[counter] != oy || element[counter] != oelement)
						{
							//unFreeze stuff around the primary particle because stuff has changed with it
							unFreezeParticles(ox, oy);
						}
						if (x[atemporary] != tempx || y[atemporary] != tempy || element[atemporary] != secondElementTemp)
						{
							//unFreeze stuff around the secondary particle because stuff has changed with it
							unFreezeParticles(tempx, tempy);
						}
					}
				}
				else //Space particle is trying to move to is free or is itself
				{
					//Space particle is trying to move to is free
					if (atemporary != counter)
					{
						//Clear the old spot
						allcoords[ox][oy] = -1;
						setBitmapColor(ox, oy, 3);

						//Unfreeze particles around old spot
						unFreezeParticles(ox, oy);

						//Set new spot
						allcoords[tempx][tempy] = counter;
						setBitmapColor(tempx, tempy, element[counter]);
					}
				}
			}
		}
	}
}
