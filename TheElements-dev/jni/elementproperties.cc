/*
 * elementproperties.c
 * ----------------------------------------
 * Contains all the arrays that define the element properties.
 * When adding a new element, you need to add it to all of
 * these arrays as well.
 *
 * Properties list:
 *              -red
 *              -green
 *              -blue
 *              -fallVel
 *              -density
 *              -state
 *              -special
 *              -specialVal
 *              -inertia
 *
 * Elements list:
 * ----Special----
 *              0 = Spawn
 *              1 = Drag
 *              2 = Eraser
 * ----Normal----
 *              3 = Sand
 *              4 = Water
 *              5 = Steam
 *              6 = Ice
 *              7 = Wall
 *              8 = Destructible Wall
 *              9 = Plant
 *              10 = Fire
 *              11 = Magma
 *              12 = Stone
 *              13 = Oil
 *              14 = C4
 *              15 = C4++
 *              16 = Fuse
 *              17 = Acid
 *              18 = Salt
 *              19 = Salt-Water
 *              20 = Glass
 *              21 = Mud
 *              22 = Generator
 *              23 = Coal
 *              24 = Ant
 *              25 = Gunpowder
 *              26 = Mosquito
 *              27 = Wood
 *              28 = Termite
 *              29 = Insect Killer
 *              30 = Electricity
 * ----Add Elements Above----
 */

//Include the global macros
#include "elementproperties.h"

// Define the names of the elements
const char* baseName[] = {
    // ----Special----
    "Spawn",               // 0 = Spawn
    "Drag",                // 1 = Drag
    "Eraser",              // 2 = Eraser
    // ----Normal----
    "Sand",                // 3 = Sand
    "Water",               // 4 = Water
    "Steam",               // 5 = Steam
    "Ice",                 // 6 = Ice
    "Wall",                // 7 = Wall
    "Destructible Wall", // 8 = Destructible Wall
    "Plant",               // 9 = Plant
    "Fire",                // 10 = Fire
    "Magma",               // 11 = Magma
    "Stone",               // 12 = Stone
    "Oil",                 // 13 = Oil
    "C4",                  // 14 = C4
    "C4++",                // 15 = C4++
    "Fuse",                // 16 = Fuse
    "Acid",                // 17 = Acid
    "Salt",                // 18 = Salt
    "Salt-Water",  // 19 = Salt-Water
    "Glass",               // 20 = Glass
    "Mud",                 // 21 = Mud
    "Generator",   // 22 = Generator
    "Coal",                // 23 = Coal
    "Ant",                 // 24 = Ant
    "Gunpowder",   // 25 = Gunpowder
    "Mosquito",    // 26 = Mosquito
    "Wood",                // 27 = Wood
    "Termite",             // 28 = Termite
    "Insect Killer", // 29 = Insect Killer
    "Electricity",  // 30 = Electricity
    "Metal",        // 31 = Metal
};

//Define the color of the element
unsigned char baseRed[] =   {255, 0, 0, 255,   0, 220, 200, 157, 176,   0, 255, 204, 100, 143, 255, 255, 135, 150, 255,  50, 200, 100, 255,  50,   0,  90,   0, 125, 200, 222, 255, 100};
unsigned char baseGreen[] = {255, 0, 0, 213,   0, 220, 200, 157, 176, 255,   0,  51, 100,  80, 255, 211,  25, 255, 255, 100, 255,  85, 255,  50,  50,  90,  50,  50, 100, 200, 255, 100};
unsigned char baseBlue[] =  {255, 0, 0, 85, 255, 225, 255, 157, 164,   0,   0,   0, 100,  80, 143, 115,  30,  50, 230, 200, 255,   0, 255,  50,   0,  90, 100,  10,  10, 0, 0, 100};

//Define each base element's fall velocity
signed char baseFallVel[] = {0, 0, 0, 1, 1, -1, 0, 0, 0, 0, -1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1,1,1,0};

//Defines the density of each base element
unsigned char baseDensity[] = {10, 10, 10, 10,  5,  1, 10, 10, 10, 10,  1,  7, 10,  3, 10, 10, 10,  4, 10,  4, 10,  9, 10, 10,  4, 10,  4,  10,  4, 8,1,10};

//Defines the state of each base element (0 = Solid, 1 = Liquid, 2 = Gaseous)
unsigned char baseState[] = {0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 2, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,0,0};

