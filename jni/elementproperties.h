/*
 * elementproperties.h
 * ----------------------------------------
 * Declares all the arrays that define the element properties.
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
#include "macros.h"

//Define the color of the element
extern unsigned char baseRed[];
extern unsigned char baseGreen[];
extern unsigned char baseBlue[];

//Define each base element's fall velocity
extern int	fallVel[] =	{0, 0, 0, 1, 1, -1, 0, 0, 0, 0, -1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1};

//Defines the density of each base element
extern int baseDensity[] = {10, 10, 10, 10,  5,  1, 10, 10, 10, 10,  1,  7, 10,  3, 10, 10, 10,  4, 10,  4, 10,  9, 10, 10};

//Defines the state of each base element (0 = Solid, 1 = Liquid, 2 = Gaseous)
extern int baseState[] = {0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 2, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0};

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
extern int baseSpecial[] = {{0}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {1}, {2,3}, {-1}, {-1}, {-1}, {3,4}, {3,4}, {3,4}, {3}, {-1}, {-1}, {-1}, {1}, {-1}, {1}, {3}};

//Defines the special value for each base element
extern int baseSpecialValue[] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {5}, {4,5}, {0}, {0}, {0}, {7,3}, {5,5}, {5,10}, {2}, {0}, {0}, {0}, {3}, {0}, {0}, {5}};

//Defines the inertia of each element
extern int baseInertia[] = {-1, -1, -1, 1, 1, 1, -1, -1, -1, -1, 0, 1, 1, 1, -1, -1, -1, 1, 1, 1, -1, 1, -1};
