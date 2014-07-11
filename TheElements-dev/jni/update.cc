/*
 * update.c
 * -----------------------------------
 * Defines the function UpdateView(), which
 * is called every frame to update all the
 * particles' positions.
 */

#include "update.h"

#include <android/log.h>

#include "specials.h"

// Per-file logging
#ifndef NDEBUG
//Debug
#define LOGGING 0
#else
//Release
#define LOGGING 0
#endif

#define HEAT_CHANGE_PROB 10 //Signifies 1/number

static int shouldKillUpdateThread = TRUE;
pthread_t updateThread;


// Draws a circle of particles around (mx, my) using brushSize as the radius.
void drawCircle(int mx, int my)
{
    int dx, dy;
    for (dy = brushSize; dy >= -brushSize; dy--)
    {
        for (dx = -brushSize; dx <= brushSize; dx++)
        {
            // Circle
            if ((dx * dx) + (dy * dy) <= (brushSize * brushSize))
            {
                if ( dx + mx < workWidth && dx + mx >= 0 && dy + my < workHeight && dy + my >= 0 )
                    //Normal drawing
                {
                    if (cElement->index >= NORMAL_ELEMENT)
                    {
                        //Draw it solid
                        if(cElement->inertia == INERTIA_UNMOVABLE || cElement->index == ELECTRICITY_ELEMENT)
                        {
                            if (allCoords[getIndex((int) (dx + mx), (int) (dy + my))] == -1)
                            {
                                createPoint(mx + dx, my + dy, cElement);
                            }
                        }
                        //Draw it randomized
                        else
                        {
                            if (rand() % 3 == 1 && allCoords[getIndex((int) (dx + mx), (int) (dy + my))] == -1)
                            {
                                createPoint(mx + dx, my + dy, cElement);
                            }
                        }
                    }
                    //Special Eraser case
                    else if (cElement->index == ERASER_ELEMENT)
                    {
                        if (allCoords[getIndex((int) (dx + mx), (int) (dy + my))] != -1)
                        {
                            deletePoint(allCoords[getIndex(mx + dx, my + dy)]);
                        }
                    }
                }
            }
        }
    }
}


