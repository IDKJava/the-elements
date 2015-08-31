/*
 * update.c
 * -----------------------------------
 * Defines the function UpdateView(), which
 * is called every frame to update all the
 * particles' positions.
 */

#include "update.h"

#include <sched.h>

#include "specials.h"
#include "gravity.h"

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

inline void brushSetPtVel(int pt) {
    if (brushProps == BRUSH_NORMAL) {
        // -1.0 to 1.0
        a_xVel[pt] = ((rand()%11)-5)*0.2;
        a_yVel[pt] = ((rand()%11)-5)*0.2;
    }
    else if (brushProps == BRUSH_SPRAY) {
        // -5.0 to 5.0
        a_xVel[pt] = ((rand()%11)-5);
        a_yVel[pt] = ((rand()%11)-5);
    }
    else if (brushProps == BRUSH_SLINGSHOT) {
        a_xVel[pt] = slingshotX;
        a_yVel[pt] = slingshotY;
    }
}


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
                                int pt = createPoint(mx + dx, my + dy, cElement);
                                if (pt >= 0) {
                                    brushSetPtVel(pt);
                                }
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
                    int pt = createPoint(x, y, cElement);
                    if (pt >= 0) {
                        brushSetPtVel(pt);
                    }
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
                    int pt = createPoint(x, y, cElement);
                    if (pt >= 0) {
                        brushSetPtVel(pt);
                    }
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
                    int pt = createPoint(x, y, cElement);
                    if (pt >= 0) {
                        brushSetPtVel(pt);
                    }
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
void updateVelocities(float *xvel, float *yvel, float inertia, float lowDragThresh)
{
    if(*xvel < -lowDragThresh)
    {
        *xvel += inertia;
        if(*xvel > 0)
        {
            *xvel = 0;
        }
    }
    else if(*xvel > lowDragThresh)
    {
        *xvel -= inertia;
        if(*xvel < 0)
        {
            *xvel = 0;
        }
    }
    // Update y vel based on inertia, always approaching 0
    if(*yvel < -lowDragThresh)
    {
        *yvel += inertia;
        if (*yvel > 0)
        {
            *yvel = 0;
        }
    }
    else if(*yvel > lowDragThresh)
    {
        *yvel -= inertia;
        if(*yvel < 0)
        {
            *yvel = 0;
        }
    }
}

// Update the positions of the particle
// Returns: TRUE if we should continue with the particle,
//          FALSE if we deleted it.
int updateKinetic(int index)
{
    float x = a_x[index];
    float y = a_y[index];
    float xvel = a_xVel[index];
    float yvel = a_yVel[index];
    float diffx, diffy;

    int fallVel = a_element[index]->fallVel;
    float gx, gy, gmag;
    getFallField(x, y, &gx, &gy, &gmag);
    float gravX = gx*gmag*fallVel, gravY = gy*gmag*fallVel;
    // How much the gravitational field couples to velocity
    float velFactor = (world == WORLD_SPACE) ? 0.2 : 0.0;
    diffy = (1.0-velFactor)*gravY + yvel;
    diffx = (1.0-velFactor)*gravX + xvel;
    // Add scaled acceleration factor. Acceleration adds less at higher velocities
    // (heuristic drag factor).
    if (gy > 0.0 && velFactor*gravY > yvel) {
        yvel += velFactor*gravY;
    }
    else if (gy < 0.0 && velFactor*gravY < yvel) {
        yvel += velFactor*gravY;
    }
    if (gx > 0.0 && velFactor*gravX > xvel) {
        xvel += velFactor*gravX;
    }
    else if (gx < 0.0 && velFactor*gravX < xvel) {
        xvel += velFactor*gravX;
    }

    //Boundary checking
    if (!checkBoundariesAndMove(&x, &y, diffx, diffy))
    {
        // Delete the particle
        deletePoint(index);
        return FALSE;
    }
    //Reduce velocities
    float inertiaScale = (world == WORLD_SPACE) ? 0.3 : 1.0;
    float lowDragThresh = (world == WORLD_SPACE) ? 0.4 : 0.0;
    updateVelocities(&xvel, &yvel, inertiaScale*a_element[index]->inertia, lowDragThresh);

    // Write-back
    a_xVel[index] = xvel;
    a_yVel[index] = yvel;
    a_x[index] = x;
    a_y[index] = y;

    //Indicate that the particle has moved
    a_hasMoved[index] = TRUE;

    return TRUE;
}

