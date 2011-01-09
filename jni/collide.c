/*
 * collide.c
 * ----------------------------
 * Defines the collide function, which is the
 * heart of our app. It processes all the collisions by
 * type number. It alone is several hundred lines
 * long, thus the need for a separate file.
 */

#include "collide.h"

void collide(Particle firstParticle, Particle secondParticle)
{
	//Temporary variables
	int oldXFirst = firstParticle.oldX, oldYFirst = firstParticle.oldY, oldXSecond = secondParticle.oldX, oldYSecond = secondParticle.oldY;
	//The type of the collision (retrieved from a static array)
	int type = collision[firstParticle.element.index][secondParticle.element.index];

	switch(type)
	{
		//Solid - Solid
		case 0:
		{
			//First particle goes back to where it was before
			firstParticle.x = oldXFirst;
			firstParticle.y = oldYFirst;

			firstParticle.xVel += (rand() % 3) - 1;
			break;
		}
		//Density Based
		case 1:
		{
			//The denser element replaces the less dense element
			if (firstParticle.element.density > secondParticle.element.density)
			{
				secondParticle.x = oldXFirst;
				secondParticle.y = oldYFirst;

				allCoords[(int) firstParticle.x][(int) firstParticle.y] = firstParticle;
				setBitmapColor((int) firstParticle.x, (int) firstParticle.y, firstParticle.element);
				allCoords[(int) secondParticle.x][(int) secondParticle.y] = secondParticle;
				setBitmapColor((int) secondParticle.x, (int) secondParticle.y, secondParticle.element);

				break;
			}
			//The less dense element bounces back and adds some velocities
			else
			{
				//-1 to 1
				int random = rand() % 3 - 1;
				firstPartivle.yVel = random;

				//-3 to 3
				random = rand() % 7 - 3;
				firstParticle.xVel = random;

				break;
			}
		}
		case 2: //Anything - Generator collision
		{
			//Change the generator to spawner
			setElement(secondParticle, SPAWN_ELEMENT);
			secondParticle.specialVals[0] = firstParticle.element.index;

			//Delete firstParticle
			allCoords[getIndex(oldXFirst, oldYFirst)] = NULL;
			setBitmapColor(oldXFirst, oldYFirst, ERASER_ELEMENT);
			free(firstParticle);

			break;
		}
		case 3: //Generator - Anything collision (should not be needed, but just in case we have moving generators in the future)
		{
			//Change the generator to spawner
			setElement(firstParticle, SPAWN_ELEMENT);
			firstParticle.specialVals[0] = secondParticle.element.index;

			//Move firstParticle back
			firstParticle.x = oldXFirst;
			firstParticle.y = oldYFirst;

			//Delete secondParticle
			allCoords[getIndex(secondParticle.x, secondParticle.y)] = NULL;
			setBitmapColor(secondParticle.x, secondParticle.y, ERASER_ELEMENT);
			free(secondParticle);

			break;
		}
		case 4: //Acid - Meltable
		{
			//Define some temporary variables
			int tempX = firstParticle.x, tempY = firstParticle.y;

			if (rand() % 3 != 0) //2/3 chance
			{
				//Acid burns away the Meltable
				//Delete the old point
				allCoords[getIndex(oldXFirst, oldYFirst)] = NULL;
				setBitmapColor(oldXFirst, oldYFirst, ERASER_ELEMENT);
				//Set the new point
				allCoords[getIndex(tempX, tempY)] = firstParticle;
				setBitmapColor(tempX, tempY, firstParticle.element);

				//Delete secondParticle
				allCoords[getIndex(oldXSecond, oldYSecond)] = NULL;
				setBitmapColor(oldXSecond, oldYSecond, ERASER_ELEMENT);
				free(secondParticle);
			}
			else if (rand() % 2 == 0) //Otherwise, 1/6 total
			{
				//Acid is neutralized
				//Delete fingerParticle
				allCoords[getIndex(oldXFirst, oldYFirst)] = NULL;
				setBitmapColor(oldXFirst, oldYFirst, ERASER_ELEMENT);
				free(firstParticle);
			}
			else //Otherwise, 1/6 total
			{
				//Acid bounces
				firstParticle.x = oldXFirst;
				firstParticle.y = oldYFirst;
			}

			break;
		}
		case 5: //Meltable - Acid
		{
			if (rand() % 3 != 0) //2/3 chance
			{
				//Meltable is destroyed

				//Delete the old point
				allCoords[getIndex(oldXFirst, oldYFirst)] = NULL;
				setBitmapColor(oldXFirst, oldYFirst, ERASER_ELEMENT);

				//Delete firstParticle
				free(firstParticle);
			}
			else if (rand() % 2 == 0) //Otherwise, 1/6 total
			{
				//Acid is neutralized

				//Delete the old point
				allCoords[getIndex(oldXFirst, oldYFirst)] = NULL;
				setBitmapColor(oldXFirst, oldYFirst, ERASER_ELEMENT);

				//Set the new point
				allCoords[firstParticle.x][firstParticle.y] = firstParticle;
				setBitmapColor(firstParticle.x, firstParticle.y, firstParticle.element);

				//Delete secondParticle
				free(secondParticle);
			}
			else //Otherwise, 1/6 total
			{
				//Meltable bounces

				firstParticle.x = oldXFirst;
				firstParticle.y = oldYFirst;
			}
			break;
		}
		case 6: //Acid - Neutralizer
		{
			int tempX = firstParticle.x, tempY = firstParticle.y;
			if (rand() % 3 == 0) //1/3 Chance
			{
				//Delete firstParticle
				allCoords[getIndex(oldXFirst, oldYFirst)] = NULL;
				setBitmapColor(oldXFirst, oldYFirst, ERASER_ELEMENT);
				free(firstParticle);
			}
			else //2/3 Change of bouncing
			{
				//Move the point back
				firstParticle.x = olxf;
				firstParticle.y = olyf;
			}

			break;
		}
		case 7: //Neutralizer - Acid
		{
			if (rand() % 3 == 0) //1/3 Chance
			{
				//Delete the old point
				allCoords[getIndex(oldXFirst, oldYFirst)] = NULL;
				setBitmapColor(oldXFirst, oldYFirst, ERASER_ELEMENT);

				//Set the new point
				allCoords[firstParticle.x][firstParticle.y] = firstParticle;
				setBitmapColor(firstParticle.x, firstParticle.y, firstParticle.element);

				//Delete secondParticle
				free(secondParticle);
			}
			else //2/3 Chance
			{
				//Move the water back
				firstParticle.x = oldXFirst;
				firstParticle.y = oldYFirst;
			}

			break;
		}
		case 8: //Salt - Water or Water - Salt or Salt - Ice or Ice - Salt
		{
			//Delete firstParticle
			allCoords[getIndex(oldXFirst, oldYFirst)] = NULL;
			setBitmapColor(oldXFirst, oldYFirst, ERASER_ELEMENT);
			free(firstParticle);

			//Change the element of secondParticle to Salt-water
			setElement(secondParticle, 19);

			break;
		}
		case 9: //Salt-water - Plant
		{
			//Delete firstParticle
			allCoords[getIndex(oldXFirst, oldYFirst)] = NULL;
			setBitmapColor(oldXFirst, oldYFirst, ERASER_ELEMENT);
			free(firstParticle);

			//Change the ele			//Delete firstParticle
			allCoords[getIndex(oldXFirst, oldYFirst)] = NULL;
			setBitmapColor(oldXFirst, oldYFirst, ERASER_ELEMENT);
			free(firstParticle);ment of secondParticle to Sand
			setElement(secondParticle, 3);
		}
		case 10: //Water - Sand or Sand - Water
		{
			//Delete firstParticle
			allCoords[getIndex(oldXFirst, oldYFirst)] = NULL;
			setBitmapColor(oldXFirst, oldYFirst, ERASER_ELEMENT);
			free(firstParticle);

			//Change the element of secondParticle to Mud
			setElement(secondParticle, 21);
		}
}
