/*
 * setup.c
 * -----------------------------
 * Defines the setup function, which initializes
 * all of the arrays and variables necessary.
 */

#include "setup.h"
#include <android/log.h>

void gameSetup()
{
    //__android_log_write(ANDROID_LOG_INFO, "TheElements", "gameSetup");
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
    /* Network stuff not needed
       for (j = 0; j < 8; j++)
       {
       username[j] = 0;
       password[j] = 0;
       }
       userlength = 0;
       passlength = 0;
    */
}

//Set up all the variable sized arrays
void arraySetup()
{
    //__android_log_write(ANDROID_LOG_INFO, "TheElements", "arraySetup");
    //Make sure everything is deallocated
    free(colors);
    free(colorsFrameBuffer);
    free(allCoords);

    //Allocate memory
    colors = malloc(3 * stupidTegra * workHeight * sizeof(char));
    colorsFrameBuffer = malloc(3 * stupidTegra * workHeight * sizeof(char));
    allCoords = malloc(workWidth * workHeight * sizeof(int*)); //Two dimensional array, so when calling use allcoords[getIndex(x, y)];
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
    __android_log_write(ANDROID_LOG_INFO, "LOG", "Element setup");
    int i, j;
    //Calculate numElements
    //Calculate special size
    //Calculate special value size
    numElements = NUM_BASE_ELEMENTS; //Changed later

    //Free and reallocate the elements array
    if (elements != NULL)
    {
        for(i = 0; i < numElements; ++i)
        {
            free(elements[i]);
        }
        free(elements);
    }

    elements = malloc(numElements * sizeof(struct Element*)); // we  will realloc later for custom elements if neede

    //Allocate and initialize all the elements
    struct Element* tempElement;
    for(i = 0; i < numElements; i++)
    {
        if(i < NUM_BASE_ELEMENTS)
        {
            tempElement = (struct Element*) malloc(sizeof(struct Element));
            elements[i] = tempElement;
            tempElement->index = i;
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
    loadCustomElements();
    __android_log_write(ANDROID_LOG_INFO, "LOG", "End element setup");
}

void particleSetup()
{
    //__android_log_write(ANDROID_LOG_INFO, "TheElements", "particleSetup");
    //TODO decrement instead
    int i;
    for(i = 0; i < MAX_POINTS; i++)
    {
        a_specialVals[i] = (int*) malloc(MAX_SPECIALS * sizeof(int));
    }
}
