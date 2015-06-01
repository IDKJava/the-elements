/*
 * setup.h
 * -----------------------------
 * Declares the rsetup function, which initializes
 * all of the arrays and variables necessary. Also
 * includes necessary headers.
 */

#ifndef SETUP_H_INCLUDED
#define SETUP_H_INCLUDED

//Include the global variables
#include "app.h"
//Include the global macros
#include "macros.h"
//Include the Element properties (yes this is obvious)
#include "elementproperties.h"
//Include the free function
#include <stdlib.h>
//Include saveload for custom element loading
#include "saveload.h"

//Set up the workspace
void gameSetup(void);
//Set up the variable sized arrays
void arraySetup(void);
//Set up the atmosphere properties struct
void atmosphereSetup(void);
//Create all the elements
void elementSetup(void);
//Create all the particle structs
void particleSetup(void);

#endif //!SETUP_H_INCLUDED
