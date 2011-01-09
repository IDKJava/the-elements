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
			if (firstParticle.element != 7)
			{
				//Change the generator to secondParticleawner
				secondParticle.element = 8;
				secondParticleawn[secondParticle] = firstParticle.element; //Set the secondParticleawn element

				//Delete the old point
				allCoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				//Delete firstParticle
				firstParticle.x = 0;
				firstParticle.y = 0;
				firstParticle.element = 0;
				xvel[firstParticle] = 0;
				yvel[firstParticle] = 0;
				set[firstParticle] = 0;
				avail[loq] = firstParticle;
				loq++;

				break;
			}
			else
			{
				//Change the generator to secondParticleawner
				firstParticle.element = 8;
				secondParticleawn[firstParticle] = secondParticle.element; //Set the secondParticleawn element

				//Delete the old point
				allCoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				//Delete secondParticle
				secondParticle.x = 0;
				secondParticle.y = 0;
				secondParticle.element = 0;
				xvel[secondParticle] = 0;
				yvel[secondParticle] = 0;
				set[secondParticle] = 0;
				avail[loq] = secondParticle;
				loq++;

				break;
			}
		}
		case 3: //Acid - Meltable
		{
			//Define some temporary variables
			int tempX = firstParticle.x, tempY = firstParticle.y;

			if (rand() % 3 != 0) //2/3 chance
			{
				//Acid burns away secondParticle
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
		case 4: //Meltable-Acid
		{
			if (rand() % 3 != 0) //2/3 chance
			{
				//Acid burns away firstParticle

				//Delete the old point
				allCoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				//Delete firstParticle
				firstParticle.x = 0;
				firstParticle.y = 0;
				firstParticle.element = 0;
				xvel[firstParticle] = 0;
				yvel[firstParticle] = 0;
				set[firstParticle] = 0;
				avail[loq] = firstParticle;
				loq++;
			}
			else if (rand() % 2 == 0) //Otherwise, 1/6 total
			{
				//Acid is neutralized

				//Delete the old point
				allCoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				//Set the new point
				allCoords[xfirstParticle][yfirstParticle] = firstParticle;
				setBitmapColor(xfirstParticle, yfirstParticle, firstParticle.element);

				//Delete secondParticle
				secondParticle.x = 0;
				secondParticle.y = 0;
				secondParticle.element = 0;
				xvel[secondParticle] = 0;
				yvel[secondParticle] = 0;
				set[secondParticle] = 0;
				avail[loq] = secondParticle;
				loq++;
			}
			else //Otherwise, 1/6 total
			{
				//Other particle bounces bounces

				firstParticle.x = olxf;
				firstParticle.y = olyf;
			}
			break;
		}
		case 4: //Acid - Water
		{
			int xfirstParticle = firstParticle.x, yfirstParticle = firstParticle.y;
			//Acid goes away 1/3 of the time, otherwise bounce back
			if (firstParticle.element == 17 || (firstParticle.element == 22
					&& colliseelement1[secondParticle.element] == 17) || (secondParticle.element == 22
					&& colliseelement1[firstParticle.element] != 17)) //firstParticle is acid
			{
				if (rand() % 3 == 0)
				{
					//Delete the old point
					allCoords[olxf][olyf] = -1;
					setBitmapColor(olxf, olyf, 3);

					//Delete firstParticle
					firstParticle.x = 0;
					firstParticle.y = 0;
					firstParticle.element = 0;
					xvel[firstParticle] = 0;
					yvel[firstParticle] = 0;
					set[firstParticle] = 0;
					avail[loq] = firstParticle;
					loq++;
				}
				else
				{
					//Move the point back
					firstParticle.x = olxf;
					firstParticle.y = olyf;
				}
			}
			else //secondParticle is acid
			{
				if (rand() % 3 == 0)
				{
					//Delete the old point
					allCoords[olxf][olyf] = -1;
					setBitmapColor(olxf, olyf, 3);

					//Set the new point
					allCoords[xfirstParticle][yfirstParticle] = firstParticle;
					setBitmapColor(xfirstParticle, yfirstParticle, firstParticle.element);

					//Delete secondParticle
					secondParticle.x = 0;
					secondParticle.y = 0;
					secondParticle.element = 0;
					xvel[secondParticle] = 0;
					yvel[secondParticle] = 0;
					set[secondParticle] = 0;
					avail[loq] = secondParticle;
					loq++;
				}
				else
				{
					//Move the water back
					firstParticle.x = olxf;
					firstParticle.y = olyf;
				}
			}

			break;
		}
		case 5: //Steam - Steam
		{
			if (rand() % 1000 == 0)//1/5 chance
			{
				//Make the two steams "condense" into water

				int xsecondParticle = secondParticle.x, ysecondParticle = secondParticle.y; //Some temp variables

				//Move firstParticle back
				firstParticle.x = olxf;
				firstParticle.y = olyf;

				//Change the elements
				firstParticle.element = 1;
				secondParticle.element = 1;

				//Set the bitmap and allCoords
				allCoords[olxf][olyf] = firstParticle;
				setBitmapColor(olxf, olyf, 1);

				allCoords[xsecondParticle][ysecondParticle] = secondParticle;
				setBitmapColor(xsecondParticle, ysecondParticle, 1);

				break;
			}
			else //Make firstParticle bounce off
			{
				//Move firstParticle back
				firstParticle.x = olxf;
				firstParticle.y = olyf;

				allCoords[olxf][olyf] = firstParticle;
				setBitmapColor(olxf, olyf, 18);

				//Add a random x velocity
				xvel[firstParticle] += rand() % 7 - 3; //-3 to 3
				//Add a random y velocity
				yvel[firstParticle] += rand() % 5 - 2; //-2 to 2

				break;
			}
		}
		case 6: //Water - Fire
		{
			if (firstParticle.element == 5 || (firstParticle.element == 22
					&& colliseelement1[secondParticle.element] == 5) || (secondParticle.element == 22
					&& colliseelement1[firstParticle.element] != 5)) //firstParticle is fire
			{
				//Delete the old point
				allCoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				//Delete the fire
				firstParticle.x = 0;
				firstParticle.y = 0;
				firstParticle.element = 0;
				xvel[firstParticle] = 0;
				yvel[firstParticle] = 0;
				set[firstParticle] = 0;
				avail[loq] = firstParticle;
				loq++;

				//Change the water to steam
				secondParticle.element = 18;

				//Change the bitmap color
				setBitmapColor(secondParticle.x, secondParticle.y, 18);

				break;
			}
			else //secondParticle is fire
			{
				int xfirstParticle = firstParticle.x, yfirstParticle = firstParticle.y; //Set temp variables

				//Delete secondParticle(fire)
				DeletePoint(secondParticle);

				//Delete the old point
				allCoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				//Change the water to steam
				firstParticle.element = 18;

				//Set the new location of water
				allCoords[xfirstParticle][yfirstParticle] = firstParticle;
				setBitmapColor(xfirstParticle, yfirstParticle, 18);
			}
		}
		case 7: //Salt - Water
		{
			if (firstParticle.element == 19 || (firstParticle.element == 22
					&& colliseelement1[secondParticle.element] == 19)) //firstParticle is salt
			{
				//Delete the old point
				allCoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				//Delete firstParticle
				firstParticle.x = 0;
				firstParticle.y = 0;
				firstParticle.element = 0;
				xvel[firstParticle] = 0;
				yvel[firstParticle] = 0;
				set[firstParticle] = 0;
				avail[loq] = firstParticle;
				loq++;

				//Change the element of secondParticle to Salt-water
				secondParticle.element = 20;
				setBitmapColor(secondParticle.x, secondParticle.y, 20);
			}
			else //secondParticle is salt
			{
				//Delete the old point
				allCoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				DeletePoint(secondParticle); //Delete the salt

				int xfirstParticle = firstParticle.x, yfirstParticle = firstParticle.y; //Some temp variables
				//Set the new coordinates
				allCoords[xfirstParticle][yfirstParticle] = firstParticle;
				setBitmapColor(xfirstParticle, yfirstParticle, firstParticle.element);
			}
		}
		case 8: //Salt - Ice
		{
			if (firstParticle.element == 6 || (firstParticle.element == 22
					&& colliseelement1[secondParticle.element] == 6) || (secondParticle.element == 22
					&& colliseelement1[firstParticle.element] != 6)) //firstParticle is ice
			{
				//Move firstParticle back
				firstParticle.x = olxf;
				firstParticle.y = olyf;

				//Change the element to water
				firstParticle.element = 1;

				//Set the bitmap stuff
				allCoords[olxf][olyf] = firstParticle;
				setBitmapColor(olxf, olyf, 1);
			}
			else //secondParticle is ice
			{
				//Move firstParticle back
				firstParticle.x = olxf;
				firstParticle.y = olyf;

				allCoords[olxf][olyf] = firstParticle;
				setBitmapColor(olxf, olyf, firstParticle.element);

				//Change the element to water
				secondParticle.element = 1;

				//Set the bitmap stuff
				setBitmapColor(secondParticle.x, secondParticle.y, 1);
			}
		}
		case 9: //Salt-water - Plant
		{
			if (firstParticle.element == 4 || (firstParticle.element == 22
					&& colliseelement1[secondParticle.element] == 4) || (secondParticle.element == 22
					&& colliseelement1[firstParticle.element] != 4)) //firstParticle is plant
			{
				//Move firstParticle back
				firstParticle.x = olxf;
				firstParticle.y = olyf;

				//Change the element to sand
				firstParticle.element = 0;

				//Set the bitmap stuff
				allCoords[olxf][olyf] = firstParticle;
				setBitmapColor(olxf, olyf, 0);
			}
			else //secondParticle is plant
			{
				//Move firstParticle back
				firstParticle.x = olxf;
				firstParticle.y = olyf;

				allCoords[olxf][olyf] = firstParticle;
				setBitmapColor(olxf, olyf, firstParticle.element);

				//Change the element to sand
				secondParticle.element = 0;

				//Set the bitmap stuff
				setBitmapColor(secondParticle.x, secondParticle.y, 0);
			}
		}
		case 10: //Water - Sand
		{
			if (firstParticle.element == 1 || (firstParticle.element == 22
					&& colliseelement1[secondParticle.element] == 1) || (secondParticle.element == 22
					&& colliseelement1[firstParticle.element] != 1)) //firstParticle is water
			{

				//Delete the old point
				allCoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				//Delete the water
				firstParticle.x = 0;
				firstParticle.y = 0;
				firstParticle.element = 0;
				xvel[firstParticle] = 0;
				yvel[firstParticle] = 0;
				set[firstParticle] = 0;
				avail[loq] = firstParticle;
				loq++;

				secondParticle.element = 23; // set the sand to be mud
				setBitmapColor(secondParticle.x, secondParticle.y, secondParticle.element); // change the color of the sand particle location to be mud
			}
			else //secondParticle is water
			{

				//Delete the old point
				allCoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				DeletePoint(secondParticle); //delete the water
				firstParticle.element = 23; //set the sand to be mud

				int xfirstParticle = firstParticle.x, yfirstParticle = firstParticle.y; //Some temp variables
				//Set the new coordinates
				allCoords[xfirstParticle][yfirstParticle] = firstParticle;
				setBitmapColor(xfirstParticle, yfirstParticle, firstParticle.element);

			}
		}
}
