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
 * ----Add Elements Above----
 */

//Include the global macros
#include "elementproperties.h"

//Define the color of the element
unsigned char baseRed[] =   {255,   0,   0, 255,   0, 220, 200, 157, 176,   0, 255, 204, 100, 143, 255, 255, 135, 150, 255,  50, 200,   0, 255,  50};
unsigned char baseGreen[] = {255,   0,   0, 213,   0, 220, 200, 157, 176, 255,   0,  51, 100,  80, 255, 211,  25, 255, 255, 100, 255, 213, 255,  50};
unsigned char baseBlue[] =  {255,   0,   0,  85, 255, 225, 255, 157, 164,   0,   0,   0, 100,  80, 143, 115,  30,  50, 230, 200, 255,  85, 255,  50};

//Define each base element's fall velocity
int	fallVel[] =	{0, 0, 0, 1, 1, -1, 0, 0, 0, 0, -1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1};

//Defines the density of each base element
int baseDensity[] = {10, 10, 10, 10,  5,  1, 10, 10, 10, 10,  1,  7, 10,  3, 10, 10, 10,  4, 10,  4, 10,  9, 10, 10};

//Defines the state of each base element (0 = Solid, 1 = Liquid, 2 = Gaseous)
int baseState[] = {0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 2, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0};

/*
 * Specials list:
 *	   -1 = None;
 *		0 = Spawn particles; Element
 *		1 = Break; Threshold velocity
 *		2 = Grow; Food element
 *		3 = Burn; Threshold
 *		4 = Explode; Explosiveness
 */
//Defines the special for each base element
int baseSpecial[][] = {{0}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {1}, {2,3}, {-1}, {-1}, {-1}, {3,4}, {3,4}, {3,4}, {3}, {-1}, {-1}, {-1}, {1}, {-1}, {1}, {3}};

//Defines the special value for each base element
int baseSpecialValue[][] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {5}, {4,5}, {0}, {0}, {0}, {7,3}, {5,5}, {5,10}, {2}, {0}, {0}, {0}, {3}, {0}, {0}, {5}};

//Defines the inertia of each element
int baseInertia[] = {-1, -1, -1, 1, 1, 1, -1, -1, -1, -1, 0, 1, 1, 1, -1, -1, -1, 1, 1, 1, -1, 1, -1};
