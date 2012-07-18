/*
 * collide.c
 * ----------------------------
 * Defines the collide function, which is the
 * heart of our app. It processes all the collisions by
 * type number. It alone is several hundred lines
 * long, thus the need for a separate file.
 */

#include "collide.h"

void collide(struct Particle* firstParticle, struct Particle* secondParticle)
{
    //Temporary variables
    int oldXFirst = firstParticle->oldX, oldYFirst = firstParticle->oldY;
    //The type of the collision (retrieved from a static array)

    int fs = firstParticle->element->index;
    int ss = secondParticle->element->index;
    int type;
    if (fs >= NUM_BASE_ELEMENTS && ss >= NUM_BASE_ELEMENTS)
    {
    	type = elements[fs]->collisions[elements[ss]->base];
    }
    else if ( fs >= NUM_BASE_ELEMENTS)
    {
    	type = elements[fs]->collisions[ss];
    }
    else if ( ss >= NUM_BASE_ELEMENTS)
    {
    	type = reciprocals[elements[ss]->collisions[fs]];
    }
    else
    {
    	type = collision[firstParticle->element->index][secondParticle->element->index];
    }

    switch(type)
    {
    case 0: //Solid - Solid
    {
	//First particle goes back to where it was before
	firstParticle->x = oldXFirst;
	firstParticle->y = oldYFirst;
	firstParticle->hasMoved = FALSE;

	firstParticle->xVel = (rand() % 3) - 1;
	break;
    }
    case 1: //Density Based
    {
        //The denser element replaces the less dense element
        if (firstParticle->element->density > secondParticle->element->density)
        {
            secondParticle->x = oldXFirst;
            secondParticle->y = oldYFirst;
            secondParticle->hasMoved = TRUE;

            break;
        }
        //The less dense element bounces back and adds some velocities
        else
        {
            firstParticle->x = oldXFirst;
            firstParticle->y = oldYFirst;
            firstParticle->hasMoved = FALSE;
                        
            //-1 to 1
            int random = rand() % 2 - 1;
            firstParticle->yVel = random;

            //-5 to 5
            random = rand() % 11 - 5;
            firstParticle->xVel = random;

            break;
        }
    }
    case 2: //Anything - Generator collision
    {
        //Change the generator to spawner
        setElement(secondParticle, elements[SPAWN_ELEMENT]);
        setParticleSpecialVal(secondParticle, SPECIAL_SPAWN, firstParticle->element->index);
        secondParticle->hasMoved = TRUE;

        //Move back and delete firstParticle
        firstParticle->x = oldXFirst;
        firstParticle->y = oldYFirst;
        deletePoint(firstParticle);
        firstParticle->hasMoved = FALSE;

        break;
    }
    case 3: //Generator - Anything collision 
        //(should not be needed, but just in case we have moving generators in the future)
    {
        //Change the generator to spawner
        setElement(firstParticle, elements[SPAWN_ELEMENT]);
        setParticleSpecialVal(firstParticle, SPECIAL_SPAWN, secondParticle->element->index);
        firstParticle->hasMoved = TRUE;

        //Delete secondParticle
        unSetPoint(secondParticle);
        secondParticle->hasMoved = FALSE;

        break;
    }
    case 4: //Acid - Meltable
    {
        //Define some temporary variables
        int tempX = firstParticle->x, tempY = firstParticle->y;
        if (rand() % 3 != 0) //2/3 chance
        {
            //Acid burns away Meltable
            unSetPoint(secondParticle);
        }
        else if (rand() % 2 == 0) //Otherwise, 1/6 total
        {
            //Acid is neutralized
            //Move firstParticle back and delete it
            firstParticle->x = firstParticle->oldX;
            firstParticle->y = firstParticle->oldY;
            deletePoint(firstParticle);
            firstParticle->hasMoved = FALSE;
        }
	else //Otherwise, 1/6 total
	{
	    //Acid bounces
	    firstParticle->x = oldXFirst;
	    firstParticle->y = oldYFirst;
	    firstParticle->hasMoved = FALSE;
	}

	break;
    }
    case 5: //Meltable - Acid
    {
	if (rand() % 3 != 0) //2/3 chance
	{
	    //Meltable is destroyed

	    //Delete firstParticle
	    firstParticle->x = firstParticle->oldX;
	    firstParticle->y = firstParticle->oldY;
	    deletePoint(firstParticle);
	    firstParticle->hasMoved = FALSE;
	}
	else if (rand() % 2 == 0) //Otherwise, 1/6 totaln
	{
	    //Acid is neutralized

	    //Delete secondParticle
	    unSetPoint(secondParticle);
	    secondParticle->hasMoved = TRUE;
	}
	else //Otherwise, 1/6 total
	{
	    //Meltable bounces
	    firstParticle->x = oldXFirst;
	    firstParticle->y = oldYFirst;
	    firstParticle->hasMoved = FALSE;
	}
	break;
    }
    case 6: //Acid - Neutralizer
    {
	int tempX = firstParticle->x, tempY = firstParticle->y;
	if ( rand() % 3 == 0 ) //1/3 Chance
	{
	    //Delete firstParticle
	    firstParticle->x = oldXFirst;
	    firstParticle->y = oldYFirst;
	    deletePoint(firstParticle);
	    firstParticle->hasMoved = FALSE;
	}
	else //2/3 Change of bouncing
	{
	    //Move the point back
	    firstParticle->x = oldXFirst;
	    firstParticle->y = oldYFirst;
	    firstParticle->hasMoved = FALSE;
	}
			
	break;
    }
    case 7: //Neutralizer - Acid
    {
	if (rand() % 3 == 0) //1/3 Chance
	{
	    //Delete secondParticle
	    unSetPoint(secondParticle);
	    secondParticle->hasMoved = FALSE;
	}
	else //2/3 Chance
	{
	    //Move the water back
	    firstParticle->x = oldXFirst;
	    firstParticle->y = oldYFirst;
	    firstParticle->hasMoved = FALSE;
	}

	break;
    }
    case 8: //Salt - Water or Water - Salt or Salt - Ice or Ice - Salt or Salt-Water with any
    {
	//Move back and delete firstParticle
	firstParticle->x = oldXFirst;
	firstParticle->y = oldYFirst;
	deletePoint(firstParticle);
	firstParticle->hasMoved = FALSE;

	//Change the element of secondParticle to Salt-water
	setElement(secondParticle, elements[19]);
	secondParticle->hasMoved = TRUE;

	break;
    }
    case 9: //Salt-water - Plant or Plant - Salt-water
    {
	//Delete firstParticle
	firstParticle->x = oldXFirst;
	firstParticle->y = oldYFirst;
	deletePoint(firstParticle);
	firstParticle->hasMoved = FALSE;
		    
	//Change the element or secondParticle to Sand
	setElement(secondParticle, elements[3]);
	secondParticle->hasMoved = TRUE;
	break;
    }
    case 10: //Water - Sand or Sand - Water
    {
	//Delete firstParticle
	firstParticle->x = oldXFirst;
	firstParticle->y = oldYFirst;
	deletePoint(firstParticle);
	firstParticle->hasMoved = FALSE;

	//Change the element of secondParticle to Mud
	setElement(secondParticle, elements[21]);
	secondParticle->hasMoved = TRUE;
	break;
    }
    }
}