// Draws a "circley" line from (startx, starty) to (endx, endy) using brushSize
// as the radius.
// startx, starty, endx, endy are all assumed to be within screen boundaries
void drawCircleyLine(int startx, int starty, int endx, int endy)
{
    int x, y;
    // First, draw a circle at the start and end
    drawCircle(startx, starty);
    drawCircle(endx, endy);

    // Now iterate over points in the relevant rectangular space, and create points in
    // unoccupied locations that fall in the right area.
    if (startx == endx) // Handle vertical case specially, since we can't compute slope
    {
        int left = startx - brushSize;
        int right = startx + brushSize;
        int top = starty > endy ? endy : starty;
        int bottom = starty > endy ? starty : endy;

        // Boundary checks (only left and right needed)
        if (left < 0) left = 0;
        if (right >= workWidth) right = workWidth-1;
        if (bottom >= workHeight) bottom = workHeight-1;
        if (top < 0 ) top = 0;

        for (y = bottom; y >= top; --y)
        {
            for (x = left; x <= right; ++x)
            {
                if (allCoords[getIndex(x, y)] == -1 && cElement->index >= NORMAL_ELEMENT)
                {
                    createPoint(x, y, cElement);
                }
                else if (allCoords[getIndex(x, y)] != -1 && cElement->index == ERASER_ELEMENT)
                {
                    deletePoint(allCoords[getIndex(x, y)]);
                }
            }
        }
    }
    else if (starty == endy) // Handle horizontal case specially
    {
        int left = startx > endx ? endx : startx;
        int right = startx > endx ? startx : endx;
        int top = starty - brushSize;
        int bottom = starty + brushSize;

        // Boundary checks (only top and bottom needed)
        if (top < 0) top = 0;
        if (bottom >= workHeight) bottom = workHeight-1;

        for (y = bottom; y >= top; --y)
        {
            for (x = left; x <= right; ++x)
            {
                if (allCoords[getIndex(x, y)] == -1 && cElement->index >= NORMAL_ELEMENT)
                {
                    createPoint(x, y, cElement);
                }
                else if (allCoords[getIndex(x, y)] != -1 && cElement->index == ERASER_ELEMENT)
                {
                    deletePoint(allCoords[getIndex(x, y)]);
                }
            }
        }
    }
    else
    {
        double slope = (endy - starty) / (double)(endx - startx);
        double inv_slope = -1/slope;

        double dx = brushSize / sqrt(1 + inv_slope*inv_slope);
        double dy = dx * inv_slope;

        // We need three points to constrain the rectangle
        int start_top_x, start_top_y, start_bottom_x, start_bottom_y, end_bottom_x, end_bottom_y;

        if (dy > 0)
        {
            start_top_x = startx + dx;
            start_top_y = starty + dy;
            start_bottom_x = startx - dx;
            start_bottom_y = starty - dy;
            end_bottom_x = endx - dx;
            end_bottom_y = endy - dy;
        }
        else
        {
            start_top_x = startx - dx;
            start_top_y = starty - dy;
            start_bottom_x = startx + dx;
            start_bottom_y = starty + dy;
            end_bottom_x = endx + dx;
            end_bottom_y = endy + dy;
        }

        int top = starty > endy ? endy - brushSize : starty - brushSize;
        int bottom = starty > endy ? starty + brushSize : endy + brushSize;
        int left = startx > endx ? endx - brushSize: startx - brushSize;
        int right = startx > endx ? startx + brushSize : endx + brushSize;

        // Boundary checks
        if (top < 0) top = 0;
        if (bottom >= workHeight) bottom = workHeight-1;
        if (left < 0) left = 0;
        if (right >= workWidth) right = workWidth-1;

        for (y = bottom; y >= top; --y)
        {
            for (x = left; x <= right; ++x)
            {
                // Check top and bottom constraints
                if (!((start_top_y + (x - start_top_x) * slope >= y)
                        && (start_bottom_y + (x - start_bottom_x) * slope <= y)))
                    continue;

                // Check left and right constraints
                if (startx > endx) // Start on the right
                {
                    if (!((start_top_x + (y - start_top_y) / inv_slope >= x)
                            && (end_bottom_x + (y - end_bottom_y) / inv_slope <= x)))
                        continue;
                }
                else // Start on the left
                {
                    if (!((start_top_x + (y - start_top_y) / inv_slope <= x)
                            && (end_bottom_x + (y - end_bottom_y) / inv_slope >= x)))
                        continue;
                }

                // We passed all constraints
                if (allCoords[getIndex(x, y)] == -1 && cElement->index >= NORMAL_ELEMENT)
                {
                    createPoint(x, y, cElement);
                }
                else if (allCoords[getIndex(x, y)] != -1 && cElement->index == ERASER_ELEMENT)
                {
                    deletePoint(allCoords[getIndex(x, y)]);
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
void updateCollisionHeat(int index1, int index2)
{
    char *p1heat = &(a_heat[index1]);
    char *p2heat = &(a_heat[index2]);

    int heatChange = (*p1heat - *p2heat)/5;
    //The hotter particle should be cooled, while the cooler particle is heated
    changeHeat(p1heat, -heatChange);
    changeHeat(p2heat, heatChange);
}

// Perform specials actions
// Assumes particle is set.
// Assumes that SPECIAL_NONE means everything beyond is also SPECIAL_NONE.
int updateSpecials(int index)
{
    struct Element* tempElement = a_element[index];
    int tempX = (int) a_x[index];
    int tempY = (int) a_y[index];
    int shouldResolveHeatChanges = TRUE;
    unsigned int i;

    if (!tempElement)
    {
        __android_log_write(ANDROID_LOG_ERROR, "TheElements", "Null tempElement in specials");
        return shouldResolveHeatChanges;
    }

    char specialLoopDone = FALSE;
    for (i = MAX_SPECIALS; i != 0; i--)
    {
        // Short-circuit if we see SPECIAL_NONE
        if(tempElement->specials[MAX_SPECIALS-i] == SPECIAL_NONE)
        {
            break;
        }

        switch(tempElement->specials[MAX_SPECIALS-i])
        {
            //Spawn
        case SPECIAL_SPAWN:
        {
            LOGD("Special spawn");
            specialSpawn(index);
            break;
        }
        //Breakable
        case SPECIAL_BREAK:
        {
            LOGD("Special break");
            specialBreak(index);
            break;
        }
        //Growing
        case SPECIAL_GROW:
        {
            LOGD("Special grow");
            specialGrow(index);
            break;
        }
        //Heater
        case SPECIAL_HEAT:
        {
            LOGD("Special heat");
            specialHeat(index);
            break;
        }
        //Explosive
        case SPECIAL_EXPLODE:
        {
            LOGD("Special explode");
            specialExplode(index);
            break;

        }
        //Disappearing
        case SPECIAL_LIFE:
        {
            LOGD("Special life");
            specialLife(index);
            break;
        }
        //Wander
        case SPECIAL_WANDER:
        {
            LOGD("Special wander");
            specialWander(index);
            break;
        }
        //Jump
        case SPECIAL_JUMP:
        {
            LOGD("Special jump");
            specialJump(index);
            break;
        }
        // Tunnel
        case SPECIAL_TUNNEL:
        {
            LOGD("Special tunnel");
            specialTunnel(index);
            break;
        }
        // Burn
        // FIXME: This very rarely causes stuck particles
        case SPECIAL_BURN:
        {
            LOGD("Special burn");
            shouldResolveHeatChanges = (shouldResolveHeatChanges && specialBurn(index));
            break;
        }
        // Conduct
        case SPECIAL_CONDUCTIVE:
        {
            LOGD("Special conductive");
            specialConductive(index);
            break;
        }
        // Trail
        case SPECIAL_TRAIL:
        {
            LOGD("Special trail");
            specialTrail(index);
            break;
        }

        //Default: do nothing
        default:
            break;
        }

        //__android_log_write(ANDROID_LOG_INFO, "LOG", "End special");

        // If we changed elements, exit the loop
        if (a_element[index] != tempElement)
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
        zoomChanged = FALSE;
    }

    // Clear screen if needed
    if (shouldClear)
    {
        shouldClear = FALSE;
        gameSetup();
        return;
    }

    // Draw update
    pthread_mutex_lock(&mouse_mutex);
    if (fingerDown)
    {
        if ( isPanMode) {
            if (!isPinch) {
                centerX -= mouseX - lastMouseX;
                centerY -= mouseY - lastMouseY;
            }
        }
        else {
            if (lastMouseX < 0 || (lastMouseX == mouseX && lastMouseY == mouseY))
            {
                drawCircle(mouseX, mouseY);
            }
            else
            {
                drawCircleyLine(lastMouseX, lastMouseY, mouseX, mouseY);
            }
        }
        lastMouseX = mouseX;
        lastMouseY = mouseY;
    }
    pthread_mutex_unlock(&mouse_mutex);


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
                            // Clear the old location only if it's still this particle
                            if (allCoords[getIndex(a_oldX[tempAllCoords], a_oldY[tempAllCoords])] == tempAllCoords)
                            {
                                allCoords[getIndex(a_oldX[tempAllCoords], a_oldY[tempAllCoords])] = -1;
                                clearBitmapColor(a_oldX[tempAllCoords], a_oldY[tempAllCoords]);
                            }
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

                int shouldResolveHeatChanges = updateSpecials(tempParticle);

                //Resolve heat changes
                if (shouldResolveHeatChanges)
                {
                    if(*heat < a_element[tempParticle]->lowestTemp)
                    {
                        struct Element* tempLowerElement = a_element[tempParticle]->lowerElement;
                        if (tempLowerElement->state < a_element[tempParticle]->state ||
                                tempLowerElement->inertia == INERTIA_UNMOVABLE)
                        {
                            //Don't go to more solid element if the particle is moving
                            if (*tempX == tempOldX && *tempY == tempOldY &&
                                *tempXVel == 0 && *tempYVel == 0)
                            {
                                if ( rand() % HEAT_CHANGE_PROB == 0 )
                                {
                                    setElement(tempParticle, tempLowerElement);
                                }
                            }
                        }
                        else
                        {
                            setElement(tempParticle, tempLowerElement);
                        }
                    }
                    else if(*heat > a_element[tempParticle]->highestTemp)
                    {
                        struct Element* tempHigherElement = a_element[tempParticle]->higherElement;
                        if (tempHigherElement->state < a_element[tempParticle]->state ||
                                tempHigherElement->inertia == INERTIA_UNMOVABLE)
                        {
                            //Don't go to more solid element if the particle is moving
                            if (*tempX == tempOldX && *tempY == tempOldY &&
                                *tempXVel == 0 && *tempYVel == 0)
                            {
                                if ( rand() % HEAT_CHANGE_PROB == 0 )
                                {
                                    setElement(tempParticle, tempHigherElement);
                                }
                            }
                        }
                        else
                        {
                            setElement(tempParticle, tempHigherElement);
                        }
                    }
                }
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

        switch(filterType) {
        case FILTER_NONE:
          memcpy(colorsFrameBuffer, colors, 3 * stupidTegra * workHeight);
          break;
        case FILTER_MOTION:
        {
            /*
             * This does a motion blur effect by composing previous value
             * and new value together.  Checks for if current color is background
             * color so that particles only fade out, not fade in.
             */
            int i = 0;
            for ( i = 0; i < stupidTegra * workHeight; i++) {
                if ( colorsFrameBuffer[3*i] == cAtmosphere->backgroundRed &&
                     colorsFrameBuffer[3*i+1] == cAtmosphere->backgroundGreen &&
                     colorsFrameBuffer[3*i+2] == cAtmosphere->backgroundBlue ) {
                    colorsFrameBuffer[3*i] = colors[3*i];
                    colorsFrameBuffer[3*i+1] = colors[3*i+1];
                    colorsFrameBuffer[3*i+2] = colors[3*i+2];
                }
                else {
                    colorsFrameBuffer[3*i] = ((float)colorsFrameBuffer[3*i] * 0.8)
                        + ((float)colors[3*i]*0.2);
                    colorsFrameBuffer[3*i+1] = ((float)colorsFrameBuffer[3*i+1] * 0.8)
                        + ((float)colors[3*i+1]*0.2);
                    colorsFrameBuffer[3*i+2] = ((float)colorsFrameBuffer[3*i+2] * 0.8)
                        + ((float)colors[3*i+2]*0.2);
                }
            }
            break;
        }
        }
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
