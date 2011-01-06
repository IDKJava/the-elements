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
	//Clear
	if (shouldClear)
	{
		//Clear and unset the flag
		setup();
		shouldClear = FALSE;
		
		//No need to do the rest of the update
		return;
	}
	//Draw points
	if (fingerState == FINGER_DOWN)
	{
		if (mouseY != 0) //Not sure why this is here...
		{
			int dx, dy;
			for (dy = size; dy >= -size; dy--)
			{
				for (dx = -size; dx <= size; dx++)
				{
					if (TRUE) //Taken out for drawing optimization (dx * dx) + (dy * dy) <= (size * size))
					{
						//Normal drawing
						if (cElement >= 0)
						{
							//Draw it solid
							if(inertia[cElement] == INERTIA_UNMOVABLE)
							{
								if (dx + xm < maxx && dx + xm > 0 && dy + ym < maxy && dy + ym > 0 && allCoords[(int) (dx + xm)][(int) (dy + ym)] == -1)
								{
									CreatePoint(xm + dx, ym + dy, celement);
								}
							}
							//Draw it randomized
							else
							{
								if (rand() % 3 == 1 && dx + xm < maxx && dx + xm > 0 && dy + ym < maxy && dy + ym > 0 && allCoords[(int) (dx + xm)][(int) (dy + ym)] == -1)
								{
									CreatePoint(xm + dx, ym + dy, celement);
								}
							}
						}
						//Special Drag case
						else if (cElement == DRAG_ELEMENT)
						{
							if (allCoords[lmx + dx][lmy + dy] != -1 && fallvel[element[allCoords[lmx + dx][lmy + dy]]] != 0 && dx + lmx < maxx && dx + lmx > 0 && dy + lmy < maxy && dy + lmy > 0)
							{
								xvel[allCoords[lmx + dx][lmy + dy]] += (xm - lmx);
								yvel[allCoords[lmx + dx][lmy + dy]] += (ym - lmy);
							}
						}
						//Special Eraser case
						else if (cElement == ERASER_ELEMENT)
						{
							if (allCoords[(int) (dx + xm)][(int) (dy + ym)] != -1 && dx + xm < maxx && dx + xm > 0 && dy + ym < maxy && dy + ym > 0)
							{
								DeletePoint(allCoords[xm + dx][ym + dy]);
							}
						}
					}
				}
			}
		}
	}

	//Particles update
	if (play)
	{
		int counter;
		int rtop; //used to prevent bugs when fire reaches the top

		int tempX, tempT, tempOldX, tempOldY, tempInertia, tempallCoords, tempElement, tempElement2; //For speed we're going to create temp variables to store stuff

		//Physics update
		for (counter = 0; counter < MAX_POINTS; counter++)
		{
			//If the particle is set and unfrozen
			if (set[counter] && frozen[counter] < 4)
			{
				//TODO: Life property cycle

				//Set the temp and old variables
				tempOldX = (int) x[counter];
				tempOldY = (int) y[counter];
				oldX[counter] = tempOldX;
				oldY[counter] = tempOldY;
				tempElement = element[counter];
				tempInertia = inertia[tempElement];

				//Update coords
				if(tempInertia != INERTIA_UNMOVABLE)
				{
					x[counter] += xVel[counter];
					y[counter] += fallVel[tempElement] + yVel[counter];
				}

				//Reduce velocities
				if(xVel < 0)
				{
					if(tempInertia >= -xVel)
					{
						xVel = 0;
					}
					else
					{
						xVel += tempInertia;
					}
				}
				else if(xVel > 0)
				{
					if(tempInertia >= xVel)
					{
						xVel = 0;
					}
					else
					{
						xVel -= tempInertia;
					}
				}
				if(yVel < 0)
				{
					if(tempInertia >= -yVel)
					{
						yVel = 0;
					}
					else
					{
						yVel += tempInertia;
					}
				}
				else if(yVel > 0)
				{
					if(tempInertia >= yVel)
					{
						yVel = 0;
					}
					else
					{
						yVel -= tempInertia;
					}
				}

				/* Acclerometer stuff taken out for now
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
				*/



				//Boundary checking
				if ((int) x[counter] >= workWidth || (int) x[counter] <= 0)
				{
					x[counter] = oldX;
					xvel[counter] = 0;
				}
				if ((int) y[counter] >= workHeight || (int) y[counter] <= 0)
				{
					y[counter] = oldY;
					yvel[counter] = 0;
				}

				//Set other temp variables
				tempX = (int) x[counter];
				tempY = (int) y[counter];

				//Special cycle
				if(special[tempElement] != SPECIAL_NOT_SET)
				{
					switch(special[tempElement])
					{
						//Stuff to come
					}
				}

				/*
				if (fireburn[element[counter]] == 1) //Fire cycle
				{
					frozen[counter] = 0;
					random = rand();
					if (collision[element[allCoords[tempX + 1][tempY]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allCoords[tempX + 1][tempY], 5);
					}
					random = rand();
					if (collision[element[allCoords[tempX][tempY - 1]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allCoords[tempX][tempY - 1], 5);
					}
					random = rand();
					if (collision[element[allCoords[tempX - 1][tempY]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allCoords[tempX - 1][tempY], 5);
					}
					random = rand();
					if (collision[element[allCoords[tempX][tempY + 1]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allCoords[tempX][tempY + 1], 5);
						yvel[allCoords[tempX][tempY + 1]] = 2;
					}
					random = rand();
					if (collision[element[allCoords[tempX + 1][tempY + 1]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allCoords[tempX + 1][tempY + 1], 5);
						yvel[allCoords[tempX][tempY + 1]] = 2;
					}
					random = rand();
					if (collision[element[allCoords[tempX - 1][tempY + 1]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allCoords[tempX - 1][tempY + 1], 5);
						yvel[allCoords[tempX][tempY + 1]] = 2;
					}
					random = rand();
					if (collision[element[allCoords[tempX + 1][tempY - 1]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allCoords[tempX + 1][tempY - 1], 5);
					}
					random = rand();
					if (collision[element[allCoords[tempX - 1][tempY - 1]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allCoords[tempX - 1][tempY - 1], 5);
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
							if (tempX + check1 > 1 && tempX + check1 < maxx && tempY + check2 >= 0 && tempY + check2 < maxy)
							{
								temp = allCoords[tempX + check1][tempY + check2];
								if (temp != -1 && element[temp] == 7) //There's a generator adjacent
								{
									element[temp] = 8; //Change the generator to a spawn
									spawn[temp] = spawn[counter]; //Make the spawn element the same as this one
								}
								else if (temp == -1 && rand() % 200 == 0 && loq < TPoints - 1) //There's an empty spot
								{
									CreatePoint(tempX + check1, tempY + check2, spawn[counter]); //1/200 chance of spawning
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
							temp = allCoords[tempX + check1][tempY + check2];
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
				*/

				//Updating allCoords and collision resolution
				tempallCoords = allCoords[getIndex(tempX, tempY)];

				//If the space the particle is trying to move to is taken and isn't itself
				if (tempallCoords != EMPTY && tempallCoords != counter)
				{
					tempElement2 = element[tempallCoords];

					//Resolve (tempallCoords != counter) the collision
					collide(counter, tempallCoords);

					//If nothing has changed
					if (x[counter] == oldX
							&& y[counter] == oldY
							&& element[counter] == tempElement
							&& xvel[counter] == 0
							&& yvel[counter] == 0)
					{
						frozen[counter]++; //Increment the frozen count
					}
					else
					{
						//Unfreeze stuff around the primary particle because stuff has changed with it
						unFreezeParticles(oldX, oldY);
					}
				}
				else if (tempallCoords != counter)//Space particle is trying to move to is free
				{
					//Clear the old spot
					allCoords[oldX][oldY] = EMPTY;
					setBitmapColor(oldX, oldY, ERASER_ELEMENT);

					//Unfreeze particles around old spot
					unFreezeParticles(oldX, oldY);
					//Possibly unfreeze particles around new spot?
					//unFreezeParticles(tempX, tempY);

					//Set new spot
					allCoords[tempX][tempY] = counter;
					setBitmapColor(tempX, tempY, element[counter]);
				}
			}
		}
	}
}
