/*
 * collide.c
 * ----------------------------
 * Defines the collide function, which is the
 * heart of our app. It processes all the collisions by
 * type number. It alone is several hundred lines
 * long, thus the need for a separate file.
 */

#include "collide.h"

#include "collisions.h"
#include "specials.h"

void collide(int firstParticle, int secondParticle)
{
    //Specials hook on collision
    bool collisionOverridden = collisionSpecials(firstParticle, secondParticle);
    if (collisionOverridden)
    {
        return;
    }

    //Temporary variables
    int oldXFirst = a_oldX[firstParticle], oldYFirst = a_oldY[firstParticle];
    //The type of the collision (retrieved from a static array)

    int fs = a_element[firstParticle]->index;
    int ss = a_element[secondParticle]->index;
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
        type = collision[fs][ss];
    }

    switch(type)
    {
    case 0: //Solid - Solid
    {
        //First particle goes back to where it was before
        a_x[firstParticle] = oldXFirst;
        a_y[firstParticle] = oldYFirst;
        a_hasMoved[firstParticle] = FALSE;

        a_xVel[firstParticle] = (rand() % 3) - 1;
        break;
    }
    case 1: //Density Based
    {
        //The denser element replaces the less dense element
        if (a_element[firstParticle]->density > a_element[secondParticle]->density)
        {
            a_x[secondParticle] = oldXFirst;
            a_y[secondParticle] = oldYFirst;
            a_hasMoved[secondParticle] = TRUE;

            break;
        }
        //The less dense element bounces back and adds some velocities
        else
        {
            a_x[firstParticle] = oldXFirst;
            a_y[firstParticle] = oldYFirst;
            a_hasMoved[firstParticle] = FALSE;
                        
            //-1 to 1
            int random = rand() % 2 - 1;
            a_yVel[firstParticle] = random;

            //-5 to 5
            random = rand() % 11 - 5;
            a_xVel[firstParticle] = random;

            break;
        }
    }
    case 2: //Anything - Generator collision
    {
        //Change the generator to spawner
        setElement(secondParticle, elements[SPAWN_ELEMENT]);
        setParticleSpecialVal(secondParticle, SPECIAL_SPAWN, a_element[firstParticle]->index);
        a_hasMoved[secondParticle] = TRUE;

        //Move back and delete firstParticle
        a_x[firstParticle] = oldXFirst;
        a_y[firstParticle] = oldYFirst;
        deletePoint(firstParticle);
        a_hasMoved[firstParticle] = FALSE;

        break;
    }
    case 3: //Generator - Anything collision 
        //(should not be needed, but just in case we have moving generators in the future)
    {
        //Change the generator to spawner
        setElement(firstParticle, elements[SPAWN_ELEMENT]);
        setParticleSpecialVal(firstParticle, SPECIAL_SPAWN, a_element[secondParticle]->index);
        a_hasMoved[firstParticle] = TRUE;

        //Delete secondParticle
        unSetPoint(secondParticle);
        a_hasMoved[secondParticle] = FALSE;

        break;
    }
    case 4: //Acid - Meltable
    {
        //Define some temporary variables
        int tempX = a_x[firstParticle], tempY = a_y[firstParticle];
        if (rand() % 3 != 0 ) //2/3 chance
        {
            //Acid burns away Meltable
            unSetPoint(secondParticle);
        }
        else if (rand() % 2 == 0 ) //Otherwise, 1/6 total
        {
            //Acid is neutralized
            //Move firstParticle back and delete it
            a_x[firstParticle] = oldXFirst;
            a_y[firstParticle] = oldYFirst;
            deletePoint(firstParticle);
            a_hasMoved[firstParticle] = FALSE;
        }
        else //Otherwise, 1/6 total
        {
            //Acid bounces
            a_x[firstParticle] = oldXFirst;
            a_y[firstParticle] = oldYFirst;
            a_hasMoved[firstParticle] = FALSE;
        }

        break;
    }
    case 5: //Meltable - Acid
    {
        if (rand() % 3 != 0 ) //2/3 chance
        {
            //Meltable is destroyed

            //Delete firstParticle
            a_x[firstParticle] = oldXFirst;
            a_y[firstParticle] = oldYFirst;
            deletePoint(firstParticle);
            a_hasMoved[firstParticle] = FALSE;
        }
        else if (rand() % 2 == 0 ) //Otherwise, 1/6 totaln
        {
            //Acid is neutralized

            //Delete secondParticle
            unSetPoint(secondParticle);
            a_hasMoved[secondParticle] = FALSE;
        }
        else //Otherwise, 1/6 total
        {
            //Meltable bounces
            a_x[firstParticle] = oldXFirst;
            a_y[firstParticle] = oldYFirst;
            a_hasMoved[firstParticle] = FALSE;
        }
        break;
    }
    case 6: //Acid - Neutralizer
    {
        int tempX = a_x[firstParticle], tempY = a_y[firstParticle];
        if (rand() % 3 == 0) //1/3 Chance
        {
            //Delete firstParticle
            a_x[firstParticle] = oldXFirst;
            a_y[firstParticle] = oldYFirst;
            deletePoint(firstParticle);
            a_hasMoved[firstParticle] = FALSE;
        }
        else //2/3 Change of bouncing
        {
            //Move the point back
            a_x[firstParticle] = oldXFirst;
            a_y[firstParticle] = oldYFirst;
            a_hasMoved[firstParticle] = FALSE;
        }
                        
        break;
    }
    case 7: //Neutralizer - Acid
    {
        if (rand() % 3 == 0) //1/3 Chance
        {
            //Delete secondParticle
            unSetPoint(secondParticle);
            a_hasMoved[secondParticle] = FALSE;
        }
        else //2/3 Chance
        {
            //Move the water back
            a_x[firstParticle] = oldXFirst;
            a_y[firstParticle] = oldYFirst;
            a_hasMoved[firstParticle] = FALSE;
        }

        break;
    }
    case 8: //Salt - Water or Water - Salt or Salt - Ice or Ice - Salt or Salt-Water with any
    {
        //Move back and delete firstParticle
        a_x[firstParticle] = oldXFirst;
        a_y[firstParticle] = oldYFirst;
        deletePoint(firstParticle);
        a_hasMoved[firstParticle] = FALSE;

        //Change the element of secondParticle to Salt-water
        setElement(secondParticle, elements[SALT_WATER_ELEMENT]);
        a_hasMoved[secondParticle] = TRUE;

        break;
    }
    case 9: //Salt-water - Plant or Plant - Salt-water
    {
        //Delete firstParticle
        a_x[firstParticle] = oldXFirst;
        a_y[firstParticle] = oldYFirst;
        deletePoint(firstParticle);
        a_hasMoved[firstParticle] = FALSE;
                    
        //Change the element or secondParticle to Sand
        setElement(secondParticle, elements[SAND_ELEMENT]);
        a_hasMoved[secondParticle] = TRUE;
        break;
    }
    case 10: //Water - Sand or Sand - Water
    {
        //Delete firstParticle
        a_x[firstParticle] = oldXFirst;
        a_y[firstParticle] = oldYFirst;
        deletePoint(firstParticle);
        a_hasMoved[firstParticle] = FALSE;

        //Change the element of secondParticle to Mud
        setElement(secondParticle, elements[21]);
        a_hasMoved[secondParticle] = TRUE;
        break;
    }
    case 11: //Destroy -- second particle is erased
    {
        unSetPoint(secondParticle);
        a_hasMoved[secondParticle] = FALSE;
        break;
    }
    }
}
