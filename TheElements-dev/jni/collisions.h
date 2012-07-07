/*
 * collisions.h
 * -------------------------------
 * Contains all the collision data in a table. At some
 * point hopefully we can make some code that will load
 * this collision data from a file. When adding an element,
 * it must be added here.
 */

#ifndef COLLISIONS_H_INCLUDED
#define COLLISIONS_H_INCLUDED

//Include the global macros
#include "macros.h"

/*
 * Elements list:
 * ----Special----
 *		0 = Spawn
 *		1 = Drag
 *		2 = Eraser
 * ----Normal----
 *		3 = Sand
 *		4 = Water
 *		5 = Steam
 *		6 = Ice
 *		7 = Wall
 *		8 = Destructible Wall
 *		9 = Plant
 *		10 = Fire
 *		11 = Magma
 *		12 = Stone
 *		13 = Oil
 *		14 = C4
 *		15 = C4++
 *		16 = Fuse
 *		17 = Acid
 *		18 = Salt
 *		19 = Salt-Water
 *		20 = Glass
 *		21 = Mud
 *		22 = Generator
 *		23 = Coal
 *		24 = Bug
 * ----Add Elements Above----
 */

/*
 * Collisions list:
 *		0 = Solid - Solid
 *		1 = Density Based
 *		2 = Anything - Generator
 *		3 = Generator - Anything (*)
 *		4 = Acid - Meltable
 *		5 = Meltable - Acid
 *		6 = Acid - Neutralizer
 *		7 = Neutralizer - Acid
 *		8 = Salt - Water, Water - Salt, Salt - Ice, Ice - Salt(*), Salt-Water - Ice, Ice - Salt-Water(*)
 *		9 = Salt-Water - Plant or Plant - Salt-Water
 *		10 = Water - Sand, Sand - Water, Coal - Water, Water - Coal
 *		* = Should not happen, something messed up
 */

char reciprocals[NUM_COLLISIONS] =
{
		0,
		1,
		3,
		2,
		5,
		4,
		7,
		6,
		8,
		9,
		10
};

char collision[NUM_BASE_ELEMENTS][NUM_BASE_ELEMENTS] =
{
	//Spawn 0
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0},
	//Drag 1
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0},
	//Eraser 2
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0},
	//Sand 3
	{0, 0, 0, 0, 10, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 2, 0, 0, 0},
	//Water 4
	{1, 1, 1, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 7, 8, 1, 1, 1, 2, 10, 1, 1},
	//Steam 5
	{1, 1, 1, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 7, 1, 1, 1, 1, 2, 1, 1, 1},
	//Ice 6
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 8, 8, 0, 1, 2, 0, 0, 0},
	//Wall 7
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 2, 0, 0, 0},
	//Destructible Wall 8
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 5, 0, 1, 0, 1, 2, 0, 0, 0},
	//Plant 9
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 5, 0, 9, 0, 1, 2, 0, 0, 0},
	//Fire 10
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 2, 1, 1, 1},
	//Magma 11
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 2, 1, 1, 1},
	//Stone 12
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 2, 0, 0, 0},
	//Oil 13
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 2, 1, 1, 1},
	//C4 14
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 5, 0, 1, 0, 1, 2, 0, 0, 0},
	//C4++ 15
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 5, 0, 1, 0, 1, 2, 0, 0, 0},
	//Fuse 16
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 5, 0, 1, 0, 1, 2, 0, 0, 0},
	//Acid 17
	{1, 1, 1, 1, 6, 6, 1, 1, 4, 4, 4, 4, 1, 4, 4, 4, 4, 1, 1, 6, 1, 6, 2, 1, 4, 1},
	//Salt 18
	{0, 0, 0, 0, 8, 1, 8, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 2, 0, 9, 0},
	//Salt-Water 19
	{1, 1, 1, 1, 1, 1, 8, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, 7, 8, 1, 1, 1, 2, 1, 9, 1},
	//Glass 20
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 2, 0, 0, 0},
	//Mud 21
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 7, 1, 1, 1, 1, 2, 1, 1, 1},
	//Generator 22
	{0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 3, 3, 3},
	//Coal 23
	{0, 0, 0, 0, 10, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 2, 0, 0, 0},
	//Bug 24
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 5, 9, 9, 0, 1, 2, 0, 0, 0},
	//Gunpowder 25
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 2, 0, 0, 0},
};

#endif //!COLLISIONS_H_INCLUDED
