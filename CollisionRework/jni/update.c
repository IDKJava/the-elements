/*
 * update.c
 * -----------------------------------
 * Defines the function UpdateView(), which
 * is called every frame to update all the
 * particles' positions.
 */

#include "update.h"
#include <android/log.h>

static int dx, dy;
//Used in for loops
static int counter;
//For speed we're going to create temp variables to store stuff
static int tempX, tempY, tempOldX, tempOldY, tempXVel, tempYVel;
static char tempInertia;
static struct Particle* tempParticle;
static struct Particle* tempAllCoords;
static struct Element* tempElement;
static struct Element* tempElement2;
//Used for heat
static signed char heatChange;

void UpdateView(void)
{
	//Clear
	if(shouldClear)
	{
		//Clear and unset the flag
		gameSetup();
		shouldClear = FALSE;
		
		//No need to do the rest of the update
		return;
	}

	//Zoom
	if(shouldZoom)
	{
		__android_log_write(ANDROID_LOG_INFO, "TheElements", "zoom section of udpate");
		if(zoom == ZOOMED_IN)
		{
			zoom = ZOOMED_OUT;
		}
		else
		{
			zoom = ZOOMED_IN;
		}
		zoomChanged = TRUE;
		if (zoom == ZOOMED_IN)
		{
			//Divide by two to zoom in
			workWidth = workWidth / 2;
			workHeight = workHeight / 2;
		}
		else
		{
			//Multiply by two to zoom out
			workWidth = workWidth * 2;
			workHeight = workHeight * 2;
		}
		//Finds nearest power of 2 to work Width
		stupidTegra = 1;
		while((stupidTegra = stupidTegra << 1) < workWidth);

		arraySetup();
		gameSetup();

		shouldZoom = FALSE;
	}

	//Draw points
	if (fingerDown)
	{
		for (dy = brushSize; dy >= -brushSize; dy--)
		{
			for (dx = -brushSize; dx <= brushSize; dx++)
			{
				if (TRUE) //Taken out for drawing optimization (dx * dx) + (dy * dy) <= (brushSize * brushSize))
				{
					//Normal drawing
					if (cElement->index >= NORMAL_ELEMENT)
					{
						//Draw it solid
						if(cElement->inertia == INERTIA_UNMOVABLE)
						{
							if (dx + mouseX < workWidth && dx + mouseX >= 0 && dy + mouseY < workHeight && dy + mouseY >= 0 && allCoords[getIndex((int) (dx + mouseX), (int) (dy + mouseY))] == NULL)
							{
								createPoint(mouseX + dx, mouseY + dy, cElement);
							}
						}
						//Draw it randomized
						else
						{
							if (rand() % 3 == 1 && dx + mouseX < workWidth && dx + mouseX >= 0 && dy + mouseY < workHeight && dy + mouseY >= 0 && allCoords[getIndex((int) (dx + mouseX), (int) (dy + mouseY))] == NULL)
							{
								createPoint(mouseX + dx, mouseY + dy, cElement);
							}
						}
					}
					//Special Drag case
					else if (cElement->index == DRAG_ELEMENT)
					{
						if (allCoords[getIndex(lastMouseX + dx, lastMouseY + dy)] && allCoords[getIndex(lastMouseX + dx, lastMouseY + dy)]->element->fallVel != 0 && dx + lastMouseX < workWidth && dx + lastMouseX > 0 && dy + lastMouseY < workHeight && dy + lastMouseY > 0)
						{
							allCoords[getIndex(lastMouseX + dx, lastMouseY + dy)]->xVel += (mouseX - lastMouseX);
							allCoords[getIndex(lastMouseX + dx, lastMouseY + dy)]->yVel += (mouseY - lastMouseY);
						}
					}
					//Special Eraser case
					else if (cElement->index == ERASER_ELEMENT)
					{
						if (allCoords[getIndex((int) (dx + mouseX), (int) (dy + mouseY))] && dx + mouseX < workWidth && dx + mouseX > 0 && dy + mouseY < workHeight && dy + mouseY > 0)
						{
							deletePoint(allCoords[getIndex(mouseX + dx, mouseY + dy)]);
						}
					}
				}
			}
		}
	}

	//__android_log_write(ANDROID_LOG_INFO, "TheElements", "WE GOT TO PARTICLES UPDATE");
	//Particles update
	if (play)
	{
		//Physics update
		for (counter = 0; counter < MAX_POINTS; counter++)
		{
			tempParticle = particles[counter];
			
			//If the particle is set and unfrozen
			if (tempParticle->set)// && tempParticle->frozen < 4)
			{
				//__android_log_write(ANDROID_LOG_INFO, "TheElements", "Processing a set particle");
				//TODO: Life property cycle

				//Set the temp and old variables
				tempOldX = (int) tempParticle->x;
				tempOldY = (int) tempParticle->y;
				tempParticle->oldX = tempOldX;
				tempParticle->oldY = tempOldY;
				tempElement = tempParticle->element;
				tempInertia = tempElement->inertia;
				tempXVel = tempParticle->xVel;
				tempYVel = tempParticle->yVel;

				//Update coords
				if(tempInertia != INERTIA_UNMOVABLE)
				{
					tempParticle->x += tempXVel;
					tempParticle->y += tempElement->fallVel + tempYVel;

					//Boundary checking
					if ((int) tempParticle->x >= workWidth)
					{
						if(cAtmosphere->borderRight)
						{
							//Bounce the particle
							tempParticle->x = tempOldX;
						}
						else
						{
							//Move it back and delete it
							tempParticle->x = tempOldX;
							tempParticle->y = tempOldY;
							deletePoint(tempParticle);
							continue;
						}
					}
					else if((int) tempParticle->x < 0)
					{
						if(cAtmosphere->borderLeft)
						{
							//Bounce the particle
							tempParticle->x = tempOldX;
						}
						else
						{
							//Move it back and delete it
							tempParticle->x = tempOldX;
							tempParticle->y = tempOldY;
							deletePoint(tempParticle);
							continue;
						}
					}
					if((int) tempParticle->y >= workHeight)
					{
						if(cAtmosphere->borderBottom)
						{
							//Bounce the particle
							tempParticle->y = tempOldY;
						}
						else
						{
							//Move it back and delete it
							tempParticle->x = tempOldX;
							tempParticle->y = tempOldY;
							deletePoint(tempParticle);
							continue;
						}
					}
					else if((int) tempParticle->y < 0)
					{
						if(cAtmosphere->borderTop)
						{
							//Bounce the particle
							tempParticle->y = tempOldY;
						}
						else
						{
							//Move it back and delete it
							tempParticle->x = tempOldX;
							tempParticle->y = tempOldY;
							deletePoint(tempParticle);
							continue;
						}
					}

					//Reduce velocities
					if(tempXVel < 0)
					{
						if(tempInertia >= -tempXVel)
						{
							tempXVel = 0;
						}
						else
						{
							tempXVel += tempInertia;
						}
					}
					else if(tempXVel > 0)
					{
						if(tempInertia >= tempXVel)
						{
							tempXVel = 0;
						}
						else
						{
							tempXVel -= tempInertia;
						}
					}
					if(tempYVel < 0)
					{
						if(tempInertia >= -tempYVel)
						{
							tempYVel = 0;
						}
						else
						{
							tempYVel += tempInertia;
						}
					}
					else if(tempYVel > 0)
					{
						if(tempInertia >= tempYVel)
						{
							tempYVel = 0;
						}
						else
						{
							tempYVel -= tempInertia;
						}
					}
					tempParticle->xVel = tempXVel;
					tempParticle->yVel = tempYVel;

					//Indicate that the particle has moved
					tempParticle->hasMoved = TRUE;

					//Set other temp variables
					tempX = (int) tempParticle->x;
					tempY = (int) tempParticle->y;

					//Updating allCoords and collision resolution
					tempAllCoords = allCoords[getIndex(tempX, tempY)];
					//__android_log_write(ANDROID_LOG_INFO, "TheElements", "Got here");

					//If the space the particle is trying to move to is taken and isn't itself
					if (tempAllCoords != NULL && tempAllCoords != tempParticle)
					{
						tempElement2 = tempAllCoords->element;

						//Update heat
						heatChange = (tempParticle->heat - tempAllCoords->heat)/5;
						tempParticle->heat -= heatChange;
						tempAllCoords->heat += heatChange;

						//Resolve second particle heat changes
						if(tempAllCoords->heat < tempAllCoords->element->lowestTemp)
						{
							//__android_log_write(ANDROID_LOG_ERROR, "TheElements", "Lower heat change");
							setElement(tempAllCoords, tempAllCoords->element->lowerElement);
						}
						else if(tempAllCoords->heat > tempAllCoords->element->highestTemp)
						{
							//__android_log_write(ANDROID_LOG_ERROR, "TheElements", "Higher heat change");
							setElement(tempAllCoords, tempAllCoords->element->higherElement);
						}

						//Resolve the collision (this updates the state of the particle, but lets this function resolve later)
						collide(tempParticle, tempAllCoords);

						//Reset tempXVel and tempYVel
						tempXVel = tempParticle->xVel;
						tempYVel = tempParticle->yVel;

						//Update allCoords and the bitmap colors if the hasMoved flag is set
						if(tempParticle->hasMoved)
						{
							tempX = tempParticle->x;
							tempY = tempParticle->y;

							allCoords[getIndex(tempOldX, tempOldY)] = NULL;
							clearBitmapColor(tempOldX, tempOldY);
							allCoords[getIndex(tempX, tempY)] = tempParticle;
							setBitmapColor(tempX, tempY, tempParticle->element);

							unFreezeParticles(tempOldX, tempOldY);
							tempParticle->hasMoved = FALSE;
						}
						else
						{
							tempParticle->frozen++;
						}
						if(tempAllCoords->hasMoved)
						{
							tempX = tempAllCoords->x;
							tempY = tempAllCoords->y;

							allCoords[getIndex(tempOldX, tempOldY)] = NULL;
							setBitmapColor(tempOldX, tempOldY, elements[ERASER_ELEMENT]);
							allCoords[getIndex(tempX, tempY)] = tempAllCoords;
							setBitmapColor(tempX, tempY, tempAllCoords->element);

							unFreezeParticles(tempOldX, tempOldY);
							tempAllCoords->hasMoved = FALSE;
						}
					}
					//Space particle is trying to move to is free
					else if (tempAllCoords != tempParticle)
					{
						allCoords[getIndex(tempOldX, tempOldY)] = NULL;
						clearBitmapColor(tempOldX, tempOldY);
						allCoords[getIndex(tempX, tempY)] = tempParticle;
						setBitmapColor(tempX, tempY, tempParticle->element);

						unFreezeParticles(tempOldX, tempOldY);
						tempParticle->hasMoved = FALSE;
					}
					//Space particle is trying to move to is itself
					else
					{
						tempParticle->hasMoved = FALSE;
						tempParticle->frozen++;
					}
				}
				//Inertia unmovable object still need to deal with velocities
				else
				{
					//Reduce velocities
					if(tempXVel < 0)
					{
						//__android_log_write(ANDROID_LOG_INFO, "TheElements", "tempXVel < 0");
						if(tempInertia >= -tempXVel)
						{
							tempXVel = 0;
						}
						else
						{
							tempXVel += tempInertia;
						}
					}
					else if(tempXVel > 0)
					{
						if(tempInertia >= tempXVel)
						{
							tempXVel = 0;
						}
						else
						{
							tempXVel -= tempInertia;
						}
					}
					if(tempYVel < 0)
					{
						if(tempInertia >= -tempYVel)
						{
							tempYVel = 0;
						}
						else
						{
							tempYVel += tempInertia;
						}
					}
					else if(tempYVel > 0)
					{
						if(tempInertia >= tempYVel)
						{
							tempYVel = 0;
						}
						else
						{
							tempYVel -= tempInertia;
						}
					}

					tempParticle->xVel = tempXVel;
					tempParticle->yVel = tempYVel;
				}

				//Update heat
				if(tempParticle->heat != cAtmosphere->heat)
				{
					if(rand() % ((3 - tempElement->state)*10)  != 0)
					{
						heatChange = 0;
					}
					else
					{
						heatChange = (tempParticle->heat - cAtmosphere->heat)/10 + rand()%3 - 1;
					}
					tempParticle->heat -= heatChange;
				}

				//Resolve heat changes
				if(tempParticle->heat < tempParticle->element->lowestTemp)
				{
					//__android_log_write(ANDROID_LOG_ERROR, "TheElements", "Lower heat change");
					setElement(tempParticle, tempParticle->element->lowerElement);
				}
				else if(tempParticle->heat > tempParticle->element->highestTemp)
				{
					//__android_log_write(ANDROID_LOG_ERROR, "TheElements", "Higher heat change");
					setElement(tempParticle, tempParticle->element->higherElement);
				}



				/* Acclerometer stuff taken out for now
				//If accel control, do tempYVel based on that
				if ((int) gravy != 0 && accelcon)
				{
					y[counter] += ((gravy / 9.8) * fallvel[oelement] + tempYVel[counter]);
				}
				//Otherwise, just do the fallvelocity
				else if (accelcon == 0)
				{
					y[counter] += fallvel[oelement] + tempYVel[counter];
				}
				//Accelerometer control, but no gravity (held horizontal)
				else
				{
					y[counter] += tempYVel[counter];
				}

				//If accel control is on, calculate new x using the gravity set
				if ((int) gravx != 0 && accelcon == 1)
				{
					x[counter] += ((gravx / 9.8) * fallvel[oelement] + tempXVel[counter]);
				}
				//Otherwise, just add tempXVel
				else
				{
					x[counter] += tempXVel[counter];
				}
				*/

				//Special cycle
				/* TODO: --- This had errors, will fix after getting it to build ---
				if(special[tempElement] != SPECIAL_NOT_SET)
				{
					switch(special[tempElement])
					{
						//Stuff to come
					}
				}
				*/

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
						tempYVel[allCoords[tempX][tempY + 1]] = 2;
					}
					random = rand();
					if (collision[element[allCoords[tempX + 1][tempY + 1]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allCoords[tempX + 1][tempY + 1], 5);
						tempYVel[allCoords[tempX][tempY + 1]] = 2;
					}
					random = rand();
					if (collision[element[allCoords[tempX - 1][tempY + 1]]][element[counter]] == 6 && random % 3 != 0)
					{
						setElement(allCoords[tempX - 1][tempY + 1], 5);
						tempYVel[allCoords[tempX][tempY + 1]] = 2;
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
							if (tempX + check1 > 1 && tempX + check1 < workWidth && tempY + check2 >= 0 && tempY + check2 < workHeight)
							{
								temp = allCoords[tempX + check1][tempY + check2];
								if (temp != -1 && element[temp] == 7) //There's a generator adjacent
								{
									element[temp] = 8; //Change the generator to a spawn
									spawn[temp] = spawn[counter]; //Make the spawn element the same as this one
								}
								else if (temp == -1 && rand() % 200 == 0 && loq < TPoints - 1) //There's an empty spot
								{
									createPoint(tempX + check1, tempY + check2, spawn[counter]); //1/200 chance of spawning
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


			}
		}
		//__android_log_write(ANDROID_LOG_INFO, "TheElements", "All particles done");
	}
}