// Update the heat when p1 collides into p2
void updateCollisionHeat(int index1, int index2)
{
    unsigned char *p1heat = &(a_heat[index1]);
    unsigned char *p2heat = &(a_heat[index2]);

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
        LOGE("Null tempElement in specials");
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
            specialSpawn(index);
            break;
        }
        //Breakable
        case SPECIAL_BREAK:
        {
            specialBreak(index);
            break;
        }
        //Growing
        case SPECIAL_GROW:
        {
            specialGrow(index);
            break;
        }
        //Heater
        case SPECIAL_HEAT:
        {
            specialHeat(index);
            break;
        }
        //Explosive
        case SPECIAL_EXPLODE:
        {
            specialExplode(index);
            break;

        }
        //Disappearing
        case SPECIAL_LIFE:
        {
            specialLife(index);
            break;
        }
        //Wander
        case SPECIAL_WANDER:
        {
            specialWander(index);
            break;
        }
        //Jump
        case SPECIAL_JUMP:
        {
            specialJump(index);
            break;
        }
        // Tunnel
        case SPECIAL_TUNNEL:
        {
            specialTunnel(index);
            break;
        }
        // Burn
        // FIXME: This very rarely causes stuck particles
        case SPECIAL_BURN:
        {
            shouldResolveHeatChanges = (shouldResolveHeatChanges && specialBurn(index));
            break;
        }
        // Conduct
        case SPECIAL_CONDUCTIVE:
        {
            specialConductive(index);
            break;
        }
        // Trail
        case SPECIAL_TRAIL:
        {
            specialTrail(index);
            break;
        }

        //Default: do nothing
        default:
            break;
        }

        // If we changed elements, exit the loop
        if (a_element[index] != tempElement)
        {
            break;
        }
    }

    return shouldResolveHeatChanges;
}

// hashText should be a preallocated char[9] (last byte will be NUL)
inline unsigned long hashSurround(int tempX, int tempY, char* hashText) {
    // Hash surroundings (topLeft -> CCW)
    bool left = (tempX-1 >= 0),
        right = (tempX+1 < workWidth),
        top = (tempY-1 >= 0),
        bottom = (tempY+1 < workHeight);
    int part;
    if (left && top) {
        part = allCoords[getIndex(tempX-1, tempY-1)];
        hashText[0] = (part == -1) ? 0xff : a_element[part]->index & 0xff;
    }
    else {
        hashText[0] = 0xff;
    }
    if (left) {
        part = allCoords[getIndex(tempX-1, tempY)];
        hashText[1] = (part == -1) ? 0xff : a_element[part]->index & 0xff;
    }
    else {
        hashText[1] = 0xff;
    }
    if (left && bottom) {
        part = allCoords[getIndex(tempX-1, tempY+1)];
        hashText[2] = (part == -1) ? 0xff : a_element[part]->index & 0xff;
    }
    else {
        hashText[2] = 0xff;
    }
    if (bottom) {
        part = allCoords[getIndex(tempX, tempY+1)];
        hashText[3] = (part == -1) ? 0xff : a_element[part]->index & 0xff;
    }
    else {
        hashText[3] = 0xff;
    }
    if (right && bottom) {
        part = allCoords[getIndex(tempX+1, tempY+1)];
        hashText[4] = (part == -1) ? 0xff : a_element[part]->index & 0xff;
    }
    else {
        hashText[4] = 0xff;
    }
    if (right) {
        part = allCoords[getIndex(tempX+1, tempY)];
        hashText[5] = (part == -1) ? 0xff : a_element[part]->index & 0xff;
    }
    else {
        hashText[5] = 0xff;
    }
    if (right && top) {
        part = allCoords[getIndex(tempX+1, tempY-1)];
        hashText[6] = (part == -1) ? 0xff : a_element[part]->index & 0xff;
    }
    else {
        hashText[6] = 0xff;
    }
    if (top) {
        part =  allCoords[getIndex(tempX, tempY-1)];
        hashText[7] = (part == -1) ? 0xff : a_element[part]->index & 0xff;
    }
    else {
        hashText[7] = 0xff;
    }
    hashText[8] = 0;

    return hashStr(hashText);
}


