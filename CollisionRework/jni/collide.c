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
	int type = collision[firstParticle->element->index][secondParticle->element->index];

	switch(type)
	{
		//Solid - Solid
		case 0:
		{
			//First particle goes back to where it was before
			firstParticle->x = oldXFirst;
			firstParticle->y = oldYFirst;
			firstParticle->hasMoved = FALSE;

			firstParticle->xVel += (rand() % 3) - 1;
			break;
		}
		//Density Based
		case 1:
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
				int random = rand() % 3 - 1;
				firstParticle->yVel += random;

				//-3 to 3
				random = rand() % 7 - 3;
				firstParticle->xVel += random;

				break;
			}
		}
		case 2: //Anything - Generator collision
		{
			//Change the generator to spawner
			setElement(secondParticle, SPAWN_ELEMENT);
			secondParticle->specialVals[0] = firstParticle->element->index;
			secondParticle->hasMoved = TRUE;

			//Delete firstParticle
			unSetPoint(firstParticle);
			firstParticle->hasMoved = TRUE;

			break;
		}
		case 3: //Generator - Anything collision (should not be needed, but just in case we have moving generators in the future)
		{
			//Change the generator to spawner
			setElement(firstParticle, SPAWN_ELEMENT);
			firstParticle->specialVals[0] = secondParticle->element->index;

			//Move firstParticle back
			firstParticle->x = oldXFirst;
			firstParticle->y = oldYFirst;
			firstParticle->hasMoved = FALSE;

			//Delete secondParticle
			unSetPoint(secondParticle);
			secondParticle->hasMoved = TRUE;

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
				secondParticle->hasMoved = TRUE;
			}
			else if (rand() % 2 == 0) //Otherwise, 1/6 total
			{
				//Acid is neutralized
				//Delete firstParticle
				unSetPoint(firstParticle);
				firstParticle->hasMoved = TRUE;
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
				unSetPoint(firstParticle);
				firstParticle->hasMoved = TRUE;
			}
			else if (rand() % 2 == 0) //Otherwise, 1/6 total
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
			if (rand() % 3 == 0) //1/3 Chance
			{
				//Delete firstParticle
				unSetPoint(firstParticle);
				firstParticle->hasMoved = TRUE;
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
				secondParticle->hasMoved = TRUE;
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
			//Delete firstParticle
			unSetPoint(firstParticle);
			firstParticle->hasMoved = TRUE;

			//Change the element of secondParticle to Salt-water
			setElement(secondParticle, elements[19]);
			secondParticle->hasMoved = TRUE;

			break;
		}
		case 9: //Salt-water - Plant or Plant - Salt-water
		{;
			//Delete firstParticle
			unSetPoint(firstParticle);
			firstParticle->hasMoved = TRUE;

			//Change the element or secondParticle to Sand
			setElement(secondParticle, elements[3]);
			secondParticle->hasMoved = TRUE;
			break;
		}
		case 10: //Water - Sand or Sand - Water
		{
			//Delete firstParticle
			unSetPoint(firstParticle);
			firstParticle->hasMoved = TRUE;

			//Change the element of secondParticle to Mud
			setElement(secondParticle, elements[21]);
			secondParticle->hasMoved = TRUE;
			break;
		}
	}
}
