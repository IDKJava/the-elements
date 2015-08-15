/*
 * setup.c
 * -----------------------------
 * Defines the setup function, which initializes
 * all of the arrays and variables necessary.
 */

#include "setup.h"

#include <android/log.h>

#include "saveload2.h"

// Per-file logging
#ifndef NDEBUG
//Debug
#define LOGGING 1
#else
//Release
#define LOGGING 0
#endif

void gameSetup()
{
    int i, j;
    loq = MAX_POINTS;
    unsigned char backgroundRed = cAtmosphere->backgroundRed;
    unsigned char backgroundGreen = cAtmosphere->backgroundGreen;
    unsigned char backgroundBlue = cAtmosphere->backgroundBlue;


    if (!cElement)
    {
        cElement = elements[NORMAL_ELEMENT];
    }

    //Unset all the particles
    for(i = 0; i < MAX_POINTS; i++)
    {
        avail[i] = i;
        a_set[i] = FALSE;
    }

    //Clear allCoords and our pixels array
    //TODO: This could be faster with pointers
    for(i = 0; i < stupidTegra; i++)
    {
        for(j = 0; j < workHeight; j++)
        {
            if(i < workWidth)
            {
                allCoords[getIndex(i, j)] = -1;
            }
            colors[3 * getColorIndex(i, j)] = backgroundRed;
            colors[3 * getColorIndex(i, j) + 1] = backgroundGreen;
            colors[3 * getColorIndex(i, j) + 2] = backgroundBlue;
        }
    }

    //Clear all space objects
    memset(gravityFieldX, 0, gfWidth*gfHeight*sizeof(float));
    memset(gravityFieldY, 0, gfWidth*gfHeight*sizeof(float));
    memset(gravityMask, 0, gfWidth*gfHeight*sizeof(int));
    numSpaceObjs = 0;
    const float gravXOffset = 0.5*GF_BLOCK_SIZE / (float)workWidth;
    const float gravYOffset = 0.5*GF_BLOCK_SIZE / (float)workHeight;
    // TODO: Add the offsets
    for (int j = 0; j < gfHeight; ++j) {
        for (int i = 0; i < gfWidth; ++i) {
            gravCoords[4*(j*gfWidth+i)] = gravCoords[4*(j*gfWidth+i)+2] =
                i/(float)gfWidth+gravXOffset;
            gravCoords[4*(j*gfWidth+i)+1] = gravCoords[4*(j*gfWidth+i)+3] =
                1.0 - j/(float)gfHeight+gravYOffset;
            gravMag[2*(j*gfWidth+i)] = 0.0;
            gravMag[2*(j*gfWidth+i)+1] = 0.0;
        }
    }
}

//Set up all the variable sized arrays
void arraySetup()
{
    //Make sure everything is deallocated
    free(colors);
    free(colorsFrameBuffer);
    free(allCoords);
    free(gravityFieldX);
    free(gravityFieldY);
    free(gravityMask);
    free(gravCoords);
    free(gravMag);

    //Allocate memory
    colors = (unsigned char*)malloc(3 * stupidTegra * workHeight * sizeof(char));
    colorsFrameBuffer = (unsigned char*)malloc(3 * stupidTegra * workHeight * sizeof(char));
    allCoords = (int*)malloc(workWidth * workHeight * sizeof(int)); //Two dimensional array, so when calling use allcoords[getIndex(x, y)];
    // Two dimensional array, use getGravityIndex(x,y) for indexing
    gravityFieldX = (float*)malloc(gfWidth*gfHeight*sizeof(float));
    gravityFieldY = (float*)malloc(gfWidth*gfHeight*sizeof(float));
    gravityMask = (int*)malloc(gfWidth*gfHeight*sizeof(int));
    gravCoords = (float*)malloc(gfWidth*gfHeight*4*sizeof(float));
    gravMag = (float*)malloc(gfWidth*gfHeight*2*sizeof(float));
}

void atmosphereSetup()
{
    free(cAtmosphere);
    cAtmosphere = (struct Atmosphere*) malloc (sizeof(struct Atmosphere));

    cAtmosphere->heat = DEFAULT_ATMOSPHERE_TEMP;
    cAtmosphere->gravity = DEFAULT_ATMOSPHERE_GRAVITY;
    cAtmosphere->backgroundRed = DEFAULT_RED;
    cAtmosphere->backgroundGreen = DEFAULT_GREEN;
    cAtmosphere->backgroundBlue = DEFAULT_BLUE;
    cAtmosphere->borderLeft = DEFAULT_BORDER_LEFT;
    cAtmosphere->borderTop = DEFAULT_BORDER_TOP;
    cAtmosphere->borderRight = DEFAULT_BORDER_RIGHT;
    cAtmosphere->borderBottom = DEFAULT_BORDER_BOTTOM;
}

void elementSetup()
{
    LOGI("Element setup");
    int i, j;

    //Free and reallocate the elements array
    if (elements != NULL)
    {
        for(i = 0; i < numElements; ++i)
        {
            LOGI("Free: %d", i);
            free(elements[i]);
        }
        free(elements);
    }
    LOGI("Elements free done");

    numElements = NUM_BASE_ELEMENTS; //Changed later
    elements = (struct Element**)malloc(numElements * sizeof(struct Element*)); // we  will realloc later for custom elements if neede

    //Allocate and initialize all the elements
    struct Element* tempElement;
    for(i = 0; i < numElements; i++)
    {
        if(i < NUM_BASE_ELEMENTS)
        {
            tempElement = (struct Element*) malloc(sizeof(struct Element));
            elements[i] = tempElement;
            tempElement->index = i;
            tempElement->base = i;
            tempElement->name = baseName[i];
            tempElement->red = baseRed[i];
            tempElement->green = baseGreen[i];
            tempElement->blue = baseBlue[i];
            tempElement->fallVel = baseFallVel[i];
            tempElement->density = baseDensity[i];
            tempElement->state = baseState[i];
            memcpy(tempElement->specials, baseSpecial[i], MAX_SPECIALS * sizeof(int));
            memcpy(tempElement->specialVals, baseSpecialValue[i], MAX_SPECIALS * sizeof(int));
            tempElement->inertia = baseInertia[i];
            tempElement->startingTemp = baseStartingTemp[i];
            tempElement->highestTemp = baseHighestTemp[i];
            tempElement->lowestTemp = baseLowestTemp[i];
        }
    }

    cElement = elements[NORMAL_ELEMENT];

    //Resolve heat pointers
    for(i = 0; i < numElements; i++)
    {
        if(i < NUM_BASE_ELEMENTS)
        {
            elements[i]->lowerElement = elements[baseLowerElement[i]];
            elements[i]->higherElement = elements[baseHigherElement[i]];
        }
    }
    loadCustomElements2();
    __android_log_write(ANDROID_LOG_INFO, "LOG", "End element setup");
}

void particleSetup()
{
    //TODO decrement instead
    int i;
    for(i = 0; i < MAX_POINTS; i++)
    {
        a_specialVals[i] = (int*) malloc(MAX_SPECIALS * sizeof(int));
    }
}
