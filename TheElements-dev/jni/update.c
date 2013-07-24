/*
 * update.c
 * -----------------------------------
 * Defines the function UpdateView(), which
 * is called every frame to update all the
 * particles' positions.
 */

#include "update.h"
#include <android/log.h>

// Per-file logging
#ifndef NDEBUG
#define LOGGING 0
#else
#define LOGGING 0
#endif

static int shouldKillUpdateThread = TRUE;
pthread_t updateThread;

void drawPoints(void)
{
    int dx, dy;
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
                            if (allCoords[getIndex((int) (dx + mouseX), (int) (dy + mouseY))] == -1)
                            {
                                createPoint(mouseX + dx, mouseY + dy, cElement);
                            }
                        }
                        //Draw it randomized
                        else
                        {
                            if (rand() % 3 == 1 && allCoords[getIndex((int) (dx + mouseX), (int) (dy + mouseY))] == -1)
                            {
                                createPoint(mouseX + dx, mouseY + dy, cElement);
                            }
                        }
                    }
                    //Special Drag case
                    else if (cElement->index == DRAG_ELEMENT)
                    {
                        if (allCoords[getIndex(lastMouseX + dx, lastMouseY + dy)] && a_element[allCoords[getIndex(lastMouseX + dx, lastMouseY + dy)]]->fallVel != 0)
                        {
                            a_xVel[allCoords[getIndex(lastMouseX + dx, lastMouseY + dy)]] += (mouseX - lastMouseX);
                            a_yVel[allCoords[getIndex(lastMouseX + dx, lastMouseY + dy)]] += (mouseY - lastMouseY);
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

// Update the positions based on the diffs
// Returns: FALSE if the particle moved offscreen and needs to be deleted now.
//          If FALSE is returned, the x and y coords will be left at the
//          original values.
int checkBoundariesAndMove(float *x, float *y, float diffx, float diffy)
{
    // Save the oldy, in case we need to roll back the change because
    // the x causes deletion (rare occurrence).
    float oldy = *y;
    // Check upper boundary
    if (diffy > 0)
    {
        if ((int)(*y + diffy) < workHeight)
        {
            *y += diffy;
        }
        else if (!cAtmosphere->borderBottom)
        {
            return FALSE;
        }
    }
    // Check lower boundary
    else
    {
        if ((int)(*y + diffy) >= 0)
        {
            *y += diffy;
        }
        else if (!cAtmosphere->borderTop)
        {
            return FALSE;
        }
    }

    // Check right boundary
    if (diffx > 0)
    {
        if ((int)(*x + diffx) < workWidth)
        {
            *x += diffx;
        }
        else if (!cAtmosphere->borderRight)
        {
            // Rollback y before returning
            *y = oldy;
            return FALSE;
        }
    }
    // Check left boundary
    else
    {
        if ((int)(*x + diffx) >= 0)
        {
            *x += diffx;
        }
        else if (!cAtmosphere->borderLeft)
        {
            // Rollback y before returning
            *y = oldy;
            return FALSE;
        }
    }

    return TRUE;
}

// Update the velocities based on inertia
void updateVelocities(short *xvel, short *yvel, int inertia)
{
    if(*xvel < 0)
    {
        if(inertia >= -(*xvel))
        {
            *xvel = 0;
        }
        else
        {
            *xvel += inertia;
        }
    }
    else if(*xvel > 0)
    {
        if(inertia >= *xvel)
        {
            *xvel = 0;
        }
        else
        {
            *xvel -= inertia;
        }
    }
    // Update y vel based on inertia, always approaching 0
    if(*yvel < 0)
    {
        *yvel += inertia;

        if (*yvel >= 0)
        {
            *yvel = 0;
        }
        else
        {
            *yvel++;
        }
    }
    else if(*yvel > 0)
    {
        *yvel -= inertia;

        if(*yvel <= 0)
        {
            *yvel = 0;
        }
        else
        {
            *yvel--;
        }
    }
}

// Update the positions of the particle
// Returns: TRUE if we should continue with the particle,
//          FALSE if we deleted it.
int updateKinetic(int index)
{
    float *x_ptr = &(a_x[index]);
    float *y_ptr = &(a_y[index]);
    short *xvel_ptr = &(a_xVel[index]);
    short *yvel_ptr = &(a_yVel[index]);
    float diffx, diffy;

    //__android_log_write(ANDROID_LOG_INFO, "LOG", "Start update coords");
    //If accelOn, do tempYVel based on that
    if (yGravity != 0 && accelOn)
    {
        diffy = ((yGravity / 9.8) * a_element[index]->fallVel + *yvel_ptr);
    }
    //Otherwise, just do the fallvelocity
    else if (!accelOn)
    {
        diffy = a_element[index]->fallVel + *yvel_ptr;
    }
    //Accelerometer on, but no gravity (held horizontal)
    else
    {
        diffy = *yvel_ptr;
    }

    //If accelOn, calculate new x using the gravity set
    //X Gravity is REVERSED! (hence the "-")
    if ((int) xGravity != 0 && accelOn == 1)
    {
        diffx = ((-xGravity / 9.8) * a_element[index]->fallVel + *xvel_ptr);
    }
    //Otherwise, just add tempXVel
    else
    {
        diffx = *xvel_ptr;
    }

    //Boundary checking
    if (!checkBoundariesAndMove(x_ptr, y_ptr, diffx, diffy))
    {
        // Delete the particle
        deletePoint(index);
        return FALSE;
    }
    //Reduce velocities
    updateVelocities(xvel_ptr, yvel_ptr, a_element[index]->inertia);

    //Indicate that the particle has moved
    a_hasMoved[index] = TRUE;

    return TRUE;
}

// Update the heat when p1 collides into p2
// Only updates the state for p2 right now. p1 state updates happen at the end
// of the physics loop for p1.
void updateCollisionHeat(int index1, int index2)
{
    char *p1heat = &(a_heat[index1]);
    char *p2heat = &(a_heat[index2]);

    int heatChange = (*p1heat - *p2heat)/5;
    //The hotter particle should be cooled, while the cooler particle is heated
    changeHeat(p1heat, -heatChange);
    changeHeat(p2heat, heatChange);

    //TODO: probably a good idea to just save a_element[index2] and not access it so many times

    //Resolve second particle heat changes
    if(*p2heat < a_element[index2]->lowestTemp)
    {
        //__android_log_write(ANDROID_LOG_ERROR, "TheElements", "Lower heat change");
        setElement(index2, a_element[index2]->lowerElement);
    }
    else if(*p2heat > a_element[index2]->highestTemp)
    {
        //__android_log_write(ANDROID_LOG_ERROR, "TheElements", "Higher heat change");
        setElement(index2, a_element[index2]->higherElement);
    }
}

// Perform specials actions
// Returns: TRUE if we should resolve heat changes afterwards
// Assumes particle is set.
// Assumes that SPECIAL_NONE means everything beyond is also SPECIAL_NONE.
int updateSpecials(int index)
{
    struct Element* tempElement = a_element[index];
    int tempX = (int) a_x[index];
    int tempY = (int) a_y[index];
    int shouldResolveHeatChanges = FALSE;
    unsigned int i;

    if (!tempElement)
    {
        __android_log_write(ANDROID_LOG_ERROR, "TheElements", "Null tempElement in specials");
    }

    char specialLoopDone = FALSE;
    for (i = MAX_SPECIALS; i != 0; i--)
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

        if(tempElement->specials[MAX_SPECIALS-i] == SPECIAL_NONE)
        {
            return shouldResolveHeatChanges;
        }
        switch((int)tempElement->specials[MAX_SPECIALS-i])
        {
            //Spawn
        case SPECIAL_SPAWN:
        {
            LOGD("Special spawn");
            //frozen[tempParticle] = 0;
            int diffX, diffY;
            int tempAllCoords;
            for (diffX = -2; diffX <= 2; diffX++)
            {
                for (diffY = -2; diffY <= 2; diffY++)
                {
                    if (tempX + diffX >= 0 && tempX + diffX < workWidth && tempY + diffY >= 0 && tempY + diffY < workHeight)
                    {
                        tempAllCoords = allCoords[getIndex(tempX+diffX,tempY+diffY)];
                        if (tempAllCoords < 0 && a_element[index] == elements[GENERATOR_ELEMENT]) //There's a generator adjacent
                        {
                            setElement(tempAllCoords, elements[SPAWN_ELEMENT]);
                            setParticleSpecialVal(tempAllCoords, SPECIAL_SPAWN, getParticleSpecialVal(index, SPECIAL_SPAWN));
                        }
                        else if (!tempAllCoords && rand() % GENERATOR_SPAWN_PROB == 0 && loq < MAX_POINTS - 1) //There's an empty spot
                        {
                            createPoint(tempX + diffX, tempY + diffY, elements[getParticleSpecialVal(index, SPECIAL_SPAWN)]);
                        }
                    }
                }
            }
            break;
        }
        //Breakable
        case SPECIAL_BREAK:
        {
            LOGD("Special break");
            if (a_xVel[index] > getElementSpecialVal(tempElement, SPECIAL_BREAK) || a_yVel[index] > getElementSpecialVal(tempElement, SPECIAL_BREAK))
            {
                setElement(index, elements[NORMAL_ELEMENT]);
            }
            break;
        }
        //Growing
        case SPECIAL_GROW:
        {
            LOGD("Special grow");
            int diffX, diffY;
            int tempAllCoords;
            for (diffX = -1; diffX <= 1; diffX++)
            {
                for (diffY = -1; diffY <= 1; diffY++)
                {
                    if (diffY + tempY >= 0 && tempY + diffY < workHeight && tempX + diffX >= 0 && diffX + diffX < workWidth)
                    {
                        tempAllCoords = allCoords[getIndex(tempX+diffX,tempY+diffY)];
                        if (tempAllCoords != -1 && a_element[tempAllCoords]->index == getElementSpecialVal(tempElement, SPECIAL_GROW) && rand() % 10 == 0)
                        {
                            setElement(tempAllCoords, a_element[tempAllCoords]);
                        }
                    }
                }
            }

            break;
        }
        //Heater
        case SPECIAL_HEAT:
        {
            LOGD("Special heat");
            int diffX, diffY;
            int tempAllCoords;
            if (rand()%5 == 0)
            {
                for (diffX = -1; diffX <= 1; diffX++)
                {
                    for(diffY = -1; diffY <=1; diffY++)
                    {
                        if((diffX!=0||diffY!=0) && tempX+diffX < workWidth && tempX+diffX >= 0 && tempY+diffY < workHeight && tempY+diffY >= 0)
                        {
                            tempAllCoords = allCoords[getIndex(tempX+diffX,tempY+diffY)];
                            if(tempAllCoords != -1)
                            {
                                changeHeat(&(a_heat[tempAllCoords]), getElementSpecialVal(tempElement, SPECIAL_HEAT));
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
            LOGD("Special explode");
            if (a_heat[index] >= a_element[index]->highestTemp) //If the heat is above the threshold
            {
                int diffX, diffY;
                int distance;
                int tempAllCoords;

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
                                    if(diffX != 0 && a_xVel[tempAllCoords] < explosiveness)
                                    {
                                        a_xVel[tempAllCoords] += (2*(diffX > 0)-1);
                                    }
                                    if(diffY != 0 && a_yVel[tempAllCoords] < explosiveness)
                                    {
                                        a_yVel[tempAllCoords] += (2*(diffY > 0)-1);
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
                            if(tempAllCoords != -1)
                            {
                                changeHeat(&(a_heat[tempAllCoords]), 50);
                            }
                        }
                    }
                }

                // Change this particle to fire, and quit the specials loop
                setElement(index, elements[10]);
                specialLoopDone = TRUE;
                break;
            }
            break;

        }
        //Disappearing
        case SPECIAL_LIFE:
        {
            LOGD("Special life");
            if (rand()%getElementSpecialVal(tempElement, SPECIAL_LIFE) == 0)
            {
                deletePoint(index);
            }
            break;
        }
        //Wander
        case SPECIAL_WANDER:
        {
            LOGD("Special wander");
            // Don't wander while tunneling
            // FIXME: This is a hacky solution, come up with something more elegant
            if (getParticleSpecialVal(index, SPECIAL_TUNNEL) != SPECIAL_VAL_UNSET)
            {
                continue;
            }

            int randVal = rand()%100;
            // Randomly wander
            int wanderVal = getElementSpecialVal(tempElement, SPECIAL_WANDER);
            if (randVal <= wanderVal)
            {
                if (a_xVel[index] <= 4)
                {
                    a_xVel[index] += 2;
                }
            }
            else if (randVal >= wanderVal+1 && randVal <= wanderVal*2)
            {
                if (a_xVel[index] >= -4)
                {
                    a_xVel[index] -= 2;
                }
            }

            randVal = rand()%100;
            if (randVal <= wanderVal)
            {
                if (a_yVel[index] >= -4)
                {
                    a_yVel[index] -= 2;
                }
            }
            if (randVal >= wanderVal + 1 && randVal <= wanderVal*2)
            {
                if (a_yVel[index] <= 4)
                {
                    a_yVel[index] += 2;
                }
            }

            break;
        }
        //Jump
        case SPECIAL_JUMP:
        {
            LOGD("Special jump");
            if ((a_y[index]+1 == workHeight) || (allCoords[getIndex(a_x[index], a_y[index]+1)] != -1))
            {
                int randVal = rand()%100;
                int jumpVal = getElementSpecialVal(tempElement, SPECIAL_JUMP);
                if (randVal < jumpVal)
                {
                    a_yVel[index] -= 5;
                }
            }
            break;
        }
        // Tunnel
        case SPECIAL_TUNNEL:
        {
            LOGD("Special tunnel");
            int targetElementIndex = getElementSpecialVal(tempElement, SPECIAL_TUNNEL);
            int state = getParticleSpecialVal(index, SPECIAL_TUNNEL);

            int curX = a_x[index], curY = a_y[index];
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
                int tempAllCoords = allCoords[getIndex(curX+diffX, curY+diffY)];
                if (tempAllCoords != -1 && a_element[tempAllCoords]->index == targetElementIndex)
                {
                    // Remove the tempAllCoords particle, and move this particle there
                    unSetPoint(tempAllCoords);
                    allCoords[getIndex(curX+diffX, curY+diffY)] = index;
                    setBitmapColor(curX+diffX, curY+diffY, tempElement);
                    allCoords[getIndex(curX, curY)] = -1;
                    clearBitmapColor(curX, curY);
                    a_x[index] = curX + diffX;
                    a_y[index] = curY + diffY;
                    // Set the y velocity to the fall velocity to counteract movement
                    a_yVel[index] = a_element[index]->fallVel;
                    setParticleSpecialVal(index, SPECIAL_TUNNEL, randomDir);


                    // Add particles around this point, forming a "tunnel"
                    if (curX+2*diffX >= 0 && curX+2*diffX < workWidth && curY+2*diffY >= 0 && curY+2*diffY < workHeight)
                    {
                        if (allCoords[getIndex(curX+2*diffX, curY+2*diffY)] == -1)
                        {
                            createPoint(curX+2*diffX, curY+2*diffY, elements[targetElementIndex]);
                        }
                        if (allCoords[getIndex(curX+2*diffX, curY+diffY)] == -1)
                        {
                            createPoint(curX+2*diffX, curY+diffY, elements[targetElementIndex]);
                        }
                        if (allCoords[getIndex(curX+diffX, curY+2*diffY)] == -1)
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
                if (allCoords[getIndex(a_oldX[index], a_oldY[index])] == -1)
                {
                    allCoords[getIndex(curX, curY)] = -1;
                    clearBitmapColor(curX, curY);
                    allCoords[getIndex(a_oldX[index], a_oldY[index])] = index;
                    setBitmapColor(a_oldX[index], a_oldY[index], tempElement);

                    curX = a_x[index] = a_oldX[index];
                    curY = a_y[index] = a_oldY[index];
                }

                diffX = 2*(state%2) - 1;
                diffY = (state - state%2) - 1;
                if (curX+diffX < 0 || curX+diffX >= workWidth || curY+diffY < 0 || curY+diffY >= workHeight)
                {
                    // Go back to the unset state
                    setParticleSpecialVal(index, SPECIAL_TUNNEL, SPECIAL_VAL_UNSET);

                    // Look in a random diagonal
                    int randomDir = rand()%4;
                    diffX = 2*(randomDir%2) - 1;
                    diffY = (randomDir - randomDir%2) - 1;
                    if (curX+diffX < 0 || curX+diffX >= workWidth || curY+diffY < 0 || curY+diffY >= workHeight)
                    {
                        continue;
                    }
                    int tempAllCoords = allCoords[getIndex(curX+diffX, curY+diffY)];
                    if (tempAllCoords != -1 && a_element[tempAllCoords]->index == targetElementIndex)
                    {
                        // Remove the tempAllCoords particle, and move this particle there
                        unSetPoint(tempAllCoords);
                        allCoords[getIndex(curX+diffX, curY+diffY)] = index;
                        setBitmapColor(curX+diffX, curY+diffY, tempElement);
                        allCoords[getIndex(curX, curY)] = -1;
                        clearBitmapColor(curX, curY);
                        a_x[index] = curX + diffX;
                        a_y[index] = curY + diffY;
                        // Set the y velocity to the fall velocity to counteract movement
                        a_yVel[index] = a_element[index]->fallVel;
                        setParticleSpecialVal(index, SPECIAL_TUNNEL, randomDir);


                        // Add particles around this point, forming a "tunnel"
                        if (curX+2*diffX >= 0 && curX+2*diffX < workWidth && curY+2*diffY >= 0 && curY+2*diffY < workHeight)
                        {
                            if (allCoords[getIndex(curX+2*diffX, curY+2*diffY)] == -1)
                            {
                                createPoint(curX+2*diffX, curY+2*diffY, elements[targetElementIndex]);
                            }
                            if (allCoords[getIndex(curX+2*diffX, curY+diffY)] == -1)
                            {
                                createPoint(curX+2*diffX, curY+diffY, elements[targetElementIndex]);
                            }
                            if (allCoords[getIndex(curX+diffX, curY+2*diffY)] == -1)
                            {
                                createPoint(curX+diffX, curY+2*diffY, elements[targetElementIndex]);
                            }
                        }
                    }
                }

                int tempAllCoords = allCoords[getIndex(curX+diffX, curY+diffY)];
                if (tempAllCoords == -1 || a_element[tempAllCoords]->index != targetElementIndex)
                {
                    // Go back to the unset state
                    setParticleSpecialVal(index, SPECIAL_TUNNEL, SPECIAL_VAL_UNSET);
                    // Look for any particle back the way we came and (if there is one)
                    // tunnel into it
                    tempAllCoords = allCoords[getIndex(curX, curY+diffY)];
                    if (tempAllCoords != -1 && a_element[tempAllCoords]->index == targetElementIndex)
                    {
                        // Remove the tempAllCoords particle, and move this particle there
                        unSetPoint(tempAllCoords);
                        allCoords[getIndex(curX, curY+diffY)] = index;
                        setBitmapColor(curX, curY+diffY, tempElement);
                        allCoords[getIndex(curX, curY)] = -1;
                        clearBitmapColor(curX, curY);
                        a_x[index] = curX;
                        a_y[index] = curY + diffY;
                    }
                    else
                    {
                        tempAllCoords = allCoords[getIndex(curX+diffX, curY)];
                        if (tempAllCoords != -1 && a_element[tempAllCoords]->index == targetElementIndex)
                        {
                            // Remove the tempAllCoords particle, and move this particle there
                            unSetPoint(tempAllCoords);
                            allCoords[getIndex(curX+diffX, curY)] = index;
                            setBitmapColor(curX+diffX, curY, tempElement);
                            allCoords[getIndex(curX, curY)] = -1;
                            clearBitmapColor(curX, curY);
                            a_x[index] = curX + diffX;
                            a_y[index] = curY;
                        }
                    }
                    // Add velocity so that the particle stays still for one step
                    a_yVel[index] = a_element[index]->fallVel;
                    continue;
                }
                else
                {
                    // Remove the tempAllCoords particle, and move this particle there
                    unSetPoint(tempAllCoords);
                    allCoords[getIndex(curX+diffX, curY+diffY)] = index;
                    setBitmapColor(curX+diffX, curY+diffY, tempElement);
                    allCoords[getIndex(curX, curY)] = -1;
                    clearBitmapColor(curX, curY);
                    a_x[index] = curX + diffX;
                    a_y[index] = curY + diffY;
                    // Set the y velocity to the fall velocity to counteract movement
                    a_yVel[index] = a_element[index]->fallVel;


                    // Add particles around this point, forming a "tunnel"
                    if (curX+2*diffX >= 0 && curX+2*diffX < workWidth && curY+2*diffY >= 0 && curY+2*diffY < workHeight)
                    {
                        if (allCoords[getIndex(curX+2*diffX, curY+2*diffY)] == -1)
                        {
                            createPoint(curX+2*diffX, curY+2*diffY, elements[targetElementIndex]);
                        }
                        if (allCoords[getIndex(curX+2*diffX, curY+diffY)] == -1)
                        {
                            createPoint(curX+2*diffX, curY+diffY, elements[targetElementIndex]);
                        }
                        if (allCoords[getIndex(curX+diffX, curY+2*diffY)] == -1)
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
            LOGD("Special burn");
            // Burn doesn't trigger when explode is set
            if (getElementSpecialVal(tempElement, SPECIAL_EXPLODE) != SPECIAL_VAL_UNSET)
            {
                continue;
            }

            int state = getParticleSpecialVal(index, SPECIAL_BURN);
            // Not burning yet
            if (state == SPECIAL_VAL_UNSET)
            {
                //If the heat is above the threshold
                if (a_heat[index] >= a_element[index]->highestTemp)
                {
                    int avgLife = getElementSpecialVal(tempElement, SPECIAL_BURN);
                    int life = avgLife + rand()%5 - 2;
                    if (life < 1) { life = 1; }
                    setParticleSpecialVal(index, SPECIAL_BURN, life);
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
                int tempAllCoords;
                for (diffX = -1; diffX <= 1; diffX++)
                {
                    for(diffY = -1; diffY <=1; diffY++)
                    {
                        if((diffX!=0||diffY!=0) && tempX+diffX < workWidth && tempX+diffX >= 0 && tempY+diffY < workHeight && tempY+diffY >= 0)
                        {
                            tempAllCoords = allCoords[getIndex(tempX+diffX,tempY+diffY)];
                            if(tempAllCoords != -1)
                            {
                                changeHeat(&(a_heat[tempAllCoords]),
                                        1000/(state*getElementSpecialVal(tempElement, SPECIAL_BURN)));
                            }
                        }
                    }
                }

                // If we're done with our life total
                if (state <= 1)
                {
                    // Change to fire
                    setElement(index, elements[FIRE_ELEMENT]);
                    //deletePoint(index);
                    specialLoopDone = TRUE;
                }
                else
                {
                    // Decrement life total left
                    setParticleSpecialVal(index, SPECIAL_BURN, state-1);

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
            int tempAllCoords;
            int curX = a_x[index], curY = a_y[index];
            property = getParticleSpecialVal(index, SPECIAL_CONDUCTIVE);
            if (property != SPECIAL_VAL_UNSET)
            {
                // Wait one frame, remove the electric wait bit
                if (property & ELECTRIC_WAIT)
                {
                    setBitmapColor(curX,curY,elements[ELECTRICITY_ELEMENT]);
                    setParticleSpecialVal(index, SPECIAL_CONDUCTIVE, property & 15); //15 == 00001111
                    break;
                }
                // If no direction, select a random direction
                if (property == ELECTRIC_NO_DIR)
                {
                    // 0 or 1 in XN, 1 in X1
                    property = (((rand()%2) << 2) & ELECTRIC_XN) | ELECTRIC_X1;
                }
                int i = (property & ELECTRIC_X1) * ((ELECTRIC_XN & property) ? -1 : 1);
                int j = ((property & ELECTRIC_Y1) >> 1) * ((ELECTRIC_YN & property) ? -1 : 1);

                int tempI, k, transfered = FALSE;
                for (k = 0; k < 4; k++)
                {
                    if (curX + i <= workWidth && curX + i > 0 && curY + j <= workHeight && curY + j > 0)
                    {
                        tempAllCoords = allCoords[getIndex(curX+i,curY+j)];
                        if (tempAllCoords != -1)
                        {
                            if(hasSpecial(tempAllCoords, SPECIAL_CONDUCTIVE))
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
                            if (tempAllCoords != -1)
                            {
                                if(hasSpecial(tempAllCoords, SPECIAL_CONDUCTIVE))
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
                            if (tempAllCoords == -1)
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
                                if (tempAllCoords == -1)
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
                setParticleSpecialVal(index, SPECIAL_CONDUCTIVE, SPECIAL_VAL_UNSET);
                setBitmapColor(curX, curY, a_element[index]);
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
            int curX = a_x[index], curY = a_y[index];
            // Seed the rand function with the current location
            // (plus a randomizer that doesn't change very often)
            srand(curX*curY + randOffset);
            // Add random velocity based on seed (so the same location
            // will get the same velocity boost)
            a_xVel[index] += rand()%variability - variability/2;

            break;
        }

        //Default: do nothing
        default:
            break;
        }

        //__android_log_write(ANDROID_LOG_INFO, "LOG", "End special");

        if (specialLoopDone == TRUE)
        {
            break;
        }
    }

    return shouldResolveHeatChanges;
}


void UpdateView(void)
{
    //For speed we're going to create temp variables to store stuff
    int tempOldX, tempOldY;
    float *tempX, *tempY;
    short *tempXVel, *tempYVel;
    char tempInertia;
    int tempParticle;
    int tempAllCoords;
    struct Element* tempElement;
    struct Element* tempElement2;
    
    //current index
    int ci;
    //Used for heat
    int heatChange;

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
        drawPoints();
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
        for (tempParticle = 0; tempParticle < MAX_POINTS; ++tempParticle)
        {
            //If the particle is set and unfrozen
            if (a_set[tempParticle])// && tempParticle->frozen < 4)
            {
                //__android_log_write(ANDROID_LOG_INFO, "TheElements", "Processing a set particle");
                //TODO: Life property cycle

                //Set the temp and old variables
                tempX = &(a_x[tempParticle]);
                tempY = &(a_y[tempParticle]);
                tempOldX = (int) a_x[tempParticle];
                tempOldY = (int) a_y[tempParticle];
                a_oldX[tempParticle] = tempOldX;
                a_oldY[tempParticle] = tempOldY;
                tempElement = a_element[tempParticle];
                tempInertia = tempElement->inertia;
                tempXVel = &(a_xVel[tempParticle]);
                tempYVel = &(a_yVel[tempParticle]);

                //Update coords
                if(tempInertia != INERTIA_UNMOVABLE)
                {
                    if (!updateKinetic(tempParticle))
                    {
                        // If we ended up deleting the particle, continue
                        continue;
                    }

                    //Updating allCoords and collision resolution
                    tempAllCoords = allCoords[getIndex((int)(*tempX), (int)(*tempY))];

                    //If the space the particle is trying to move to is taken and isn't itself
                    if (tempAllCoords != -1 && tempAllCoords != tempParticle)
                    {
                        tempElement2 = a_element[tempAllCoords];

                        //Update heat
                        updateCollisionHeat(tempParticle, tempAllCoords);

                        //Resolve the collision (this updates the state of the particle, but lets this function resolve later)
                        collide(tempParticle, tempAllCoords);

                        //Update the particles and the bitmap colors if the hasMoved flag is set
                        if(a_hasMoved[tempParticle])
                        {
                            allCoords[getIndex(tempOldX, tempOldY)] = -1;
                            clearBitmapColor(tempOldX, tempOldY);
                            allCoords[getIndex((int)(*tempX), (int)(*tempY))] = tempParticle;
                            setBitmapColor((int)(*tempX), (int)(*tempY), a_element[tempParticle]);

                            //unFreezeParticles(tempOldX, tempOldY);
                            a_hasMoved[tempParticle] = FALSE;
                        }
                        if(a_hasMoved[tempAllCoords])
                        {
                            allCoords[getIndex(a_oldX[tempAllCoords], a_oldY[tempAllCoords])] = -1;
                            clearBitmapColor(a_oldX[tempAllCoords], a_oldY[tempAllCoords]);
                            allCoords[getIndex((int)a_x[tempAllCoords], (int)a_y[tempAllCoords])] = tempAllCoords;
                            setBitmapColor(a_x[tempAllCoords], a_y[tempAllCoords], a_element[tempAllCoords]);

                            //unFreezeParticles(tempOldX, tempOldY);
                            a_hasMoved[tempAllCoords] = FALSE;
                        }
                    }
                    //Space particle is trying to move to is free
                    else if (tempAllCoords != tempParticle)
                    {
                        allCoords[getIndex(tempOldX, tempOldY)] = -1;
                        clearBitmapColor(tempOldX, tempOldY);
                        allCoords[getIndex((int)(*tempX), (int)(*tempY))] = tempParticle;
                        setBitmapColor((int)(*tempX), (int)(*tempY), a_element[tempParticle]);

                        a_hasMoved[tempParticle] = FALSE;
                    }
                    //Space particle is trying to move to is itself
                    else
                    {
                        a_hasMoved[tempParticle] = FALSE;
                    }

                    //__android_log_write(ANDROID_LOG_INFO, "LOG", "End resolve collisions");
                }
                //Inertia unmovable object still need to deal with velocities
                else
                {
                    //Reduce velocities
                    if(*tempXVel < 0)
                    {
                        (*tempXVel)++;
                    }
                    else if(*tempXVel > 0)
                    {
                        (*tempXVel)--;
                    }
                    if(*tempYVel < 0)
                    {
                        (*tempYVel)++;
                    }
                    else if(*tempYVel > 0)
                    {
                        (*tempYVel)--;
                    }
                }

                //Update heat
                char *heat = &(a_heat[tempParticle]);
                if(*heat != cAtmosphere->heat)
                {
                    if(rand() % ((3 - tempElement->state)*16)  != 0)
                    {
                        heatChange = 0;
                    }
                    else
                    {
                        heatChange = (*heat - cAtmosphere->heat)/16 + rand()%3 - 1;
                    }
                    //If tempParticle is hotter than the atmosphere, we want to subtract temperature
                    changeHeat(heat, -heatChange);
                }

                //__android_log_write(ANDROID_LOG_INFO, "LOG", "End update heat");

                int shouldResolveHeatChanges = updateSpecials(tempParticle);

                //__android_log_write(ANDROID_LOG_INFO, "LOG",  "End specials loop");

                //Resolve heat changes
                if (shouldResolveHeatChanges)
                {
                    if(*heat < a_element[tempParticle]->lowestTemp)
                    {
                        setElement(tempParticle, a_element[tempParticle]->lowerElement);
                    }
                    else if(*heat > a_element[tempParticle]->highestTemp)
                    {
                        setElement(tempParticle, a_element[tempParticle]->higherElement);
                    }
                }


                //__android_log_write(ANDROID_LOG_INFO, "LOG", "End resolve heat changes");
            }
        }
        //__android_log_write(ANDROID_LOG_INFO, "TheElements", "All particles done");
    }
}

void *updateThreadFunc(void *args)
{
    while (!shouldKillUpdateThread)
    {
        pthread_mutex_lock(&update_mutex);
        UpdateView();
        pthread_mutex_unlock(&update_mutex);

        // Synchronization
        pthread_mutex_lock(&buffer_free_mutex);
        while (!bufferFree)
        {
            pthread_cond_wait(&buffer_free_cond, &buffer_free_mutex);
        }
        bufferFree = FALSE;
        pthread_mutex_unlock(&buffer_free_mutex);

        // Copy the frame into the colorsFrameBuffer
        pthread_mutex_lock(&update_mutex);
        memcpy(colorsFrameBuffer, colors, 3 * stupidTegra * workHeight);
        pthread_mutex_unlock(&update_mutex);

        pthread_mutex_lock(&frame_ready_mutex);
        if (!frameReady)
        {
            frameReady = TRUE;
            pthread_cond_signal(&frame_ready_cond);
        }
        pthread_mutex_unlock(&frame_ready_mutex);
    }
}

void startUpdateThread()
{
    // Only start the thread if one doesn't exist already
    if (shouldKillUpdateThread)
    {
        shouldKillUpdateThread = FALSE;
        pthread_create(&updateThread, NULL, &updateThreadFunc, NULL);
    }
}

void killUpdateThread()
{
    // Only kill the thread if it's not already dead
    if (!shouldKillUpdateThread)
    {
        shouldKillUpdateThread = TRUE;
        pthread_join(updateThread, NULL);
    }
}
