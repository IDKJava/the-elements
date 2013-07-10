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
static int heatChange;

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
    if(zoomChanged)
    {
        //Update workWidth and workHeight
        workWidth = screenWidth / zoomFactor;
        workHeight = screenHeight / zoomFactor;

        //Finds nearest power of 2 to work Width
        stupidTegra = 1;
        while((stupidTegra = stupidTegra << 1) < workWidth);

        arraySetup();
        gameSetup();
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
                    if ( dx + mouseX < workWidth && dx + mouseX >= 0 && dy + mouseY < workHeight && dy + mouseY >= 0 )
                        //Normal drawing
                    {
                        if (cElement->index >= NORMAL_ELEMENT)
                        {
                            //Draw it solid
                            if(cElement->inertia == INERTIA_UNMOVABLE || cElement->index == ELECTRICITY_ELEMENT)
                            {
                                if (allCoords[getIndex((int) (dx + mouseX), (int) (dy + mouseY))] == NULL)
                                {
                                    createPoint(mouseX + dx, mouseY + dy, cElement);
                                }
                            }
                            //Draw it randomized
                            else
                            {
                                if (rand() % 3 == 1 && allCoords[getIndex((int) (dx + mouseX), (int) (dy + mouseY))] == NULL)
                                {
                                    createPoint(mouseX + dx, mouseY + dy, cElement);
                                }
                            }
                        }
                        //Special Drag case
                        else if (cElement->index == DRAG_ELEMENT)
                        {
                            if (allCoords[getIndex(lastMouseX + dx, lastMouseY + dy)] && allCoords[getIndex(lastMouseX + dx, lastMouseY + dy)]->element->fallVel != 0)
                            {
                                allCoords[getIndex(lastMouseX + dx, lastMouseY + dy)]->xVel += (mouseX - lastMouseX);
                                allCoords[getIndex(lastMouseX + dx, lastMouseY + dy)]->yVel += (mouseY - lastMouseY);
                            }
                        }
                        //Special Eraser case
                        else if (cElement->index == ERASER_ELEMENT)
                        {
                            if (allCoords[getIndex((int) (dx + mouseX), (int) (dy + mouseY))])
                            {
                                deletePoint(allCoords[getIndex(mouseX + dx, mouseY + dy)]);
                            }
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
        // Change the random offset every so often (used in Trail)
        if (rand()%1000 == 1)
        {
            randOffset = rand();
        }
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
                    //__android_log_write(ANDROID_LOG_INFO, "LOG", "Start update coords");
                    //If accelOn, do tempYVel based on that
                    if (yGravity != 0 && accelOn)
                    {
                        tempParticle->y += ((yGravity / 9.8) * tempElement->fallVel + tempYVel);
                    }
                    //Otherwise, just do the fallvelocity
                    else if (!accelOn)
                    {
                        tempParticle->y += tempElement->fallVel + tempYVel;
                    }
                    //Accelerometer on, but no gravity (held horizontal)
                    else
                    {
                        tempParticle->y += tempYVel;
                    }

                    //If accelOn, calculate new x using the gravity set
                    //X Gravity is REVERSED! (hence the "-")
                    if ((int) xGravity != 0 && accelOn == 1)
                    {
                        tempParticle->x += ((-xGravity / 9.8) * tempElement->fallVel + tempXVel);
                    }
                    //Otherwise, just add tempXVel
                    else
                    {
                        tempParticle->x += tempXVel;
                    }

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
                        if(cAtmosphere->borderBottom && !(tempParticle->element == elements[ELECTRICITY_ELEMENT]))
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
                        if(cAtmosphere->borderTop && !hasSpecial(tempParticle, SPECIAL_LIFE))
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
                    // Update y vel based on inertia, always approaching 0
                    if(tempYVel < 0)
                    {
                        tempYVel += tempInertia;

                        if (tempYVel >= 0)
                        {
                            tempYVel = 0;
                        }
                        else
                        {
                            tempYVel++;
                        }
                    }
                    else if(tempYVel > 0)
                    {
                        tempYVel -= tempInertia;

                        if(tempYVel <= 0)
                        {
                            tempYVel = 0;
                        }
                        else
                        {
                            tempYVel--;
                        }
                    }
                    tempParticle->xVel = tempXVel;
                    tempParticle->yVel = tempYVel;


                    //__android_log_write(ANDROID_LOG_INFO, "LOG", "End update coords");

                    //Indicate that the particle has moved
                    tempParticle->hasMoved = TRUE;

                    //Set other temp variables
                    tempX = (int) tempParticle->x;
                    tempY = (int) tempParticle->y;

                    //Updating allCoords and collision resolution
                    tempAllCoords = allCoords[getIndex(tempX, tempY)];

                    //If the space the particle is trying to move to is taken and isn't itself
                    if (tempAllCoords != NULL && tempAllCoords != tempParticle)
                    {
                        tempElement2 = tempAllCoords->element;

                        //Update heat
                        heatChange = (tempParticle->heat - tempAllCoords->heat)/5;
                        //The hotter particle should be cooled, while the cooler particle is heated
                        changeHeat(tempParticle, -heatChange);
                        changeHeat(tempAllCoords, heatChange);

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

                            //unFreezeParticles(tempOldX, tempOldY);
                            tempParticle->hasMoved = FALSE;
                        }
                        else
                        {
                            //tempParticle->frozen++;
                        }
                        if(tempAllCoords->hasMoved)
                        {
                            tempX = tempAllCoords->x;
                            tempY = tempAllCoords->y;

                            allCoords[getIndex(tempOldX, tempOldY)] = NULL;
                            clearBitmapColor(tempOldX, tempOldY);
                            allCoords[getIndex(tempX, tempY)] = tempAllCoords;
                            setBitmapColor(tempX, tempY, tempAllCoords->element);

                            //unFreezeParticles(tempOldX, tempOldY);
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

                        //unFreezeParticles(tempOldX, tempOldY);
                        tempParticle->hasMoved = FALSE;
                    }
                    //Space particle is trying to move to is itself
                    else
                    {
                        tempParticle->hasMoved = FALSE;
                        tempParticle->frozen++;
                    }

                    //__android_log_write(ANDROID_LOG_INFO, "LOG", "End resolve collisions");
                }
                //Inertia unmovable object still need to deal with velocities
                else
                {
                    //Reduce velocities
                    if(tempXVel < 0)
                    {
                        tempXVel += 1;
                    }
                    else if(tempXVel > 0)
                    {
                        tempXVel -= 1;
                    }
                    if(tempYVel < 0)
                    {
                        tempYVel += 1;
                    }
                    else if(tempYVel > 0)
                    {
                        tempYVel -= 1;
                    }

                    tempParticle->xVel = tempXVel;
                    tempParticle->yVel = tempYVel;
                    tempX = (int) tempParticle->x;
                    tempY = (int) tempParticle->y;
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
                    //If tempParticle is hotter than the atmosphere, we want to subtract temperature
                    changeHeat(tempParticle, -heatChange);
                }

                //__android_log_write(ANDROID_LOG_INFO, "LOG", "End update heat");

                int i;

                char specialLoopDone = FALSE;
                char shouldResolveHeatChanges = TRUE;
                for (i = 0; i < MAX_SPECIALS; i++)
                {
                    if (!tempElement->specials)
                    {
                        __android_log_write(ANDROID_LOG_ERROR, "LOG", "Null specials array!");
                        break;
                    }
                    /*
                      char buffer[256];
                      sprintf(buffer, "Processing special: %d, val: %d", i, tempElement->specials[i]);
                      __android_log_write(ANDROID_LOG_INFO, "LOG", buffer);
                    */
                    if(tempParticle->set && tempElement && tempElement->specials[i] != SPECIAL_NONE)
                    {
                        switch((int)tempElement->specials[i])
                        {
                            //Spawn
                        case SPECIAL_SPAWN:
                        {
                            //__android_log_write(ANDROID_LOG_INFO, "LOG", "Special spawn");
                            //frozen[counter] = 0;
                            int diffX, diffY;
                            struct Particle* tempAllCoords;
                            for (diffX = -2; diffX <= 2; diffX++)
                            {
                                for (diffY = -2; diffY <= 2; diffY++)
                                {
                                    if (tempX + diffX >= 0 && tempX + diffX < workWidth && tempY + diffY >= 0 && tempY + diffY < workHeight)
                                    {
                                        tempAllCoords = allCoords[getIndex(tempX+diffX,tempY+diffY)];
                                        if (tempAllCoords && tempAllCoords->element == elements[GENERATOR_ELEMENT]) //There's a generator adjacent
                                        {
                                            setElement(tempAllCoords, elements[SPAWN_ELEMENT]);
                                            setParticleSpecialVal(tempAllCoords, SPECIAL_SPAWN, getParticleSpecialVal(tempParticle, SPECIAL_SPAWN));
                                        }
                                        else if (!tempAllCoords && rand() % GENERATOR_SPAWN_PROB == 0 && loq < MAX_POINTS - 1) //There's an empty spot
                                        {
                                            createPoint(tempX + diffX, tempY + diffY, elements[getParticleSpecialVal(tempParticle, SPECIAL_SPAWN)]);
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        //Breakable
                        case SPECIAL_BREAK:
                        {
                            //__android_log_write(ANDROID_LOG_INFO, "LOG", "Special break");
                            if (tempParticle->xVel > getElementSpecialVal(tempElement, SPECIAL_BREAK) || tempParticle->yVel > getElementSpecialVal(tempElement, SPECIAL_BREAK))
                            {
                                setElement(tempParticle, elements[NORMAL_ELEMENT]);
                            }
                            break;
                        }
                        //Growing
                        case SPECIAL_GROW:
                        {
                            //__android_log_write(ANDROID_LOG_INFO, "LOG", "Special grow");
                            int diffX, diffY;
                            struct Particle* tempAllCoords;
                            for (diffX = -1; diffX <= 1; diffX++)
                            {
                                for (diffY = -1; diffY <= 1; diffY++)
                                {
                                    if (diffY + tempY >= 0 && tempY + diffY < workHeight && tempX + diffX >= 0 && diffX + diffX < workWidth)
                                    {
                                        tempAllCoords = allCoords[getIndex(tempX+diffX,tempY+diffY)];
                                        if (tempAllCoords && tempAllCoords->element->index == getElementSpecialVal(tempElement, SPECIAL_GROW) && rand() % 10 == 0)
                                        {
                                            setElement(tempAllCoords, tempParticle->element);
                                        }
                                    }
                                }
                            }

                            break;
                        }
                        //Heater
                        case SPECIAL_HEAT:
                        {
                            //__android_log_write(ANDROID_LOG_INFO, "LOG", "Special heat");
                            int diffX, diffY;
                            struct Particle* tempAllCoords;
                            if (rand()%5 == 0)
                            {
                                for (diffX = -1; diffX <= 1; diffX++)
                                {
                                    for(diffY = -1; diffY <=1; diffY++)
                                    {
                                        if((diffX!=0||diffY!=0) && tempX+diffX < workWidth && tempX+diffX >= 0 && tempY+diffY < workHeight && tempY+diffY >= 0)
                                        {
                                            tempAllCoords = allCoords[getIndex(tempX+diffX,tempY+diffY)];
                                            if(tempAllCoords)
                                            {
                                                changeHeat(tempAllCoords, getElementSpecialVal(tempElement, SPECIAL_HEAT));
                                            }
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        //Explosive
                        case SPECIAL_EXPLODE:
                        {
                            //__android_log_write(ANDROID_LOG_INFO, "LOG", "Special explode");
                            if (tempParticle->heat >= tempParticle->element->highestTemp) //If the heat is above the threshold
                            {
                                int diffX, diffY;
                                int distance;
                                struct Particle* tempAllCoords;

                                //In radius of explosion, add velocity with a 5% chance
                                if(rand()%20 == 0)
                                {
                                    int explosiveness = getElementSpecialVal(tempElement, SPECIAL_EXPLODE);
                                    for (diffX = -explosiveness; diffX <= explosiveness; diffX++)
                                    {
                                        for (diffY = -explosiveness; diffY <= explosiveness; diffY++)
                                        {
                                            if (tempX + diffX >= 0 && tempX + diffX < workWidth && tempY + diffY >= 0 && tempY + diffY < workHeight)
                                            {
                                                tempAllCoords = allCoords[getIndex(tempX + diffX, tempY + diffY)];
                                                if (tempAllCoords)
                                                {
                                                    if(diffX != 0 && tempAllCoords->xVel < explosiveness)
                                                    {
                                                        tempAllCoords->xVel += (2*(diffX > 0)-1);
                                                    }
                                                    if(diffY != 0 && tempAllCoords->yVel < explosiveness)
                                                    {
                                                        tempAllCoords->yVel += (2*(diffY > 0)-1);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                                // Add heat
                                for(diffX = -2; diffX <= 2; diffX++)
                                {
                                    for(diffY = -2; diffY <= 2; diffY++)
                                    {
                                        if (tempX + diffX >= 0 && tempX + diffX < workWidth && tempY + diffY >= 0 && tempY + diffY < workHeight)
                                        {
                                            tempAllCoords = allCoords[getIndex(tempX + diffX, tempY + diffY)];
                                            if(tempAllCoords)
                                            {
                                                changeHeat(tempAllCoords, 50);
                                            }
                                        }
                                    }
                                }

                                // Change this particle to fire, and quit the specials loop
                                setElement(tempParticle, elements[10]);
                                specialLoopDone = TRUE;
                                break;
                            }
                            break;

                        }
                        //Disappearing
                        case SPECIAL_LIFE:
                        {
                            //__android_log_write(ANDROID_LOG_INFO, "LOG", "Special life");
                            if (rand()%getElementSpecialVal(tempElement, SPECIAL_LIFE) == 0)
                            {
                                deletePoint(tempParticle);
                            }
                            break;
                        }
                        //Wander
                        case SPECIAL_WANDER:
                        {
                            // Don't wander while tunneling
                            // FIXME: This is a hacky solution, come up with something more elegant
                            if (getParticleSpecialVal(tempParticle, SPECIAL_TUNNEL) != SPECIAL_VAL_UNSET)
                            {
                                continue;
                            }

                            int randVal = rand()%100;
                            // Randomly wander
                            int wanderVal = getElementSpecialVal(tempElement, SPECIAL_WANDER);
                            if (randVal <= wanderVal)
                            {
                                if (tempParticle->xVel <= 4)
                                {
                                    tempParticle->xVel += 2;
                                }
                            }
                            else if (randVal >= wanderVal+1 && randVal <= wanderVal*2)
                            {
                                if (tempParticle->xVel >= -4)
                                {
                                    tempParticle->xVel -= 2;
                                }
                            }

                            randVal = rand()%100;
                            if ( randVal <= wanderVal)
                            {
                                if ( tempParticle->yVel >= -4)
                                {
                                    tempParticle->yVel -= 2;
                                }
                            }
                            if ( randVal >= wanderVal + 1 && randVal <= wanderVal*2)
                            {
                                if ( tempParticle->yVel <= 4)
                                {
                                    tempParticle->yVel += 2;
                                }
                            }

                            break;
                        }
                        //Jump
                        case SPECIAL_JUMP:
                        {
                            if ((tempParticle->y+1 == workHeight) || (allCoords[getIndex(tempParticle->x, tempParticle->y+1)] != NULL))
                            {
                                int randVal = rand()%100;
                                int jumpVal = getElementSpecialVal(tempElement, SPECIAL_JUMP);
                                if (randVal < jumpVal)
                                {
                                    tempParticle->yVel -= 5;
                                }
                            }
                            break;
                        }
                        // Tunnel
                        case SPECIAL_TUNNEL:
                        {
                            int targetElementIndex = getElementSpecialVal(tempElement, SPECIAL_TUNNEL);
                            int state = getParticleSpecialVal(tempParticle, SPECIAL_TUNNEL);

                            int curX = tempParticle->x, curY = tempParticle->y;
                            int diffX, diffY;

                            // TODO: Break tunneling stuff out into its own function (make a specials.c file?)
                            if (state == SPECIAL_VAL_UNSET)
                            {
                                // Look in a random diagonal
                                int randomDir = rand()%4;
                                diffX = 2*(randomDir%2) - 1;
                                diffY = (randomDir - randomDir%2) - 1;
                                if (curX+diffX < 0 || curX+diffX >= workWidth || curY+diffY < 0 || curY+diffY >= workHeight)
                                {
                                    continue;
                                }
                                struct Particle* tempAllCoords = allCoords[getIndex(curX+diffX, curY+diffY)];
                                if (tempAllCoords != NULL && tempAllCoords->element->index == targetElementIndex)
                                {
                                    // Remove the tempAllCoords particle, and move this particle there
                                    unSetPoint(tempAllCoords);
                                    allCoords[getIndex(curX+diffX, curY+diffY)] = tempParticle;
                                    setBitmapColor(curX+diffX, curY+diffY, tempElement);
                                    allCoords[getIndex(curX, curY)] = NULL;
                                    clearBitmapColor(curX, curY);
                                    tempParticle->x = curX + diffX;
                                    tempParticle->y = curY + diffY;
                                    // Set the y velocity to the fall velocity to counteract movement
                                    tempParticle->yVel = tempParticle->element->fallVel;
                                    setParticleSpecialVal(tempParticle, SPECIAL_TUNNEL, randomDir);


                                    // Add particles around this point, forming a "tunnel"
                                    if (curX+2*diffX >= 0 && curX+2*diffX < workWidth && curY+2*diffY >= 0 && curY+2*diffY < workHeight)
                                    {
                                        if (allCoords[getIndex(curX+2*diffX, curY+2*diffY)] == NULL)
                                        {
                                            createPoint(curX+2*diffX, curY+2*diffY, elements[targetElementIndex]);
                                        }
                                        if (allCoords[getIndex(curX+2*diffX, curY+diffY)] == NULL)
                                        {
                                            createPoint(curX+2*diffX, curY+diffY, elements[targetElementIndex]);
                                        }
                                        if (allCoords[getIndex(curX+diffX, curY+2*diffY)] == NULL)
                                        {
                                            createPoint(curX+diffX, curY+2*diffY, elements[targetElementIndex]);
                                        }
                                    }
                                }
                            }
                            // We're already moving in a direction
                            else
                            {
                                // Move the particle back to it's old location, to avoid
                                // collision velocities messing up tunneling
                                if (allCoords[getIndex(tempOldX, tempOldY)] == NULL)
                                {
                                    allCoords[getIndex(curX, curY)] = NULL;
                                    clearBitmapColor(curX, curY);
                                    allCoords[getIndex(tempOldX, tempOldY)] = tempParticle;
                                    setBitmapColor(tempOldX, tempOldY, tempElement);

                                    tempParticle->x = tempOldX;
                                    tempParticle->y = tempOldY;
                                    curX = tempOldX;
                                    curY = tempOldY;
                                }

                                diffX = 2*(state%2) - 1;
                                diffY = (state - state%2) - 1;
                                if (curX+diffX < 0 || curX+diffX >= workWidth || curY+diffY < 0 || curY+diffY >= workHeight)
                                {
                                    // Go back to the unset state
                                    setParticleSpecialVal(tempParticle, SPECIAL_TUNNEL, SPECIAL_VAL_UNSET);

                                    // Look in a random diagonal
                                    int randomDir = rand()%4;
                                    diffX = 2*(randomDir%2) - 1;
                                    diffY = (randomDir - randomDir%2) - 1;
                                    if (curX+diffX < 0 || curX+diffX >= workWidth || curY+diffY < 0 || curY+diffY >= workHeight)
                                    {
                                        continue;
                                    }
                                    struct Particle* tempAllCoords = allCoords[getIndex(curX+diffX, curY+diffY)];
                                    if (tempAllCoords != NULL && tempAllCoords->element->index == targetElementIndex)
                                    {
                                        // Remove the tempAllCoords particle, and move this particle there
                                        unSetPoint(tempAllCoords);
                                        allCoords[getIndex(curX+diffX, curY+diffY)] = tempParticle;
                                        setBitmapColor(curX+diffX, curY+diffY, tempElement);
                                        allCoords[getIndex(curX, curY)] = NULL;
                                        clearBitmapColor(curX, curY);
                                        tempParticle->x = curX + diffX;
                                        tempParticle->y = curY + diffY;
                                        // Set the y velocity to the fall velocity to counteract movement
                                        tempParticle->yVel = tempParticle->element->fallVel;
                                        setParticleSpecialVal(tempParticle, SPECIAL_TUNNEL, randomDir);


                                        // Add particles around this point, forming a "tunnel"
                                        if (curX+2*diffX >= 0 && curX+2*diffX < workWidth && curY+2*diffY >= 0 && curY+2*diffY < workHeight)
                                        {
                                            if (allCoords[getIndex(curX+2*diffX, curY+2*diffY)] == NULL)
                                            {
                                                createPoint(curX+2*diffX, curY+2*diffY, elements[targetElementIndex]);
                                            }
                                            if (allCoords[getIndex(curX+2*diffX, curY+diffY)] == NULL)
                                            {
                                                createPoint(curX+2*diffX, curY+diffY, elements[targetElementIndex]);
                                            }
                                            if (allCoords[getIndex(curX+diffX, curY+2*diffY)] == NULL)
                                            {
                                                createPoint(curX+diffX, curY+2*diffY, elements[targetElementIndex]);
                                            }
                                        }
                                    }
                                }

                                struct Particle* tempAllCoords = allCoords[getIndex(curX+diffX, curY+diffY)];
                                if (tempAllCoords == NULL || tempAllCoords->element->index != targetElementIndex)
                                {
                                    // Go back to the unset state
                                    setParticleSpecialVal(tempParticle, SPECIAL_TUNNEL, SPECIAL_VAL_UNSET);
                                    // Look for any particle back the way we came and (if there is one)
                                    // tunnel into it
                                    tempAllCoords = allCoords[getIndex(curX, curY+diffY)];
                                    if (tempAllCoords != NULL && tempAllCoords->element->index == targetElementIndex)
                                    {
                                        // Remove the tempAllCoords particle, and move this particle there
                                        unSetPoint(tempAllCoords);
                                        allCoords[getIndex(curX, curY+diffY)] = tempParticle;
                                        setBitmapColor(curX, curY+diffY, tempElement);
                                        allCoords[getIndex(curX, curY)] = NULL;
                                        clearBitmapColor(curX, curY);
                                        tempParticle->x = curX;
                                        tempParticle->y = curY + diffY;
                                    }
                                    else
                                    {
                                        tempAllCoords = allCoords[getIndex(curX+diffX, curY)];
                                        if (tempAllCoords != NULL && tempAllCoords->element->index == targetElementIndex)
                                        {
                                            // Remove the tempAllCoords particle, and move this particle there
                                            unSetPoint(tempAllCoords);
                                            allCoords[getIndex(curX+diffX, curY)] = tempParticle;
                                            setBitmapColor(curX+diffX, curY, tempElement);
                                            allCoords[getIndex(curX, curY)] = NULL;
                                            clearBitmapColor(curX, curY);
                                            tempParticle->x = curX + diffX;
                                            tempParticle->y = curY;
                                        }
                                    }
                                    // Add velocity so that the particle stays still for one step
                                    tempParticle->yVel = tempParticle->element->fallVel;
                                    continue;
                                }
                                else
                                {
                                    // Remove the tempAllCoords particle, and move this particle there
                                    unSetPoint(tempAllCoords);
                                    allCoords[getIndex(curX+diffX, curY+diffY)] = tempParticle;
                                    setBitmapColor(curX+diffX, curY+diffY, tempElement);
                                    allCoords[getIndex(curX, curY)] = NULL;
                                    clearBitmapColor(curX, curY);
                                    tempParticle->x = curX + diffX;
                                    tempParticle->y = curY + diffY;
                                    // Set the y velocity to the fall velocity to counteract movement
                                    tempParticle->yVel = tempParticle->element->fallVel;


                                    // Add particles around this point, forming a "tunnel"
                                    if (curX+2*diffX >= 0 && curX+2*diffX < workWidth && curY+2*diffY >= 0 && curY+2*diffY < workHeight)
                                    {
                                        if (allCoords[getIndex(curX+2*diffX, curY+2*diffY)] == NULL)
                                        {
                                            createPoint(curX+2*diffX, curY+2*diffY, elements[targetElementIndex]);
                                        }
                                        if (allCoords[getIndex(curX+2*diffX, curY+diffY)] == NULL)
                                        {
                                            createPoint(curX+2*diffX, curY+diffY, elements[targetElementIndex]);
                                        }
                                        if (allCoords[getIndex(curX+diffX, curY+2*diffY)] == NULL)
                                        {
                                            createPoint(curX+diffX, curY+2*diffY, elements[targetElementIndex]);
                                        }
                                    }
                                }
                            }

                            break;
                        }
                        // Burn
                        // FIXME: This very rarely causes stuck particles
                        case SPECIAL_BURN:
                        {
                            // Burn doesn't trigger when explode is set
                            if (getElementSpecialVal(tempElement, SPECIAL_EXPLODE) != SPECIAL_VAL_UNSET)
                            {
                                continue;
                            }

                            int state = getParticleSpecialVal(tempParticle, SPECIAL_BURN);
                            // Not burning yet
                            if (state == SPECIAL_VAL_UNSET)
                            {
                                //If the heat is above the threshold
                                if (tempParticle->heat >= tempParticle->element->highestTemp)
                                {
                                    int avgLife = getElementSpecialVal(tempElement, SPECIAL_BURN);
                                    int life = avgLife + rand()%5 - 2;
                                    if (life < 1) { life = 1; }
                                    setParticleSpecialVal(tempParticle, SPECIAL_BURN, life);
                                    shouldResolveHeatChanges = FALSE;
                                }
                            }
                            // Already burning
                            else
                            {
                                // Make sure the wood doesn't change states yet
                                shouldResolveHeatChanges = FALSE;

                                // FIXME: This is identical to special heat, break this function out into specials.c
                                int diffX, diffY;
                                struct Particle* tempAllCoords;
                                for (diffX = -1; diffX <= 1; diffX++)
                                {
                                    for(diffY = -1; diffY <=1; diffY++)
                                    {
                                        if((diffX!=0||diffY!=0) && tempX+diffX < workWidth && tempX+diffX >= 0 && tempY+diffY < workHeight && tempY+diffY >= 0)
                                        {
                                            tempAllCoords = allCoords[getIndex(tempX+diffX,tempY+diffY)];
                                            if(tempAllCoords)
                                            {
                                                changeHeat(tempAllCoords, 1000/(state*getElementSpecialVal(tempElement, SPECIAL_BURN)));
                                            }
                                        }
                                    }
                                }

                                // If we're done with our life total
                                if (state <= 1)
                                {
                                    // Change to fire
                                    setElement(tempParticle, elements[FIRE_ELEMENT]);
                                    //deletePoint(tempParticle);
                                    specialLoopDone = TRUE;
                                }
                                else
                                {
                                    // Decrement life total left
                                    setParticleSpecialVal(tempParticle, SPECIAL_BURN, state-1);

                                    // Set the color to fire
                                    setBitmapColor(tempX, tempY, elements[FIRE_ELEMENT]);
                                }
                            }

                            break;
                        }
                        // Conduct
                        case SPECIAL_CONDUCTIVE:
                        {
                            int property;
                            struct Particle* tempAllCoords;
                            int curX = tempParticle->x, curY = tempParticle->y;
                            property = getParticleSpecialVal(tempParticle, SPECIAL_CONDUCTIVE);
                            if ( property != SPECIAL_VAL_UNSET )
                            {
                                // Wait one frame, remove the electric wait bit
                                if (property & ELECTRIC_WAIT)
                                {
                                    setBitmapColor(curX,curY,elements[ELECTRICITY_ELEMENT]);
                                    setParticleSpecialVal(tempParticle, SPECIAL_CONDUCTIVE, property & 15); //15 == 00001111
                                    break;
                                }
                                // If no direction, select a random direction
                                if( property == ELECTRIC_NO_DIR )
                                {
                                    // 0 or 1 in XN, 1 in X1
                                    property = (((rand()%2) << 2) & ELECTRIC_XN) | ELECTRIC_X1;
                                }
                                int i = (property & ELECTRIC_X1) * ((ELECTRIC_XN & property) ? -1 : 1);
                                int j = ((property & ELECTRIC_Y1) >> 1) * ((ELECTRIC_YN & property) ? -1 : 1);

                                int tempI, k, transfered = FALSE;
                                for ( k = 0; k < 4; k++)
                                {
                                    if ( curX + i <= workWidth && curX + i > 0 && curY + j <= workHeight && curY + j > 0)
                                    {
                                        tempAllCoords = allCoords[getIndex(curX+i,curY+j)];
                                        if ( tempAllCoords != NULL)
                                        {
                                            if( hasSpecial(tempAllCoords, SPECIAL_CONDUCTIVE) )
                                            {
                                                setParticleSpecialVal(tempAllCoords, SPECIAL_CONDUCTIVE,
                                                                      (i ? ELECTRIC_X1 : 0) |
                                                                      (i < 0 ? ELECTRIC_XN : 0) |
                                                                      (j ? ELECTRIC_Y1 : 0) |
                                                                      (j < 0 ? ELECTRIC_YN :0) |
                                                                      ELECTRIC_WAIT);
                                                setBitmapColor(curX+i,curY+j,elements[ELECTRICITY_ELEMENT]);
                                                transfered = TRUE;
                                                break;
                                            }
                                        }
                                    }
                                    //There exists a nice binary operation to do this ( check right, left behind, then return), but this is clearer
                                    //If we need speed we can use binary operations to get these numbers
                                    tempI = i;
                                    switch ( k )
                                    {
                                    case 0:
                                    {
                                        i = j;
                                        j = tempI;
                                        break;
                                    }
                                    case 1:
                                    {
                                        i = -i;
                                        j = -j;
                                        break;
                                    }
                                    case 2:
                                    {
                                        i = j;
                                        j = -tempI;
                                        break;
                                    }
                                    case 3:
                                    {
                                        i = -i;
                                        j = -j;
                                        break;
                                    }
                                    }
                                }
                                if(!transfered)
                                {
                                    i = i - j;
                                    j = i + j;
                                    for (k = 0; k < 4; k++ )
                                    {
                                        if ( curX + i <= workWidth && curX + i > 0 && curY + j <= workHeight && curY + j > 0)
                                        {
                                            tempAllCoords = allCoords[getIndex(curX+i,curY+j)];
                                            if ( tempAllCoords != NULL)
                                            {
                                                if( hasSpecial(tempAllCoords, SPECIAL_CONDUCTIVE) )
                                                {
                                                    setParticleSpecialVal(tempAllCoords, SPECIAL_CONDUCTIVE,
                                                                          (i ? ELECTRIC_X1 : 0) |
                                                                          (i < 0 ? ELECTRIC_XN : 0) |
                                                                          (j ? ELECTRIC_Y1 : 0) |
                                                                          (j < 0 ? ELECTRIC_YN :0) |
                                                                          ELECTRIC_WAIT);
                                                    transfered = TRUE;
                                                    break;
                                                }
                                            }
                                        }
                                        switch ( k )
                                        {
                                        case 0:
                                        {
                                            i = j;
                                            j = -tempI;
                                            break;
                                        }
                                        case 1:
                                        {
                                            i = -i;
                                            j = -j;
                                            break;
                                        }
                                        case 2:
                                        {
                                            i = j;
                                            j = tempI;
                                            break;
                                        }
                                        case 3:
                                        {
                                            i = -i;
                                            j = -j;
                                            break;
                                        }
                                        }
                                    }
                                }
                                if(!transfered)
                                {
                                    for ( k = 0; k < 4; k++)
                                    {
                                        if ( curX + i <= workWidth && curX + i > 0 && curY + j <= workHeight && curY + j > 0)
                                        {
                                            tempAllCoords = allCoords[getIndex(curX+i,curY+j)];
                                            if ( tempAllCoords == NULL)
                                            {
                                                createPoint(curX + i, curY + j, elements[ELECTRICITY_ELEMENT]);
                                                transfered = TRUE;
                                                break;
                                            }
                                        }
                                        //There exists a nice binary operation to do this ( check right, left behind, then return), but this is clearer
                                        //If we need speed we can use binary operations to get these numbers
                                        tempI = i;
                                        switch ( k )
                                        {
                                        case 0:
                                        {
                                            i = j;
                                            j = tempI;
                                            break;
                                        }
                                        case 1:
                                        {
                                            i = -i;
                                            j = -j;
                                            break;
                                        }
                                        case 2:
                                        {
                                            i = j;
                                            j = -tempI;
                                            break;
                                        }
                                        case 3:
                                        {
                                            i = -i;
                                            j = -j;
                                            break;
                                        }
                                        }
                                    }
                                    if(!transfered)
                                    {
                                        i = i - j;
                                        j = i + j;
                                        for (k = 0; k < 4; k++ )
                                        {
                                            if ( curX + i <= workWidth && curX + i > 0 && curY + j <= workHeight && curY + j > 0)
                                            {
                                                tempAllCoords = allCoords[getIndex(curX+i,curY+j)];
                                                if ( tempAllCoords == NULL)
                                                {
                                                    createPoint(curX + i, curY + j, elements[ELECTRICITY_ELEMENT]);
                                                    transfered = TRUE;
                                                }
                                            }
                                            switch ( k )
                                            {
                                            case 0:
                                            {
                                                i = j;
                                                j = -tempI;
                                                break;
                                            }
                                            case 1:
                                            {
                                                i = -i;
                                                j = -j;
                                                break;
                                            }
                                            case 2:
                                            {
                                                i = j;
                                                j = tempI;
                                                break;
                                            }
                                            case 3:
                                            {
                                                i = -i;
                                                j = -j;
                                                break;
                                            }
                                            }
                                        }
                                    }
                                }
                                setParticleSpecialVal(tempParticle, SPECIAL_CONDUCTIVE, SPECIAL_VAL_UNSET);
                                setBitmapColor(curX,curY,tempParticle->element);
                            }
                            break;
                        }
                        // Trail
                        case SPECIAL_TRAIL:
                        {
                            // Pull out the special val and clean it, it will determine
                            // how variable the path is (i.e. how much velocity is the max added)
                            int variability = getElementSpecialVal(tempElement, SPECIAL_TRAIL);
                            if (variability < 3)
                            {
                                variability = 3;
                            }
                            variability -= (variability+1)%2;

                            struct Particle* tempAllCoords;
                            int i, j,tempIndex, found = FALSE;
                            int curX = tempParticle->x, curY = tempParticle->y;
                            // Seed the rand function with the current location
                            // (plus a randomizer that doesn't change very often)
                            srand(curX*curY + randOffset);
                            // Add random velocity based on seed (so the same location
                            // will get the same velocity boost)
                            tempParticle->xVel += rand()%variability - variability/2;

                            break;
                        }

                        //Default: do nothing
                        default:
                            break;
                        }

                        //__android_log_write(ANDROID_LOG_INFO, "LOG", "End special");
                    }

                    if (specialLoopDone == TRUE)
                    {
                        break;
                    }
                }

                //__android_log_write(ANDROID_LOG_INFO, "LOG",  "End specials loop");

                //Resolve heat changes
                if (shouldResolveHeatChanges)
                {
                    if(tempParticle->heat < tempParticle->element->lowestTemp)
                    {
                        setElement(tempParticle, tempParticle->element->lowerElement);
                    }
                    else if(tempParticle->heat > tempParticle->element->highestTemp)
                    {
                        setElement(tempParticle, tempParticle->element->higherElement);
                    }
                }


                //__android_log_write(ANDROID_LOG_INFO, "LOG", "End resolve heat changes");
            }
        }
        //__android_log_write(ANDROID_LOG_INFO, "TheElements", "All particles done");
    }
}
