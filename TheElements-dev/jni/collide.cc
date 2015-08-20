/*
 * collide.c
 * ----------------------------
 * Defines the collide function, which is the
 * heart of our app. It processes all the collisions by
 * type number. It alone is several hundred lines
 * long, thus the need for a separate file.
 */

#include "collide.h"

#include <cmath>

#include "collisions.h"
#include "specials.h"

inline void removeSp(int p) {
    unSetPoint(p);
    a_hasMoved[p] = FALSE;
}
inline void bounceFp(float ox, float oy, int p) {
    a_x[p] = ox;
    a_y[p] = oy;
    a_hasMoved[p] = FALSE;
}

void collide(int firstParticle, int secondParticle)
{
    //Specials hook on collision
    bool collisionOverridden = collisionSpecials(firstParticle, secondParticle);
    if (collisionOverridden)
    {
        return;
    }

    //Temporary variables
    float oldXFirst = a_oldX[firstParticle], oldYFirst = a_oldY[firstParticle];
    float dx = (a_oldX[firstParticle]-a_x[firstParticle]), dy = (a_oldY[firstParticle]-a_y[firstParticle]);
    float velMag = sqrt((float)dx*dx+dy*dy);
    float perpXVel = -dy/velMag;
    float perpYVel = dx/velMag;
    float tangXVel = dx/velMag;
    float tangYVel = dy/velMag;
    //The type of the collision (retrieved from a static array)

    //TODO(gkanwar): Lot of array referencing churn here. Fix this.
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
        bounceFp(oldXFirst, oldYFirst, firstParticle);

        // -5/3 to 5/3
        float perpDelta = ((rand() % 101) - 50)/30.0;
        a_xVel[firstParticle] += perpXVel*perpDelta;
        a_yVel[firstParticle] += perpYVel*perpDelta;
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
        //The less dense element bounces back
        else
        {
            // -1/2 to 1/2
            float perpDelta = ((rand() % 11) - 5)/10.0;
            a_xVel[firstParticle] += perpXVel*perpDelta;
            a_yVel[firstParticle] += perpYVel*perpDelta;
            bounceFp(oldXFirst, oldYFirst, firstParticle);
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
        bounceFp(oldXFirst, oldYFirst, firstParticle);
        deletePoint(firstParticle);

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
        removeSp(secondParticle);

        break;
    }
    case 4: //Acid - Meltable
    {
        //Define some temporary variables
        int tempX = a_x[firstParticle], tempY = a_y[firstParticle];
        if (rand() % 3 != 0 ) //2/3 chance
        {
            //Acid burns away Meltable
            removeSp(secondParticle);
        }
        else if (rand() % 2 == 0 ) //Otherwise, 1/6 total
        {
            //Acid is neutralized
            //Move firstParticle back and delete it
            bounceFp(oldXFirst, oldYFirst, firstParticle);
            deletePoint(firstParticle);
        }
        else //Otherwise, 1/6 total
        {
            //Acid bounces
            bounceFp(oldXFirst, oldYFirst, firstParticle);
        }

        break;
    }
    case 5: //Meltable - Acid
    {
        if (rand() % 3 != 0 ) //2/3 chance
        {
            //Meltable is destroyed

            //Delete firstParticle
            bounceFp(oldXFirst, oldYFirst, firstParticle);
            deletePoint(firstParticle);
        }
        else if (rand() % 2 == 0 ) //Otherwise, 1/6 totaln
        {
            //Acid is neutralized

            //Delete secondParticle
            removeSp(secondParticle);
        }
        else //Otherwise, 1/6 total
        {
            //Meltable bounces
            bounceFp(oldXFirst, oldYFirst, firstParticle);
        }
        break;
    }
    case 6: //Acid - Neutralizer
    {
        int tempX = a_x[firstParticle], tempY = a_y[firstParticle];
        if (rand() % 3 == 0) //1/3 Chance
        {
            //Delete firstParticle
            bounceFp(oldXFirst, oldYFirst, firstParticle);
            deletePoint(firstParticle);
        }
        else //2/3 Change of bouncing
        {
            //Move the point back
            bounceFp(oldXFirst, oldYFirst, firstParticle);
        }
                        
        break;
    }
    case 7: //Neutralizer - Acid
    {
        if (rand() % 3 == 0) //1/3 Chance
        {
            //Delete secondParticle
            removeSp(secondParticle);
        }
        else //2/3 Chance
        {
            //Move firstParticle back
            bounceFp(oldXFirst, oldYFirst, firstParticle);
        }

        break;
    }
    case 8: //Salt - Water or Water - Salt or Salt - Ice or Ice - Salt or Salt-Water with any
    {
        //Move back and delete firstParticle
        bounceFp(oldXFirst, oldYFirst, firstParticle);
        deletePoint(firstParticle);

        //Change the element of secondParticle to Salt-water
        setElement(secondParticle, elements[SALT_WATER_ELEMENT]);
        a_hasMoved[secondParticle] = TRUE;

        break;
    }
    case 9: //Salt-water - Plant or Plant - Salt-water
    {
        //Delete firstParticle
        bounceFp(oldXFirst, oldYFirst, firstParticle);
        deletePoint(firstParticle);
                    
        //Change the element or secondParticle to Sand
        setElement(secondParticle, elements[SAND_ELEMENT]);
        a_hasMoved[secondParticle] = TRUE;
        break;
    }
    case 10: //Water - Sand or Sand - Water
    {
        //Delete firstParticle
        bounceFp(oldXFirst, oldYFirst, firstParticle);
        deletePoint(firstParticle);

        //Change the element of secondParticle to Mud
        setElement(secondParticle, elements[MUD_ELEMENT]);
        a_hasMoved[secondParticle] = TRUE;
        break;
    }
    case 11: //Destroy -- second particle is erased
    {
        removeSp(secondParticle);
        break;
    }
    case 12: //Stacking - Solid
    {
        //First particle goes back to where it was before
        bounceFp(oldXFirst, oldYFirst, firstParticle);
    }
    }
}
