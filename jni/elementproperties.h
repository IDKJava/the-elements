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
extern int	fallVel[];
//Defines the density of each base element
extern int baseDensity[];
//Defines the state of each base element (0 = Solid, 1 = Liquid, 2 = Gaseous)
extern int baseState[];
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
extern int baseSpecial[][];
//Defines the special value for each base element
extern int baseSpecialValue[][];
//Defines the inertia of each element
extern int baseInertia[];
