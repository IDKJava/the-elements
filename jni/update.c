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

	if (fd == 1) // if the finger is down
	{
		// If the mouse is above the menu, there are not too many points, and there isn't already sand there, then make a piece of sand
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

						if (solid[celement] != 1 && celement != 16 && celement
								!= 3) //not wall, eraser, or plant, wind or fuse
						{
							if (xc + xm < maxx && xc + xm > 0 && yc + ym < maxy
									&& yc + ym > 0
									&& allcoords[(int) (xc + xm)][(int) (yc
											+ ym)] == -1 && rand() % 3 == 1)
							{
								CreatePoint(xm + xc, ym + yc, celement);
							}
						}
						else if (solid[celement] == 1) //wall or plant or fuse should be drawn solid
						{
							if (xc + xm < maxx && xc + xm > 0 && yc + ym < maxy
									&& yc + ym > 0
									&& allcoords[(int) (xc + xm)][(int) (yc
											+ ym)] == -1)
							{
								CreatePoint(xm + xc, ym + yc, celement);
							}
						}
						else if (celement == 16) //wind
						{
							if (xc + lmx < maxx && xc + lmx > 0 && yc + lmy
									< maxy && yc + lmy > 0)
							{
								if (allcoords[lmx + xc][lmy + yc] != -1)
								{
									if (fallvel[element[allcoords[lmx + xc][lmy
											+ yc]]] != 0)
									{
										xvel[allcoords[lmx + xc][lmy + yc]]
												+= (xm - lmx);
										yvel[allcoords[lmx + xc][lmy + yc]]
												+= (ym - lmy);
									}
								}
							}
						}
						else
						{ //eraser

							if (xc + xm < maxx && xc + xm > 0 && yc + ym < maxy
									&& yc + ym > 0
									&& allcoords[(int) (xc + xm)][(int) (yc
											+ ym)] != -1)
							{

								DeletePoint(allcoords[xm + xc][ym + yc]);
							}

						}
					}
				}
			}
		}

	}

	if (play == 1)
	{
		int counter;
		int rtop; //used to prevent bugs when fire reaches the top

		int tempx, tempy, ox, oy; //For speed we're going to create temp variables to store stuff
		int oelement; //these are also used to check to see if the element has changed to do stuff about freezing particles

		//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "0");

		// Move the particles and do collisions
		for (counter = 0; counter < TPoints; counter++)
		{
			if (set[counter] == 1 && frozen[counter] < 4)
			{
				//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "1");


				int random = rand();
				if (element[counter] == 5 && ((random % 7) == 0))
				{
					DeletePoint(counter);
				}
				else
				{
					oy = (int) y[counter];
					ox = (int) x[counter];
					oldy[counter] = oy;
					oldx[counter] = ox;
					oelement = element[counter];
					if ((int) gravy != 0 && accelcon == 1)
					{
						y[counter] += ((gravy / 9.8) * fallvel[oelement]
								+ yvel[counter]);
					}
					else if (accelcon == 0)
					{ // no accelerometer control still needs to have stuff fall
						y[counter] += fallvel[oelement] + yvel[counter];
					}
					else
					{
						y[counter] += yvel[counter]; //with accel tho, it shouldn't
					}
					if ((int) gravx != 0 && accelcon == 1)
					{
						x[counter] += ((gravx / 9.8) * fallvel[oelement]
								+ xvel[counter]);
					}
					else
					{
						x[counter] += xvel[counter];
					}
					if (xvel[counter] > 0)
					{
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
							xvel[counter] -= 1;
						}
					}
					//element could have changed so don't use olement anymore
					else if (xvel[counter] < 0)
					{
						if ((element[counter] == 15 || element[counter] == 21)
								&& xvel[counter] < -5)
						{
							element[counter] = 0; //change particle to sand if the velocity on the wall is great enough							
							setBitmapColor((int) x[counter], (int) y[counter] ,0); //Set the color also
						}
						else if (element[counter] == 15 || element[counter] == 21)
						{
							x[counter] = ox;
							y[counter] = oy;
							xvel[counter] = 0;
						}
						else
						{
							xvel[counter] += 1;
						}
					}
					if (yvel[counter] > 0)
					{
						if ((element[counter] == 15 || element[counter] == 21)
								&& yvel[counter] > 5)
						{
							element[counter] = 0; //change particle to sand if the velocity on the wall is great enough							
							setBitmapColor((int) x[counter], (int) y[counter], 0); //Set the color also
						}
						else if (element[counter] == 15 || element[counter]
								== 21)
						{
							x[counter] = ox;
							y[counter] = oy;
							yvel[counter] = 0;
						}
						else
						{
							yvel[counter] -= 1;
						}
					}
					else if (yvel[counter] < 0)
					{
						if ((element[counter] == 15 || element[counter] == 21)
								&& yvel[counter] < -5)
						{
							element[counter] = 0; //change particle to sand if the velocity on the wall is great enough							
							setBitmapColor((int) x[counter], (int) y[counter],
									0); //Set the color also
						}
						else if (element[counter] == 15 || element[counter]
								== 21)
						{
							x[counter] = ox;
							y[counter] = oy;
							yvel[counter] = 0;
						}
						else
						{
							yvel[counter] += 1;
						}
					}
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
							//Delete it
							x[counter] = 0;
							y[counter] = 0;
							element[counter] = 0;
							xvel[counter] = 0;
							yvel[counter] = 0;
							set[counter] = 0;
							avail[loq] = counter;
							loq++;
							setBitmapColor(ox, oy, 3);
							allcoords[ox][oy] = -1;
							rtop = 1;
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
					if (rtop == 0)
					{
						tempx = (int) x[counter];
						tempy = (int) y[counter];

						if (fireburn[element[counter]] == 1) //Fire cycle
						{
							frozen[counter] = 0;
							random = rand();
							if (collision[element[allcoords[tempx + 1][tempy]]][element[counter]]
									== 6 && random % 3 != 0)
							{
								element[allcoords[tempx + 1][tempy]] = 5;
								setBitmapColor(tempx + 1, tempy, 5);
							}
							random = rand();
							if (collision[element[allcoords[tempx][tempy - 1]]][element[counter]]
									== 6 && random % 3 != 0)
							{
								element[allcoords[tempx][tempy - 1]] = 5;
								setBitmapColor(tempx, tempy - 1, 5);
							}
							random = rand();
							if (collision[element[allcoords[tempx - 1][tempy]]][element[counter]]
									== 6 && random % 3 != 0)
							{
								element[allcoords[tempx - 1][tempy]] = 5;
								setBitmapColor(tempx - 1, tempy, 5);
							}
							random = rand();
							if (collision[element[allcoords[tempx][tempy + 1]]][element[counter]]
									== 6 && random % 3 != 0)
							{
								element[allcoords[tempx][tempy + 1]] = 5;
								setBitmapColor(tempx, tempy + 1, 5);
								yvel[allcoords[tempx][tempy + 1]] = 2;
							}
							random = rand();
							if (collision[element[allcoords[tempx + 1][tempy
									+ 1]]][element[counter]] == 6 && random % 3
									!= 0)
							{
								element[allcoords[tempx + 1][tempy + 1]] = 5;
								setBitmapColor(tempx + 1, tempy + 1, 5);
								yvel[allcoords[tempx][tempy + 1]] = 2;
							}
							random = rand();
							if (collision[element[allcoords[tempx - 1][tempy
									+ 1]]][element[counter]] == 6 && random % 3
									!= 0)
							{
								element[allcoords[tempx - 1][tempy + 1]] = 5;
								setBitmapColor(tempx - 1, tempy + 1, 5);
								yvel[allcoords[tempx][tempy + 1]] = 2;
							}
							random = rand();
							if (collision[element[allcoords[tempx + 1][tempy
									- 1]]][element[counter]] == 6 && random % 3
									!= 0)
							{
								element[allcoords[tempx + 1][tempy - 1]] = 5;
								setBitmapColor(tempx + 1, tempy - 1, 5);
							}
							random = rand();
							if (collision[element[allcoords[tempx - 1][tempy
									- 1]]][element[counter]] == 6 && random % 3
									!= 0)
							{
								element[allcoords[tempx - 1][tempy - 1]] = 5;
								setBitmapColor(tempx - 1, tempy - 1, 5);
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
									if (tempx + check1 > 1 && tempx + check1
											< maxx && tempy + check2 >= 0
											&& tempy + check2 < maxy)
									{
										temp = allcoords[tempx + check1][tempy
												+ check2];
										if (temp != -1 && element[temp] == 7) //There's a generator adjacent
										{
											element[temp] = 8; //Change the generator to a spawn
											spawn[temp] = spawn[counter]; //Make the spawn element the same as this one
										}
										else if (temp == -1 && rand() % 200
												== 0 && loq < TPoints - 1) //There's an empty spot
										{
											//Change the probability to change how much spawns overall
											CreatePoint(tempx + check1, tempy
													+ check2, spawn[counter]); //1/200 chance of spawning
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
									temp = allcoords[tempx + check1][tempy
											+ check2];
									if (temp != -1 && element[temp] == 1 && rand() % 10 == 0)
									{
										element[temp] = element[counter]; //Change water to ice
										setBitmapColor(x[temp], y[temp], element[temp]); //not quite sure why this wasn't here before, idk how it was working
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
								element[counter] = condensing[element[counter]]; //"Condense" the steam
								setBitmapColor(x[counter], y[counter],
										element[counter]);
							}
						}
						int atemporary = allcoords[tempx][tempy];

						//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "start");

						//if the space the particle is trying to move to is taken
						if (atemporary != -1 && atemporary != counter)
						{
							//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "mid1");
							int secondElementTemp = element[atemporary];

							collide(counter, atemporary); //collision between the two particles
							//if nothing has changed
							//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "mid");
							if (x[counter] == ox && y[counter] == oy
									&& element[counter] == oelement
									&& x[atemporary] == tempx && y[atemporary]
									== tempy && element[atemporary]
									== secondElementTemp)
							{

								frozen[counter]++; //increase freeze rounds count


							}
							else
							{
								//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "mid2");
								if (x[counter] != ox || y[counter] != oy
										|| element[counter] != oelement)
								{
									//unFreeze stuff around the primary particle because stuff has changed with it
									//unFreezeParticles(x[counter],y[counter]);
									unFreezeParticles(ox, oy);
								}
								if (x[atemporary] != tempx || y[atemporary]
										!= tempy || element[atemporary]
										!= secondElementTemp)
								{
									//unFreeze stuff around the secondary particle because stuff has changed with it
									//unFreezeParticles(x[atemporary],y[atemporary]);
									unFreezeParticles(tempx, tempy);
								}
								//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "2");
							}
						}
						else
						{

							if (atemporary != counter)
							{

								//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "unfreeze stuff");
								//Clear the old spot
								allcoords[ox][oy] = -1;
								setBitmapColor(ox, oy, 3);

								//unfreeze particles around old spot

								unFreezeParticles(ox, oy);

								//Set new spot
								allcoords[tempx][tempy] = counter;
								setBitmapColor(tempx, tempy, element[counter]);

								//unFreeze paritlces around new spot
								//unFreezeParticles( tempx, tempy );
								//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "3");

							}

						}
						//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "end");
					}
				}
				rtop = 0;
			}
		}
	}
}
