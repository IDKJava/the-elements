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

//Set up the workspace
void gameSetup(void);
//Set up the variable sized arrays
void arraySetup(void);

#endif //!SETUP_H_INCLUDED