/*
 * Specials list:
 *         -1 = None;
 *              1 = Spawn particles; Element
 *              2 = Break; Threshold velocity
 *              3 = Grow; Food element
 *              4 = Burn;
 *              5 = Explode; Explosiveness
 *              6 = Life; 1/(Probability of deletion)
 */
//Defines the special for each base element

int baseSpecial[][MAX_SPECIALS] =
{
    {SPECIAL_SPAWN, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Spawn
    {SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Drag
    {SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Eraser
    {SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Sand
    {SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Water
    {SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Steam
    {SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Ice
    {SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Wall
    {SPECIAL_BREAK, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE},  //Destructible Wall
    {SPECIAL_GROW, SPECIAL_BURN, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Plant
    {SPECIAL_HEAT, SPECIAL_LIFE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Fire
    {SPECIAL_HEAT, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Magma
    {SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Stone
    {SPECIAL_EXPLODE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Oil
    {SPECIAL_EXPLODE, SPECIAL_CONDUCTIVE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //C4
    {SPECIAL_EXPLODE, SPECIAL_CONDUCTIVE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //C4++
    {SPECIAL_EXPLODE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, // Fuse
    {SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Acid
    {SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Salt
    {SPECIAL_CONDUCTIVE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Salt-water
    {SPECIAL_BREAK, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Glass
    {SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Mud
    {SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Generator
    {SPECIAL_BURN, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Coal
    {SPECIAL_WANDER, SPECIAL_JUMP, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Ant
    {SPECIAL_EXPLODE, SPECIAL_CONDUCTIVE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Gunpowder
    {SPECIAL_WANDER, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Mosquito
    {SPECIAL_BURN, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Wood
    {SPECIAL_WANDER, SPECIAL_TUNNEL, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Termite
    {SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Insect Killer
    {SPECIAL_TRAIL, SPECIAL_CONDUCTABLE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE}, //Electricity
    {SPECIAL_CONDUCTIVE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE, SPECIAL_NONE} //Metal
};


//Defines the special value for each base element
int baseSpecialValue[][MAX_SPECIALS] =
{
    {SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET}, //TODO: Add the element names next to all of these columns
    {SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {5,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {4,5,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET}, //Plant
    {20,5,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET}, //Fire
    {3,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET}, //Magma
    {SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {7,3,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {10,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET}, //C4
    {20,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET}, //C4++
    {2,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {2,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET},
    {20,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET}, //Coal
    {40,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET}, //Ant
    {20,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET}, //Gunpowder
    {10,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET}, //Mosquito
    {10,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET}, //Wood
    {10,27,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET}, //Termite
    {SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET}, //Insect Killer
    {3,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET}, //Electricity
    {SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET,SPECIAL_VAL_UNSET} //Metal
};

//Defines the inertia of each element
unsigned char baseInertia[] = {255, 255, 255, 1, 1, 1, 255, 255, 255, 255, 1, 1, 3, 1, 255, 255, 255, 1, 1, 1, 255, 1, 255, 1, 1, 1, 1, 255, 1, 1,1,255};

//Defines the initial temp of anything of that element that is spawned (0 = atmosphere)
unsigned char baseStartingTemp[] = {0, 0, 0, 0, 100, 160, 1, 0, 0, 0, 255, 205, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 100, 0, 100, 0, 200, 0};

//Defines the highest and lowest temps that the element exists at
unsigned char baseHighestTemp[] = {255, 255, 255, 154, 150, 255, 75, 255, 200, 170, 255, 255, 200, 125, 125, 125, 125, 150, 255, 150, 255, 150, 255, 175, 175, 125, 175, 150, 175, 255, 255, 205};
unsigned char baseLowestTemp[] = {0, 0, 0, 0, 76, 151, 0, 0, 0, 0, 110, 201, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 25, 0, 25, 0, 25, 0, 0, 0};

//Defines the higher and lower elements to change to outside of the correct temp range
unsigned char baseHigherElement[] = {3, 3, 3, 20, 5, 5, 4, 7, 11, 10, 10, 11, 11, 10, 10, 10, 10, 5, 18, 5, 20, 3, 22, 10, 10, 10, 10, 10, 10, 29, 30, 11};
unsigned char baseLowerElement[] = {3, 3, 3, 3, 6, 4, 6, 7, 8, 9, 23, 12, 12, 13, 14, 15, 16, 17, 18, 6, 20, 21, 22, 23, 3, 25, 3, 27, 3, 29, 30, 31};
