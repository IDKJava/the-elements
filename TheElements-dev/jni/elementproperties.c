/*
 * elementproperties.c
 * ----------------------------------------
 * Contains all the arrays that define the element properties.
 * When adding a new element, you need to add it to all of
 * these arrays as well.
 *
 * Properties list:
 * 		-red
 * 		-green
 * 		-blue
 *		-fallVel
 *		-density
 *		-state
 *		-special
 *		-specialVal
 *		-inertia
 *
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

//Include the global macros
#include "elementproperties.h"

// Define the names of the elements
char* baseName[] = {
		 // ----Special----
		 "Spawn",		// 0 = Spawn
		 "Drag",		// 1 = Drag
		 "Eraser",		// 2 = Eraser
		 // ----Normal----
		 "Sand",		// 3 = Sand
		 "Water",		// 4 = Water
		 "Steam",		// 5 = Steam
		 "Ice",			// 6 = Ice
		 "Wall",		// 7 = Wall
		 "Destructible Wall", // 8 = Destructible Wall
		 "Plant",		// 9 = Plant
		 "Fire",		// 10 = Fire
		 "Magma",		// 11 = Magma
		 "Stone",		// 12 = Stone
		 "Oil",			// 13 = Oil
		 "C4",			// 14 = C4
		 "C4++",		// 15 = C4++
		 "Fuse",		// 16 = Fuse
		 "Acid",		// 17 = Acid
		 "Salt",		// 18 = Salt
		 "Salt-Water",	// 19 = Salt-Water
		 "Glass",		// 20 = Glass
		 "Mud",			// 21 = Mud
		 "Generator",	// 22 = Generator
		 "Coal",		// 23 = Coal
		 "Bug",			// 24 = Bug
};

//Define the color of the element
unsigned char baseRed[] =   {255,   0,   0, 255,   0, 220, 200, 157, 176,   0, 255, 204, 100, 143, 255, 255, 135, 150, 255,  50, 200, 100, 255,  50,   0};
unsigned char baseGreen[] = {255,   0,   0, 213,   0, 220, 200, 157, 176, 255,   0,  51, 100,  80, 255, 211,  25, 255, 255, 100, 255,  85, 255,  50,  50};
unsigned char baseBlue[] =  {255,   0,   0,  85, 255, 225, 255, 157, 164,   0,   0,   0, 100,  80, 143, 115,  30,  50, 230, 200, 255,   0, 255,  50,   0};

//Define each base element's fall velocity
signed char baseFallVel[] = {0, 0, 0, 1, 1, -1, 0, 0, 0, 0, -1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1};

//Defines the density of each base element
char baseDensity[] = {10, 10, 10, 10,  5,  1, 10, 10, 10, 10,  1,  7, 10,  3, 10, 10, 10,  4, 10,  4, 10,  9, 10, 10, 4};

//Defines the state of each base element (0 = Solid, 1 = Liquid, 2 = Gaseous)
char baseState[] = {0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 2, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0};

/*
 * Specials list:
 *	   -1 = None;
 *		1 = Spawn particles; Element
 *		2 = Break; Threshold velocity
 *		3 = Grow; Food element
 *		4 = Burn;
 *		5 = Explode; Explosiveness
 *		6 = Life; 1/(Probability of deletion)
 */
//Defines the special for each base element

signed char baseSpecial[][MAX_SPECIALS] =
{
		{SPECIAL_SPAWN, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_BREAK, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},  //Destructible Wall
		{SPECIAL_GROW, SPECIAL_EXPLODE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_HEAT, SPECIAL_LIFE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Fire
		{SPECIAL_HEAT, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Magma
		{SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_EXPLODE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_EXPLODE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_EXPLODE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_EXPLODE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_BREAK, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},
		{SPECIAL_ALIVE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE} //Bug
};


//Defines the special value for each base element
char baseSpecialValue[][MAX_SPECIALS] =
{
		{0,0,0,0,0,0}, //TODO: Add the element names next to all of these columns
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{4,0,0,0,0,0},
		{20,5,0,0,0,0}, //Fire
		{3,0,0,0,0,0}, //Magma
		{0,0,0,0,0,0},
		{7,3,0,0,0,0},
		{10,0,0,0,0,0},
		{20,0,0,0,0,0},
		{2,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{5,0,0,0,0,0},
		{0,0,0,0,0,0}, //Bug
};

//Specifies whether particles of this element should only use the default special vals
char baseUseElementSpecialVals[] = {FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};

//Defines the inertia of each element
char baseInertia[] = {255, 255, 255, 1, 1, 1, 255, 255, 255, 255, 1, 1, 3, 1, 255, 255, 255, 1, 1, 1, 255, 1, 255, 1, 1};

//Defines the initial temp of anything of that element that is spawned (0 = atmosphere)
char baseStartingTemp[] = {0, 0, 0, 0, 100, 160, 1, 0, 0, 0, 255, 205, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//Defines the highest and lowest temps that the element exists at
char baseHighestTemp[] = {255, 255, 255, 154, 150, 255, 75, 255, 200, 170, 255, 255, 200, 125, 125, 125, 125, 150, 255, 150, 255, 150, 255, 225, 125};
char baseLowestTemp[] = {0, 0, 0, 0, 76, 151, 0, 0, 0, 0, 110, 201, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 75};

//Defines the higher and lower elements to change to outside of the correct temp range
char baseHigherElement[] = {0, 0, 0, 20, 5, 0, 4, 0, 11, 10, 0, 0, 11, 10, 10, 10, 10, 5, 0, 5, 0, 3, 0, 10, 10};
char baseLowerElement[] = {0, 0, 0, 0, 6, 4, 0, 0, 0, 0, 23, 12, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 23};
