/*
 * collide.c
 * ----------------------------
 * Defines the collide function, which is the
 * heart of our app. It processes all the collisions by
 * type number. It alone is several hundred lines
 * long, thus the need for a separate file.
 */

#include "collide.h"

void collide(int fp, int sp)//first particle and second particle
{
	int temporary = collision[22][0]; //not sure what this does
	int olyf = oldy[fp];
	int olxf = oldx[fp];
	int olxs = oldx[sp];
	int olys = oldy[sp];
	int type = collision[element[fp]][element[sp]];

	//Solid - Solid generic collision
	if (type == 0)
	{
		y[fp] = olyf; // First particle goes back to where it was before
		x[fp] = olxf;

		allcoords[(int) x[fp]][(int) y[fp]] = fp;
		setBitmapColor((int) x[fp], (int) y[fp], element[fp]);

		xvel[fp] += (rand() % 3) - 1;
		// no change in allcoords
	}
	//Solid - Liquid generic collision
	else if (type == 1)
	{
		if (density[element[fp]] >= density[element[sp]])
		{
			y[sp] = olyf; // Solid replaces Liquid (switch)
			x[sp] = olxf;

			allcoords[(int) x[sp]][(int) y[sp]] = sp;
			setBitmapColor((int) x[sp], (int) y[sp], element[sp]);
			allcoords[(int) x[fp]][(int) y[fp]] = fp;
			setBitmapColor((int) x[fp], (int) y[fp], element[fp]);
		}
		else
		{
			int ycounter;
			int xcounter;
			if (element[fp] == 1 || (element[fp] == 22 && colliseelement1[4]
					== 1))
			{
				for (ycounter = -1; ycounter <= 1; ycounter++)
				{
					for (xcounter = -1; xcounter <= 1; xcounter++)
					{
						if (element[allcoords[olxf + xcounter][olyf + ycounter]]
								== 4)
						{ // if there's anythign around that point that's plant
							int random = rand() % 2;
							random = random * 2 - 1;
							if (random == 1)
							{
								y[fp] = olyf; //put the water particle back
								x[fp] = olxf;
								element[fp] = 4; // change it to plant
								allcoords[(int) x[fp]][(int) y[fp]] = fp; //set the allcoords
								setBitmapColor((int) x[fp], (int) y[fp],
										element[fp]); //set the bitmapcolor
								return;
							}

						}
					}
				}
			}

			int random = rand() % 2;
			random = random * 2 - 1;
			if (allcoords[olxf + random][olyf + 1] == -1 && olxf + random > 1
					&& olyf + 1 < maxy && olxf + random < maxx) // left down
			{
				allcoords[olxf][olyf] = -1;
				//clear old spot
				setBitmapColor(olxf, olyf, 3);

				allcoords[olxf + random][olyf + 1] = fp;
				setBitmapColor(olxf + random, olyf + 1, element[fp]); //set new spot

				y[fp] = olyf + 1;
				x[fp] = olxf + random;
			}
			else if (allcoords[olxf - random][olyf + 1] == -1 && olxf - random
					> 1 && olyf + 1 < maxy && olxf - random < maxx) // left down
			{
				allcoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				allcoords[olxf - random][olyf + 1] = fp;
				setBitmapColor(olxf - random, olyf + 1, element[fp]); //set new spot

				y[fp] = olyf + 1;
				x[fp] = olxf - random;
			}
			else if (allcoords[olxf + random][olyf] == -1 && olxf + random
					< maxx && olxf + random > 1) // right
			{
				allcoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				allcoords[olxf + random][olyf] = fp;
				setBitmapColor(olxf + random, olyf, element[fp]); //set new spot

				y[fp] = olyf;
				x[fp] = olxf + random;
			}
			else if (allcoords[olxf - random][olyf] == -1 && olxf - random
					< maxx && olxf - random > 1) // right
			{
				allcoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				allcoords[olxf - random][olyf] = fp;
				setBitmapColor(olxf - random, olyf, element[fp]); //set new spot

				y[fp] = olyf;
				x[fp] = olxf - random;
			}
			else
			{
				y[fp] = olyf;
				x[fp] = olxf;

				allcoords[(int) x[fp]][(int) y[fp]] = fp;
				setBitmapColor((int) x[fp], (int) y[fp], element[fp]);
			}
			random = rand() % 7 - 3;
			xvel[fp] = random;
		}
	}
	//Liquid - Liquid generic collision
	else if (type == 3)
	{
		int ycounter;
		int xcounter;
		if (element[fp] == 1 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 1) || (element[sp] == 22
				&& colliseelement1[fp] != 1))
		{
			for (ycounter = -1; ycounter <= 1; ycounter++)
			{
				for (xcounter = -1; xcounter <= 1; xcounter++)
				{

					if (element[allcoords[olxf + xcounter][olyf + ycounter]]
							== 4)
					{ // if there's anythign around that point that's plant
						int random = rand() % 2;
						random = random * 2 - 1;
						if (random == 1)
						{
							y[fp] = olyf; //put the water particle back
							x[fp] = olxf;
							element[fp] = 4; // change it to plant
							allcoords[(int) x[fp]][(int) y[fp]] = fp; //set the allcoords
							setBitmapColor((int) x[fp], (int) y[fp],
									element[fp]); //set the bitmapcolor
							return;
						}

					}
				}
			}
		}
		int random = rand() % 2;
		random = random * 2 - 1;
		if (density[element[fp]] > density[element[sp]])
		{
			y[sp] = olyf; // Denser liquid replaces other one (switch)
			x[sp] = olxf;

			allcoords[(int) x[sp]][(int) y[sp]] = sp;
			setBitmapColor((int) x[sp], (int) y[sp], element[sp]);
			allcoords[(int) x[fp]][(int) y[fp]] = fp;
			setBitmapColor((int) x[fp], (int) y[fp], element[fp]);
		}
		else if (allcoords[olxf + random][olyf + 1] == -1 && olxf + random > 1
				&& olyf + 1 < maxy && olxf + random < maxx) // left down
		{
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			allcoords[olxf + random][olyf + 1] = fp;
			setBitmapColor(olxf + random, olyf + 1, element[fp]); //set new spot

			y[fp] = olyf + 1;
			x[fp] = olxf + random;
		}
		else if (allcoords[olxf - random][olyf + 1] == -1 && olxf - random > 1
				&& olyf + 1 < maxy && olxf - random < maxx) // left down
		{
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			allcoords[olxf - random][olyf + 1] = fp;
			setBitmapColor(olxf - random, olyf + 1, element[fp]); //set new spot

			y[fp] = olyf + 1;
			x[fp] = olxf - random;
		}
		else if (allcoords[olxf + random][olyf] == -1 && olxf + random < maxx
				&& olxf + random > 1) // right
		{
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			allcoords[olxf + random][olyf] = fp;
			setBitmapColor(olxf + random, olyf, element[fp]); //set new spot

			y[fp] = olyf;
			x[fp] = olxf + random;
		}
		else if (allcoords[olxf - random][olyf] == -1 && olxf - random < maxx
				&& olxf - random > 1) // right
		{
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			allcoords[olxf - random][olyf] = fp;
			setBitmapColor(olxf - random, olyf, element[fp]); //set new spot

			y[fp] = olyf;
			x[fp] = olxf - random;
		}
		else
		{
			y[fp] = olyf;
			x[fp] = olxf;

			allcoords[(int) x[fp]][(int) y[fp]] = fp;
			setBitmapColor((int) x[fp], (int) y[fp], element[fp]);
		}
		random = rand() % 7 - 3;
		xvel[fp] = random;
	}
	else if (type == 4) //Water plant
	{
		int random = rand() % 3;
		y[fp] = olyf; //put the water particle back
		x[fp] = olxf;
		if (random == 1)
		{
			element[fp] = 4; // change it to plant
			allcoords[(int) x[fp]][(int) y[fp]] = fp; //set the allcoords
			setBitmapColor((int) x[fp], (int) y[fp], element[fp]); //set the bitmapcolor
		}
	}
	else if (type == 5) //Nonflammable fire
	{
		int xfp = x[fp], yfp = y[fp];
		if (element[fp] == 5 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 5)) //fp is fire
		{
			//Move the fire back
			allcoords[olxf][olyf] = fp;
			setBitmapColor(olxf, olyf, element[fp]);

			x[fp] = olxf;
			y[fp] = olyf;

			//Add a random x velocity
			xvel[fp] += rand() % 7 - 3; //-3 to 3
		}
		else //sp is fire
		{
			//Move the fire to the old spot
			allcoords[olxf][olyf] = sp;
			setBitmapColor(olxf, olyf, element[sp]);

			x[sp] = olxf;
			y[sp] = olyf;

			//Set the new spot
			allcoords[xfp][yfp] = fp;
			setBitmapColor(xfp, yfp, element[fp]);

			//Add a random x velocity for the fire
			xvel[sp] += rand() % 7 - 3; //-3 to 3
		}
	}
	else if (type == 6) //Flammable Hot
	{
		if (element[fp] == 5 || element[fp] == 10 || (element[fp] == 22
				&& (colliseelement1[element[sp]] == 5
						|| colliseelement1[element[sp]] == 10)) || (element[sp]
				== 22 && (colliseelement1[element[fp]] != 5
				&& colliseelement1[element[fp]] != 10))) //fp is Hot
		{
			int xfp = (int) x[fp];
			int yfp = (int) y[fp];
			int counter;

			//Clear the spot the fire came from
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			//Set the new location to be fire
			element[fp] = 5;
			setBitmapColor(xfp, yfp, 5);
			allcoords[xfp][yfp] = fp;

			//Delete flammable (sp)
			x[sp] = 0;
			y[sp] = 0;
			element[sp] = 0;
			xvel[sp] = 0;
			yvel[sp] = 0;
			set[sp] = 0;
			avail[loq] = sp;
			loq++;
		}
		else //The flammable is fp
		{
			int xsp = (int) x[sp];
			int ysp = (int) y[sp];
			int counter;

			//Delete the old place
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);
			//Don't need to do this
			setBitmapColor(xsp, ysp, element[sp]);
			allcoords[xsp][ysp] = sp;

			//Delete the flammable (fp)
			x[fp] = 0;
			y[fp] = 0;
			element[fp] = 0;
			xvel[fp] = 0;
			yvel[fp] = 0;
			set[fp] = 0;
			avail[loq] = fp;
			loq++;
		}
	}
	//Fire - Fire generic collision
	else if (type == 7)
	{
		//Delete the old point
		allcoords[olxf][olyf] = -1;
		setBitmapColor(olxf, olyf, 3);

		int counter;

		//Delete fp
		x[fp] = 0;
		y[fp] = 0;
		element[fp] = 0;
		xvel[fp] = 0;
		yvel[fp] = 0;
		set[fp] = 0;
		avail[loq] = fp;
		loq++;
	}
	else if (type == 8) //Ice - Water collision
	{
		int random = rand() % 3;
		if (random == 0)
		{
			if (element[fp] == 1 || (element[fp] == 22
					&& colliseelement1[element[sp]] == 1) || (element[sp] == 22
					&& colliseelement1[element[fp]] != 1)) //fp is water
			{
				x[fp] = olxf; //Move it back
				y[fp] = olyf;
				element[fp] = 6; //Set it to ice
				allcoords[(int) x[fp]][(int) y[fp]] = fp; //Set the allcoords
				setBitmapColor((int) x[fp], (int) y[fp], 6); //Change the color
			}
			else
			{
				x[fp] = olxf; //Move the first particle back
				y[fp] = olyf;
				element[sp] = 6; //Change the water to ice
				setBitmapColor((int) x[fp], (int) y[fp], 6); //Change the color
				allcoords[(int) x[fp]][(int) y[fp]] = fp; //Set allcoords
				setBitmapColor((int) x[fp], (int) y[fp], element[fp]); //Set bitmap color
			}
		}
		else
		{
			if (density[element[fp]] >= density[element[sp]])
			{
				y[sp] = olyf; // Solid replaces Liquid (switch)
				x[sp] = olxf;

				allcoords[(int) x[sp]][(int) y[sp]] = sp;
				setBitmapColor((int) x[sp], (int) y[sp], element[sp]);
				allcoords[(int) x[fp]][(int) y[fp]] = fp;
				setBitmapColor((int) x[fp], (int) y[fp], element[fp]);
			}
			else
			{
				int ycounter;
				int xcounter;
				for (ycounter = -1; ycounter <= 1; ycounter++)
				{
					for (xcounter = -1; xcounter <= 1; xcounter++)
					{

						if (element[allcoords[olxf + xcounter][olyf + ycounter]]
								== 4)
						{ // if there's anythign around that point that's plant
							int random = rand() % 2;
							random = random * 2 - 1;
							if (random == 1)
							{
								y[fp] = olyf; //put the water particle back
								x[fp] = olxf;
								element[fp] = 4; // change it to plant
								allcoords[(int) x[fp]][(int) y[fp]] = fp; //set the allcoords
								setBitmapColor((int) x[fp], (int) y[fp], element[fp]); //set the bitmapcolor
								return;
							}

						}
					}
				}

				int random = rand() % 2;
				random = random * 2 - 1;
				if (allcoords[olxf + random][olyf + 1] == -1 && olxf + random
						> 1 && olyf + 1 < maxy && olxf + random < maxx) // left down
				{
					allcoords[olxf][olyf] = -1;
					//clear old spot
					setBitmapColor(olxf, olyf, 3);

					allcoords[olxf + random][olyf + 1] = fp;
					setBitmapColor(olxf + random, olyf + 1, element[fp]); //set new spot

					y[fp] = olyf + 1;
					x[fp] = olxf + random;
				}
				else if (allcoords[olxf - random][olyf + 1] == -1 && olxf
						- random > 1 && olyf + 1 < maxy && olxf - random < maxx) // left down
				{
					allcoords[olxf][olyf] = -1;
					setBitmapColor(olxf, olyf, 3);

					allcoords[olxf - random][olyf + 1] = fp;
					setBitmapColor(olxf - random, olyf + 1, element[fp]); //set new spot

					y[fp] = olyf + 1;
					x[fp] = olxf - random;
				}
				else if (allcoords[olxf + random][olyf] == -1 && olxf + random
						< maxx && olxf + random > 1) // right
				{
					allcoords[olxf][olyf] = -1;
					setBitmapColor(olxf, olyf, 3);

					allcoords[olxf + random][olyf] = fp;
					setBitmapColor(olxf + random, olyf, element[fp]); //set new spot

					y[fp] = olyf;
					x[fp] = olxf + random;
				}
				else if (allcoords[olxf - random][olyf] == -1 && olxf - random
						< maxx && olxf - random > 1) // right
				{
					allcoords[olxf][olyf] = -1;
					setBitmapColor(olxf, olyf, 3);

					allcoords[olxf - random][olyf] = fp;
					setBitmapColor(olxf - random, olyf, element[fp]); //set new spot

					y[fp] = olyf;
					x[fp] = olxf - random;
				}
				else
				{
					y[fp] = olyf;
					x[fp] = olxf;

					allcoords[(int) x[fp]][(int) y[fp]] = fp;
					setBitmapColor((int) x[fp], (int) y[fp], element[fp]);
				}
			}
		}
	}
	else if (type == 9) //Hot(Magma, Fire) - Ice
	{
		if (element[fp] == 6 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 6) || (element[sp] == 22
				&& colliseelement1[element[fp]] != 6)) //fp is ice
		{
			//This should almost never happen

			element[fp] = 1; //Change ice to water

			//Put it back
			x[fp] = olxf;
			y[fp] = olyf;
		}
		else //sp is ice
		{
			element[sp] = 1; //Change ice to water
			setBitmapColor((int) x[sp], (int) y[sp], 1);

			//Put hot back
			x[fp] = olxf;
			y[fp] = olyf;
		}
	}
	else if (type == 10) //Anything - Generator collision
	{
		if (element[fp] != 7)
		{
			//Change the generator to spawner
			element[sp] = 8;
			spawn[sp] = element[fp]; //Set the spawn element

			//Delete the old point
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			//Delete fp
			x[fp] = 0;
			y[fp] = 0;
			element[fp] = 0;
			xvel[fp] = 0;
			yvel[fp] = 0;
			set[fp] = 0;
			avail[loq] = fp;
			loq++;
		}
		else
		{
			//Change the generator to spawner
			element[fp] = 8;
			spawn[fp] = element[sp]; //Set the spawn element

			//Delete the old point
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			//Delete sp
			x[sp] = 0;
			y[sp] = 0;
			element[sp] = 0;
			xvel[sp] = 0;
			yvel[sp] = 0;
			set[sp] = 0;
			avail[loq] = sp;
			loq++;
		}
	}
	else if (type == 11) //Hot - Explosive generic
	{
		if (element[fp] == 5 || element[fp] == 11 || (element[fp] == 22
				&& (colliseelement1[element[sp]] == 5
						|| colliseelement1[element[sp]] == 11)) || (element[sp]
				== 22 && !(colliseelement1[element[fp]] == 5
				|| colliseelement1[element[fp]] == 11)))
		{
			//Delete the old point
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			//Set the new location of the fire
			allcoords[(int) x[fp]][(int) y[fp]] = fp;
			setBitmapColor((int) x[fp], (int) y[fp], element[fp]);

			//Delete sp
			x[sp] = 0;
			y[sp] = 0;
			element[sp] = 0;
			xvel[sp] = 0;
			yvel[sp] = 0;
			set[sp] = 0;
			avail[loq] = sp;
			loq++;

			int random = rand() % 10;
			random -= 5;
			xvel[fp] = random; //xvel is a random int from -5 to 5

			random = rand() % 10;
			random -= 5;
			yvel[fp] = random; //yvel is a random int from -5 to 5

			//Look at all the points around the explosion, if explosive change to fire and add velocity,
			// if flammable, just change to fire, if empty, change to fire and add velocity
			int i, j, xfp = (int) x[fp], yfp = (int) y[fp], a;
			for (i = -2; i <= 2; i++)
			{
				for (j = -2; j <= 2; j++)
				{
					if (xfp + i > 1 && xfp + i < maxx && yfp + j > 1 && yfp + j
							< maxy && !(i == 0 && j == 0))
					{
						a = allcoords[xfp + i][yfp + j]; //The allcoords at the prospective point
						if (a != -1 && collision[element[a]][5] == 6)
						{
							element[a] = 5;
							setBitmapColor(xfp + i, yfp + j, 5);
						}
						else if (a != -1 && collision[element[a]][5] == 11)
						{
							element[a] = 5;
							setBitmapColor(xfp + i, yfp + j, 5);

							int random = rand() % 10;
							random -= 5;
							xvel[a] = random; //xvel is a random int from -5 to 5

							random = rand() % 10;
							random -= 5;
							yvel[a] = random; //yvel is a random int from -5 to 5
						}
						else if (a == -1)
						{
							CreatePoint(xfp + i, yfp + j, 5);
						}
					}
				}
			}
		}
		else
		{
			//Delete the old point
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			//Delete fp
			x[fp] = 0;
			y[fp] = 0;
			element[fp] = 0;
			xvel[fp] = 0;
			yvel[fp] = 0;
			set[fp] = 0;
			avail[loq] = fp;
			loq++;

			//Set sp
			allcoords[(int) x[sp]][(int) y[sp]] = sp;
			setBitmapColor((int) x[sp], (int) y[sp], element[sp]);

			int random = rand() % 10;
			random -= 5;
			xvel[sp] = random; //xvel is a random int from -5 to 5

			random = rand() % 10;
			random -= 5;
			yvel[sp] = random; //yvel is a random int from -5 to 5

			int i, j, xsp = x[sp], ysp = y[sp], a;
			for (i = -2; i <= 2; i++)
			{
				for (j = -2; j <= 2; j++)
				{
					if (xsp + i > 1 && xsp + i < maxx && ysp + j > 1 && ysp + j
							< maxy && !(i == 0 && j == 0))
					{
						a = allcoords[xsp + i][ysp + j];
						if (a != -1 && collision[element[a]][5] == 6)
						{
							element[a] = 5;
							setBitmapColor(xsp + i, ysp + j, 5);
						}
						else if (a != -1 && collision[element[a]][5] == 11)
						{
							element[a] = 5;
							setBitmapColor(xsp + i, ysp + j, 5);

							int random = rand() % 10;
							random -= 5;
							xvel[a] = random; //xvel is a random int from -5 to 5

							random = rand() % 10;
							random -= 5;
							yvel[a] = random; //yvel is a random int from -5 to 5
						}
						else if (a == -1)
						{
							CreatePoint(xsp + i, ysp + j, 5);
						}
					}
				}
			}
		}
	}
	else if (type == 12) //Water - Magma Collision
	{
		if (element[fp] == 10 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 10) || (element[sp] == 22
				&& colliseelement1[element[fp]] != 10)) //fp is magma
		{
			//Move the water to magma's location and make it steam
			element[sp] = 18;

			x[sp] = olxf;
			y[sp] = olyf;
			allcoords[olxf][olyf] = sp;
			setBitmapColor(olxf, olyf, 18);

			element[fp] = 11; //Make the magma into stone
			setBitmapColor((int) x[fp], (int) y[fp], 11); //Set the color
			allcoords[(int) x[fp]][(int) y[fp]] = fp;

			if (rand() % 3 == 0) //1/3 chance
			{
				DeletePoint(fp); //Delete magma
			}
		}
		else //sp is magma
		{
			element[sp] = 11; //Change magma to stone
			setBitmapColor((int) x[sp], (int) y[sp], 11); //Set the color

			//Change water to steam
			element[fp] = 18;

			//Move it back
			allcoords[olxf][olyf] = fp;
			setBitmapColor(olxf, olyf, 18);

			x[fp] = olxf;
			y[fp] = olyf;

			if (rand() % 3 == 0) //1/3 chance
			{
				DeletePoint(sp); //Delete magma
			}
		}
	}
	else if (type == 13) //Hot(Magma, Fire) - Stone Collision
	{
		if (element[fp] == 11 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 11) || (element[sp] == 22
				&& colliseelement1[element[fp]] != 11)) //fp is stone
		{
			if (rand() % 10 == 0) //1/10 chance
			{
				x[fp] = olxf;
				y[fp] = olyf;
				allcoords[(int) x[fp]][(int) y[fp]] = fp; //Move the stone back

				element[fp] = 10; //Change stone to magma
				setBitmapColor((int) x[fp], (int) y[fp], 10); //Set the color also
			}
			else
			{
				//Clear old spot
				allcoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				y[sp] = olyf; //Stone replaces Hot (switch)
				x[sp] = olxf;

				allcoords[olxf][olyf] = sp;
				setBitmapColor(olxf, olyf, element[sp]);
				allcoords[(int) x[fp]][(int) y[fp]] = fp;
				setBitmapColor((int) x[fp], (int) y[fp], element[fp]);

				//Set a random xvel
				xvel[sp] = rand() % 3 - 1;
			}
		}
		else
		{
			//Either way, move fp back
			x[fp] = olxf;
			y[fp] = olyf;
			allcoords[olxf][olyf] = fp;
			setBitmapColor(olxf, olyf, element[fp]);

			if (rand() % 10 == 0) //1/10 chance
			{
				element[sp] = 10; //Change stone to magma
				setBitmapColor((int) x[sp], (int) y[sp], 10); //Set the color also
				allcoords[(int) x[sp]][(int) y[sp]] = sp;
			}

			//Set a random xvel
			xvel[fp] = rand() % 3 - 1;
		}
	}
	else if (type == 14) //Stone - Stone
	{
		y[fp] = olyf; // First particle goes back to where it was before
		x[fp] = olxf;

		allcoords[(int) x[fp]][(int) y[fp]] = fp;
		setBitmapColor((int) x[fp], (int) y[fp], element[fp]);
	}
	else if (type == 15) // C4 - hot
	{
		if (element[fp] == 12 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 12) || (element[sp] == 22
				&& colliseelement1[element[fp]] != 12)) //fp is C4
		{
			//Put back the C4
			x[fp] = olxf;
			y[fp] = olyf;
			//allcoords[olxs][olys] = sp;
			//setBitmapColor(olxs,olys,element[sp]);

			//Change C4 to fire
			element[fp] = 5;
			setBitmapColor((int) x[fp], (int) y[fp], 5);

			int txpos = (int) x[fp], typos = (int) y[fp]; //temporary x and ypositions for quick calcs
			//this section is for explosion, giving velocities to particles around explosion
			int i, j; //counters, i is the x, j is the y
			int tpart, telement; //temp particle index /element
			for (i = -0; i <= 0; i++)
			{
				for (j = -0; j <= 0; j++)
				{
					if (txpos + i < maxx && txpos + i > 1 && typos + j < maxy
							&& typos + j > 1) //if within bounds
					{
						tpart = allcoords[txpos + i][typos + j];

						if (tpart != -1
								&& (fallvel[element[tpart]] != 0
										|| element[tpart] == 15
										|| element[tpart] == 21)) //if there's a particle there & it's not immovable
						{
							if (abs(xvel[tpart]) < 20)
							{
								xvel[tpart] += 1 * i / abs(i);
							}
							if (abs(yvel[tpart]) < 20)
							{
								yvel[tpart] += 1 * j / abs(j);
							}
						}
					}
				}
			}

			//Look at all the points around the explosion, if explosive change to fire and add velocity,
			// if flammable, just change to fire, if empty, change to fire and add velocity
			int a; //Temp allcoords
			for (i = -2; i <= 2; i++)
			{
				for (j = -2; j <= 2; j++)
				{
					if (txpos + i > 1 && txpos + i < maxx && typos + j > 1
							&& typos + j < maxy)
					{
						a = allcoords[txpos + i][typos + j]; //The allcoords at the prospective point
						if (a != -1 && collision[element[a]][5] == 6)
						{
							element[a] = 5;
							setBitmapColor(txpos + i, typos + j, 5);
						}
						else if (a != -1 && (collision[element[a]][5] == 11
								|| collision[element[a]][5] == 15))
						{
							element[a] = 5;
							setBitmapColor(txpos + i, typos + j, 5);

							int random = rand() % 11;
							random -= 5;
							xvel[a] = random; //xvel is a random int from -5 to 5

							random = rand() % 11; //mod 11 goes from 0 to 10
							random -= 5;
							yvel[a] = random; //yvel is a random int from -5 to 5
						}
						else if (a == -1)
						{
							CreatePoint(txpos + i, typos + j, 5);
						}
					}
				}
			}
		}
		else //sp is C4
		{
			//put back the hot
			x[fp] = olxf;
			y[fp] = olyf;

			int spelement = element[sp];
			//change C4 to fire
			element[sp] = 5;
			setBitmapColor((int) x[sp], (int) y[sp], 5);

			int txpos = (int) x[sp], typos = (int) y[sp]; //temporary x and ypositions for quick calcs
			//this section is for explosion, giving velocities to particles around explosion
			int i, j; //counters, i is the x, j is the y
			int tpart, telement; //temp particle index /element
			for (i = -2 * exploness[spelement]; i <= 2 * exploness[spelement]; i++)
			{
				for (j = -2 * exploness[spelement]; j <= 2
						* exploness[spelement]; j++)
				{
					if (txpos + i < maxx && txpos + i > 1 && typos + j < maxy
							&& typos + j > 1) //if within bounds
					{
						tpart = allcoords[txpos + i][typos + j];

						if (tpart != -1
								&& (fallvel[element[tpart]] != 0
										|| element[tpart] == 15
										|| element[tpart] == 21)) //if there's a particle there
						{
							if (abs(xvel[tpart]) < exploness[spelement])
							{
								xvel[tpart] += ((1 / i) * 3) + i / abs(i);
							}
							if (abs(yvel[tpart]) < exploness[spelement])
							{
								yvel[tpart] += ((1 / j) * 3) + j / abs(j);
							}
						}
					}
				}
			}

			//Look at all the points around the explosion, if explosive change to fire and add velocity,
			// if flammable, just change to fire, if empty, change to fire and add velocity
			int a; //Temp allcoords
			for (i = -2; i <= 2; i++)
			{
				for (j = -2; j <= 2; j++)
				{
					if (txpos + i > 1 && txpos + i < maxx && typos + j > 1
							&& typos + j < maxy)
					{
						a = allcoords[txpos + i][typos + j]; //The allcoords at the prospective point
						if (a != -1 && collision[element[a]][5] == 6)
						{
							element[a] = 5;
							setBitmapColor(txpos + i, typos + j, 5);
						}
						else if (a != -1 && (collision[element[a]][5] == 11
								|| collision[element[a]][5] == 15))
						{
							element[a] = 5;
							setBitmapColor(txpos + i, typos + j, 5);

							int random = rand() % 11; //mod 11 goes from 0 to 10
							random -= 5;
							xvel[a] = random; //xvel is a random int from -5 to 5

							random = rand() % 11;
							random -= 5;
							yvel[a] = random; //yvel is a random int from -5 to 5
						}
						else if (a == -1)
						{
							CreatePoint(txpos + i, typos + j, 5);
						}
					}
				}
			}
		}
	}
	else if (type == 16) // C4++ - hot
	{
		if (element[fp] == 13 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 13) || (element[sp] == 22
				&& colliseelement1[element[fp]] != 13)) //fp is C4++
		{
			//Put back the hot
			x[sp] = olxs;
			y[sp] = olys;
			allcoords[olxs][olys] = sp;
			setBitmapColor(olxs, olys, element[sp]);

			//Change C4++ to fire
			element[fp] = 5;
			setBitmapColor((int) x[fp], (int) y[fp], 5);

			int txpos = (int) x[fp], typos = (int) y[fp]; //temporary x and ypositions for quick calcs
			//this section is for explosion, giving velocities to particles around explosion
			int i, j; //counters, i is the x, j is the y
			int tpart, telement; //temp particle index /element
			for (i = -0; i <= 0; i++)
			{
				for (j = -0; j <= 0; j++)
				{
					if (txpos + i < maxx && txpos + i > 1 && typos + j < maxy
							&& typos + j > 1) //if within bounds
					{
						tpart = allcoords[txpos + i][typos + j];

						if (tpart != -1
								&& (fallvel[element[tpart]] != 0
										|| element[tpart] == 15
										|| element[tpart] == 21)) //if there's a particle there & it's not immovable
						{
							if (abs(xvel[tpart]) < 20)
							{
								xvel[tpart] += ((1 / i) * 0) + 1 * i / abs(i);
							}
							if (abs(yvel[tpart]) < 20)
							{
								yvel[tpart] += ((1 / j) * 0) + 1 * j / abs(j);
							}
						}
					}
				}
			}

			//Look at all the points around the explosion, if explosive change to fire and add velocity,
			// if flammable, just change to fire, if empty, change to fire and add velocity
			int a; //Temp allcoords
			for (i = -2; i <= 2; i++)
			{
				for (j = -2; j <= 2; j++)
				{
					if (txpos + i > 1 && txpos + i < maxx && typos + j > 1
							&& typos + j < maxy)
					{
						a = allcoords[txpos + i][typos + j]; //The allcoords at the prospective point
						if (a != -1 && collision[element[a]][5] == 6)
						{
							element[a] = 5;
							setBitmapColor(txpos + i, typos + j, 5);
						}
						else if (a != -1 && (collision[element[a]][5] == 11
								|| collision[element[a]][5] == 15
								|| collision[element[a]][5] == 16))
						{
							element[a] = 5; //change a to fire
							setBitmapColor(txpos + i, typos + j, 5); //change color of a to that of fire

							int random = rand() % 11; // mod 11 goes from 0 to 10
							random -= 5;
							xvel[a] = random; //xvel is a random int from -5 to 5

							random = rand() % 11;
							random -= 5;
							yvel[a] = random; //yvel is a random int from -5 to 5
						}
						else if (a == -1)
						{
							CreatePoint(txpos + i, typos + j, 5);
						}
					}
				}
			}
		}
		else //sp is C4++
		{
			//put back the hot
			x[fp] = olxf;
			y[fp] = olyf;

			int spelement = element[sp];
			//change C4++ to fire
			element[sp] = 5;
			setBitmapColor((int) x[sp], (int) y[sp], 5);

			int txpos = (int) x[sp], typos = (int) y[sp]; //temporary x and ypositions for quick calcs
			//this section is for explosion, giving velocities to particles around explosion
			int i, j; //counters, i is the x, j is the y
			int tpart, telement; //temp particle index /element
			for (i = -2 * exploness[spelement]; i <= 2 * exploness[spelement]; i++)
			{
				for (j = -2 * exploness[spelement]; j <= 2
						* exploness[spelement]; j++)
				{
					if (txpos + i < maxx && txpos + i > 1 && typos + j < maxy
							&& typos + j > 1) //if within bounds
					{
						tpart = allcoords[txpos + i][typos + j];

						if (tpart != -1
								&& (fallvel[element[tpart]] != 0
										|| element[tpart] == 15
										|| element[tpart] == 21)) //if there's a particle there
						{
							if (abs(xvel[tpart]) < exploness[spelement])
							{
								xvel[tpart] += ((1 / i) * 3) + i / abs(i);
							}
							if (abs(yvel[tpart]) < exploness[spelement])
							{
								yvel[tpart] += ((1 / j) * 3) + j / abs(j);
							}
						}
					}
				}
			}

			//Look at all the points around the explosion, if explosive change to fire and add velocity,
			// if flammable, just change to fire, if empty, change to fire and add velocity
			int a; //Temp allcoords
			for (i = -2; i <= 2; i++)
			{
				for (j = -2; j <= 2; j++)
				{
					if (txpos + i > 1 && txpos + i < maxx && typos + j > 1
							&& typos + j < maxy)
					{
						a = allcoords[txpos + i][typos + j]; //The allcoords at the prospective point
						if (a != -1 && collision[element[a]][5] == 6)
						{
							element[a] = 5;
							setBitmapColor(txpos + i, typos + j, 5);
						}
						else if (a != -1 && (collision[element[a]][5] == 11
								|| collision[element[a]][5] == 15
								|| collision[element[a]][5] == 16))
						{
							element[a] = 5;
							setBitmapColor(txpos + i, typos + j, 5);

							int random = rand() % 11; //mod 11 goes from 0 to 10
							random -= 5;
							xvel[a] = random; //xvel is a random int from -5 to 5

							random = rand() % 11; //mod 11 goes from 0 to 10
							random -= 5;
							yvel[a] = random; //yvel is a random int from -5 to 5
						}
						else if (a == -1)
						{
							CreatePoint(txpos + i, typos + j, 5);
						}
					}
				}
			}
		}
	}
	else if (type == 17) //Magma - Destructible Wall
	{
		//We can assume that fp is magma since wall doesn't move
		//move magma back
		//TODO: Edit this to allow a Destructible wall - Magma collision
		x[fp] = olxf;
		y[fp] = olyf;
		allcoords[olxf][olyf] = fp;
		setBitmapColor(olxf, olyf, element[fp]);

		if (rand() % 10 == 0) //1/10 chance
		{
			element[sp] = 10; //Change wall to magma
			setBitmapColor((int) x[sp], (int) y[sp], 10); //Set the color also
			allcoords[(int) x[sp]][(int) y[sp]] = sp;
		}

		//Set a random xvel to magma
		xvel[fp] = rand() % 3 - 1;
	}
	else if (type == 18) //Acid - Meltable
	{
		int xfp = x[fp], yfp = y[fp];
		if (element[fp] == 17 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 17) || (element[sp] == 22
				&& colliseelement1[element[fp]] != 17)) //fp is acid
		{
			if (rand() % 3 != 0) //2/3 chance
			{
				//Acid burns away sp

				//Delete the old point
				allcoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				//Set the new point
				allcoords[xfp][yfp] = fp;
				setBitmapColor(xfp, yfp, element[fp]);

				//Delete sp
				x[sp] = 0;
				y[sp] = 0;
				element[sp] = 0;
				xvel[sp] = 0;
				yvel[sp] = 0;
				set[sp] = 0;
				avail[loq] = sp;
				loq++;
			}
			else if (rand() % 2 == 0) //Otherwise, 1/6 total
			{
				//Acid is neutralized

				//Delete the old point
				allcoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				//Delete fp
				x[fp] = 0;
				y[fp] = 0;
				element[fp] = 0;
				xvel[fp] = 0;
				yvel[fp] = 0;
				set[fp] = 0;
				avail[loq] = fp;
				loq++;
			}
			else //Otherwise, 1/6 total
			{
				//Acid bounces

				x[fp] = olxf;
				y[fp] = olyf;
			}
		}
		else //sp is acid
		{
			if (rand() % 3 != 0) //2/3 chance
			{
				//Acid burns away fp

				//Delete the old point
				allcoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				//Delete fp
				x[fp] = 0;
				y[fp] = 0;
				element[fp] = 0;
				xvel[fp] = 0;
				yvel[fp] = 0;
				set[fp] = 0;
				avail[loq] = fp;
				loq++;
			}
			else if (rand() % 2 == 0) //Otherwise, 1/6 total
			{
				//Acid is neutralized

				//Delete the old point
				allcoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				//Set the new point
				allcoords[xfp][yfp] = fp;
				setBitmapColor(xfp, yfp, element[fp]);

				//Delete sp
				x[sp] = 0;
				y[sp] = 0;
				element[sp] = 0;
				xvel[sp] = 0;
				yvel[sp] = 0;
				set[sp] = 0;
				avail[loq] = sp;
				loq++;
			}
			else //Otherwise, 1/6 total
			{
				//Other particle bounces bounces

				x[fp] = olxf;
				y[fp] = olyf;
			}
		}
	}
	else if (type == 19) //Acid - Water
	{
		int xfp = x[fp], yfp = y[fp];
		//Acid goes away 1/3 of the time, otherwise bounce back
		if (element[fp] == 17 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 17) || (element[sp] == 22
				&& colliseelement1[element[fp]] != 17)) //fp is acid
		{
			if (rand() % 3 == 0)
			{
				//Delete the old point
				allcoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				//Delete fp
				x[fp] = 0;
				y[fp] = 0;
				element[fp] = 0;
				xvel[fp] = 0;
				yvel[fp] = 0;
				set[fp] = 0;
				avail[loq] = fp;
				loq++;
			}
			else
			{
				//Move the point back
				x[fp] = olxf;
				y[fp] = olyf;
			}
		}
		else //sp is acid
		{
			if (rand() % 3 == 0)
			{
				//Delete the old point
				allcoords[olxf][olyf] = -1;
				setBitmapColor(olxf, olyf, 3);

				//Set the new point
				allcoords[xfp][yfp] = fp;
				setBitmapColor(xfp, yfp, element[fp]);

				//Delete sp
				x[sp] = 0;
				y[sp] = 0;
				element[sp] = 0;
				xvel[sp] = 0;
				yvel[sp] = 0;
				set[sp] = 0;
				avail[loq] = sp;
				loq++;
			}
			else
			{
				//Move the water back
				x[fp] = olxf;
				y[fp] = olyf;
			}
		}
	}
	else if (type == 20) //Steam - Ice/Water
	{
		if (rand() % 200 == 0) //1/200 chance for steam to condense
		{
			if (element[fp] == 18 || (element[fp] == 22
					&& colliseelement1[element[sp]] == 18) || (element[sp]
					== 22 && colliseelement1[element[fp]] != 18)) //fp is steam
			{
				//Change the steam to water
				element[fp] = 1;

				//Move it back
				x[fp] = olxf;
				y[fp] = olyf;

				allcoords[olxf][olyf] = fp;
				setBitmapColor(olxf, olyf, 1);
			}
			else //sp is steam
			{
				//Change the steam to water
				element[sp] = 1;

				//Move fp back
				x[fp] = olxf;
				y[fp] = olyf;

				allcoords[olxf][olyf] = fp;
				setBitmapColor(olxf, olyf, element[fp]);
			}
		}
		else //Just bounce around the water/ice
		{
			if (element[fp] == 18 || (element[fp] == 22
					&& colliseelement1[element[sp]] == 18)) //fp is steam
			{
				//Move it back
				x[fp] = olxf;
				y[fp] = olyf;

				allcoords[olxf][olyf] = fp;
				setBitmapColor(olxf, olyf, 18);

				//Add a random x velocity
				xvel[fp] += rand() % 7 - 3; //-3 to 3
			}
			else //sp is steam
			{
				int xfp = x[fp], yfp = y[fp]; //Set some temp variables

				//Switch places
				x[sp] = olxf;
				y[sp] = olyf;

				allcoords[olxf][olyf] = sp;
				setBitmapColor(olxf, olyf, 18);

				allcoords[xfp][yfp] = fp;
				setBitmapColor(xfp, yfp, element[fp]);
			}
		}
	}
	else if (type == 21) //Steam - Steam
	{
		if (rand() % 1000 == 0)//1/5 chance
		{
			//Make the two steams "condense" into water

			int xsp = x[sp], ysp = y[sp]; //Some temp variables

			//Move fp back
			x[fp] = olxf;
			y[fp] = olyf;

			//Change the elements
			element[fp] = 1;
			element[sp] = 1;

			//Set the bitmap and allcoords
			allcoords[olxf][olyf] = fp;
			setBitmapColor(olxf, olyf, 1);

			allcoords[xsp][ysp] = sp;
			setBitmapColor(xsp, ysp, 1);
		}
		else //Make fp bounce off
		{
			//Move fp back
			x[fp] = olxf;
			y[fp] = olyf;

			allcoords[olxf][olyf] = fp;
			setBitmapColor(olxf, olyf, 18);

			//Add a random x velocity
			xvel[fp] += rand() % 7 - 3; //-3 to 3
			//Add a random y velocity
			yvel[fp] += rand() % 5 - 2; //-2 to 2
		}
	}
	else if (type == 22) //Steam - Other
	{
		int xfp = x[fp], yfp = y[fp];
		if (element[fp] == 18 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 18) || (element[sp] == 22
				&& colliseelement1[element[fp]] != 18)) //fp is steam
		{
			//Move the steam back
			allcoords[olxf][olyf] = fp;
			setBitmapColor(olxf, olyf, element[sp]);

			x[fp] = olxf;
			y[fp] = olyf;

			//Set a random x velocity
			xvel[fp] = rand() % 7 - 3; //-3 to 3
		}
		else //sp is steam
		{
			//Move the steam to the old spot
			allcoords[olxf][olyf] = sp;
			setBitmapColor(olxf, olyf, element[sp]);

			x[sp] = olxf;
			y[sp] = olyf;

			//Set the new spot
			allcoords[xfp][yfp] = fp;
			setBitmapColor(xfp, yfp, element[fp]);

			//Set a random x velocity
			xvel[sp] = rand() % 7 - 3; //-3 to 3
		}
	}
	else if (type == 23) //Water - Fire
	{
		if (element[fp] == 5 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 5) || (element[sp] == 22
				&& colliseelement1[element[fp]] != 5)) //fp is fire
		{
			//Delete the old point
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			//Delete the fire
			x[fp] = 0;
			y[fp] = 0;
			element[fp] = 0;
			xvel[fp] = 0;
			yvel[fp] = 0;
			set[fp] = 0;
			avail[loq] = fp;
			loq++;

			//Change the water to steam
			element[sp] = 18;

			//Change the bitmap color
			setBitmapColor(x[sp], y[sp], 18);
		}
		else //sp is fire
		{
			int xfp = x[fp], yfp = y[fp]; //Set temp variables

			//Delete sp(fire)
			DeletePoint(sp);

			//Delete the old point
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			//Change the water to steam
			element[fp] = 18;

			//Set the new location of water
			allcoords[xfp][yfp] = fp;
			setBitmapColor(xfp, yfp, 18);
		}
	}
	else if (type == 24) //Salt - Water
	{
		if (element[fp] == 19 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 19)) //fp is salt
		{
			//Delete the old point
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			//Delete fp
			x[fp] = 0;
			y[fp] = 0;
			element[fp] = 0;
			xvel[fp] = 0;
			yvel[fp] = 0;
			set[fp] = 0;
			avail[loq] = fp;
			loq++;

			//Change the element of sp to Salt-water
			element[sp] = 20;
			setBitmapColor(x[sp], y[sp], 20);
		}
		else //sp is salt
		{
			//Delete the old point
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			DeletePoint(sp); //Delete the salt

			int xfp = x[fp], yfp = y[fp]; //Some temp variables
			//Set the new coordinates
			allcoords[xfp][yfp] = fp;
			setBitmapColor(xfp, yfp, element[fp]);
		}
	}
	else if (type == 25) //Salt - Ice
	{
		if (element[fp] == 6 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 6) || (element[sp] == 22
				&& colliseelement1[element[fp]] != 6)) //fp is ice
		{
			//Move fp back
			x[fp] = olxf;
			y[fp] = olyf;

			//Change the element to water
			element[fp] = 1;

			//Set the bitmap stuff
			allcoords[olxf][olyf] = fp;
			setBitmapColor(olxf, olyf, 1);
		}
		else //sp is ice
		{
			//Move fp back
			x[fp] = olxf;
			y[fp] = olyf;

			allcoords[olxf][olyf] = fp;
			setBitmapColor(olxf, olyf, element[fp]);

			//Change the element to water
			element[sp] = 1;

			//Set the bitmap stuff
			setBitmapColor(x[sp], y[sp], 1);
		}
	}
	else if (type == 26) //Salt-water - Plant
	{
		if (element[fp] == 4 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 4) || (element[sp] == 22
				&& colliseelement1[element[fp]] != 4)) //fp is plant
		{
			//Move fp back
			x[fp] = olxf;
			y[fp] = olyf;

			//Change the element to sand
			element[fp] = 0;

			//Set the bitmap stuff
			allcoords[olxf][olyf] = fp;
			setBitmapColor(olxf, olyf, 0);
		}
		else //sp is plant
		{
			//Move fp back
			x[fp] = olxf;
			y[fp] = olyf;

			allcoords[olxf][olyf] = fp;
			setBitmapColor(olxf, olyf, element[fp]);

			//Change the element to sand
			element[sp] = 0;

			//Set the bitmap stuff
			setBitmapColor(x[sp], y[sp], 0);
		}
	}
	else if (type == 27) //Magma - Sand
	{
		if (element[fp] == 10 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 10) || (element[sp] == 22
				&& colliseelement1[element[fp]] != 10)) //fp is magma
		{
			//Move fp back
			x[fp] = olxf;
			y[fp] = olyf;

			//Change the element to glass
			element[sp] = 21;

			//Set the bitmap stuff
			setBitmapColor(x[sp], y[sp], 21);
		}
		else //sp is magma
		{
			//Move fp back
			x[fp] = olxf;
			y[fp] = olyf;

			//Change the element to glass
			element[fp] = 21;
			setBitmapColor(x[fp], y[fp], 21);

		}
	}
	else if (type == 28) //Water - Sand
	{
		if (element[fp] == 1 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 1) || (element[sp] == 22
				&& colliseelement1[element[fp]] != 1)) //fp is water
		{

			//Delete the old point
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			//Delete the water
			x[fp] = 0;
			y[fp] = 0;
			element[fp] = 0;
			xvel[fp] = 0;
			yvel[fp] = 0;
			set[fp] = 0;
			avail[loq] = fp;
			loq++;

			element[sp] = 23; // set the sand to be mud
			setBitmapColor(x[sp], y[sp], element[sp]); // change the color of the sand particle location to be mud
		}
		else //sp is water
		{

			//Delete the old point
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			DeletePoint(sp); //delete the water
			element[fp] = 23; //set the sand to be mud

			int xfp = x[fp], yfp = y[fp]; //Some temp variables
			//Set the new coordinates
			allcoords[xfp][yfp] = fp;
			setBitmapColor(xfp, yfp, element[fp]);

		}

	}
	else if (type == 29)
	{ //Mud - Fire

		if (element[fp] == 23 || (element[fp] == 22
				&& colliseelement1[element[sp]] == 23) || (element[sp] == 22
				&& colliseelement1[element[fp]] != 23))
		{ //sp is mud

			//Delete the old point
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			DeletePoint(sp); //delete the fire
			element[fp] = 0; //change the mud to sand

			int xfp = x[fp], yfp = y[fp]; //Some temp variables

			allcoords[xfp][yfp] = fp;
			setBitmapColor(xfp, yfp, element[fp]); //update the color of mud

		}
		else
		{ // fp is fire

			//Delete the old point
			allcoords[olxf][olyf] = -1;
			setBitmapColor(olxf, olyf, 3);

			//Delete the fire
			x[fp] = 0;
			y[fp] = 0;
			element[fp] = 0;
			xvel[fp] = 0;
			yvel[fp] = 0;
			set[fp] = 0;
			avail[loq] = fp;
			loq++;

			element[sp] = 0;
			setBitmapColor(x[sp], y[sp], element[sp]);
		}
	}
	else if (type == 30)
	{ //Mud - Magma

		x[fp] = olxf; //send the fp back
		y[fp] = olyf;

		element[fp] = 11; //change fp to stone (it really doesn't matter which one we are changing to what
		element[sp] = 18; //change sp to steam

		setBitmapColor(y[fp], y[fp], element[fp]); //update colors
		setBitmapColor(x[sp], y[sp], element[sp]);
	}
}