void UpdateView(void)
{
    //For speed we're going to create temp variables to store stuff
    float tempOldX, tempOldY;
    float *tempX, *tempY;
    float *tempXVel, *tempYVel;
    unsigned char tempInertia;
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
    pthread_mutex_lock(&brush_mutex);
    if (brushNextLocX >= 0) {
        // Draw a line to next brush position
        drawCircleyLine(brushLocX, brushLocY, brushNextLocX, brushNextLocY);
        brushLocX = brushNextLocX;
        brushLocY = brushNextLocY;
        brushNextLocX = -1;
        brushNextLocY = -1;
    }
    else if (brushOn) {
        // Continuous draw
        drawCircle(brushLocX, brushLocY);
    }
    pthread_mutex_unlock(&brush_mutex);

    // Compute the empty hash, so we never freeze on this
    char txt[9];
    txt[0] = txt[1] = txt[2] = txt[3] = txt[4] = txt[5] = txt[6] = txt[7] = 0xff;
    txt[8] = 0;
    unsigned long emptyHash = hashStr(txt);


    //Particles update
    if (play)
    {
        // Change the random offset every so often (used in Trail)
        if (rand()%1000 == 1)
        {
            randOffset = rand();
        }

        //Physics update
        char hashText[9];
        for (tempParticle = 0; tempParticle < MAX_POINTS; ++tempParticle)
        {
            //If the particle is set and unfrozen
            if (a_set[tempParticle])// && tempParticle->frozen < 4)
            {
                //TODO: Life property cycle

                //Set the temp and old variables
                tempX = &(a_x[tempParticle]);
                tempY = &(a_y[tempParticle]);
                tempOldX = a_x[tempParticle];
                tempOldY = a_y[tempParticle];
                tempElement = a_element[tempParticle];
                tempInertia = tempElement->inertia;
                tempXVel = &(a_xVel[tempParticle]);
                tempYVel = &(a_yVel[tempParticle]);

                unsigned long h = hashSurround(*tempX, *tempY, hashText);

                //Update coords
                if(tempInertia != INERTIA_UNMOVABLE && (a_frozen[tempParticle] != h || h == emptyHash))
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
                        collide(tempParticle, tempAllCoords, tempOldX, tempOldY);

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
                            if (allCoords[getIndex(tempOldX, tempOldY)] == tempAllCoords)
                            {
                                allCoords[getIndex(tempOldX, tempOldY)] = -1;
                                clearBitmapColor(tempOldX, tempOldY);
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

                    // Maybe freeze
                    if ((int)tempOldX == (int)*tempX &&
                        (int)tempOldY == (int)*tempY && rand()%10 == 0) {
                        a_frozen[tempParticle] = h;
                    }

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

                //Check for particle destroyed
                if (!a_set[tempParticle])
                {
                    continue;
                }

                //Update heat
                unsigned char *heat = &(a_heat[tempParticle]);
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
                        if (tempLowerElement->inertia == INERTIA_UNMOVABLE)
                        {
                            //Don't go to more solid element if the particle is moving
                            if (fabs(*tempX-tempOldX) < 0.5 && fabs(*tempY-tempOldY) < 0.5 &&
                                *tempXVel < 0.5 && *tempYVel < 0.5)
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
                        if (tempHigherElement->inertia == INERTIA_UNMOVABLE)
                        {
                            //Don't go to more solid element if the particle is moving
                            if (fabs(*tempX-tempOldX) < 0.5 && fabs(*tempY-tempOldY) < 0.5 &&
                                *tempXVel < 0.5 && *tempYVel < 0.5)
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
        //pthread_mutex_lock(&buffer_free_mutex);
        while (!bufferFree)
        {
            //pthread_cond_wait(&buffer_free_cond, &buffer_free_mutex);
            sched_yield();
        }
        bufferFree = FALSE;
        //pthread_mutex_unlock(&buffer_free_mutex);

        // Copy the frame into the colorsFrameBuffer
        //pthread_mutex_lock(&update_mutex);

            /*
             * This does a motion blur effect by composing previous value
             * and new value together.  Checks for if current color is background
             * color so that particles only fade out, not fade in.
             */
        /*
        switch(filterType) {
        case FILTER_NONE:
          memcpy(colorsFrameBuffer, colors, 3 * stupidTegra * workHeight);
          break;
        case FILTER_MOTION:
        {
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
        */
        //pthread_mutex_unlock(&update_mutex);

        //pthread_mutex_lock(&frame_ready_mutex);
        if (!frameReady)
        {
            frameReady = TRUE;
            //pthread_cond_signal(&frame_ready_cond);
        }
        //pthread_mutex_unlock(&frame_ready_mutex);
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
